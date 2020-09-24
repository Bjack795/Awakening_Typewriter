int16_t charY1(const char ch)
{
  char onecharstring[2] = {ch, 0};
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(onecharstring, 0, 0, &x1, &y1, &w, &h);
  return y1;
}

///////////////////////////////////////////////////////////////////////////////////////////

int16_t charHeight(const char ch)
{
  char onecharstring[2] = {ch, 0};
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(onecharstring, 0, 0, &x1, &y1, &w, &h);
  return h+y1;
}

///////////////////////////////////////////////////////////////////////////////////////////

void maxminHeight()
{
  int heigh = 0;
  for(int i = 0;i<128;i++)
  {
    heigh = charHeight((char)(i));
    if (heigh > MAX_HEIGHT)
    {
      MAX_HEIGHT = heigh;     
    }
  }
  Serial.print("Il massimo dell'altezza+y1 è: ");
  Serial.println(MAX_HEIGHT);

  heigh = 0;
  for(int i = 0;i<128;i++)
  {
    heigh = charY1((char)(i));
    if (heigh < MIN_Y1)
    {
      MIN_Y1 = heigh;   
    }
  }
  Serial.print("Il minimo dell'y1 è: ");
  Serial.println(MIN_Y1);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t charWidth(const char ch)
{
  char onecharstring[2] = {ch, 0};
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(onecharstring, 0, 0, &x1, &y1, &w, &h);
  return w+x1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t textWidth(const char* text)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  return w; //devo sommare x1?
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void maxCharWidth()
{
  int widt = 0;
  for(int i = 0;i<128;i++)
  {
    widt = charWidth((char)(i));
    if (widt > MAX_WIDTH)
    {
      MAX_WIDTH = widt;     
    }
  }
  Serial.print("Il massimo della larghezza è: ");
  Serial.println(MAX_WIDTH);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void paginaBianca()
{
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.display();
  /*display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
  }
  while (display.nextPage());*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void scrivichar(char car,uint16_t x,uint16_t y)
{
  uint16_t box_w = charWidth(car);
  uint16_t box_h = MAX_HEIGHT+abs(MIN_Y1);
  uint16_t box_x = XCUR;
  uint16_t box_y = YCUR;
  uint16_t cursor_y = box_y + box_h-MAX_HEIGHT;
  //display.setFont(&FreeMonoBold9pt7b);
  
  if((int)(car) == 32) //spacebar
   {      
          XCUR = XCUR + SPAZIATURA;
   }

   else
   {
      display.setTextColor(GxEPD_BLACK);
      display.setCursor(x,y);
      display.print(car);
      display.display(true);
      XCUR = XCUR + box_w; //go on with the cursor
    }
  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void scriviString(String scritta,uint16_t x,uint16_t y)
{
  //scritta = "Awakening Typewriter";
  //display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  display.setCursor(x, y);
  display.println(scritta);
  display.display(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void disegnaWallpaper(const unsigned char* immagine)
{
  /*const unsigned char* immagine =
  {
    sabo2 //nome array
  };*/
    display.setRotation(0);
    bool m = display.mirror(true);
    display.fillScreen(GxEPD_WHITE);
    display.drawInvertedBitmap(0, 0, immagine, display.epd2.WIDTH, display.epd2.HEIGHT, GxEPD_BLACK);
    display.display(true);
    display.mirror(m);
    display.setRotation(1);
  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void disegnaPicture(const unsigned char* immagine,uint16_t x,uint16_t y,uint16_t w,uint16_t h)
{
    //bool m = display.mirror(true);
    //display.fillScreen(GxEPD_WHITE);
    display.drawInvertedBitmap(x, y, immagine, w, h, GxEPD_BLACK);
    //display.display(true);      
    //display.mirror(m);
  
}


