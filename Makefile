all:
	gcc src/speed_sender.c -o speed_sender
	gcc src/temp_sender.c -o temp_sender
	gcc src/battery_sender.c -o battery_sender
	gcc src/dashboard_receiver.c -o dashboard_receiver

clean:
	rm -f speed_sender temp_sender battery_sender dashboard_receiver