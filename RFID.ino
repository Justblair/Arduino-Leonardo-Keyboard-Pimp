// Pimped up keyboard Sketch by Blair Thompson (aka Justblair)
// Project is documented at http://www.justblair.co.uk
// For this sketch to run install the CapacitiveSensor Library
// written by Paul Badger available at
// http://playground.arduino.cc//Main/CapacitiveSensor?from=Main.CapSense
// The project was origionally created using a Cherry G80-3000 Keyboard
// Though other keyboards may be suitable

// Enable DEBUG by uncommenting if you want to see the readings that 
// come from the Capacitive Sensor in a serial console
#define DEBUG  

// Enable Test if you want to test the servo positions.  When the following
// line is uncommented, the switch will set the servos.  In Test mode the
// Servos will be on constantly.  Do not leave the keyboard in Test mode!
// #define TEST

// The following includes invoke the required Libraries
#include <CapacitiveSensor.h>
#include <Servo.h>

// Create the Capacitive Sensor
CapacitiveSensor cs_4_2 = CapacitiveSensor(4,5);        
// 10 mega ohm resistor between pins 4 & 2, pin 2 is sensor pin, add wire, foil

// Create instances of Servo Class
Servo foot_left, foot_right, foot_left2, foot_right2;

// I can put the servos in an object array, This is more a note for later...
// Servo servo[]={foot_left,foot_left2,foot_right, foot_right2 }; 

// Here are some variables
unsigned long last_Time = 0;	// This is used to control the delay before the board drops
int foot_value;					// The foot Value is used to control the current positions of servos
char code[11];					// code is used to store RFID tags for checking against the list
boolean keyboard_state;			// used to record if the keyboard is up or down
boolean prevSwitchState = 0;	// used to check if the switch has been changed recently

// And some usefull constants
const int wait_time = 2000;		// This adds a slight delay before the keyboard lowers.
const int foot_value_up = 130;  // maximum amount of travel of keyboard foot
const int foot_value_down = 20; // minimum amount of travel of keybaord foot
const int capSensitivity = 200; // This sets the threshold for the capacitive sensor

// And this is my RFID tags and their corresponding passwords (not the real passwords obviously)
char* rfidTag[][2] = {
	{"3E00FCC847", "MyWebbyPSWD"},
	{"3D0061DA2D", "MyLoginPSWD"},
	{"3D00492C25", "MyEmailPSWD"},
	{"3E00FCD176", "Etc"        },
};

// I could work this out with sizeoff, but I am too lazy
#define PSWDARRAYSIZE 4

// The setup will only run once
void setup()
{
	Serial.begin(115200);	    // Open a serial Port to the PC

	// Set up the Capacitive sensor
	// turn off auto-calibrate as otherwise the keyboard will 
	// start ignoring me if I am typing for any length of time
	cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);    

	pinMode(7,INPUT);			// Activate the ON/Off switch
	digitalWrite(7,HIGH);		// Activate the internal Pull up resistors
	Keyboard.begin();			// Start the Arduino Keyboard Emulator

	// Little bit of serial printing to say hello
	while (!Serial  && millis() <700){
		;
	}
	Serial.println("Hello");
	Serial.print ("Compiled at ");
	Serial.println(__TIMESTAMP__);

	// The Rfid is connected to the hardware serial port of the Leo
	Serial1.begin(9600);        

	// Set the timer to the current Arduino Time
	last_Time = millis();		
}

// The loop runs forever.
void loop()
{
	// Check the state of the side mounted switch
	boolean switchState = digitalRead(7);

	// Check to see if the switch has changed.  If it has we 
	// will perform a calibration of the capacitive sensor
	// it gradually drifts it's calibration so this gives us 
	// a way to manually calibrate it.
	if (prevSwitchState == 0 && switchState == 1){   
		cs_4_2.reset_CS_AutoCal();
		prevSwitchState = 1;
		Serial.print("Switch on");
		last_Time = millis();
	}
	if (prevSwitchState == 1 && switchState == 0){
		prevSwitchState = 0;
		Serial.println("Switch Off");
	}

#ifndef TEST
	// Providing we are not testing then we do the do
	if (switchState == 1){		// Check the state of the On/Off Switch
		read_capsensor();		// If Switched on, check for humans
	}
	read_RFID();				// Read the RFID to see if there is a chip nearby
#endif


#ifdef TEST
	// If we are testing the sytsem we simply move the 
	// feet up and down depending on the position of the side switch
	Raise_Keyboard(digitalRead(7));  
#endif
}

void read_capsensor(){
	long total_CapacitiveSensor =  cs_4_2.capacitiveSensor(100);

# ifdef DEBUG
	Serial.println(total_CapacitiveSensor);
# endif

	if (total_CapacitiveSensor > capSensitivity){
		last_Time = millis();
		// We check to see if this is a new detection and if so move the legs
		// if not, we do nothing.  We do this so the servos are not constantly
		// in operation 
		if (keyboard_state == 0){
			keyboard_state = 1;
			Raise_Keyboard(1);
		}
	} else if (last_Time + wait_time < millis() && keyboard_state==1){
		// If there is no human present we wait a while and then lower the keyboard
		Raise_Keyboard(0);
		keyboard_state = 0;
	}
}

// This function checks the RFID and if it is correct it writes types the password to the PC.
// The function also sends the RFID ID's to the Serial Console.
void check_password(){
	Serial.print(code);
	Serial.print(": ");

	// The for loop will scan the array for a match to the code we took
	for (int i = 0; i < PSWDARRAYSIZE; i++){
		if (strcmp(code,rfidTag[i][0]) == 0) {
			Keyboard.println (rfidTag[i][1]);
			Serial.println ("OK");
			return;  // We found a match, come out the function now.
		}
	}  // end for

	// We did not find a match in the for loop
	Serial.println("Wrong");  
}  

// This function checks for RFID tags and then runs the check_password function
void read_RFID(){
	if (Serial1.available()){
		if (Serial1.read() == 2){
			Serial1.readBytesUntil (3, code, 10);
			Serial1.flush();
			check_password();
		}
	}
}

// This function moves all of the feet to their correct positions
void Raise_Keyboard(bool foot_state){

	// Lets check the foot state and send the status to the console
	if (foot_state == 1){
		foot_value = foot_value_up;
		Serial.println("Up");
	} else {
		foot_value = foot_value_down;
		Serial.println("Down");
	}

	// We start by attaching the servos that control the feet.
	foot_left.attach(9);  
	foot_left2.attach(11);
	foot_right.attach(10);
	foot_right2.attach(8);

	// We move the feet together.  Because the servo tops are opposing one anther we have to flip
	// the positions around.  
	foot_left.write(foot_value);
	foot_left2.write(map(foot_value,foot_value_down,foot_value_up,foot_value_up,foot_value_down));
	foot_right.write(foot_value);
	foot_right2.write(map(foot_value,foot_value_down,foot_value_up,foot_value_up,foot_value_down));

	// We need to give a short delay to let the servos reach their final position
	delay(250);  

#ifndef TEST
	// Unless we are testing we switch off the feet to preserve the servos.
	foot_left.detach();
	foot_left2.detach();
	foot_right.detach();
	foot_right2.detach();
#endif
}