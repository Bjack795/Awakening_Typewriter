//SD_functions.ino

//*******************************************
//file.position() POSITION OF THE CURSOR
//file.peek() READ THE BYTE WITHOUT ADVANCING
//file.size() READ THE SIZE OF THE FILE
void initializeSD()
{
  #ifdef ESP32
    // Note: SD_Card readers on the ESP32 will NOT work unless there is a pull-up on MISO, either do this or wire one on (1K to 4K7)
    Serial.println(MISO);
    pinMode(19,INPUT_PULLUP);
  #endif
  Serial.print(F("Initializing SD card...")); 
  if (!SD.begin(SD_CS_pin)) { // see if the card is present and can be initialised. Wemos SD-Card CS uses D8 
    Serial.println(F("Card failed or not present, no SD Card data logging possible..."));
    SD_present = false; 
    display.setTextColor(GxEPD_BLACK);
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(MARGINE_SX,0*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
    display.print("SD Card not found");
    display.setCursor(MARGINE_SX,2*(INGOMBRO+INTERLINEA)+ MARGINE_UP);
    display.print("Restarting...");
    display.display(true);
    delay(1000);
    ESP.restart();
  } 
  else
  {
    Serial.println(F("Card initialised... file access enabled..."));
    SD_present = true; 
  }
  // Note: Using the ESP32 and SD_Card readers requires a 1K to 4K7 pull-up to 3v3 on the MISO line, otherwise they do-not function.
  //----------------------------------------------------------------------   
}

///////////////////////////////////////////////////PRINT DIRECTORY
void printDirectory(File dir, int numTabs) {
int colcnt =0;
while(true) {
File entry = dir.openNextFile();
if (! entry) {
// no more files
break;
}
if (numTabs > 0) {
for (uint8_t i=0; i<=numTabs; i++) {
Serial.print('\t');
}
}
Serial.print(entry.name());
if (entry.isDirectory()) {
Serial.println("/");
printDirectory(entry, numTabs+1);
} else
{
// files have sizes, directories do not
Serial.print("\t");
Serial.println(entry.size(), DEC);
}
entry.close();
}
}
//////////////////////////////////////////////////////////////////////SOSTITUISCE IL CARATTERE IN CORRISPONDENZA DEL CURSORE

void subsCharacter(String path, char car, int sk )
{
  File testo;
  testo = SD.open(path, FILE_READ);
  int lung = testo.size();
  int rimanenti = lung-(sk+1);
  char resto [rimanenti];
  //riempio il buffer dei caratteri rimanenti
  for (int i = sk+1; i<rimanenti+sk+1;i++)
  {
    testo.seek(i);
    resto[i-(sk+1)] = (char)(testo.read());
  }
  testo.close();
  testo = SD.open(path, FILE_WRITE);
  testo.seek(sk);
  testo.write(car);
  for (int j = sk+1; j<rimanenti+sk+1;j++)
  {
    testo.seek(j);
    testo.write(resto[j-(sk+1)]);
  }
  testo.close();
  
}

//////////////////////////////////////////////////////////////////////INSERISCE IL CARATTERE IN CORRISPONDENZA DEL CURSORE
////////////////////////////////////////////////////////se sto agendo su una riga devo cancellare la fine di quella riga ed aggiungere uno \n alla fine della riga
void inserisciCarattere(String path, char car, int sk )
{
  File testo;
  testo = SD.open(path, FILE_READ);
  int lung = testo.size();
  int rimanenti = lung-(sk+1);
  char resto [rimanenti];
  //riempio il buffer dei caratteri rimanenti
  for (int i = sk; i<rimanenti+sk+1;i++)
  {
    testo.seek(i);
    resto[i-sk] = (char)(testo.read());
  }
  testo.close();
  
  testo = SD.open(path, FILE_WRITE);
  for (int i = 0;i<sk;i++) //riempio i caratteri prima nulli o vuoti con degli spazi
  {
    testo.seek(i);
    if ((int)(testo.read()) == 0 or (int)(testo.read()) == 255)
    { testo.seek(i);
      testo.write(' ');}   
  }
  testo.seek(sk);
  testo.write(car);
  for (int j = sk+1; j<rimanenti+sk+2;j++)
  {
    testo.seek(j);
    testo.write(resto[j-(sk+1)]);
  }
  testo.close();
  
}

//////////////////////////////////////////////////////////////////////CANCELLA IL CARATTERE IN CORRISPONDENZA DEL CURSORE

void cancellaCarattere(String path, int pos_canc )
{
  File testo;
  testo = SD.open(path, FILE_READ);
  int lung = testo.size();
  int car;
  testo.seek(pos_canc);
  car = testo.read();
  if(car == 10)
  {
    pos_canc--;
  }
  int rimanenti = lung-(pos_canc+1); //tutti quelli dopo il carattere da cancellare
  int skip;
  int phase = 0;
  if (rimanenti>1)  ////////////////occhio
  {
    skip = 0;
  }
  else
  {
    rimanenti = 1;
    skip = 1;
  }
  char resto [rimanenti];
  if (skip == 0)
  {
  //riempio il buffer dei caratteri rimanenti
  for (int i = pos_canc+1; i<lung;i++)
  {
    testo.seek(i);
    car = testo.read();
    if (i == pos_canc+1 and car == 10)
    {
      rimanenti--;
      phase--;
    }
    else
    { resto[i-(pos_canc+1) + phase] = (char)(car); }
    
  }
  testo.close();
  phase = 0;
  testo = SD.open(path, FILE_WRITE);
  for (int j = pos_canc; j<rimanenti+pos_canc;j++)
  {
    testo.seek(j);
    testo.write(resto[j-pos_canc]);
  }
  testo.close();
  }
  else
  {
   //resto[0] = ' ';
   testo.close();
  }
   
  
  
}
//////////////////////////////////////////////////////////////////////////STAMPA IL FILE DA/A

void stampaFile(String path, File testo, int inizio, int fine)
{ /*
  //Serial.println("dentro");
    testo = SD.open(path,FILE_READ);
    int lung = testo.size();
    if (inizio<0)
    {
    inizio = 0;
    }
    if (fine<0)
    {
    fine = lung;
    }
    for (int i = inizio;i<(inizio+fine);i++)
    {
      testo.seek(i);
      lcd.setCursor(XCUR,0);
      lcd.print((char)(testo.read()));
      Serial.print((char)(testo.read()));      
          }
          testo.close();
*/
}

////////////////////////////////////////////////////////////////////RIVERSA UN FILE IN UN ALTRO

void copy_file(String pathold, String pathnew)
{
  File textold = SD.open(pathold, FILE_READ);
  File textnew = SD.open(pathnew, FILE_WRITE);
  int lung = textold.size();
  int phase = 0;
  int car;
  int cir;
  char wes;
  int clin = 0, flin = 0, itemp;
  for (int i = 0;i<lung;i++)
    {
      textold.seek(i);
      car = textold.read();
      textnew.seek(i+phase);
      textnew.write(car); 
      if(car == 10)
      {
        textold.seek(i-1);
        if(textold.read()!=13)
      {
        textnew.seek(i+phase);
        textnew.write(13);
        textold.seek(i+phase+1);
        textnew.write(10);
        phase++;
      }
      } 
    }
  textold.seek(lung-2);
  if(textold.read() != 13) {textnew.write(13);}
  textold.seek(lung-1);
  if(textold.read() != 10) {textnew.write(10);}
  textold.close();
  textnew.close();
}
/////////////////////////////////////////////////////////////LUNGHEZZE DELLE RIGHE DEL TESTO
//////manca qualcosa nel conteggio
void count_lines(File wt)
{
  int j = 0;
  int k = 0;
  int lung = wt.size();
  int posizione = 0;
  char wer;
  for (int i = 0;i<MAXLINES;i++)
  {
    LINEE[i] = 0;
  }
  for (int i = 0;i<lung;i++)
    {
      wt.seek(i);
      wer = wt.read();   
      k++; 
      if ((int)(wer) == 13)
    {
      if (k>0)
      { LINEE[j] = k+1;  }
      else
      { LINEE[j] = 0;    }
      i++;
      k = 0;
      j++;
    }
        
    else if (k>(RES_WIDTH-1))
    {
      if (k>0)
      { LINEE[j] = -k;  }
      else
      { LINEE[j] = 0;    }
        k = 0;
        j++; 
    }
     }
  if (k<RES_WIDTH)  {LINEE[j] = k;}
  else {LINEE[j] = RES_WIDTH-1;}
  LINEE_LUNG = j+1;
  
  for(int i = 0; i<YPAG+YCUR;i++)
  {
    if(LINEE[i]==0)
    { 
      posizione = 0;
      for(int z = 0; z<i;z++)
      {
        posizione = posizione + LINEE[z]+1;
      }
      //Serial.println("Riga a zero");
      //Serial.println(i);
      for (int j = posizione; j<posizione+(RES_WIDTH-1);j++)
      {
        subsCharacter(PATHTEMP, ' ', j);
      }
      subsCharacter(PATHTEMP, '\n', posizione+(RES_WIDTH-1));
      LINEE[i]=(RES_WIDTH-1);
    }
  }
  /*Serial.println("Linea");
  Serial.println(LINEE[2]);*/
  /*Serial.println("Linee");
  for(int i = 0; i<LINEE_LUNG;i++)
  {
    Serial.println(LINEE[i]);
  }*/
}
///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////RIVERSA UN FILE IN UN ALTRO (IN INTERI)

void ASCII_file(String pathold, String pathnew)
{
  File textold = SD.open(pathold, FILE_READ);
  File textnew = SD.open(pathnew, FILE_WRITE);
  int lung = textold.size();
  char wes;
  int count = 0;
  for (int i = 0;i<lung;i++)
    {
      textold.seek(i);
      wes = textold.read();
      textnew.write('('); 
      textnew.print((int)(wes)); 
      textnew.write(')');  
      count++;
      if(wes == '\n') {textnew.println(); count = 0;} 
      else if(count>RES_WIDTH-1){textnew.println(); count = 0;}
     }
  textold.close();
  textnew.close();
}
///////////////////////////////////////////////////////////////////////////Cursor saving
void cursor_file_save(char* pathsave)
{
  File textcursor = SD.open(PATHCURSOR, FILE_READ);
  int lung_cursor = textcursor.size();
  int lung_save = strlen(pathsave);
  char wes;
  int was;
  char allthatstuff [lung_cursor];
  int found = 0;
  int count = 0;
  int slash = 0;

  for (int j = strlen(pathsave)-1; j>-1; j--)
  {
    if(pathsave[j]=='/')
    {
      slash = j+1;
      break;
    }
  }
  char pathsave2[strlen(pathsave)-slash];
  for(int i = slash; i<strlen(pathsave); i++)
   { pathsave2[i-slash] = pathsave[i];}
  for (int j = 0; j<lung_cursor;j++)
  {
    textcursor.seek(j);
    wes = (char)(textcursor.read());
    allthatstuff[j] = wes;
    if ( wes == pathsave2[count])
    {
      count++;      
    }
    else
    {
      count = 0;
    }
    if (count == sizeof(pathsave2))
    {
      found = j+1;
      break;
    }
  } 
  textcursor.close();
  textcursor = SD.open(PATHCURSOR, FILE_WRITE);
  for (int i = 0; i<lung_cursor;i++)
  {
    textcursor.seek(i);
    textcursor.print(allthatstuff[i]);    
  }
 if(found > 0)
 {
    textcursor.seek(found);
    textcursor.write(44);
    textcursor.print(YPAG);
    textcursor.write(44);
    textcursor.print(XCUR);
    textcursor.write(44);
    textcursor.print(YCUR);
    textcursor.write(44);
    textcursor.println();
    
 }
 else
 {
    textcursor.seek(lung_cursor);
    for(int i = 0; i<sizeof(pathsave2); i++)
    {textcursor.print(pathsave2[i]); }
    textcursor.write(44);
    textcursor.print(YPAG);
    textcursor.write(44);
    textcursor.print(XCUR);
    textcursor.write(44);
    textcursor.print(YCUR);
    textcursor.write(44);
    textcursor.println();
    
 }
      
  textcursor.close();
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*****************************************************************************************************************************************************
void cursor_file_read(char* pathsave)
{
  File textcursor = SD.open(PATHCURSOR, FILE_READ);
  int lung_cursor = textcursor.size();
  int lung_save = strlen(pathsave);
  char wes;
  int was;
  char allthatstuff [lung_cursor];
  int found = 0;
  int count = 0;
  int slash = 0;
  for (int j = strlen(pathsave)-1; j>-1; j--)
  {
    if(pathsave[j]=='/')
    {
      slash = j+1;
      break;
    }
  }
  char pathsave2[strlen(pathsave)-slash];
  for(int i = slash; i<strlen(pathsave); i++)
   { pathsave2[i-slash] = pathsave[i];}
  for (int j = 0; j<lung_cursor;j++)
  {
    textcursor.seek(j);
    wes = (char)(textcursor.read());
    allthatstuff[j] = wes;
    if ( wes == pathsave2[count])
    {
      count++;      
    }
    else
    {
      count = 0;
    }
    if (count == sizeof(pathsave2))
    {
      found = j+1;
      break;
    }
  }  
  if(found>0)
  {
    textcursor.seek(found);
    YPAG = textcursor.parseInt();
    XCUR = textcursor.parseInt();
    YCUR = textcursor.parseInt();
    textcursor.close();
    return;
  }
  else
  {
    YPAG = 0;
    XCUR = 0;
    YCUR = 0;
    textcursor.close();
    cursor_file_save(pathsave);
    return;
  }
  

   
  
  
  
}
















