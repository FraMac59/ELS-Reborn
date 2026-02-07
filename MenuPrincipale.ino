
const char MP0[] = "*MENU  PRINCIPALE*";
const char MP1[] = "Velocita' mandrino";
const char MP2[] = " Movimento Veloce ";
const char MP3[] = " Movimento Lento  ";
const char MP4[] = "Avanzamento Libero";
const char MP5[] = "Avanza.  Vincolato";
const char MP6[] = "    Filettatura   ";
const char MP7[] = "Posizione angolare";
const char MP8[] = "   Impostazioni   ";
const char MP9[] = "   Salva  setup   ";
const char MP10[]= "Ripristina Default";
const char MP11[]= "    Test EMI      ";
const char* const MenuPrincipale[] {MP0, MP1, MP2, MP3, MP4, MP5, MP6, MP7, MP8, MP9, MP10, MP11 };


byte Principale()
{
  byte current = 1;
  byte previous = 11;
  byte next = 2;
  boolean stay = true;
  boolean innerstay = true;
  int joyY;
  
  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print(">");
  lcd.setCursor(19,2);
  lcd.print("<");
  lcd.setCursor(1,0);
  lcd.print(MenuPrincipale[0]);
  
  while (stay)
  {
 	  if( current == 0 ) current = 11;
	  if( current == 12 ) current = 1;
    previous = current - 1;
    next = current +1;
    if (previous == 0) previous = 11;
    if (next == 12) next = 1;
      
    lcd.setCursor(1,1);
    lcd.print("                  ");
    lcd.setCursor(1,1);
    lcd.print(MenuPrincipale[previous]);
    
    lcd.setCursor(1,2);
    lcd.print("                  ");
    lcd.setCursor(1,2);
    lcd.print(MenuPrincipale[current]);
    
    lcd.setCursor(1,3);
    lcd.print("                  ");
    lcd.setCursor(1,3);
    lcd.print(MenuPrincipale[next]);
    
    innerstay = true;
    while (innerstay)
    {      
      if (digitalRead(SEL) == PRESSED) { stay = false; innerstay = false; }
      joyY=analogRead(ANALOG_Y);
      if(vsjy == -1)joyY=1024-joyY;
      
      if (joyY < LOW_1) { current += 1; innerstay = false; }   
      if (joyY > HIGH_1) { current -= 1; innerstay = false; }
    } 
    
    delay (200);
  }
  
  return current;  
}
