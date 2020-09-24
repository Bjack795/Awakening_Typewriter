//GUI_functions.ino

int choose_page(int num, char choices[][XRES-4*SPAZIATURA]) //choice menu 
{
  //paginaBianca();
  int arrow = 0;
  int ypage = 0;
  int FLAG_ESC = 0;
  int tasto = 1;
  int limite = _min(num,RES_LINES);
  int limite2 = _max(num,RES_LINES);
  CHOSEN = 0;
  while (CHOSEN == 0)
  {
    Webserver_loop();
  if (tasto == 1)
  {
  
      display.setTextColor(GxEPD_BLACK);
      display.setFullWindow();
      display.fillScreen(GxEPD_WHITE);
      for (int i = 0; i<limite ;i++)
      {
        display.setCursor(MARGINE_SX, i*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
        display.println(choices[i+ypage]);
      } 
  
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(XRES-MARGINE_DX-textWidth("<-"), arrow*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
  display.println("<-");
 
  if(LEVEL == 0)
  {
    display.setCursor(200, 5*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
    if (OK_WIFI == true)
    {
      display.println("ONLINE");
    }
    else
    {
      display.println("OFFLINE");
    }
    display.setCursor(100, 5*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
    voltaggio();
    display.print(voltage);
    display.println('v');
  }
   display.display(true); 
  tasto = 0;
  }
               
  if (Tastiera.available() > 0) {
      ANSWER = keyboard_loop();
      if (ANSWER == "[Enter]")
      {
        SAVE_TRIGGER = 0;
        CHOSEN = 1;
        break;
        
      }
      else if (ANSWER == "[Up]") //arrow up
      {
        tasto = 1;
        FLAG_ESC = 0;
        if(arrow > 0) {arrow--;}
        else if (ypage > 0) {ypage--;}
      }
      else if (ANSWER == "[Down]") //arrow down
      {
        tasto = 1;
        FLAG_ESC = 0;
        if(arrow < limite-1) {arrow++;}
        else if (ypage < limite2-RES_LINES) {ypage++;}
      }
      else if (ANSWER == "[Right]") 
      { FLAG_ESC = 0; }
      else if (ANSWER == "[Left]") 
      { FLAG_ESC = 0; }
      else if (ANSWER == "Control-B") 
      { FLAG_ESC = 0; paginaBianca(); tasto = 1;  }      
      
      else if (ANSWER == "[Esc]")
      {
         FLAG_ESC = 1;      
        
      }
  }
  else if(FLAG_ESC == 1)
        {SAVE_TRIGGER = -1; if(PREV_LEV>-1) {LEVEL = PREV_LEV;  break;   }  }
  }
  Serial.print("scelta:");
  Serial.println(ypage+arrow);
  return ypage+arrow;  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int open_file(String tpath,int* nem,char choices[][XRES-4*SPAZIATURA]) //choice of the file
{
  getDirectory(tpath, nem, choices);
  //paginaBianca();
  int num = *nem;
  int ypage = 0;
  int arrow = 0;
  int FLAG_ESC = 0;
  String pit;
  int tasto = 1;
  int limite = _min(num,RES_LINES);
  int limite2 = _max(num,RES_LINES);
  CHOSEN = 0;
  while (CHOSEN == 0)
  {
    Webserver_loop();
  if (tasto == 1)
  {
      display.setTextColor(GxEPD_BLACK);
      display.setFullWindow();
      display.fillScreen(GxEPD_WHITE);
      for (int i = 0; i<limite;i++)
      {
        display.setCursor(MARGINE_SX, i*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
        display.println(choices[i+ypage]);
      } 
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(XRES-MARGINE_DX-textWidth("<-"), arrow*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
    display.println("<-");
    display.display(true); 
    tasto = 0;
  }

  if (Tastiera.available() > 0) {
      ANSWER = keyboard_loop();
      if (ANSWER == "[Up]") //arrow up
      {
        tasto = 1;
        FLAG_ESC = 0;
        if(arrow > 0) {arrow--;}
        else if (ypage > 0) {ypage--;}
      }
      else if (ANSWER == "[Down]") //arrow down
      {
        tasto = 1;
        FLAG_ESC = 0;
        if(arrow < limite-1) {arrow++;}
        else if (ypage < limite2-RES_LINES) {ypage++;}
      }
      else if (ANSWER == "[Right]") 
      { FLAG_ESC = 0; }
      else if (ANSWER == "[Left]") 
      { FLAG_ESC = 0; }
      
      else if (ANSWER == "[Enter]")
      {
       
        if(LISTA[ypage+arrow][0]=='/')
        {
          PREV_LOC = tpath;
          pit = tpath+(String)(LISTA[ypage+arrow]);
          null_array(ArrayCount(LISTA), LISTA);
          LEVEL++;
          open_file(pit,&LENG,LISTA);
          return ypage+arrow;
        }

        else
        { LAST_LEVEL = LEVEL;
          LEVEL = 500;
          CHOSEN = 0;
          return ypage+arrow; }
        
      }
      else if (ANSWER == "[Esc]")
      {
         FLAG_ESC = 1;      
        
      }
      //lcd.clear();
  }
  else if(FLAG_ESC == 1)
        {
          if (LEVEL>100)
          { FLAG_ESC = 0;
            LEVEL--;
            null_array(ArrayCount(LISTA), LISTA);
            open_file(PREV_LOC,&LENG,LISTA);
            return ypage+arrow;
          }
                    
          else 
          { CHOSEN = 0;
            //if (LEVEL>0)  {LEVEL--;}
            LEVEL = 0;
            FLAG_ESC = 0;
            return ypage+arrow; }
        }
  }  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void getDirectory(String funpath, int* lung, char list [][XRES-4*SPAZIATURA]) //show the directory
{
File dir;
dir = SD.open(funpath);
dir.rewindDirectory();  
int files = 0;
int lex;
int slash;
while(true) {
Webserver_loop();
File entry = dir.openNextFile();
slash = 0;
if (! entry) {
// no more files
*lung = files;
dir.close();
break;
}

if (entry.isDirectory ())
{
  for (int j = strlen(entry.name())-1; j>-1; j--)
  {
    if(entry.name()[j]=='/')
    {
      slash = j;
      j = -1;
    }
  }
    if (strlen(entry.name())-slash < (XRES-2)) {lex = strlen(entry.name());}
    else                             {lex = slash+(XRES-2);}
     for(int i = slash; i<lex; i++)
   {
    list[files][i-(slash)] = entry.name()[i];
   }
    files++;
 }
 else
{
  for (int j = strlen(entry.name())-1; j>-1; j--)
  {
    if(entry.name()[j]=='/')
    {
      slash = j+1;
      j = -1;
    }
  }
  if (strlen(entry.name())-slash < (XRES-2)) {lex = strlen(entry.name());}
  else                             {lex = slash+(XRES-2);}
     for(int i = slash; i<lex; i++)
   {
    list[files][i-slash] = entry.name()[i];
         
   }
   
   
    files++;
 }


entry.close();
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*****************
int insert_name(char* entry) //make you insert the new file name
{
  XCUR = MARGINE_SX;
  YCUR = MARGINE_UP;
  FLAG_ESC = 0;
  bool ent = false;
  int rap = 0; 
  int tasto = 1;
  int index = 0;
  int mole;
  int riga_lung = 0;
  int magazzino [XRES-4*SPAZIATURA];
  int svuotabuffer = Tastiera.available();
  //paginaBianca();
  for(int i = 0; i<XRES-MARGINE_DX;i++)
  {
    entry[i] = ' ';
  }
  while(ent == false)
  {
    if(tasto == 1)
    {
      Serial.println(riga_lung);
      display.setTextColor(GxEPD_BLACK);
      display.setFullWindow();
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(MARGINE_SX,MARGINE_UP);
      display.println("Insert name:");
      XCUR = MARGINE_SX;
      if(rap == 0)
      {
        disegnaPicture(cursorImage,MARGINE_SX,MARGINE_UP+INGOMBRO+INTERLINEA-abs(MIN_Y1),1,abs(MIN_Y1));
      }
      for (int i = 0; i<rap;i++)
      {
        Serial.print("index: ");
        Serial.println(index);
        if(i == index-1)
        { Serial.println("EEY");
          disegnaPicture(cursorImage,XCUR + magazzino[i],MARGINE_UP+INGOMBRO+INTERLINEA-abs(MIN_Y1),1,17);
        }
        display.setCursor(XCUR,MARGINE_UP+INGOMBRO+INTERLINEA);
        display.println(entry[i]);
        XCUR = XCUR + magazzino[i]+SPAZIATURA;
        
      }
      
      display.display(true);
      tasto = 0;
    }
    
    while(Tastiera.available() == 0)
    {
      Webserver_loop();
       if(FLAG_ESC == 1)
       {    
          FLAG_ESC = 0;
          LEVEL = 0;
          ent = true;
          XCUR = MARGINE_SX;
          YCUR = MARGINE_UP;
          return rap;  
        }
    }

      if (Tastiera.available() > 0)
      {
        ANSWER = keyboard_loop();
        if (ANSWER == "[Up]") //arrow up
        { FLAG_ESC = 0; }
        else if (ANSWER == "[Down]") //arrow down
        { FLAG_ESC = 0; }
        else if (ANSWER == "[Left]") 
        { tasto = 1;
          if(index>0) {index--;}
          FLAG_ESC = 0; 
                }
        else if (ANSWER == "[Right]") 
        { 
          if(index<rap) {index++;}
          tasto = 1;
          FLAG_ESC = 0; 
                }
        else if (ANSWER == "[Esc]")
        { FLAG_ESC = 1;}
        else if (ANSWER == "[Enter]")
        { FLAG_ESC = 0;
          ent = true;
          XCUR = 0;
          YCUR = 0;
          LEVEL = 500;
          return rap;     
        }
        else if (ANSWER == "[Del]")
        { 
          tasto = 1;
          FLAG_ESC = 0;
          if (index<rap && rap>0) 
          {
           riga_lung = riga_lung-charWidth(entry[index])-SPAZIATURA;
          
          }
          for(int i = index; i<rap;i++)
          {
            entry[i] = entry[i+1];
          }
          entry[rap] = ' ';
          if (index<rap && rap>0) 
          {rap--;
           if(riga_lung < 0) {riga_lung = 0;}
          }
         }  
        else if (ANSWER == "Backspace")
        { 
          tasto = 1;
          if(index>0)
          {
            if (rap>0) 
          { 
            riga_lung = riga_lung-charWidth(entry[index-1])-SPAZIATURA;
            if(riga_lung < 0) {riga_lung = 0;}
            }     
          for(int i = index-1; i<rap-1;i++)
          {
            entry[i] = entry[i+1];
            if((int)(entry[i]) == 0)
            {
             
            }
          }
          entry[rap-1] = ' ';
          if (rap>0) 
          { rap--;
            if(riga_lung < 0) {riga_lung = 0;}
            index--;
            }          
          }
         }  
        
        else if ((int)(ANSWER[0]) != 10)
        {
          tasto = 1;
          FLAG_ESC = 0;
          mole = charWidth(ANSWER[0]);
          if((int)(ANSWER[0]) == 32) {mole = 9*SPAZIATURA;}
          if(riga_lung + mole + SPAZIATURA < XRES-MARGINE_DX-MARGINE_SX && rap < XRES-4*SPAZIATURA) 
          { 
            riga_lung = riga_lung + mole + SPAZIATURA;
            rap++;
            for(int i = index;i<rap;i++)
            {
              entry[i+1] = entry[i];
              magazzino[i+1] = magazzino[i];
            }
            entry[index] = ANSWER[0];
            magazzino[index] = mole;
            if(index<XRES-4*SPAZIATURA-1) {index++;}
            }
        }
         
    }
 
  

  }

  
}
///////////////////////////////////////******************************************************************** Margini

void marginFunction()
{
  char margini_stringhe [][XRES-4*SPAZIATURA] = {"DX:   ","SX:   ", "UP:   ","DOWN: ", "DEFAULT"};
  int margini_valori [4] = {MARGINE_DX, MARGINE_SX, MARGINE_UP, MARGINE_DOWN};
  int freccia = 0;
  int tasto = 1;
  CHOSEN = 0;

  while (CHOSEN == 0)
  {
    Webserver_loop();
    //Serial.println("MARGINI");
    if(tasto == 1)
    {
      display.setTextColor(GxEPD_BLACK);
      display.setFullWindow();
      display.fillScreen(GxEPD_WHITE);
   for (int i = 0; i<4; i++)
   {
      display.setCursor(MARGINE_SX,i*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
      display.print(margini_stringhe[i]);
      if (i == freccia) {display.print("<");} else {display.print(" ");} 
      display.print(margini_valori[i]);
      if (i == freccia) {display.print(">");}
   }

   display.setCursor(MARGINE_SX,5*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
   display.print(margini_stringhe[4]);
   if (4 == freccia) {display.print("<-");} 
   
   
   
      display.display(true);
        tasto = 0;
    }
    if (Tastiera.available() == 0) 
    {
        if(FLAG_ESC == 1) //ho premuto ESC
       {   
        int saving = choose_page(2,SAVE_MENU); 
        if (saving == 0)
          {
             
            MARGINE_DX = margini_valori[0];
            MARGINE_SX = margini_valori[1];
            MARGINE_UP = margini_valori[2];
            MARGINE_DOWN = margini_valori[3];
            EEPROM.put(0,MARGINE_DX);
            EEPROM.put(4,MARGINE_SX);
            EEPROM.put(8,MARGINE_UP);
            EEPROM.put(16,MARGINE_DOWN);
            EEPROM.end();
            
          }
        LEVEL = 10;
          
       
       }
    }

    if (Tastiera.available() > 0) 
    {
      tasto = 1;
      ANSWER = keyboard_loop();
        if (ANSWER == "[Up]") //arrow up
        { FLAG_ESC = 0; if(freccia>0) {freccia--;}     }
        else if (ANSWER == "[Down]") //arrow down
        { FLAG_ESC = 0; if(freccia<4) {freccia++;}    }
        else if (ANSWER == "[Left]") 
        { FLAG_ESC = 0; 
          if(margini_valori[freccia]>0){margini_valori[freccia]--;}
          }
        else if (ANSWER == "[Right]") 
        { FLAG_ESC = 0; 
          if(margini_valori[freccia]<YRES){margini_valori[freccia]++;}
                }
        else if (ANSWER == "[Enter]" && freccia == 4)
        {
          margini_valori[0] = MARGINE_DX_STD;
          margini_valori[1] = MARGINE_SX_STD;
          margini_valori[2] = MARGINE_UP_STD;
          margini_valori[3]= MARGINE_DOWN_STD;
        }
        else if (ANSWER == "[Esc]")
        { FLAG_ESC = 1;}

    }
    
  }
}


///////////////////////////////////////******************************************************************** WiFi
void wifiInfoFunction()
{
  int tasto = 1;
  CHOSEN = 0;
  Serial.println("WIFI");
  while (CHOSEN == 0)
  {
    Webserver_loop();
    if(tasto == 1)
    {
      display.setTextColor(GxEPD_BLACK);
      display.setFullWindow();
      display.fillScreen(GxEPD_WHITE);

      display.setCursor(MARGINE_SX,0*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
      display.print("Status: ");
      if (OK_WIFI == true)
      {
        display.print("ONLINE");
      }
      else
      {
        display.print("OFFLINE");
      }

      display.setCursor(MARGINE_SX,2*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
      display.print("WiFi: ");
      if (OK_WIFI == true)
      {
        display.print(CONNECTED_WIFI);
      }
      display.setCursor(MARGINE_SX,4*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
      display.print("IP: ");
      if (OK_WIFI == true)
      {
        display.print(PASSPORT_IP);
      }
      display.display(true);
        tasto = 0;
    }
    if (Tastiera.available() == 0) 
    {
        if(FLAG_ESC == 1) //ho premuto ESC
       {   
          CHOSEN = 1;
          LEVEL = PREV_LEV;
       }
    }

    if (Tastiera.available() > 0) 
    {
      tasto = 1;
      ANSWER = keyboard_loop();
        if (ANSWER == "[Up]") //arrow up
        { FLAG_ESC = 0;}
        else if (ANSWER == "[Down]") //arrow down
        { FLAG_ESC = 0; }
        else if (ANSWER == "[Left]") 
        { FLAG_ESC = 0; }
        else if (ANSWER == "[Right]") 
        { FLAG_ESC = 0; }
        else if (ANSWER == "[Esc]")
        { FLAG_ESC = 1;}
    }
    
  }
}

///////////////////////////////////////******************************************************************** Portal
void wifiPortalFunction()
{
  int inizio = 1;
  int enter = 0;
  CHOSEN = 0;
  Serial.println("PORTAL");
  while (CHOSEN == 0)
  {
    Webserver_loop();
    if(inizio == 1)
    {
      display.setTextColor(GxEPD_BLACK);
      display.setFullWindow();
      display.fillScreen(GxEPD_WHITE);

      display.setCursor(MARGINE_SX,0*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
      display.print("Press ENTER to start");
      display.setCursor(MARGINE_SX,2*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
      display.print("Press ESC to exit");
      
      display.display(true);
      inizio = 0;
    }

    if(enter == 1)
    {
      display.setTextColor(GxEPD_BLACK);
      display.setFullWindow();
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(MARGINE_SX,0*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
      display.print("Starting the portal...");
      display.setCursor(MARGINE_SX,1*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
      display.print("Closing in: ");
      display.print(TIMEOUT);
      display.print(" seconds");
      display.setCursor(MARGINE_SX,3*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
      display.print("Portal WiFi: ");
      display.print(SERVER_NAME);
      display.setCursor(MARGINE_SX,5*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
      display.print("Portal IP: ");
      display.print(WiFi.softAPIP());
      
      display.display(true);
      
      Serial.println("PORTALE");
      Serial.println(WiFi.softAPIP());
      Serial.println();
      Serial.println();
      wm.setEnableConfigPortal(true); 
      server.stop();
      server.close();
      OK_SERVER = false;
      WiFi.disconnect();
      wm.setConfigPortalTimeout(TIMEOUT);
      Webserver_portal(TIMEOUT);
      
      


    }


    
    if (Tastiera.available() == 0) 
    {
        if(FLAG_ESC == 1) //ho premuto ESC
       {   
          CHOSEN = 1;
          LEVEL = PREV_LEV;
       }
    }

    if (Tastiera.available() > 0) 
    {
      ANSWER = keyboard_loop();
        if (ANSWER == "[Up]") //arrow up
        { FLAG_ESC = 0;}
        else if (ANSWER == "[Down]") //arrow down
        { FLAG_ESC = 0; }
        else if (ANSWER == "[Left]") 
        { FLAG_ESC = 0; }
        else if (ANSWER == "[Right]") 
        { FLAG_ESC = 0; }
        else if (ANSWER == "[Enter]")
        {
          FLAG_ESC = 0;
          enter = 1;
        }
        
        else if (ANSWER == "[Esc]")
        { FLAG_ESC = 1;}
    }
    
  }
}
