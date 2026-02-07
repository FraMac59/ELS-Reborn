
const char AVL0[]  = "*AVANZAMENTO LIBERO*";
const char AVL1[]  = "cent/g      RPM     ";
const char AVL2[]  = "Posizione         mm";
const char AVL3[]  = "JY: CH VEL    SEL:OK";
const char AVL4[]  = "            ESC:fine";
const char AVL5[]  = "RESET       ESC:fine";
const char AVL6[]  = "ACCENDERE MANDRINO  ";
const char AVL7[]  = "            ESC:fine";

const char* const MenuAvanzaLibero[] = {AVL0, AVL1, AVL2, AVL3, AVL4, AVL5, AVL6, AVL7 };

byte AvanzamentoLibero()
{
  byte current = 1;  // valore di ritorno al menu principale
  bool stay ;  // variabile per il loop
  int current_value = 10; // velocità di avanzamento in cent/giro poi sempre convertita in mm/min
  int VelLim;   // velocità limite della passata
  int JoyY; // valore letto per il canale Y del joystick

  int RpmMan; //velocità rotazione mandrino in rpm ,da misurare per definire velocità limite di passata
  float Posizione = 0.00;        // position of cutting tool
  long Vela;  // velocita avanzamento richiesta in step/sec
  int DS=0;         // incremento degli step motore +1 o -1 a seconda della direzione da comunicare alla routine di movimento
  int jox; // valore letto per il canale X del joystick


  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(MenuAvanzaLibero[0]);
  lcd.setCursor(0,1);
  lcd.print(MenuAvanzaLibero[1]);
  lcd.setCursor(0,2);
  lcd.print(MenuAvanzaLibero[3]);
  lcd.setCursor(0,3);
  lcd.print(MenuAvanzaLibero[4]);

  RpmMan=GetSpeed(duve); //controllo mandrino in moto per valutare se il parametro di avanzamento che verra' richiesto in cent/giro è possibile

  while(RpmMan == 0 )
  {
    lcd.setCursor(0,2);  
    lcd.print(MenuAvanzaLibero[6]); 
    lcd.setCursor(0,3);
    lcd.print(MenuAvanzaLibero[7]);       
    delay(500);
    RpmMan=GetSpeed(duve);
    if (digitalRead(ESC) == PRESSED)return current;
    if(RpmMan != 0)  // inserisco un ritardo per far andare a regime il Mandrino
    {
      delay(AccMan);
      RpmMan=GetSpeed(duve);
      lcd.setCursor(0,2);
      lcd.print(MenuAvanzaLibero[3]);
      lcd.setCursor(0,3);
      lcd.print(MenuAvanzaLibero[4]);
    }
  }

  lcd.setCursor(16,1);
  lcd.print(RpmMan);

  VelLim = int(screw_pitch*float(MaxStepperSpeed));     // velocita limite in mm/1'
    
  VelLim = int(float(VelLim)/float(RpmMan)*100.0);  //converto in cent/giro


// ---------------------------- setup velocità avanzamento --------------------------------------------------------------
  stay=true;
  while (stay)
  {

    if (digitalRead(SEL) == PRESSED)
    {
      stay=false;
      lcd.setCursor(0,2);
      lcd.print(MenuAvanzaLibero[2]);      
      lcd.setCursor(0,3);
      lcd.print(MenuAvanzaLibero[5]);
    }

    if (digitalRead(ESC) == PRESSED)return current;

    if (digitalRead(RESET) == PRESSED) current_value = 10;

    JoyY =analogRead(ANALOG_Y);
    if(vsjy == -1)JoyY=1024-JoyY;

    if ((JoyY <= LOW_TOL) && ( JoyY > LOW_1)) current_value -= 1;
    if (( JoyY <= LOW_1) && ( JoyY > LOW_2)) current_value -= 1;
    if (( JoyY <= LOW_2) && ( JoyY > LOW_3)) current_value -= 2;
    if ( JoyY <= LOW_3) current_value -= 2;

    if ((JoyY >= HIGH_TOL) && (JoyY < HIGH_1)) current_value += 1;
    if ((JoyY >= HIGH_1) && (JoyY < HIGH_2)) current_value += 1;
    if ((JoyY >= HIGH_2) && (JoyY < HIGH_3)) current_value += 2;
    if (JoyY >= HIGH_3) current_value += 2;

    if(current_value > VelLim)current_value=VelLim;  // check limiti
    if(current_value < 1)current_value=1;            // check limiti

    lcd.setCursor(7,1);
    lcd.print("    ");
    lcd.setCursor(7,1);
    lcd.print(current_value);
    delay(200);

  } 

// ---------------------------- fine setup velocità avanzamento --------------------------------------------------------------

  absolute_steps=0;
  Posizione = 0;
  lcd.setCursor(10,2);
  lcd.print(Posizione,2);


  current_value = int(float(current_value)*float(RpmMan)/100.0);   //converto in mm/min per routine di movimento

  Vela = long(((float(current_value)/60.0)/screw_pitch)*float(one_turn_screw_steps));    //converto in step/s per routine di movimento


// ------------------------------ inizio movimenti --------------------------------------------------------

  stay=true;
// parte inserita copiata da Movimento veloce
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


void InterruptEncoderSpeed()  //this is the Interrupt Service Routine which manage the encoder inputs while feeding
                              //activate this interrupt only with RISING on Encoder A channel
{
  absolute_encoder_steps ++;
}


int GetSpeed ( int frame)
{
  int ManSpeed = 0;          //speed of the mandrel calculated
  const float speed_multiplier = (tau_man_enc*60000.0)/(float(one_turn_enc_steps)*float(frame));  // moltiplicatore per la velocita in giri/1' frame e' in millisecondi

  // in general the mandrel turn at a different speed of the encoder the relation is done by tau_man_enc
  // this is the multiplier used by to calculate the speed

    
  absolute_encoder_steps = 0;

  attachInterrupt(digitalPinToInterrupt(ENCA), InterruptEncoderSpeed, RISING);
  delay(frame);
  detachInterrupt(digitalPinToInterrupt(ENCA));

  
  ManSpeed = int( float(absolute_encoder_steps)*speed_multiplier); //calculates the speed

  
      // CALCULATION OF SPEED
      // this routine calculates how many steps are done by the mandrel during "frame" (in milliseconds)
      // the number of steps are multiplied by the "speed_multiplier" which is calculated above in the constant definition
      // the result is the exact speed in round per minute
  return ManSpeed;
}


// ----------------------------------------     routine per movimento con avanzamento libero    -----------------------------------
//
// il controllo e' tramite il movimento in x del joystick per movimento sx, fermo, movimento dx

void StepToLibe(int DS,long Vel) {

//  long Vel       // velocita massima richiesta in step/s a cui percorrere il tragitto usando la massima accelerazione possibile
//  int DS         // incremento degli step motore +1 o -1 a seconda della direzione



  int ite; // iterazione in corso per transitorio acc/dec
  float deltt; // durata totale del transitorio
  float deltp; // durata del passo di transitoro
  int nip; // numero di iterazioni all'interno del passo di transitorio
  int iti; // iterazione interna del transitorio
  long velp; // velocita nel passo in corso
  int delv; // delta velocita per ogni passo
  int perc;  // periodo per la velocita costante in microsecondi corretto della durata dell'impulso
  int peri;  // periodo per il transitorio in microsecondi corretto della durata dell'impulso
  
  int nsr;      // numero di macro passi ridotti in accelerazione e decellerazione per velocita inferiori alla massima possibile ( impostata come nsa )

  bool instay ;  // variabile per il loop
  int jox; // valore letto per il canale X del joystick

  lcd.setCursor(10,2);
  lcd.print(" MOV    ");

// non uso fattore di compensazione in quanto piccolo e non e' richiesta grande precisione sulla velocita'
/* disabilito fattore compensazione calcolato sul Mega2560
  Vel=int( float(Vel)*(1.0+0.325/4285.0*float(Vel)));    // fattore di compensazione da prove
*/
  
  
  MaxStepSec=int((float(MaxStepperSpeed)/60.0)*float(one_turn_screw_steps));

  nsr=int((float(nsa)/float(MaxStepSec))*float(Vel));  // numero di macro passi per raggiungere la velocita richiesta o calcolata ( in proporzione di nsa richiesti per la velocita massima )

  perc=int(1000000.0/float(Vel))-impulso; //semiperiodo della velocita  (corretto con la durata dell'impulso ) in microsecondi per la parte a velocita costante 

  digitalWrite(DIR,Direction); //attivo una direzione
  delayMicroseconds(10);   // ritardo per rendere attiva la scelta sul driver


// realizzazione movimento 

// parte accelerazione
  if( nsr > 0 )
  {
    delv= int(float(Vel)/float(nsr));
    deltt= float(Vel)/float(Acceleration);
    deltp= deltt/float(nsr);

      for( ite= 1; ite <= nsr; ite++)   
      {
        velp=delv*ite;
        nip=int(float(velp)*deltp);
        if(nip < 1)nip=1;

        peri=int(1000000.0*deltp/float(nip))-impulso; // in microsecondi corretto della durata dell'impulso
     
        for( iti= 1; iti <= nip; iti++)
        {
        
          absolute_steps = absolute_steps + DS;
             
          digitalWrite(STEP, HIGH);
          delayMicroseconds(impulso); // pulse per attivare il driver di uno step
          digitalWrite(STEP, LOW);
          delayMicroseconds(peri);

        }

     }
  }

// parte velocita costante


  instay=true; // attivo la parte a velocita costante

    
  while ( instay)
  {

    jox = analogRead(ANALOG_X);

    
    if (jox > LOW_TOL  && jox < HIGH_TOL )instay= false ;

    for( ite = 1; ite <= freou; ite++)     // avanzo a pacchetti predefiniti per fare meno analogRead e rallentare meno la velocita
    {
      absolute_steps = absolute_steps + DS;
      digitalWrite(STEP, HIGH);
      delayMicroseconds(impulso); // pulse per attivare il driver di uno step
      digitalWrite(STEP, LOW);
      delayMicroseconds(perc);
    }

  }

// fase di decellerazione da eseguire sempre

  if(nsr > 0)
  {
    deltt= float(Vel)/float(Deceleration);
    deltp= deltt/float(nsr);
    for( ite= 1; ite <= nsr; ite++)
    {

      velp=Vel-delv*ite;  // velocita durante la rampa di decellerazione a inizio macrostep
      nip=int(float(velp)*deltp);
      if(nip < 1)nip=1;

      peri=int(1000000.0*deltp/float(nip))-impulso; // in microsecondi

      for( iti= 1; iti <= nip; iti++)
      {
         absolute_steps = absolute_steps + DS;
         digitalWrite(STEP, HIGH);
         delayMicroseconds(impulso); // pulse per attivare il driver di uno step
         digitalWrite(STEP, LOW);
         delayMicroseconds(peri); 

      }
    }
  }


  return;
  
}
