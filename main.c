#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>

#define LOGI_VENDOR 0x046d
#define LOGI_PRODUCT 0xc52b

int main(){
	int err = 0;
	libusb_context *ctx;
	libusb_device_handle *logi_h;
	libusb_device *logi;

	err = libusb_init(&ctx);
	if(err){
		fprintf(stderr, "failed to init libusb\n");
	   	return EXIT_FAILURE;
	}

	printf("libusb initialized, finding device\n");

	logi_h = libusb_open_device_with_vid_pid(ctx, LOGI_VENDOR, LOGI_PRODUCT);
	if (logi_h == NULL) {
		fprintf(stderr, "Failed to find device\n");
		return EXIT_FAILURE;
	}

	printf("Found device, resetting\n");

	if (libusb_reset_device(logi_h) != 0){
		fprintf(stderr, "Failed to reset device\n");
		return EXIT_FAILURE;
	}

	libusb_exit(ctx);

	printf("Reset successful?\n");
	return 0;
}
