#include <OneWire.h> //include 1Wire Library

int Internal_Top_Pin     = 19; //Sensor in top of cooking chamber
int Internal_Bottom_Pin = 18; //Sensor in bottom of cooking chamber

int Smoke_Temp_Pin         = 17; //Smoke Sensor - to know when to blow smoke
int Steam_Temp_Pin        = 16; //Steam Sensor - to know when to blow steam

int Desired_Temp        = 0; //Initialise Default internal Temp to 0 (all off)
int Desired_Smoke        = 1; //Initialise smoke analog value to 1
int Desired_Smoke_Blow    = 70; //Temp to blow the smoke in at
int Desired_Steam_Blow    = 89; //Temp to blow the steam in at
int Desired_Diff        = 2;  //Temp which internal fan starts if there is a difference

int Smoke_Fan_Pin        = 15; //Smoke Fan Pin
int Steam_Fan_Pin        = 14; //Steam Fan Pin
int Inter_Fan_Pin        = 13; //Internal Fan Pin

int Smoke_Heat_Pin        = 12; //Smoke Heat
int Steam_Heat_Pin        = 11; //Steam Heat

int menucount           = 1 ; //display the first menu
int dispint             = 1 ; //initialise the display

int Steam_Status        = 0 ; //Steam OFF
int Smoke_Status        = 0 ; //Smoke OFF
int Circu_Status        = 0 ; //Internal Fan OFF

int Internal_Top_Temp     = 99;
int Internal_Bottom_Temp= 99;
int Smoke_Temp            = 99;
int Steam_Temp            = 99;

//Setup Loop

void setup() {

    Serial.begin(9600); //Setup Serial

    Serial.print(0xFE, BYTE);

    Serial.print(0x42, BYTE);  

    Serial.print(0xFE, BYTE);

    Serial.print(0x41, BYTE);

    Serial.print(0xFE, BYTE);

    Serial.print(0x48, BYTE);

    backlightOn();  //Turn on LCD and BackLight

    //Initialise Inputs
    pinMode(Internal_Top_Pin, INPUT);
    pinMode(Internal_Bottom_Pin, INPUT);
    pinMode(Smoke_Temp_Pin, INPUT);
    pinMode(Steam_Temp_Pin, INPUT);
   
    //Initialise Outputs
    pinMode(Smoke_Fan_Pin, OUTPUT);
    pinMode(Steam_Fan_Pin, OUTPUT);
    pinMode(Inter_Fan_Pin, OUTPUT);
    pinMode(Smoke_Heat_Pin, OUTPUT);
    pinMode(Steam_Heat_Pin, OUTPUT);
   
    clearLCD();

    delay(100);//Add a short delay after clearing

    Serial.print("Welcome");

    delay(1000);

    clearLCD();

            }
           
void loop()  {

switch (menucount) { //Case Statement to print the Menu Screens

    case 1:

        DrawMenu1();

        break;

    case 2:

        DrawMenu2();

        break;

    case 3:

        menureset();

        break;

    case 4:

        DrawMenu4();

        break;   

    case 5:

        DrawMenu5();

        break;   
		
	case 6:

        DrawMenu6();

        break;  
		
	case 7:

        DrawMenu7();

        break;  

    default:

        menucount = 1;
		dispint = 1; 

        break;       

                    }

SteamMgr(); //Call the steam management module
SmokeMgr(); //Call The Smoke management module
CircuMgr();    //Call The Circulation Management Module
InitiBBQ(); //User management module outputs to turn on/off heat etc
}


void SteamMgr()    {
if ( Internal_Top_Temp == 99){

        Internal_Top_Temp         = ReadTemp(Internal_Top_Pin);
        Internal_Bottom_Temp    = ReadTemp(Internal_Bottom_Pin);

        }

if ( ((Internal_Top_Temp + Internal_Bottom_Temp) / 2)> DesiredTemp) {

    Steam_Status = 1;
                                        }
else                                    {
    Steam_Status = 0;
                                        }
                }               

