const char IMP0[]   = "** IMPOSTAZIONI **";
const char IMP1[]   = "passi encoder/giro";
const char IMP2[]   = "tau mandrel encod ";
const char IMP3[]   = "passi stepper/giro";
const char IMP4[]   = "passo vite madre  ";
const char IMP5[]   = "gioco             ";
const char IMP6[]   = "Vel max stepper   ";
const char IMP7[]   = "Acceleraz step/s^2";
const char IMP8[]   = "Deceleraz step/s^2";
const char IMP9[]   = "rit. per mandr. on";
const char IMP10[]  = "Valore logico CW  ";
const char IMP11[]  = "default verso vite";
const char IMP12[]  = "default passo vite";
const char IMP13[]  = "def lunghezza vite";
const char IMP14[]  = "def min dist lib  ";
const char IMP15[]  = "verso joystick X  ";
const char IMP16[]  = "verso joystick Y  ";
const char IMP17[]  = "contr iniz filet  ";   //<=== FINE MENU' 
const char IMP18[]  = "RESET       SEL:OK";

const char* const MenuImpostazioni[] = {IMP0, IMP1, IMP2, IMP3, IMP4, IMP5, IMP6, IMP7, IMP8, IMP9,
                                        IMP10, IMP11, IMP12, IMP13, IMP14, IMP15, IMP16, IMP17, IMP18 };

bool setup_logici(bool LogPass)
{
  bool stay;
  bool TeLo;
  int JoyY;

  stay=true;
  TeLo = LogPass;

  while (stay)
  {
    JoyY =analogRead(ANALOG_Y);
    
    if ((JoyY <= LOW_TOL) || ( JoyY >= HIGH_TOL)) TeLo = !TeLo; 
  
    lcd.setCursor(2,2);
    lcd.print("     ");
    lcd.setCursor(2,2);
    if(TeLo)lcd.print("true");else lcd.print("false");

    if (digitalRead(SEL) == PRESSED) return TeLo;
    if (digitalRead(ESC) == PRESSED) return LogPass; 
    if (digitalRead(RESET) == PRESSED) TeLo=LogPass;

    delay(200);
  } 

  return LogPass;

}

float setup_reali_fi(float current)
{
  bool stay;
  float TeRe;
  int JoyY;

  stay=true;
  TeRe = current;
  while (stay)
  {
    JoyY =analogRead(ANALOG_Y);
    if(vsjy == -1)JoyY=1024-JoyY;

    if ((JoyY <= LOW_TOL) && ( JoyY > LOW_1)) TeRe -= 0.0001;
    if (( JoyY <= LOW_1) && ( JoyY > LOW_2)) TeRe -= 0.0001;
    if (( JoyY <= LOW_2) && ( JoyY > LOW_3)) TeRe -= 0.001;
    if ( JoyY <= LOW_3) TeRe -= 0.01;

    if ((JoyY >= HIGH_TOL) && (JoyY < HIGH_1)) TeRe += 0.0001;
    if ((JoyY >= HIGH_1) && (JoyY < HIGH_2)) TeRe += 0.0001;
    if ((JoyY >= HIGH_2) && (JoyY < HIGH_3)) TeRe += 0.001;
    if (JoyY >= HIGH_3) TeRe += 0.01;
    lcd.setCursor(2,2);
    lcd.print("      ");
    lcd.setCursor(2,2);
    lcd.print(TeRe,4);

    if (digitalRead(SEL) == PRESSED) return TeRe;
    if (digitalRead(ESC) == PRESSED) return current; 
    if (digitalRead(RESET) == PRESSED) TeRe=current;

    delay(100);
  } 

  return current;

}


float setup_reali(float current)
{
  bool stay;
  float TeRe;
  int JoyY;

  stay=true;
  TeRe = current;
  while (stay)
  {
    JoyY =analogRead(ANALOG_Y);
    if(vsjy == -1)JoyY=1024-JoyY;

    if ((JoyY <= LOW_TOL) && ( JoyY > LOW_1)) TeRe -= 0.01;
    if (( JoyY <= LOW_1) && ( JoyY > LOW_2)) TeRe -= 0.01;
    if (( JoyY <= LOW_2) && ( JoyY > LOW_3)) TeRe -= 0.1;
    if ( JoyY <= LOW_3) TeRe -= 0.1;

    if ((JoyY >= HIGH_TOL) && (JoyY < HIGH_1)) TeRe += 0.01;
    if ((JoyY >= HIGH_1) && (JoyY < HIGH_2)) TeRe += 0.01;
    if ((JoyY >= HIGH_2) && (JoyY < HIGH_3)) TeRe += 0.1;
    if (JoyY >= HIGH_3) TeRe += 0.1;
    lcd.setCursor(2,2);
    lcd.print("     ");
    lcd.setCursor(2,2);
    lcd.print(TeRe);

    if (digitalRead(SEL) == PRESSED) return TeRe;
    if (digitalRead(ESC) == PRESSED) return current; 
    if (digitalRead(RESET) == PRESSED) TeRe=current;

    delay(100);
  } 

  return current;

}

