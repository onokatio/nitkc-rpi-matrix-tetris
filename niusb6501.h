#ifndef _NIUSB6501_H_
#define _NIUSB6501_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <usb.h>

struct usb_dev_handle  *HANDLE;

extern size_t niusb6501_list_devices(struct usb_device *devices[], size_t size);
extern struct usb_dev_handle *niusb6501_open_device(struct usb_device *device);
extern int niusb6501_close_device(struct usb_dev_handle *handle);

extern int niusb6501_start_counter(struct usb_dev_handle *handle);
extern int niusb6501_stop_counter(struct usb_dev_handle *handle);
extern int niusb6501_read_counter(struct usb_dev_handle *handle, unsigned long *value);
extern int niusb6501_write_counter(struct usb_dev_handle *handle, unsigned long value);

extern int niusb6501_read_port(struct usb_dev_handle *handle, unsigned char port, unsigned char *value);
extern int niusb6501_write_port(struct usb_dev_handle *handle, unsigned char port, unsigned char value);

extern int niusb6501_set_io_mode(struct usb_dev_handle *handle, unsigned char port0, unsigned char port1, unsigned char port2);


/*** original code ***/
extern
void
init_device(void);

extern
void
close_device(void);

extern
void
set_io_mode(unsigned char  uc_port0,
	    unsigned char  uc_port1,
	    unsigned char  uc_port2);

extern
unsigned char
read_port(unsigned char  uc_port);

extern
void
write_port(unsigned char  uc_port,
	   unsigned char  uc_value);



#endif