void SmokeMgr() { // Start Smoking if too little smoke in pot, how to detect, nobody knows!
//TBA Pending Smoke Sensor
Smoke_Status = 1;

}           

void CircuMgr() { //ciculate air if difference between top and bottom
if ((Internal_Top_Temp - Internal_Bottom_Temp) > Desired_Diff && (Internal_Top_Temp - Internal_Bottom_Temp) < (Desired_Diff * -1) ) {

    Circu_Status = 1;

                                        }
                                       
else                                    {
    Circu_Status = 0;
                                        }


}

void InitiBBQ() { //Shall I turn on heat and fans?
if (Steam_Status == 1 && ReadTemp(Steam_Temp_Pin) >= Desired_Steam_Blow) { //Turn on Steam Heat
    digitalWrite(Steam_Heat_Pin, HIGH);
                      }
else { digitalWrite(Steam_Heat_Pin, LOW);} //Turn off Steam Heat
                     
                     
if (Smoke_Status == 1 && ReadTemp(Smoke_Temp_Pin) >= Desired_Smoke_Blow) { //Turn on Smoke Heat
    digitalWrite(Smoke_Heat_Pin, HIGH);
                      }   
else { digitalWrite(Smoke_Heat_Pin, LOW);}    //Turn off smoke heat             

if (Circu_Status == 1) { //Circulate Internal Heat
    digitalWrite(Inter_Fan_Pin, HIGH);
                      }                    
else {digitalWrite(Inter_Fan_Pin, LOW);} //Turn off internal fan

}

void DrawMenu1() { // First Menu, containing Temperature Summary
		Internal_Top_Temp = ReadTemp(Internal_Top_Pin);
		Internal_Bottom_Temp = ReadTemp(Internal_Bottom_Pin);
		Smoke_Temp = ReadTemp(Smoke_Temp_Pin);
		Steam_Temp = ReadTemp(Steam_Temp_Pin);
	
if (dispint != 99) {//initialise display for this menu- set to always initialise for now
		clearLCD();
		delay(100);
		cursorHome();
		Serial.print("INT U:");
		Serial.print(Internal_Top_Temp);
		Serial.print("c");
		Serial.print("  L:");
		Serial.print(Internal_Bottom_Temp);
		Serial.print("c");
		cursorSet(1,0);
		Serial.print("SMOKE:");
		Serial.print(Smoke_Temp);
		Serial.print("c ST:");
		Serial.print(Steam_Temp);
		
		dispint++; //set initialise flag to 2
				  } //leave loop and begin to print temp and voltage fields
				  else{
	cursorSet(7,0);
	//backSpace(6);
	Serial.print(Internal_Top_Temp); //print external temperature
	Serial.print("c");
	cursorSet(14,0);
	//backSpace(6);
	Serial.print(Internal_Bottom_Temp); //print internal temperature
	Serial.print("c");}
}

void DrawMenu2() { // Second Status menu- displaying fan status
if (dispint != 99) {//initialise display for this menu- set to always initialise for now
		clearLCD();
		delay(100);
		cursorHome();
		Serial.print("FANS   STEAM:");
		if (Steam_Status == 1) {Serial.print("ON");} else {Serial.print("OFF");}
		cursorSet(0,1);
		Serial.print("SMOKE:");
		if (Smoke_Status == 1) {Serial.print("ON  ");} else {Serial.print("OFF ");}
		Serial.print("IN:");
		if (Circu_Status == 1) {Serial.print("ON");} else {Serial.print("OFF");}
		
		//dispint++; //set initialise flag to 2
				  } //leave loop and begin to print temp and voltage fields
				  else{ }




}

void DrawMenu3() { // Third Status menu- displaying heat status



}

void DrawMenu4() { // Forth menu- options- desired steam temperature



}

void DrawMenu5() { //Fifth menu- options - desired smoke temperature



}

void DrawMenu6() {//Sixth menu- options- desired internal temp variance



}

void DrawMenu7() {//Seventh menu- desired internal temperature



}