int setup_verso(int current)   // setup del verso del joystick
{
  bool stay;
  int JoyY;
  int tempo;

  stay=true;
  tempo=current;

  while (stay)
  {
    JoyY =analogRead(ANALOG_Y);
    
    if ((JoyY <= LOW_TOL) || ( JoyY >= HIGH_TOL))
    {
       if( tempo > 0)tempo = -1; else tempo = 1;      
    }
  
    lcd.setCursor(2,2);
    lcd.print("     ");
    lcd.setCursor(2,2);
    lcd.print(tempo);

    if (digitalRead(SEL) == PRESSED) return tempo;
    if (digitalRead(ESC) == PRESSED) return current; 
    if (digitalRead(RESET) == PRESSED) tempo=current;

    delay(200);
  } 

  return current;
}


int setup_interi(int current)
{
  int tempo;
  bool stay;
  int JoyY;

  stay=true;
  tempo = current;
  while (stay)
  {
    JoyY =analogRead(ANALOG_Y);
    if(vsjy == -1)JoyY=1024-JoyY;

    if ((JoyY <= LOW_TOL) && ( JoyY > LOW_1)) tempo -= 1;
    if (( JoyY <= LOW_1) && ( JoyY > LOW_2)) tempo -= 1;
    if (( JoyY <= LOW_2) && ( JoyY > LOW_3)) tempo -= 2;
    if ( JoyY <= LOW_3) tempo -= 2;

    if ((JoyY >= HIGH_TOL) && (JoyY < HIGH_1)) tempo += 1;
    if ((JoyY >= HIGH_1) && (JoyY < HIGH_2)) tempo += 1;
    if ((JoyY >= HIGH_2) && (JoyY < HIGH_3)) tempo += 2;
    if (JoyY >= HIGH_3) tempo += 2;
    lcd.setCursor(2,2);
    lcd.print("     ");
    lcd.setCursor(2,2);
    lcd.print(tempo);

    if (digitalRead(SEL) == PRESSED) return tempo;
    if (digitalRead(ESC) == PRESSED) return current; 
    if (digitalRead(RESET) == PRESSED) tempo=current;

    delay(100);
  }

  return current;

}

