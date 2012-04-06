// Simple RFID Reader


#include <SoftwareSerial.h>

char code[11];
SoftwareSerial RFID (2,3);
char password[] = "3E00FCD176";

void setup()
{
	Serial.begin(9600);
	Serial.write("Hello");
	RFID.begin(9600);
}

void loop()
{
	read_Rfid();
}

void check_password(){
	if (strcmp(code,password) == 0) {
		Serial.println("MyPassword");
	} else {
		Serial.println("Wrong");
	// Keyboard.print ("MyPassword");
	}
}
void read_Rfid(){
	
	if (RFID.available()){
		if (RFID.read() == 2){
			RFID.readBytesUntil (3, code, 10);
			RFID.flush();
			Serial.write(code);
			Serial.println(":");
			check_password();
		}
	}
}