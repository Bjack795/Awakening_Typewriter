//TYPEWRITER.ino
#include <SD.h>
#include <EEPROM.h>
#define ArrayCount(array) (sizeof array / sizeof array[0])
//ArrayCount(myarray) --> rows
//ArrayCount(myarray[0]) --> rows
/////////////////////////////////////////////////////////////////////////////////////////////
  #include <WiFiManager.h> 
////////////////////////////////////////////////////////////////////////
  #include <WiFi.h>              // Built-in
  #include <WebServer.h>
  #include <ESPmDNS.h>
  #include "FS.h"
  #include "Sys_Variables.h"
  #include "CSS.h"
  #include <SD.h> 
  #include <SPI.h>
  WebServer server(80);
void Webserver_setup();
void Webserver_loop();
void copy_file(String pathold, String pathnew);
void cursor_file(String pathsave);
WiFiManager wm;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PIN_RX 33
#define PIN_TX 25
//devono essere pin digitali
int FLAG_ESC=0;
HardwareSerial Tastiera(1);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////SCREEN
#define ENABLE_GxEPD2_GFX 0
#define A_PIN 32
#define RES_PIN 34
#define PIN_RX 33
#define PIN_TX 25
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "DATABASE_bitmaps.h" 
#define XRES 296
#define YRES 128
#define SPAZIATURA    1
#define INTERLINEA    3
#define MARGINE_DX_STD    10*SPAZIATURA
#define MARGINE_SX_STD    10*SPAZIATURA
#define MARGINE_UP_STD    6*INTERLINEA
#define MARGINE_DOWN_STD  6*INTERLINEA
#define TIMEOUT 100

GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=5*/ 5, /*DC=*/ 13, /*RST=*/ 14, /*BUSY=*/ 15));

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String ANSWER;
File TEMPFILE;
File NEWFILE;
String PATHTEMP = "/Settings/temp.txt";
String PATHCURSOR = "/Settings/cursor_file.txt";
String PATHTESTI = "/Testi";
String PATHSETTINGS = "/Settings";
String CONNECTED_WIFI;
String PASSPORT_IP;
const char SERVER_NAME[] = "BjackWiFi";
//bool RESET_WIFI = false;
bool OK_WIFI = false;
bool OK_SERVER = false;
bool INSIDE = false; //check if I can search for wifi
float voltage;
int SAVE_TRIGGER = -1;
/////////////////////////////////////////////////
#define MAXLINES 10000
int MAX_HEIGHT = 0;
int MAX_WIDTH = 0;
int MIN_Y1 = 0;
int INGOMBRO;
int RES_LINES;
int RES_WIDTH;
int MARGINE_DX = MARGINE_DX_STD;
int MARGINE_SX = MARGINE_SX_STD;
int MARGINE_UP = MARGINE_UP_STD;
int MARGINE_DOWN = MARGINE_DOWN_STD;
int MARGINE_PROVA;
int LUCE = 1;
int YPAG = 0, XCUR = 0, YCUR = 0;
/////////////////////////////////////////////////
int LEVEL = -8;
int PREV_LEV = -8;
char MAIN_MENU [][XRES-4*SPAZIATURA] = {"Open","New", "Settings","Restart"};
char SETTINGS_MENU [][XRES-4*SPAZIATURA] = {"Margins","Wifi"};
char SAVE_MENU [][XRES-4*SPAZIATURA] = {"Save","Don't save"};
char WIFI_MENU [][XRES-4*SPAZIATURA] = {"Info","Portal"};
int CHOSEN = 0;
int OPTION;

//////////////
char LISTA [50][XRES-4*SPAZIATURA];
int LENG = 0;
int TITOLO;
int LAST_LEVEL;
int CRYPTO = -1;
String PREV_LOC;
int LINEE [MAXLINES];
int LINEE_LUNG = 0;
String TEXT_NAME;
bool IS_OLD;
int LENG_NEW = 0;
char entry_1[XRES-4*SPAZIATURA];
/////////////////////////////

void setup() {
  Serial.begin(115200);
  EEPROM.begin(32);
  pinMode(35,INPUT);
//  esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, 0);
//  print_wakeup_reason();
  btStop();
  Tastiera.begin(9600,SERIAL_8N1, PIN_RX, PIN_TX);
  display.init(115200);
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  maxminHeight(); 
  maxCharWidth(); 
  INGOMBRO = MAX_HEIGHT+abs(MIN_Y1);
  RES_LINES = (YRES-MARGINE_UP+INTERLINEA)/(INGOMBRO+INTERLINEA)+1;
  RES_WIDTH = (XRES-MARGINE_SX+SPAZIATURA)/(MAX_WIDTH+SPAZIATURA);
  //attachInterrupt(digitalPinToInterrupt(36),sleepFunction, RISING);
  EEPROM.get(0,MARGINE_PROVA);
  if(MARGINE_PROVA>4)
  {
    EEPROM.get(0,MARGINE_DX);
    EEPROM.get(4,MARGINE_SX);
    EEPROM.get(8,MARGINE_UP);
    EEPROM.get(16,MARGINE_DOWN);
  }
  Serial.println(RES_WIDTH);
  delay(50);
  initializeSD();
  for (int i = 0; i<2; i++)
  {
    Webserver_autoconnect(2);
    if(OK_WIFI == true) break;
  }
  ledcAttachPin(A_PIN, 1);
  ledcSetup(1, 12000, 8);
  //pinMode(RES_PIN, OUTPUT);
  paginaBianca();
  if(!SD.exists(PATHSETTINGS))
  {
    SD.mkdir(PATHSETTINGS);

  }
   if(!SD.exists(PATHTESTI))
  {
    SD.mkdir(PATHTESTI);
  }
   if(!SD.exists(PATHCURSOR))
  {
    TEMPFILE = SD.open(PATHCURSOR, FILE_WRITE);
    TEMPFILE.close();
  }
 voltaggio();
 delay(50);
}

