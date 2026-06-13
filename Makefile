all:
	gcc src/speed_sender.c -o speed_sender
	gcc src/temp_sender.c -o temp_sender
	gcc src/battery_sender.c -o battery_sender
	gcc src/dashboard_receiver.c -o dashboard_receiver
	gcc src/diagnostic_tool.c -o diagnostic_tool
	gcc src/dtc_reader.c -o dtc_reader
	gcc src/gateway_ecu.c -o gateway_ecu

clean:
	rm -f speed_sender temp_sender battery_sender dashboard_receiver diagnostic_tool dtc_reader gateway_ecu