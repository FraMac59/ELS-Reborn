//interrupt di gestione dei fine corsa SX

void InterruptFC1()  
{
  
  StepperOFF();   //fermo il motore stepper
  CheckMotor = false;
  lcd.setCursor(0,0);
  lcd.print(" MOTORE DISABILITATO");
  lcd.setCursor(0,1);
  lcd.print("    SPEGNERE ELS    ");
  lcd.setCursor(0,2);
  lcd.print("    ED ELIMINARE    ");
  lcd.setCursor(0,3);
  lcd.print("COLLISIONE FINE CORS");      
  delay(10000);
  
}


//interrupt di gestione dei fine corsa DX

void InterruptFC2()  
{
  
  StepperOFF();   //fermo il motore stepper
  CheckMotor = false;
  lcd.setCursor(0,0);
  lcd.print(" MOTORE DISABILITATO");
  lcd.setCursor(0,1);
  lcd.print("    SPEGNERE ELS    ");
  lcd.setCursor(0,2);
  lcd.print("    ED ELIMINARE    ");
  lcd.setCursor(0,3);
  lcd.print("COLLISIONE FINE CORS");      
  delay(10000);
   
}