void loop() {

  //***************************************************************** Level WALPAPER (-8)
  if(LEVEL == -8)
  {
    PREV_LEV = -8; 
    disegnaWallpaper(screen1);
    while(LEVEL == -8)
    {
    ANSWER = keyboard_loop();
    if(ANSWER == "[Enter]")
    {
      LEVEL = 0;
    }
    Webserver_loop();
    ledcWrite(1, (120+LUCE*120));
    }
  }
  Webserver_loop();
  ledcWrite(1, (120+LUCE*120));  
  
  //*************************************************************** Level MAIN MENU (0)
  if (LEVEL == 0) //In the main menu
  { PREV_LEV = -8; 
    OPTION = choose_page(ArrayCount(MAIN_MENU), MAIN_MENU); 
    if(CHOSEN == 1)
    {
    if(OPTION == 0)
    {LEVEL = 100;} //choose the file
    else if(OPTION == 1)
    {LEVEL = 300;} //write the name of the new file
    else if(OPTION == 2)
    {LEVEL = 10;} //settings
    else if(OPTION == 3)
    {
      display.setTextColor(GxEPD_BLACK);
      display.setFullWindow();
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(MARGINE_SX,0*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
      display.print("Restarting...");
      display.display(true);
      delay(1000);
      ESP.restart();
    } //restart
    }
  }

  //**************************************************************** Level CHOICE OF THE FILE (100-199)
  else if (LEVEL > 99 and LEVEL <200) //choice of the file
  { PREV_LEV = 0; 
    if (CHOSEN == 1)
    {
      if((String)(MAIN_MENU[OPTION]) == "Open")
      { TITOLO = open_file("/Testi",&LENG,LISTA);
      }
        IS_OLD = true; //I'm opening an old file
     }
  }
  
  //**************************************************************** Level NEW FILE (300)
  else if (LEVEL == 300) //I have to create a new file
    {
      PREV_LEV = 0; 
      LENG_NEW = insert_name(entry_1); 
      IS_OLD = false;
    }
    
 //***************************************************************** Level TEXT (500)
  else if (LEVEL == 500) //inside a text
  {  if(IS_OLD == true) //I'm opening an existent file
      {
        TEXT_NAME = "/Testi/"+(String)(LISTA[TITOLO]);
        char text_name_char[TEXT_NAME.length()];
        TEXT_NAME.toCharArray(text_name_char,TEXT_NAME.length()+1); //string to char array
        cursor_file_read(text_name_char);
      }
    else //new file
    { 
      String entry_2; //char to string
      for (int i = 0; i<LENG_NEW;i++)
      {
        if((int)(entry_1[i])== 32) { entry_1[i] = (char)(95);}
        entry_2 +=entry_1[i];        
      }     
        TEXT_NAME = "/Testi/"+ entry_2 +".txt";
        Serial.println(TEXT_NAME);
        char text_name_char[TEXT_NAME.length()];
        TEXT_NAME.toCharArray(text_name_char,TEXT_NAME.length()+1); //string to char
        cursor_file_save(text_name_char);
        NEWFILE = SD.open(TEXT_NAME, FILE_WRITE);
        NEWFILE.close();
        LAST_LEVEL = 0;
        XCUR = 0;
        YCUR = 0;
        YPAG = 0;
      
    }    
      print_page(TEXT_NAME);  
  }
  
   //***************************************************************** Level SETTINGS (10)
  else if (LEVEL == 10)
  {
    PREV_LEV = 0;
    OPTION = choose_page(ArrayCount(SETTINGS_MENU), SETTINGS_MENU); 
    if(CHOSEN == 1)
    {
    if(OPTION == 0)
    {PREV_LEV = LEVEL; LEVEL = 11;   } //margins
    else if(OPTION == 1)
    {PREV_LEV = LEVEL; LEVEL = 12; } //wifi
    }   
  }
  //***************************************************************** Level MARGINS (11)
  else if (LEVEL == 11)
  {
    PREV_LEV = 10;
    marginFunction();
  }
   //***************************************************************** Level WIFI (12)
  else if (LEVEL == 12)
  {
    PREV_LEV = 10;
    OPTION = choose_page(ArrayCount(WIFI_MENU), WIFI_MENU); 
    if(CHOSEN == 1)
    {
    if(OPTION == 0)
    {PREV_LEV = 12; LEVEL = 13;   } //margins
    else if(OPTION == 1)
    {PREV_LEV = 12; LEVEL = 14;   } //wifi
    }   
  }
    //***************************************************************** Level INFO (13)
  else if (LEVEL == 13)
  {
    wifiInfoFunction();
    
  }
   //***************************************************************** Level PORTAL (14)
  else if (LEVEL == 14)
  {
    wifiPortalFunction();
    
  }
  


}
