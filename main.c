#define _GNU_SOURCE
#include <libusb-1.0/libusb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>



#define LOGI_VENDOR 0x046d
#define LOGI_PRODUCT 0xc52b

int main(){
	libusb_context *ctx;
	int err = 0;
	struct libusb_device **list;
	struct libusb_device_descriptor desc;
	ssize_t num_devs;
	libusb_device *logi;
	uint8_t bnum, dnum; 
	char *devpath;

	err = libusb_init(&ctx);
	if(err){
		fprintf(stderr, "failed to init libusb\n");
	   	return EXIT_FAILURE;
	}

	printf("libusb initialized, finding device\n");

	num_devs = libusb_get_device_list(ctx, &list);

	for (int i = 0; i< num_devs; i++){
		logi = list[i];
		libusb_get_device_descriptor(logi, &desc);
		if (desc.idVendor == LOGI_VENDOR && desc.idProduct == LOGI_PRODUCT)
			break;
		logi = NULL; //if the device is not found, the dev pointer will be null
	}

	if (logi == NULL) {
		fprintf(stderr, "failed to find transciever\n");
		return EXIT_FAILURE;
	}

	printf("Device found, getting bus information\n");

	bnum = libusb_get_bus_number(logi);
	dnum = libusb_get_device_address(logi);

	asprintf(&devpath, "/dev/bus/usb/%0.3hhi/%0.3hhi",bnum,dnum);

	printf("Device found at %s\n", devpath);

	int fd;
	fd = open(devpath, O_WRONLY);
	if (fd < 0) {
		perror("Error opening device file");
		return EXIT_FAILURE;
	}

	int rc;
	printf("resetting Logitech Transciever\n");
	rc = ioctl(fd, USBDEVFS_RESET, 0);
	if (rc < 0){
		perror("reset ioctl failed");
		return EXIT_FAILURE;
	}

	//cleanup
	close(fd);
	free(devpath);
	libusb_free_device_list(list,0);
	libusb_exit(ctx);

	printf("Reset successful?\n");
	return 0;
}