byte Impostazioni()
{
  byte current = 1;

  byte CurrentImpostazioni=1;
  byte previous = 17;
  byte next = 2;
  bool outer;
  bool stay = true;
  bool innerstay = true;

  byte scelta_impostazioni;
  int JoyY;
  
  outer=true;

  while (outer)
  {

    lcd.setCursor(0,0);
    lcd.print(MenuImpostazioni[0]);

    lcd.clear();
    lcd.setCursor(0,2);
    lcd.print(">");
    lcd.setCursor(19,2);
    lcd.print("<");

    stay=true;

    while (stay)
    {
  	   if( CurrentImpostazioni == 0 ) CurrentImpostazioni = 17;
    	 if( CurrentImpostazioni == 18 ) CurrentImpostazioni = 1;
       previous = CurrentImpostazioni - 1;
       next = CurrentImpostazioni +1;
       if (previous == 0) previous = 17;
       if (next == 18) next = 1;
      
       lcd.setCursor(1,1);
       lcd.print(MenuImpostazioni[previous]);
    
       lcd.setCursor(1,2);
       lcd.print(MenuImpostazioni[CurrentImpostazioni]);
    
       lcd.setCursor(1,3);
       lcd.print(MenuImpostazioni[next]);
    
       innerstay = true;
       while (innerstay)
       {      
         if (digitalRead(SEL) == PRESSED) { stay = false; innerstay = false; }
         if (digitalRead(ESC) == PRESSED)return current;
         JoyY=analogRead(ANALOG_Y);
         if(vsjy == -1) JoyY=1024-JoyY;      
         if (JoyY < LOW_1) { CurrentImpostazioni += 1; innerstay = false; }
         if (JoyY > HIGH_1) { CurrentImpostazioni -= 1; innerstay = false; }
       } 
    
       delay (200);


    }  // fine stay

// zona setup parametro selezionato


    lcd.setCursor(1,1);
    lcd.print(MenuImpostazioni[CurrentImpostazioni]);  
    lcd.setCursor(0,2);
    lcd.print("                    ");  // pulisco tutta la linea
    lcd.setCursor(1,3);
    lcd.print(MenuImpostazioni[18]);   


    scelta_impostazioni = CurrentImpostazioni;
    switch (scelta_impostazioni) 
    {
      case 1:
            lcd.setCursor(2,2);
            lcd.print(one_turn_enc_steps);
            lcd.setCursor(7,2);
            lcd.print("step/giro");
            
            one_turn_enc_steps=setup_interi(one_turn_enc_steps);

            break;
      case 2:
            lcd.setCursor(2,2);
            lcd.print(tau_man_enc,2);
            lcd.setCursor(9,2);
            lcd.print("       ");

            tau_man_enc=setup_reali_fi(tau_man_enc);

            break;            
      case 3:
            lcd.setCursor(2,2);
            lcd.print(one_turn_screw_steps);
            lcd.setCursor(7,2);
            lcd.print("step/giro"); 

            one_turn_screw_steps=setup_interi(one_turn_screw_steps);
        
            break;
      case 4:
            lcd.setCursor(2,2);
            lcd.print(screw_pitch,2);
            lcd.setCursor(9,2);
            lcd.print("mm/giro");

            screw_pitch=setup_reali(screw_pitch);

            break;
      case 5:
            lcd.setCursor(2,2);
            lcd.print(gioco,2);
            lcd.setCursor(7,2);
            lcd.print("mm"); 

            gioco=setup_reali(gioco);

            break;
      case 6:
            lcd.setCursor(2,2);
            lcd.print(MaxStepperSpeed);
            lcd.setCursor(7,2);
            lcd.print("rpm");  

            MaxStepperSpeed=setup_interi(MaxStepperSpeed);

            break;
      case 7:
            lcd.setCursor(2,2);
            lcd.print(Acceleration);
            lcd.setCursor(7,2);
            lcd.print("microsec"); 

            Acceleration=setup_interi(Acceleration); 

            break;
      case 8:
            lcd.setCursor(2,2);
            lcd.print(Deceleration);
            lcd.setCursor(7,2);
            lcd.print("microsec"); 

            Deceleration=setup_interi(Deceleration);

            break;
      case 9:
            lcd.setCursor(2,2);
            lcd.print(AccMan);
            lcd.setCursor(7,2);
            lcd.print("millisec"); 

            AccMan=setup_interi(AccMan);

            break;
      case 10:
            lcd.setCursor(2,2);
            if(CW)lcd.print("true");else lcd.print("false");

            CW=setup_logici(CW);
            CCW = !CW;

            break;
      case 11:
            lcd.setCursor(2,2);
            lcd.print(DefDirVite );

            DefDirVite=setup_logici(DefDirVite);

            break;
      case 12:
            lcd.setCursor(2,2);
            lcd.print(DefPassoVite,2);
            lcd.setCursor(10,2);
            lcd.print("mm"); 

            DefPassoVite=setup_reali(DefPassoVite);

            break;
      case 13:
            lcd.setCursor(2,2);
            lcd.print(DefLenghtVite,2);
            lcd.setCursor(10,2);
            lcd.print("mm");    

            DefLenghtVite=setup_reali(DefLenghtVite);            

            break; 
       case 14:
            lcd.setCursor(2,2);
            lcd.print(DefMinDist,2);
            lcd.setCursor(10,2);
            lcd.print("mm");    

            DefMinDist=setup_reali(DefMinDist);            

            break;
       case 15:
            lcd.setCursor(2,2);
            lcd.print(vsjx);

            vsjx=setup_verso(vsjx);            

            break;
       case 16:
            lcd.setCursor(2,2);
            lcd.print(vsjy);

            vsjy=setup_verso(vsjy);            

            break; 
        case 17:
            lcd.setCursor(2,2);
            if(IfZA)lcd.print("HIGH");else lcd.print("LOW");

            IfZA=setup_logici(IfZA);
            break;

    }  


  }  // fine outer
  

  return current;

}
