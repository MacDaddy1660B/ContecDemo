#include <stdio.h>
#include <time.h>
#include "cdio.h"

#define DEVICENAME	"DIO000"

#define PORT0		0
#define PORT1		1
#define PORT2		2
#define	PORT3		3

#define INPORTS		{PORT0, PORT1}
#define	OUTPORTS	{PORT2, PORT3}

#define ERRORBUFFER	256
#define INBUFFERLEN	1000
#define OUTBUFFERLEN	2048


//Function prototypes
long printDioErrorString(long, char *);

int main() {

	long		rc;
	short		Id;
	unsigned int	InBuff[INBUFFERLEN],
			OutBuff[OUTBUFFERLEN];

	// Initialize device.
	rc = DioInit(DEVICENAME, &Id);
	rc = printDioErrorString(rc, "DioInit");


	// Set the IO direction.
	rc = DioDmSetDirection(Id, PIO_1616);
	rc = printDioErrorString(rc, "DioDmSetDirection");


	// Set stand-alone mode since we're only using one board.
	rc = DioDmSetStandAlone(Id);
	rc = printDioErrorString(rc, "DioDmSetStandAlone");


	// Set IO triggers.
	// Set start trigger.
	rc = DioDmSetStartTrg(Id, DIODM_DIR_IN, DIODM_START_SOFT);
	rc = printDioErrorString(rc, "DioDmSetStartTrg");
	rc = DioDmSetStartTrg(Id, DIODM_DIR_OUT, DIODM_START_SOFT);
	rc = printDioErrorString(rc, "DioDmSetStartTrg");

	// Set stop trigger.
	rc = DioDmSetStopTrg(Id, DIODM_DIR_IN, DIODM_STOP_SOFT);
	rc = printDioErrorString(rc, "DioDmSetStopTrg");
	rc = DioDmSetStopTrg(Id, DIODM_DIR_OUT, DIODM_STOP_SOFT);
	rc = printDioErrorString(rc, "DioDmSetStopTrg");


	// Reset device to clear counters, FIFO.
	rc = DioDmReset(Id, DIODM_RESET_FIFO_IN);
	rc = printDioErrorString(rc, "DioDmReset");
	rc = DioDmReset(Id, DIODM_RESET_FIFO_OUT);
	rc = printDioErrorString(rc, "DioDmReset");


	// Set buffer and length.
	rc = DioDmSetBuff(Id, DIODM_DIR_IN, (unsigned long *)InBuff, INBUFFERLEN, DIODM_WRITE_ONCE);
	rc = printDioErrorString(rc, "DioDmSetBuff");
	rc = DioDmSetBuff(Id, DIODM_DIR_OUT, (unsigned long *)OutBuff, OUTBUFFERLEN, DIODM_WRITE_ONCE);
	rc = printDioErrorString(rc, "DioDmSetBuff");


	// Start bus-mastered input.
	rc = DioDmStart(Id, DIODM_DIR_IN);
	rc = printDioErrorString(rc, "DioDmStart");
	unsigned long Status = DIODM_STATUS_PIOSTART;
	unsigned long Err;
	rc = DioDmGetStatus(Id, DIODM_DIR_IN, &Status, &Err);
	rc = printDioErrorString(rc, "DioDmGetStatus");
	//while (Status == DIODM_STATUS_PIOSTART) {
	//	rc = DioDmGetStatus(Id, DIODM_DIR_IN, &Status, &Err);
	//	//rc = printDioErrorString(rc, "DioDmGetStatus");
	//	//printf("Status: %li\tErr: %li\n", Status, Err);
	//}
	struct timespec req;
	req.tv_sec = 1;
	req.tv_nsec = 1000;
	nanosleep(&req, NULL);
	rc = DioDmStop(Id, DIODM_DIR_IN);
	rc = printDioErrorString(rc, "DioDmStop");

	// Print the input data
	printf("INPUT BUFFER CONTENTS:\n");
	for (int i=0; i<INBUFFERLEN; i++) {printf("%i ", InBuff[i]);}
	printf("\n");


	// De-initialize device.
	rc = DioExit(Id);
	rc = printDioErrorString(rc, "DioExit");

	return 0;
}


long printDioErrorString(long ErrorCode, char * caller) {
	long rc;
	char ErrorString[ERRORBUFFER];

	rc = DioGetErrorString(ErrorCode, ErrorString);
	printf("%s: \t\t%s\n", caller, ErrorString);

	return rc;
}
