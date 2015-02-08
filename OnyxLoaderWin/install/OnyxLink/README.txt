OnyxLoader installer README

FTDI device driver will be automatically installed if an internet connection is present


OnyxLink version 1.1
	Fix bug when saving data - plotting is attempted, but disappeared on user prompt, can't get back to original size
	fixed bug with saving data - data was being saved anyway.
	
	Added ctrl press to enable "advanced features" (updating from local firmware and to beta/exp across internet)

	Add from -> to version numbers when updating from internet
	
	Show "firmware flash completed" user message as soon as finished, not waiting for confirmation box to be OK'ed.
	
	Can make form smaller

OnyxLoader version 0.5
	Parse and display firmware version number to be programmed from network or local file, parse and display
	current programmed version number through serial interface, offer ok/cancel choice

OnyxLoader version 0.4
	Windows firmware loader sets EEPROM 
	Added charging patch (500 mA instead of 50 mA when fully discharged when using wall socket)
	Fixed all warnings in build

OnyxLoader version 0.3
	Added plotting of data

OnyxLoader version 0.2
	Added installer, progress bars

