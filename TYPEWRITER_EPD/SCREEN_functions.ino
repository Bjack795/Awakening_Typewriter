//SCREEN_functions.ino

void print_page(String pathtext)
{ 
  int posizione = YPAG*RES_WIDTH;
  int place;
  int mole;
  int deltaTime;
  int oldTime = millis();
  int formattazione = 0;
  int cursor_trigger = 1;
  int riga_lung = 0;
  int tasto = 1;
  int saving = -1;
  int contacar = 0;
  int mediacar [50];
  int mediacount = 0;
  int char_non_stampati = 0;
  int carmedia = 200;
  SAVE_TRIGGER = -1;
  copy_file(pathtext,PATHTEMP);
  ASCII_file(PATHTEMP, "/Settings/ASCII.txt");
  File texto = SD.open(PATHTEMP, FILE_READ);
  count_lines(texto); // LINEE e LINEE_LUNG
  char car;
  //paginaBianca();
  for (int i = 0;i<50;i++)
  {
    mediacar[i] = 0;
  }
  while (true)
  {
    /*for(int i = 0; i<RES_LINES;i++)
    {
      Serial.print("Linea ");Serial.print(i+1);Serial.print(" : ");
      Serial.println(LINEE[i]);
    }*/
    posizione = 0;
    for(int z = 0; z<YPAG;z++)
    {
      posizione = posizione + abs(LINEE[z]);  //mi pongo all'inizio della pagina
    }
    texto.seek(posizione); //mi posiziono all'inizio della pagina
    ASCII_file(PATHTEMP, "/Settings/ASCII.txt");
    //if (tasto == 1 && (deltaTime>carmedia || millis()-oldTime >carmedia))
    if (tasto == 1 && Tastiera.available() == 0)
    {
      Serial.print("Il serial.available: ");
      Serial.println(Tastiera.available());
      char_non_stampati = 0;
      contacar = posizione;
      display.setTextColor(GxEPD_BLACK);
      display.setFullWindow();
      display.fillScreen(GxEPD_WHITE);
      for(int j = 0; j<RES_LINES;j++)
      {
        for (int i = 0; i<RES_WIDTH;i++)
        {
          Webserver_loop();
          car = (char)(texto.read());
          mole = charWidth(car);
          if(i == XCUR and j == YCUR)
          {
            if(cursor_trigger > 0)
            {
              disegnaPicture(cursorImage,MARGINE_SX+riga_lung,MARGINE_UP+j*INGOMBRO+(j-1)*INTERLINEA-abs(MIN_Y1),1,abs(MIN_Y1));
            }
          }
          //ASCII 10 - line feed (una linea avanti) - il testo finisce con \r\n, salto l'"\r", poi scalo avanti ed esco dal ciclo
           if(car == '\n') 
           {   display.setCursor(MARGINE_SX+riga_lung, MARGINE_UP+j*INGOMBRO+(j-1)*INTERLINEA);
               //car = '|';     //termino la linea perchè sono andato a capo
               if (formattazione > 0)
               {
                display.print("/"); //car è già i e j
               }
               i = RES_WIDTH; //vado a capo
               riga_lung = 0;
            }
 
          //ASCII 13 - carriage return (vado ad inizio linea)
          else if(car != '\r') //è un carattere normale
          {
            display.setCursor(MARGINE_SX+riga_lung, MARGINE_UP+j*INGOMBRO+(j-1)*INTERLINEA);
            if((int)(car) == 32) {mole = 9*SPAZIATURA;}
            else if((int)(car) == 255 or (int)(car) == 0) {car = ' '; mole = -SPAZIATURA;}
            else if (CRYPTO == 1) {car = '*';}
            display.print(car);
            contacar++;
            riga_lung = riga_lung + mole + SPAZIATURA;                     
          }
          else // è '\r'
          {  
            i--;
          }
        
        }
        riga_lung = 0;
    }
    display.display(true);
    tasto = 0;
    }

    
  if(Tastiera.available() == 0)
    {
      Webserver_loop();
      
       if(FLAG_ESC == 1) //ho premuto ESC
       {     texto.close();
             saving = choose_page(2,SAVE_MENU);
             if(saving == 0 && SAVE_TRIGGER != -1)
             {
              copy_file(PATHTEMP,pathtext);
              char pathtext_char[pathtext.length()];
              pathtext.toCharArray(pathtext_char,pathtext.length()+1);
              cursor_file_save(pathtext_char);
              ASCII_file(PATHTEMP, "/Settings/ASCII.txt");
              display.setTextColor(GxEPD_BLACK);
              display.setFullWindow();
              display.fillScreen(GxEPD_WHITE);
              display.setCursor(display.width()/2-textWidth("SAVED!")/2,display.height()/2);
              display.print("SAVED!");
              display.display(true);
              delay(1000);
              SAVE_TRIGGER = -1;
             }
             CHOSEN = 1;
             LEVEL = LAST_LEVEL;
             FLAG_ESC = 0;             
             return;
       }
    }
    
    if (Tastiera.available() > 0) 
 {
      deltaTime = millis()-oldTime;
      oldTime = millis();
      Serial.print("Il tempo trascorso dall'ultimo tasto è: ");
      Serial.println(deltaTime);
      if(deltaTime<400) {deltaTime = 400;}
      char_non_stampati = char_non_stampati+1;
      //Serial.print("i caratteri nel buffer sono : ");
      //Serial.println(char_non_stampati);

    //if(deltaTime<5*carmedia && deltaTime> carmedia/3)
 
      if(mediacount<49) 
      {
        
             mediacar[mediacount] = deltaTime;
             mediacount++;
        
       }
      else
      {
        mediacount = 50;
        for (int nummed = 0; nummed<49;nummed++)
        {

             mediacar[nummed] = mediacar[nummed+1];
             
          
        }
        mediacar[49] = deltaTime;
      }
      carmedia = 0;
      for (int nummed = 0; nummed<mediacount;nummed++)
      {

             carmedia = carmedia + mediacar[nummed];
      }
      carmedia = carmedia/mediacount;
      Serial.print("la media è: ");
      Serial.println(carmedia);

//      Serial.print("il numero di caratteri è ");
//      Serial.println(contacar+1);
//      Serial.print("la media è ");
//      Serial.println(carmedia);
      tasto = 1;
      ANSWER = keyboard_loop();
       if (ANSWER == "[Up]") //arrow up
      { 
        if(YCUR>0)
        {
          YCUR--; 
          if(XCUR>abs(LINEE[YPAG+YCUR])+(-2-2*sgn(LINEE[YPAG+YCUR]))/2)
            {XCUR = abs(LINEE[YPAG+YCUR])+(-2-2*sgn(LINEE[YPAG+YCUR]))/2;}
        }
        else if(YPAG>0)
        {
          YPAG--;
          YCUR = 0;  
          if(XCUR>abs(LINEE[YPAG+YCUR])+(-2-2*sgn(LINEE[YPAG+YCUR]))/2)
            {XCUR = abs(LINEE[YPAG+YCUR])+(-2-2*sgn(LINEE[YPAG+YCUR]))/2;}
         }      
          FLAG_ESC = 0; 
        }
        
      else if (ANSWER == "[Down]") //arrow down
      { 
        if(YCUR<RES_LINES-1 and LINEE[YPAG+YCUR+1] != 0)
          {YCUR++;
           if(XCUR>abs(LINEE[YPAG+YCUR])+(-2-2*sgn(LINEE[YPAG+YCUR]))/2)
           {XCUR = abs(LINEE[YPAG+YCUR])+(-2-2*sgn(LINEE[YPAG+YCUR]))/2;} 
           }
        else if (abs(LINEE[YPAG+YCUR+1])+(-1-1*sgn(LINEE[YPAG+YCUR+1]))/2>0 and YPAG<MAXLINES and LINEE[YPAG+YCUR+1] != 0) 
        {
          YPAG++;
          YCUR = RES_LINES-1;  
         }              
        FLAG_ESC = 0; 
      }
      else if (ANSWER == "[Right]") 
      { 
        if(XCUR<abs(LINEE[YPAG+YCUR])+(-1-1*sgn(LINEE[YPAG+YCUR]))/2 -1)
          {XCUR++;}
        else
        { 
          if(YCUR<RES_LINES-1 and LINEE[YPAG+YCUR+1] != 0)
          {YCUR++; XCUR = 0;} 
          else if (abs(LINEE[YPAG+YCUR+1])+(-1-1*sgn(LINEE[YPAG+YCUR+1]))/2>0 and YPAG<MAXLINES and LINEE[YPAG+YCUR+1] != 0)
          {YPAG++; 
           YCUR = RES_LINES-1;  
           XCUR = 0;}        
        }
        FLAG_ESC = 0; }
      else if (ANSWER == "[Left]") 
      { if(XCUR>0){XCUR--;}
        else
        {
          if(YCUR>0){YCUR--; XCUR = abs(LINEE[YPAG+YCUR])+(-1-1*sgn(LINEE[YPAG+YCUR]))/2-1;}
          else if(YPAG>0)
          {YPAG--;
           YCUR = 0;  
           XCUR = abs(LINEE[YPAG+YCUR])+(-1-1*sgn(LINEE[YPAG+YCUR]))/2-1;} 
        }
        FLAG_ESC = 0; }
      else if (ANSWER == "[Esc]")
      { FLAG_ESC = 1; }
      else if (ANSWER == "[Enter]")
      { FLAG_ESC = 0;
        texto.close();
        place = posizione;
        for(int i = 0; i<YCUR;i++)
        {
          place = place + (abs(LINEE[YPAG+i]));
        }
        place = place+XCUR;
        inserisciCarattere(PATHTEMP,'\n', place);
        inserisciCarattere(PATHTEMP,'\r', place);
        XCUR = 0;
        texto = SD.open(PATHTEMP, FILE_READ);
        count_lines(texto);
        if(YCUR<RES_LINES-1){YCUR++;}
        else if (YPAG<MAXLINES)
        {YPAG++; YCUR = RES_LINES-1;}     
      }
      else if (ANSWER == "[Del]")
      { FLAG_ESC = 0; 
        texto.close();
        place = posizione;
        for(int i = 0; i<YCUR;i++)
        {
          place = place + (abs(LINEE[YPAG+i]));
        }
        place = place+XCUR;
        cancellaCarattere(PATHTEMP,place);
        ASCII_file(PATHTEMP, "/Settings/ASCII.txt");
        texto = SD.open(PATHTEMP, FILE_READ);
        count_lines(texto);
        }  
        
       else if (ANSWER == "Control-S")
      { FLAG_ESC = 0; 
        texto.close();
        copy_file(PATHTEMP,pathtext);
        char pathtext_char[pathtext.length()];
        pathtext.toCharArray(pathtext_char,pathtext.length()+1);
        cursor_file_save(pathtext_char);
        ASCII_file(PATHTEMP, "/Settings/ASCII.txt");
        display.setTextColor(GxEPD_BLACK);
        display.setFullWindow();
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(display.width()/2-textWidth("SAVED!")/2,display.height()/2);
        display.print("SAVED!");
        display.display(true);
        delay(1000);
        texto = SD.open(PATHTEMP, FILE_READ);
        count_lines(texto);
      }  
        else if (ANSWER == "Control-P")
      { FLAG_ESC = 0;
        CRYPTO = -CRYPTO;
      }
      else if (ANSWER == "Control-F")
      { FLAG_ESC = 0;
        formattazione = -formattazione;
      }
      else if (ANSWER == "Control-C")
      { FLAG_ESC = 0;
        cursor_trigger = -cursor_trigger;
      }
      else if (ANSWER == "Control-B")
      { FLAG_ESC = 0;
        paginaBianca();
      }        
       else if (ANSWER == "Backspace")
      { //FLAG_ESC = 0;
       if(XCUR>0 or YCUR+YPAG>0)
       {
        texto.close();
        place = posizione;
        for(int i = 0; i<YCUR;i++)
        {
          place = place + (abs(LINEE[YPAG+i]));
        }
        place = place+XCUR-1;
        cancellaCarattere(PATHTEMP,place);
        ASCII_file(PATHTEMP, "/Settings/ASCII.txt");
        texto = SD.open(PATHTEMP, FILE_READ);
        count_lines(texto);
        if(XCUR>0){XCUR--;}
        else
        {
          if(YCUR>0)
          {YCUR--; XCUR = abs(LINEE[YPAG+YCUR])+(-2-2*sgn(LINEE[YPAG+YCUR]))/2;} 
          else if(YPAG>0)
          {YPAG--;
           YCUR = 0;
           XCUR = abs(LINEE[YPAG+YCUR])+(-2-2*sgn(LINEE[YPAG+YCUR]))/2;} 
        }
        
       }
      }  
        
      else if ((int)(ANSWER[0]) != 10)
      {          
        FLAG_ESC = 0;
        count_lines(texto);
        texto.close();
        place = posizione;
        for(int i = 0; i<YCUR;i++)
        {
          place = place + (abs(LINEE[YPAG+i]));
        }
        place = place+XCUR;
        inserisciCarattere(PATHTEMP, ANSWER[0], place);
        if(XCUR<abs(LINEE[YPAG+YCUR])+(-1-1*sgn(LINEE[YPAG+YCUR]))/2 and XCUR<RES_WIDTH-1) //se XCUR è meno del limite************
        { XCUR++;}
        else
        { if(YCUR<RES_LINES-1){YCUR++; XCUR = 0;}
          else if (abs(LINEE[YPAG+YCUR+1])+(-1-1*sgn(LINEE[YPAG+YCUR]))/2>0 and YPAG<MAXLINES)
          {YPAG++;
           YCUR = RES_LINES-1; 
           XCUR = 0;}        
        }
        ASCII_file(PATHTEMP, "/Settings/ASCII.txt");
        texto = SD.open(PATHTEMP, FILE_READ);
        
        count_lines(texto);
      }
        
  }
  
  } 
  texto.close();
  
}

///////////////////////////////////////////////////////////////////////////////////////////////
