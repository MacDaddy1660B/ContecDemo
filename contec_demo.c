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

#define DEVICENAME	"DIO000"

int main() {
	
	short Id;
	long rc;
	char ErrorString[256];
	int param;
	short shortParam;
	unsigned char Data;

	// Get some device info
	rc = DioGetDeviceInfo(DEVICENAME, IDIO_DEVICE_TYPE, &shortParam, NULL, NULL);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioGetDeviceInfo: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioGetDeviceInfo: IDIO_DEVICE_TYPE: %hi", shortParam);
	}
	rc = DioGetDeviceInfo(DEVICENAME, IDIO_NUMBER_OF_DI_PORT, &shortParam, NULL, NULL);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioGetDeviceInfo: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioGetDeviceInfo: IDIO_NUMBER_OF_DI_PORT: %hi", shortParam);
	}
	rc = DioGetDeviceInfo(DEVICENAME, IDIO_NUMBER_OF_DO_PORT, &shortParam, NULL, NULL);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioGetDeviceInfo: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioGetDeviceInfo: IDIO_NUMBER_OF_DO_PORT: %hi", shortParam);
	}
	rc = DioGetDeviceInfo(DEVICENAME, IDIO_IS_DIRECTION, &param, NULL, NULL);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioGetDeviceInfo: %li: %s\n", rc, ErrorString);
	} else {
		printf("DioGetDeviceInfo: IDIO_IS_DIRECTION: %i", param);
	}

	// Initialize the device
	rc = DioInit( DEVICENAME, &Id);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioInit: %li: %s\n", rc, ErrorString);
	}

	// Set IO direction
	// Port 0-1: Output
	// Port 2-3: Input
	rc = DioSetIoDirection(Id, 0x03);	// 0000-0011
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioSetIoDirection: %li: %s\n", rc, ErrorString);
	}

	//
	// Put some calls in here to output and input some data.
	//

	// Close the device
	rc = DioExit(Id);
	if (rc != DIO_ERR_SUCCESS) {
		DioGetErrorString(rc, ErrorString);
		printf("DioExit: %li: %s\n", rc, ErrorString);
	}

	return 0;
}
