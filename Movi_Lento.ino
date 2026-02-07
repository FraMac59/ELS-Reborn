
const char MLE0[] = "*  MOVIMENTO LENTO *";
const char MLE1[] = "<< - < ------ > - >>";
const char MLE2[] = "posizione         mm";
const char MLE3[] = "RESET            ESC";
const char* const MenuMovimentoLento[] = {MLE0, MLE1, MLE2, MLE3};


byte MovimentoLento()        //muove lo stepper a velocita' lenta predefinita per il massimo controllo sulla posizione
                              
{
  byte current;
  bool stay = true;
  bool stayin;
  float Posizione = 0.00;
  int jox;

  long Vela;  // velocita avanzamento richiesta in step/sec
  int perc;   // semiperiodo definito dalla velocita' e dalla durata dell'impulso
  int DS=0;   // incremento degli step motore +1 o -1 a seconda della direzione da comunicare alla routine di movimento


  current=1;
   

  Speed = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print (MenuMovimentoLento[0]);
  lcd.setCursor(0,1);
  lcd.print (MenuMovimentoLento[1]);
  lcd.setCursor(0,2);
  lcd.print (MenuMovimentoLento[2]);
  lcd.setCursor(0,3);
  lcd.print (MenuMovimentoLento[3]);


  absolute_steps=0;
  // non applico fattore di correzzione velocita' in quanto voglio un movimento lento la velocita' reale non e' importante
  Vela = long(((float(VeLe)/60.0)/screw_pitch)*float(one_turn_screw_steps));    //converto in step/s per routine di movimento
  perc=int(1000000.0/float(Vela))-impulso; //semiperiodo della velocita  (corretto con la durata dell'impulso ) in microsecondi per la parte a velocita costante 

    
  StepperON(); 

    while (stay)        
    {

      jox = analogRead(ANALOG_X);
      if(vsjx == -1)jox=1024-jox;

      if (jox > HIGH_TOL) 
      {

        Direction = CW;
        DS=-1;
        stayin = true;

      }   
      else if ( jox < LOW_TOL) 
      {

        Direction = CCW;
        DS=1;
        stayin = true;
        
      }

      
      digitalWrite(DIR,Direction); //attivo la direzione selezionata
      delayMicroseconds(10);   // ritardo per rendere attiva la scelta sul driver
      
      while( stayin)
      {
             
        absolute_steps = absolute_steps + DS;
        digitalWrite(STEP, HIGH);
        delayMicroseconds(impulso); // pulse per attivare il driver di uno step
        digitalWrite(STEP, LOW);
        delayMicroseconds(perc);

        Posizione = (float(absolute_steps)/float(one_turn_screw_steps))*screw_pitch;   
        lcd.setCursor(10,2);
        lcd.print("        ");
        lcd.setCursor(10,2);
        lcd.print(Posizione,2);  

        jox = analogRead(ANALOG_X);
        if(vsjx == -1)jox=1024-jox;
        
        if(jox > LOW_TOL && jox < HIGH_TOL )stayin = false;
              
      }

      if (digitalRead(ESC) == PRESSED) stay = false;
      if (digitalRead(RESET) == PRESSED)
      {
        absolute_steps = 0;
        Posizione = 0.0;
        lcd.setCursor(10,2);
        lcd.print("        ");
        lcd.setCursor(10,2);
        lcd.print(Posizione,2);  
      }

      
      
      delay(200);

    }
  

  StepperOFF();

  return current;


}
