
const char ML0[] = "* MOVIMENTO VELOCE *";
const char ML1[] = "<< - < ------ > - >>";
const char ML2[] = "posizione         mm";
const char ML3[] = "RESET            ESC";
const char* const MenuMovimentoLibero[] = {ML0, ML1, ML2, ML3};


byte MovimentoLibero()        //moves the stepper motor freely using the joystick.
                              
{
  byte current;
  bool stay = true;
  float Posizione = 0.00;
  int jox;

  long Vela;  // velocita avanzamento richiesta in step/sec
  int DS=0;   // incremento degli step motore +1 o -1 a seconda della direzione da comunicare alla routine di movimento


  current=1;
   

  Speed = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print (MenuMovimentoLibero[0]);
  lcd.setCursor(0,1);
  lcd.print (MenuMovimentoLibero[1]);
  lcd.setCursor(0,2);
  lcd.print (MenuMovimentoLibero[2]);
  lcd.setCursor(0,3);
  lcd.print (MenuMovimentoLibero[3]);


  absolute_steps=0;
  Vela= long(float(MaxStepperSpeed)*float(one_turn_screw_steps)/60.0);      // per movimento veloce alla massima velocita

  StepperON(); 


    while (stay)        
    {


       jox = analogRead(ANALOG_X);
       if(vsjx == -1)jox=1024-jox;

       if (jox > HIGH_TOL) 
       {


          Direction = CW;
          DS=-1;
          StepToLibe(DS,Vela);     // entro nella routine solo se il joystick in SX/dx richiede movimento ne esco quanto finisce richiesta o inverto
       }
    
       else if ( jox < LOW_TOL) 
       {


          Direction = CCW;
          DS=1;
          StepToLibe(DS,Vela);     // entro nella routine solo se il joystick in SX/dx richiede movimento ne esco quanto finisce richiesta o inverto

       }



      if (digitalRead(ESC) == PRESSED) stay = false;
      if (digitalRead(RESET) == PRESSED) absolute_steps = 0;
      
      Posizione = (float(absolute_steps)/float(one_turn_screw_steps))*screw_pitch;   
      lcd.setCursor(10,2);
      lcd.print("        ");
      lcd.setCursor(10,2);
      lcd.print(Posizione,2);

      delay(200);

    }
  

  StepperOFF();

  return current;


}
