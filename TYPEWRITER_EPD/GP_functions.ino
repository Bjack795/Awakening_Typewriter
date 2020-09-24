//GP_functions.ino

void svuotaSeriale() {
  for (int i = 0;i<200;i++)
  {
      Serial.println();
    }
}
////////////////////////
void null_array(int num, char list [][XRES-4*SPAZIATURA]) //all array to null values 
{
  for (int i = 0; i < num; i++)
  {
    for (int j = 0; j < ArrayCount(list[0]);j++)
    {
      list [i][j] = NULL;
      
    }
  }
}
///////////////////////////
int sgn(double value)  //function giving the sign of a double
{
  if (value < 0) return -1;
  if (value > 0) return 1;
  return 1;
}
/////////////////////////////
void voltaggio()
{
  voltage = (float)analogRead(35)/4096*3.3;
  //4 è il minimo voltaggio
  voltage = voltage*2.2;
  Serial.print(voltage,1);
  Serial.println("v");
}
/////////////////////////////
void sleepFunction()
{
  Serial.println("premuto");
  esp_deep_sleep_start();
  
}
////////////////////////////
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
  delay(500);
}
