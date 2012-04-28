// Simple Serial1 Reader


#include <CapSense.h>
#include <Servo.h>

CapSense   cs_4_2 = CapSense(4,2);        // 10 megohm resistor between pins 4 & 2, pin 2 is sensor pin, add wire, foil
Servo foot;

long last_Time = 0;
int wait_time = 2000;
int LED_Pin = 13;

Servo Foot1;Servo Foot2;

char code[11];
// char password[] = "3E00FCD176";
char password[] = "3E00FCC847";
char login[] = "3D0061DA2D";



void setup()
{
	// Set up the Capacitive sensor
	cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
	cs_4_2.reset_CS_AutoCal();

	// Just a light for debug purposes
	pinMode(LED_Pin, OUTPUT);
	Serial.begin(9600);
	for (int i=0; i<5; i++){
		digitalWrite(LED_Pin, HIGH);
		delay(500);
		digitalWrite(LED_Pin, LOW);
		delay(500);
	}

	Serial.println("Hello");
	Serial.print ("Compiled at ");
	Serial.println(__TIMESTAMP__);

	Serial1.begin(9600);

	last_Time = millis();

	Foot1.attach(9);
	Foot2.attach(10);
}

void loop()
{
	read_RFID();
	read_capsensor();
}

void read_capsensor(){

	if (last_Time + wait_time < millis()){
		long total_capsense =  cs_4_2.capSense(100);
		if (total_capsense > 450){
			digitalWrite(13, HIGH);
			Raise_Keyboard(1);
			last_Time = millis();
			Serial.println(total_capsense);
		}
		else {
			digitalWrite(13, LOW);
			// cs_4_2.reset_CS_AutoCal();
			Raise_Keyboard(0);  
		}
		//last_Time = millis();
	}
}

void check_password(){
	if (strcmp(code,password) == 0) {
		Serial.println ("MyPassword");
		Keyboard.println ("MyPassword");
		// Raise_Keyboard ();
	} else 	if (strcmp(code,login) == 0) {
		Serial.println ("Login");
		Keyboard.println ("Login Password");
		// Raise_Keyboard ();
	} else {
		Serial.println ("Wrong");
	}
}

void read_RFID(){

	if (Serial1.available()){
		if (Serial1.read() == 2){
			Serial1.readBytesUntil (3, code, 10);
			Serial1.flush();
			Serial.print(code);
			Serial.println(":");
			check_password();
		}
	}
}

void Raise_Keyboard(bool foot_state){
	if (foot_state == 1){
		Foot1.write(180);
		Foot2.write(180);
		// foot_state = 1;
		Serial.println("Up");
	} else {
		Foot1.write(90);
		Foot2.write(90);
		// foot_state = 0;
		Serial.println("Down");
	}
}