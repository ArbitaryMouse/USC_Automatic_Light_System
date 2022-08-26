### MacOS
- Find the device name of the USB serial adapter by opening a Terminal window, and with the adapter
not plugged in, use the command `ls /dev/tty.usb*` to find all the files in the `/dev` directory that
have names that start with tty.usb. Then plug in the USB serial adapter, run the command again and
note what new files have appeared. The adapter’s device file should now be present with a name like
`/dev/tty.usbserial-A901C419`. 
- From the Terminal window enter the command: `screen name - of - device 9600`
	+ where “name-of-device” is the device filename you found above for the adapter device file. At this point anything you type in the window goes out the serial adapter.
