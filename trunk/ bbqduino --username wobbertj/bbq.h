#include <OneWire.h> //include 1Wire Library

int Internal_Top_Pin     = 19; //Sensor in top of cooking chamber
int Internal_Bottom_Pin = 18; //Sensor in bottom of cooking chamber

int Smoke_Temp_Pin         = 17; //Smoke Sensor - to know when to blow smoke
int Steam_Temp_Pin        = 16; //Steam Sensor - to know when to blow steam

int Desired_Temp        = 0; //Initialise Default internal Temp to 0 (all off)
int Desired_Smoke        = 1; //Initialise smoke analog value to 1

int Smoke_Fan_Pin        = 15; //Smoke Fan Pin
int Steam_Fan_Pin        = 14; //Steam Fan Pin
int Inter_Fan_Pin        = 13; //Internal Fan Pin

int Smoke_Heat_Pin        = 12; //Smoke Heat
int Steam_Heat_Pin        = 11; //Steam Heat

int menucount           = 0 ; //display the first menu
int dispint             = 0 ; //initialise the display

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

    default:

        menureset();

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

void SmokeMgr() {
//TBA Pending Smoke Sensor
}            

void CircuMgr() {
if (Internal_Top_Temp - Internal_Bottom_Temp != 0 ) {

    Circu_Status = 1;

                                        }
                                        
else                                    {
    Circu_Status = 0;
                                        }


}

void InitiBBQ() {
if (Steam_Status = 1) { //Turn on Steam Heat
    digitalWrite(Steam_Heat_Pin, HIGH);
                      }
else { digitalWrite(Steam_Heat_Pin, LOW);} //Turn off Steam Heat
                      
                      
if (Smoke_Status = 1) { //Turn on Smoke Heat
    digitalWrite(Smoke_Heat_Pin, HIGH);
                      }    
else { digitalWrite(Smoke_Heat_Pin, LOW);}    //Turn off smoke heat              

if (Circu_Status = 1) { //Circulate Internal Heat
    digitalWrite(Inter_Fan_Pin, HIGH);
                      }                     
else {digitalWrite(Inter_Fan_Pin, LOW);} //Turn off internal fan

// So I have the heat on- when do I decide to start blowing the heat in?

}