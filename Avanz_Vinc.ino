const char AVV0[] = "**  AVANZ   VINC  **";
const char AVV1[] = "SX        DX        ";
const char AVV2[] = "cg    RPM    P      ";
const char AVV3[] = "SEL OK PAR JX CH-PAR";
const char AVV4[] = "JX SX/DX    ESC:fine";
const char AVV5[] = "ACCENDERE MANDRINO  ";

const char* const MenuAvanzaVinc[] = {AVV0, AVV1, AVV2, AVV3, AVV4, AVV5 };

byte AvanzamentoVincolato()
{
  byte current = 1;  // valore di ritorno al menu principale
  bool stay ;  // variabile per il loop
  bool outer ;  // variabile per il loop
  long current_value = 10; // velocità di avanzamento nell'unità selezionata ( in centesimi al giro ) poi trasformata in steps/s per routine movimento
  int VelLim;   // velocità limite della passata
  int JoyY; // valore letto per il canale Y del joystick
  int JoyX; // valore letto per il canale Y del joystick
  int nupar; // variabile per  sapere che parametro è in aggiornamento ( da 1 a 3) 1 SX_Pos, 2 DX_Pos, 3 cg

  float SX_Pos = 0.00;             //left limit position
  float DX_Pos = 0.00;             //right limit position
  float Current_pos = 0.00;        //Current position of the tool

 
  int RpmMan;                    //da misurare per definire velocità limite di passata

// variabili con routine nopwm
  float dista;      // distanza da percorrere tra limite sinistro e destro

//



// nuova versione setup parametri usando in joystick in x per muoversi da un parametro a quello successivo
// inizializzo lo schermo al nuovo menu
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(MenuAvanzaVinc[0]);
  lcd.setCursor(0,1);
  lcd.print(MenuAvanzaVinc[1]);
  lcd.setCursor(0,2);
  lcd.print(MenuAvanzaVinc[2]);
  lcd.setCursor(0,3);
  lcd.print(MenuAvanzaVinc[3]);

  lcd.setCursor(3,1);
  lcd.print(SX_Pos,2);
  lcd.setCursor(13,1);
  lcd.print(DX_Pos,2); 
  lcd.setCursor(14,2);
  lcd.print(Current_pos,2);   

  lcd.setCursor(3,2);
  lcd.print(current_value); // già inizializzata = 0
  nupar = 1;  // inizializzo da che parametri partire con il setup
  outer = true ;
  while (outer)
  {

// scrol parametri verifica e cambio eventuale di parametro
    JoyX=analogRead(ANALOG_X); 
    if(vsjx == -1)JoyX=1024-JoyX;
    
    if (JoyX <= LOW_1) nupar = nupar -1;
    if (JoyX >= HIGH_1) nupar = nupar +1;
    if ( nupar < 1) nupar=3;
    if ( nupar > 3) nupar=1;
// fine scrol parametri

// print del menu secondo il parametro in modifica
    if ( nupar ==1)
    {  
      lcd.setCursor(0,1);
      lcd.print("SX*");
      lcd.setCursor(10,1);
      lcd.print("DX ");
      lcd.setCursor(0,2);
      lcd.print("cg ");
    } 
    else if ( nupar ==2)
    {  
      lcd.setCursor(0,1);
      lcd.print("SX ");
      lcd.setCursor(10,1);
      lcd.print("DX*");
      lcd.setCursor(0,2);
      lcd.print("cg ");
    } 
    else
    {  
      lcd.setCursor(0,1);
      lcd.print("SX ");
      lcd.setCursor(10,1);
      lcd.print("DX ");
      lcd.setCursor(0,2);
      lcd.print("cg*");
    }
// fine print del menu secondo il parametro in modifica

// setup parametro attivo
    JoyY=analogRead(ANALOG_Y); //leggo il joystick in Y per la variazione del parametro attivo
    if(vsjy == -1)JoyY=1024-JoyY;

    if ( nupar == 1)  // setup limite sinistro
    {
      if ((JoyY <= LOW_TOL) && ( JoyY > LOW_1)) SX_Pos -= 0.01;
      if (( JoyY <= LOW_1) && ( JoyY > LOW_2))  SX_Pos -= 0.01;
      if (( JoyY <= LOW_2) && ( JoyY > LOW_3))  SX_Pos -= 0.05;
      if ( JoyY <= LOW_3) SX_Pos -= 0.1;

      if ((JoyY >= HIGH_TOL) && (JoyY < HIGH_1)) SX_Pos += 0.01;
      if ((JoyY >= HIGH_1) && (JoyY < HIGH_2))   SX_Pos += 0.01;
      if ((JoyY >= HIGH_2) && (JoyY < HIGH_3)) SX_Pos += 0.05;
      if (JoyY >= HIGH_3) SX_Pos += 0.1;

      if(SX_Pos > 0.0 )SX_Pos=0.0;
      lcd.setCursor(3,1);
      lcd.print("       ");
      lcd.setCursor(3,1);
      lcd.print(SX_Pos,2);

    }

    if ( nupar == 2)  // setup limite destro
    {
      if ((JoyY <= LOW_TOL) && ( JoyY > LOW_1)) DX_Pos -= 0.01;
      if (( JoyY <= LOW_1) && ( JoyY > LOW_2))  DX_Pos -= 0.01;
      if (( JoyY <= LOW_2) && ( JoyY > LOW_3))  DX_Pos -= 0.05;
      if ( JoyY <= LOW_3) DX_Pos -= 0.1;

      if ((JoyY >= HIGH_TOL) && (JoyY < HIGH_1)) DX_Pos += 0.01;
      if ((JoyY >= HIGH_1) && (JoyY < HIGH_2))   DX_Pos += 0.01;
      if ((JoyY >= HIGH_2) && (JoyY < HIGH_3)) DX_Pos += 0.05;
      if (JoyY >= HIGH_3) DX_Pos += 0.1;

      if(DX_Pos < 0.0 )DX_Pos=0.0;
        
      lcd.setCursor(13,1);
      lcd.print("       "); 
      lcd.setCursor(13,1);
      lcd.print(DX_Pos,2); 
    }

    if ( nupar == 3)  // setup avanzamento in centesimi giro
    {

      if ((JoyY <= LOW_TOL) && ( JoyY > LOW_1)) current_value -= 1;
      if (( JoyY <= LOW_1) && ( JoyY > LOW_2)) current_value -= 1;
      if (( JoyY <= LOW_2) && ( JoyY > LOW_3)) current_value -= 2;
      if ( JoyY <= LOW_3) current_value -= 2;

      if ((JoyY >= HIGH_TOL) && (JoyY < HIGH_1)) current_value += 1;
      if ((JoyY >= HIGH_1) && (JoyY < HIGH_2)) current_value += 1;
      if ((JoyY >= HIGH_2) && (JoyY < HIGH_3)) current_value += 2;
      if (JoyY >= HIGH_3) current_value += 2;

      if(current_value < 0)current_value=0;            // check limite inferiore quello superiore lo controllo dopo  l'accettazione di tutti i parametri
                                                       // e la lettura della velocità mandrino e al limite viene settato pari alla velocità limite
      lcd.setCursor(3,2);
      lcd.print("   ");
      lcd.setCursor(3,2);
      lcd.print(current_value);

    }
// fine setup parametro attivo

    if (digitalRead(SEL)  == PRESSED)outer=false;  // fine setup parametri
    if (digitalRead(ESC) == PRESSED)return current; // ritorno al menu principale
    delay(200);  // per evitare rimbalzi
  }

  lcd.setCursor(0,1);
  lcd.print("SX ");
  lcd.setCursor(10,1);
  lcd.print("DX ");
  lcd.setCursor(0,2);
  lcd.print("cg ");

// fine nuova versione setup parametri



  lcd.setCursor(0,3);
  lcd.print(MenuAvanzaVinc[4]);
  delay(100); //disaccoppiamento menu

  RpmMan=GetSpeed(duve); //controllo mandrino in moto

  while(RpmMan == 0 )
  {
    lcd.setCursor(0,3);  
    lcd.print(MenuAvanzaVinc[5]);       
    delay(500);
    RpmMan=GetSpeed(duve);
    if (digitalRead(ESC) == PRESSED)return current;
    if(RpmMan != 0)  // inserisco un ritardo per far andare a regime il Mandrino
    {
      delay(AccMan);
      RpmMan=GetSpeed(duve);
    }
  }  
    
  lcd.setCursor(9,2);
  lcd.print(RpmMan);

  VelLim = int((screw_pitch*float(MaxStepperSpeed))/(float(RpmMan))*100.0);   // velocita limite in c/g

  lcd.setCursor(0,3);
  lcd.print(MenuAvanzaVinc[4]);  
  
  if (current_value > VelLim)
  {
    current_value =VelLim;
    lcd.setCursor(3,2);
    lcd.print("   ");
    lcd.setCursor(3,2);
    lcd.print(current_value);
  }

  current_value = int((float(current_value)*float(RpmMan)*float(one_turn_screw_steps))/(screw_pitch*6000.0)); //converto in step/s per routine di movimento no pwm

// -------------------------------------------  inizio gestione movimenti  -----------------------------------
  StepperON();

  delay(200);
  stay=true;
       
  while (stay)        
  {

    if (digitalRead(ESC) == PRESSED) //fermo l'utensile ed esco
    {
      stay = false;
      StepperOFF();
      return current;
    }

    JoyX = analogRead(ANALOG_X);
    if(vsjx == -1)JoyX=1024-JoyX;

    if(JoyX <= LOW_TOL && Current_pos > SX_Pos)  
    {

      lcd.setCursor(14,2);
      lcd.print(" MOV >>");      
            
      Direction = CCW;
      dista= Current_pos-SX_Pos;
      digitalWrite(DIR, Direction);
      delay(10);       // ritardo per permettere al driver di recepire il cambio di direzione
      StepToPos(dista,current_value);
      Current_pos=SX_Pos;

    }

    if(JoyX >= HIGH_TOL  && Current_pos < DX_Pos) 
    {

      lcd.setCursor(14,2);
      lcd.print(" MOV <<");      
            
      Direction = CW;
      dista=DX_Pos-Current_pos;
      digitalWrite(DIR, Direction);
      delay(10);       // ritardo per permettere al driver di recepire il cambio di direzione
      StepToPos(dista,current_value);
      Current_pos=DX_Pos;
    }

    lcd.setCursor(14,2);
    lcd.print("       ");      
    lcd.setCursor(14,2);
    lcd.print(Current_pos,2);

    delay(100);

  }


  StepperOFF();  

// -------------------------------------------  fine gestione movimenti  -----------------------------------
  return current;

}


