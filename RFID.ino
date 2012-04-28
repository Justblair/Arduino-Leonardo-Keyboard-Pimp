// Simple Serial1 Reader
//#define DEBUG

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
char* password[] = {"3E00FCC847", "MyPassword"};
char* login[] = {"3D0061DA2D", "MyLogin"};



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
	long total_capsense =  cs_4_2.capSense(100);
	if (total_capsense > 450){
		digitalWrite(13, HIGH);
		Raise_Keyboard(1);
		last_Time = millis();
# ifdef DEBUG
		Serial.println(total_capsense);
# endif
	}
	if (last_Time + wait_time < millis()){
				digitalWrite(13, LOW);
		Raise_Keyboard(0);
	}
}

void check_password(){
	if (strcmp(code,password[0]) == 0) {
#ifdef DEBUG 
		Serial.println (password[1]); 
#endif
		Keyboard.println (password[1]);
	} else 	if (strcmp(code,login[0]) == 0) {
#ifdef DEBUG
		Serial.println (login[1]);
#endif
		Keyboard.println (login[1]);
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
# ifdef DEBUG 
	Serial.println("Up");
# endif
	} else {
		Foot1.write(90);
		Foot2.write(90);
# ifdef DEBUG
		Serial.println("Down");
# endif
	}
}