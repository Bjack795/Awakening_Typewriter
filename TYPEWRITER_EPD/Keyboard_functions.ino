//Keyboard_functions.ino

String keyboard_loop() {
  String risposta;
  
    int inByte = Tastiera.read();  
    if (inByte != -1)
    {Serial.println(inByte); }
    
    if(inByte == 27)
    {
       FLAG_ESC = 1;
       risposta = ("[Esc]");
    }   
    else
    {
      if(FLAG_ESC==1)
      {
        // Previous char was ESC - Decode all the escaped keys
        switch(inByte)
        {
            case 0x49:
              risposta = ("[PgUp]");
              break;
            case 0x51:
              risposta = ("[PgDn]");
              break;    
            case 0x47:
              risposta = ("[Home]");
              break;
            case 0x4F:
              risposta = ("[End]");
              break;     
            case 0x52:
              risposta = ("[Ins]");
              break;
            case 0x53:
              risposta = ("[Del]");
              break;               
            case 0x3B:
              risposta = ("[F1]");
              break;
            case 0x3C:
              risposta = ("[F2]");
              break;    
            case 0x3D:
              risposta = ("[F3]");
              break;
            case 0x3E:
              risposta = ("[F4]");
              break;     
            case 0x3F:
              risposta = ("[F5]");
              break;
            case 0x40:
              risposta = ("[F6]");
              break;          
            case 0x41:
              risposta = ("[F7]");
              break; 
            case 0x42:
              risposta = ("[F8]");
              break; 
            case 0x43:
              risposta = ("[F9]");
              break; 
            case 0x44:
              risposta = ("[F10]");
              break; 
            case 0x57:
              risposta = ("[F11]");
              break; 
            case 0x58:
              risposta = ("[F12]");
              break;     
            case 0x48:
              risposta = ("[Up]");
              break; 
            case 0x50:
              risposta = ("[Down]");
              break; 
            case 0x4B:
              risposta = ("[Left]");
              break; 
            case 0x4D:
              risposta = ("[Right]");
              break;         
            case 0x54:
              risposta = ("[Print]");
              break; 
            case 0x5B:
              risposta = ("[Windows]");
              break;  
            case 13:
              risposta = ("[Enter]");
              break;   
            default:
              risposta = ("[?]");
              break;            
        }
        FLAG_ESC=0;    
      }
      else
      {  

        if(inByte==1)
        {
           risposta = ("Control-A");
        }  
        else if(inByte==2)
        {
           risposta = ("Control-B");
        }
        else if(inByte==3)
        {
           risposta = ("Control-C");
        }
        else if(inByte==4)
        {
           risposta = ("Control-D");
        }
        else if(inByte==5)
        {
           risposta = ("Control-E");
        }
        else if(inByte==6)
        {
           risposta = ("Control-F");
        }
        else if(inByte==7)
        {
           risposta = ("Control-G");
        }
        // Dont decode 8 - backspace
        else if(inByte==8)
        {
           risposta = ("Backspace");
        }
        else if(inByte==9)
        {
           risposta = ("Tab");
        }
        // Dont decode 10 - Line Feed
        else if(inByte==11)
        {
           risposta = ("Control-K");
        }
        else if(inByte==12)
        {
           risposta = ("Control-L");
        }
       else if(inByte==13)
        {
           risposta = ("[Enter]");
        }
        else if(inByte==14)
        {
           risposta = ("Control-N");
        }
        else if(inByte==15)
        {
           risposta = ("Control-O");
        }
        else if(inByte==16)
        {
           risposta = ("Control-P");
        }
        else if(inByte==17)
        {
           risposta = ("Control-Q");
        }
        else if(inByte==18)
        {
           risposta = ("Control-R");
        }
        else if(inByte==19)
        {
           risposta = ("Control-S");
        }
        else if(inByte==20)
        {
           risposta = ("Control-T");
        }
        else if(inByte==21)
        {
           risposta = ("Control-U");
        }
        else if(inByte==22)
        {
           risposta = ("Control-V");
        }
        else if(inByte==23)
        {
           risposta = ("Control-W");
        }
        else if(inByte==24)
        {
           risposta = ("Control-X");
        }
        else if(inByte==25)
        {
           risposta = ("Control-Y");
        }        
        else if(inByte==26)
        {
           risposta = ("Control-Z");
        }

        else   
        {
          // Its a normal key
          risposta = (String)(char)(inByte);
          if(risposta != "?")
          {
            //Serial.println(risposta);
          }
          
        }  
      }
           
    }
 
    return risposta;
   
}