// StepToPos          routine per fare il percorso  richiesto alla velocita richiesta partendo da fermo e fermandosi all'arrivo 
//                    ( nel caso si faccia partire fasato con il mandrino permetterebbe di filettare ma riscritta per ottimizzazione fasatura)

void StepToPos(float dist,long Vel) {

//  float dist    // distanza da percorrere in mm
//  long Vel       // velocita massima richiesta in step/s a cui percorrere il tragitto usando la massima accelerazione possibile

  int ite; // iterazione in corso per transitorio acc/dec
  float deltt; // durata totale del transitorio
  float deltp; // durata del passo di transitoro
  int nip; // numero di iterazioni all'interno del passo di transitorio
  int iti; // iterazione interna del transitorio
  long velp; // velocita nel passo in corso
  int delv; // delta velocita per ogni passo
  int perc;  // periodo per la velocita costante in microsecondi corretto della durata dell'impulso
  int peri;  // periodo per il transitorio in microsecondi corretto della durata dell'impulso
  
  long npd;      // numero di step motore per percorrere la distanza richiesta
  long npl;      // numero di step minimi con andamento a triangolo per raggiungere la velocita richiesta
  long nia;     // numero di step motore nella rampa di accelerazione
  long nid;     // numero di step nella rampa di decellerazione
  long nic;     // numero di step a velocita costante
  int nsr;      // numero di macro passi ridotti in accelerazione e decellerazione per velocita inferiori alla massima possibile ( impostata come nsa )


  npd= long((dist/screw_pitch)*float(one_turn_screw_steps));

  npl=long(0.5*float(sq(Vel))/float(Acceleration)+0.5*float(sq(Vel))/float(Deceleration));  //calcolo numero passi minimi necessari per raggiungere la velocita Vel e frenare a 0

  if(npl > npd) 
  {

    Vel= long(sqrt((2.0*float(npd))/(1.0/float(Acceleration)+1.0/float(Deceleration))));     // impongo la velocita limite per fare il numero di passi richiesti ( inferiore a npl ) -> minore la distanza + bassa la velocita
                                                                                             // diagramma velocita a triangolo
  }

  MaxStepSec=int((float(MaxStepperSpeed)/60.0)*float(one_turn_screw_steps));
  nsr=int((float(nsa)/float(MaxStepSec))*float(Vel));  // numero di macro passi per raggiungere la velocita richiesta o calcolata ( in proporzione di nsa richiesti per la velocita massima )
  
  // calcolo iterativo degli step reali per la salita e discesa in modo da settare correttamente quelli a velocita costante
  nia=0;
  nid=0;
  
  if(nsr > 0)
  {
    delv= int(float(Vel)/float(nsr));

    deltt= float(Vel)/float(Acceleration);      // parte accelerazione
    deltp= deltt/float(nsr);
  
    for( ite= 1; ite <= nsr; ite++)      
    {
      velp=delv*ite;
      nip=int(float(velp)*deltp);
      if(nip < 1)nip=1;
      nia=nia+nip;
    }

    deltt= float(Vel)/float(Deceleration);      // parte decelerazione
    deltp= deltt/float(nsr);
    for( ite= 1; ite <= nsr; ite++)     
    {
      velp=Vel-delv*ite;  // velocita durante la rampa di decelerazione
      nip=int(float(velp)*deltp);
      if(nip < 1)nip=1;
      nid=nid+nip;
    }

  }

// fine calcolo iterativo step accelerazione e decellerazione

  nic=npd-nia-nid;
  perc=int(1000000.0/float(Vel))-impulso; //semiperiodo della velocita  (corretto con la durata dell'impulso ) in microsecondi per la parte a velocita costante  
  
// realizzazione movimento

  if(nsr > 0)
  {
    deltt= float(Vel)/float(Acceleration);
    deltp= deltt/float(nsr);

    for( ite= 1; ite <= nsr; ite++)   // parte accelerazione
    {
      velp=delv*ite;
      nip=int(float(velp)*deltp);
      if(nip < 1)nip=1;
      peri=int(1000000.0*deltp/float(nip))-impulso; // in microsecondi corretto della durata dell'impulso
      for( iti= 1; iti <= nip; iti++)
      {
         digitalWrite(STEP, HIGH);
         delayMicroseconds(impulso); // pulse per attivare il driver di uno step
         digitalWrite(STEP, LOW);
         delayMicroseconds(peri); 
      }
    }
  }

// parte velocita costante

  for( ite= 1; ite <= nic ; ite++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(impulso); // pulse per attivare il driver di uno step
    digitalWrite(STEP, LOW);
    delayMicroseconds(perc); // 
  } 

// fase di decellerazione
  if(nsr > 0)
  {
    deltt= float(Vel)/float(Deceleration);
    deltp= deltt/float(nsr);
    for( ite= 1; ite <= nsr; ite++)
    {
      velp=Vel-delv*ite;  // velocita durante la rampa di decellerazione
      nip=int(float(velp)*deltp);
      if(nip < 1)nip=1;

      peri=int(1000000.0*deltp/float(nip))-impulso; // in microsecondi
      
      for( iti= 1; iti <= nip; iti++)
      {
        digitalWrite(STEP, HIGH);
        delayMicroseconds(impulso); // pulse per attivare il driver di uno step
        digitalWrite(STEP, LOW);
        delayMicroseconds(peri); 
      }
    }
  }

  return;
  
}
