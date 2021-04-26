Automated Antenna Switch
	This will replace the manual switch
	This will also allow remote control
	
Things to do
 
	Work on Node-Red code for interface
		After circuit is complete fully test node-red code
	Design circuit and test
	Order box
		-- Order Power poles for box
	Add manual switch to prevent remote control
	Add power strip 
		
Completed Items
	Refactor code to remove redundent loops - 21-Mar-21
	Work on code to get/set switch via Serial - 22-Mar-21
		Looks like the code is looking for closing comma but still need to investigate
		-- Removed code for comma and only expect 1 argument for Set command
	Refactor code to use arrays for buttons and relays - 23-Mar-21
	Decide if I will include LEDs - 23-Mar-21
	Remove unused variables - 24-Mar-21
	Refactor code to improve flow and better names - 24-Mar-21
	Code for relays - 24-Mar-21
	Work on Node-Red code for interface - 26-Mar-2021
		-- Added Start of Packet to all messages
		-- First pass worked but need to clean up UI and messages
	Added code to support Device Control Network DCN - 28-Mar-2021
	Updated Node-Red to send DCN packtets - 28-Mar-21
	Added Get to Set so that the current switch is known - 29-Mar-21
	Remove LCD code as it does not work well with MKR WiFi 1010 - 	17-Apr-21
	Write code for MQTT to communication - 20-Apr-21
		--Need to impliment XQTT pub/sub messages for Arduino - 20-Apr-21
	Look at other means for communication - XQTT - 20-Apr-21
		Will use a MKR WiFi 1010 board	- 20-Apr-21
	Add LED to indicate when connected to WiFi and MQTT - 25-Apr-21
		