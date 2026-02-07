
const char PXA0[]  = "*POSIZIONE ANGOLARE*";
const char PXA1[]  = "gradi:              ";
const char PXA2[]  = " RESET          ESC ";
const char* const MenuPosizioneAngolare[]  = {PXA0, PXA1, PXA2};


void InterruptEncoderA()              //this is the Interrupt Service Routine which manage the encoder inputs
{

  byte portA;
  byte portB;

  portA=digitalRead(ENCA);
  portB=digitalRead(ENCB);

  if( portA == 1) {
  
    if(portB == 1){
      absolute_encoder_steps++;
    }else{
      absolute_encoder_steps--;
    }
  }
  else {
    if(portB == 0){
      absolute_encoder_steps++;
    }else{
      absolute_encoder_steps--;
    } 
     
  }

}

void InterruptEncoderB()              //this is the Interrupt Service Routine which manage the encoder inputs
{
 
  byte portA;
  byte portB;

  portA=digitalRead(ENCA);
  portB=digitalRead(ENCB);

  if( portB == 0) {
  
    if(portA == 1){
      absolute_encoder_steps++;
    }else{
      absolute_encoder_steps--;
    }
  }
  else {
    if(portA == 0){
      absolute_encoder_steps++;
    }else{
      absolute_encoder_steps--;
    } 
     
  }


}


byte PosizioneAngolare() // this function displays the angular position of the mandrel
{
  byte current;                                                  // variabile per il ritorno al menu principale
  float Degrees = 0.00;                                          //variable used to store degrees
  float old_Degrees =0.01;                                       //variable to check if the position is changed. Set different from Degrees to print LCD output on the first cycle 
// se encoder e mandrino ruotano insieme
//  const float AngularRatio = 360.0/float(one_turn_mandrel_steps*4);  //calculates the degrees per single mandrel encoder step is multiplied by 4 because use both channels and both step ( rising and falling)
//
// encoder e mandrino non ruotano insieme e possono avere un rapporto di trasmissione qualsiasi
  const float AngularRatio = 360.0/(float(one_turn_enc_steps*4)/tau_man_enc);  //calculates the degrees per single  encoder step is multiplied by 4 because use both channels and both step ( rising and falling)


  current=1;
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print(MenuPosizioneAngolare[0]);
  
  lcd.setCursor(0,2);
  lcd.print(MenuPosizioneAngolare[1]);
  
  lcd.setCursor(0,3);
  lcd.print(MenuPosizioneAngolare[2]);

  absolute_encoder_steps=2000;
  
  attachInterrupt(digitalPinToInterrupt(ENCA), InterruptEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCB), InterruptEncoderB, CHANGE);
  
  while (digitalRead(ESC) != PRESSED)
  {
    if (digitalRead(RESET) == PRESSED)
    {
      absolute_encoder_steps = 2000;
      Degrees=0.0;
    }

    Degrees = Degrees+float(absolute_encoder_steps-2000)*AngularRatio;
    absolute_encoder_steps=2000;
    if (Degrees < 0) Degrees = 360 + Degrees;
    if (Degrees > 360.0) Degrees = Degrees-360.0; 
    if (Degrees != old_Degrees)
    {
      lcd.setCursor(8,2);
      lcd.print(Degrees);
      lcd.print("   ");
      old_Degrees = Degrees;
    } 
    delay(100);
  }  
  detachInterrupt(digitalPinToInterrupt(ENCA));
  detachInterrupt(digitalPinToInterrupt(ENCB));  

  return current;
}
