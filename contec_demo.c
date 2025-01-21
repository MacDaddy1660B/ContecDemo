/*
 * This is a test program for controlling
 * a Contec digital I/O card. The card's drivers
 * (or kernel modules on a Linux system) must
 * first be installed, and then the card needs
 * to be configured using Contec's configuration
 * utility. The default name given to the
 * card is DIO000, and is defined in the the
 * DEVICENAME constant below. On a Linux system,
 * the card is also visible as /dev/dio000.
 *
 * This program depends on libcdio to run.
 * Visit Contec's website to get a copy of it.
 * 
 * The program performs the following steps:
 *
 * 1) It gets some information about the card:
 *	a) Device type.
 *	b) Number of DI ports.
 *	c) Number of DO ports.
 *	d) Whether or not DioSetIoDirection
 *	   is callable for the device.
 * 2) The device DIO000 is initialized.
 * 3) An attempt is made to set the IO direction.
 * 4) Some IO operations are performed.
 * 5) The device is closed.
 *
 * The intent is to familiarize myself with
 * Contec's C API and their port and bit
 * numbering scheme.
 *
 * I'm using the Contec 32-channel Digital IO
 * card model DIO-32DM3-PE.
 *
 * AUTHOR: Gordon A. MacDonald, Ph.D
 */


#include <stdio.h>
#include "cdio.h"

#define DEVICE		"DIO-32DM3-PE"
#define DEVICENAME	"DIO000"

#define PORT0		0
#define PORT1		1
#define PORT2		2
#define PORT3		3

int main() {
	
	long 		rc;
	char 		ErrorString[256];
	int 		param;
	short 		Id,
			shortParam;

	// Get some device info
	rc = DioGetDeviceInfo(DEVICE, IDIO_DEVICE_TYPE, &shortParam, NULL, NULL);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioGetDeviceInfo: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioGetDeviceInfo: IDIO_DEVICE_TYPE: %hi\n", shortParam);
	}
	rc = DioGetDeviceInfo(DEVICE, IDIO_NUMBER_OF_DI_PORT, &shortParam, NULL, NULL);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioGetDeviceInfo: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioGetDeviceInfo: IDIO_NUMBER_OF_DI_PORT: %hi\n", shortParam);
	}
	rc = DioGetDeviceInfo(DEVICE, IDIO_NUMBER_OF_DO_PORT, &shortParam, NULL, NULL);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioGetDeviceInfo: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioGetDeviceInfo: IDIO_NUMBER_OF_DO_PORT: %hi\n", shortParam);
	}
	rc = DioGetDeviceInfo(DEVICE, IDIO_NUMBER_OF_DI_BIT, &shortParam, NULL, NULL);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioGetDeviceInfo: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioGetDeviceInfo: IDIO_NUMBER_OF_DI_BIT: %hi\n", shortParam);
	}
	rc = DioGetDeviceInfo(DEVICE, IDIO_NUMBER_OF_DO_BIT, &shortParam, NULL, NULL);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioGetDeviceInfo: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioGetDeviceInfo: IDIO_NUMBER_OF_DO_BIT: %hi\n", shortParam);
	}
	rc = DioGetDeviceInfo(DEVICE, IDIO_IS_DIRECTION, &param, NULL, NULL);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioGetDeviceInfo: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioGetDeviceInfo: IDIO_IS_DIRECTION: %i\n", param);
	}

	// Initialize the device
	rc = DioInit( DEVICENAME, &Id);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioInit: %li: %s\n", rc, ErrorString);
	}


	// Multi-byte output.
	short OutPorts[2] = {PORT0, PORT1};
	unsigned char OutPortArray[2] = {0xf0, 0xf0};
	rc = DioOutMultiByte(Id, OutPorts, 2, OutPortArray);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioOutMultiByte: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioOutMultiByte: output:\t");
		for (int i=0; i<2; i++) { printf("\t%i", OutPortArray[i]); }
		printf("\n");
	}

	// Multi-byte input.
	short InpPorts[2] = {PORT2, PORT3};
	unsigned char InPortArray[2] = {0, 0};
	rc = DioInpMultiByte(Id, InpPorts, 2, InPortArray);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioInpMultiByte: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioInpMultiByte: got InPortArray: ");
		for (int i=0; i<2; i++) { printf("\t%i", InPortArray[i]); }
		printf("\n");
	}
	
	
	// Reset the device.
	rc = DioResetDevice(Id);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioResetDevice: %li: %s\n", rc, ErrorString);
	}

	
	// Multi-bit output.
	short OutBitNo[8] = {0,1,2,3,4,5,6,7};
	unsigned char ByteOut[8] = {0,1,0,1,0,1,0,1};
	rc = DioOutMultiBit(Id, OutBitNo, 8, ByteOut);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioOutMultiBit: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioOutMultiBit: output:\t\t");
		for (int i=0; i<8; i++) { printf("\t%i", ByteOut[i]); }
		printf("\n");
	}

	// Multi-bit Echo-back
	short EchoBitNo[8] = {0,1,2,3,4,5,6,7};
	unsigned char EchoBack[8] = {0,0,0,0,0,0,0,0}; // These must be set to zero, or else.
	rc = DioEchoBackMultiBit(Id, EchoBitNo, 8, EchoBack);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioEchoBackMultiBit: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioEchoBackMultiBit: got EchoBack: ");
		for (int i=0; i<8; i++) { printf("\t%i", ByteOut[i]); }
		printf("\n");
	}

	// Multi-bit input.
	short InpBitNo[8] = {0,1,2,3,4,5,6,7};
	unsigned char ByteIn[8] = {0,0,0,0,0,0,0,0}; // These must be set to zero, or else.
	rc = DioInpMultiBit(Id, InpBitNo, 8, ByteIn);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioInpMultiBit: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioInpMultiBit: got ByteIn:\t");
		for (int i=0; i<8; i++) { printf("\t%i", ByteIn[i]); }
		printf("\n");
	}

	// Multi-bit Echo-back
	short EchoBitNo[8] = {0,1,2,3,4,5,6,7};
	unsigned char EchoBack[8] = {0,0,0,0,0,0,0,0}; // These must be set to zero, or else.
	rc = DioEchoBackMultiBit(Id, EchoBitNo, 8, EchoBack);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioEchoBackMultiBit: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioEchoBackMultiBit: got EchoBack: ");
		for (int i=0; i<8; i++) { printf("\t%i", ByteOut[i]); }
		printf("\n");
	}

	
	// Reset the device.
	rc = DioResetDevice(Id);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioResetDevice: %li: %s\n", rc, ErrorString);
	}

	
	// Close the device
	rc = DioExit(Id);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioExit: %li: %s\n", rc, ErrorString);
	}

	return 0;
}
