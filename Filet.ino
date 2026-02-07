const char FIL0[] = "**  FILETTATURA   **";
const char FIL1[] = "DX L       P        ";
const char FIL2[] = "SEL:OK PAR JX:CH-PAR";
const char FIL3[] = "ESC: fine  JY: U/D  ";
const char FIL4[] = "SEL:LAVORA  ESC:fine";
const char FIL5[] = "SEL:RITORNA ESC:fine";
const char FIL6[] = " PASSO > DEL LIMITE ";
const char FIL7[] = "ACCENDERE MANDRINO  ";
const char FIL8[] = "            ESC:fine";
const char FIL9[] = " LUNG  < DEL LIMITE ";
const char FIL10[] = "VEL MAN =        RPM";

const char* const MenuFilettatura[] = {FIL0, FIL1, FIL2, FIL3, FIL4, FIL5, FIL6, FIL7, FIL8, FIL9, FIL10 };

byte Filettatura()
{
  byte current = 1;
  bool outer=true; // variabile per loop esterno
  bool stay;       // variabile per loop
  int nupar; // variabile per  sapere che parametro è in aggiornamento ( da 1 a 3) 1 senso vite, lunghezza vite, passo vite
  float PassoVite; // passo della filettatura mm
  float PassoLimite; // passo limite eseguibile data una velocità mandrino, un passo vite carro e un limite motore stepper
  float LenghtVite; // lunghezza filettatura mm
  bool DirVite;     // direzione filettatura ( DX true  - SX false)
  int JoyY; // valore letto per il canale Y del joystick
  int JoyX; // valore letto per il canale X del joystick
  int RpmMan;                    //da misurare per definire velocità limite di passata

  long current_value; // velocità di avanzamento in mm/min
// int stepperSpeed;      // velocità da mandare alla routine di avanzamento in giri/min
//  long LavStep;           // numero di encoder step per lavorare la lunghezza vite
//  long stop_steps;        // variabile per arrestare la lavorazione a fine corsa
  long VeLim;       // velocita limite dello stepper in steps/s per ritorno veloce
  byte contLe;      // byte di controllo fattibilita lunghezza minima filettatura

// ----------------------------------------- variabile usata se si ha un encoder con solo i canali A e B --------------------------
  long tapafi;        // numero di tacche encoder per far partire la filettatura se si usa il canale A

  tapafi=long(float(one_turn_enc_steps)/tau_man_enc);

// ---------------------------------------------------------------------------------------------------------------------------------
  
  PassoVite=DefPassoVite;      // setup dei default di filettatura
  LenghtVite=DefLenghtVite;
  DirVite=DefDirVite;

// ----------------------------------inizio zona di sviluppo nuova gestione menu----------------------------------------------------

// nuova versione setup parametri usando in joystick in x per muoversi da un parametro a quello successivo
// inizializzo lo schermo al nuovo menu
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(MenuFilettatura[0]);
  lcd.setCursor(0,1);
  lcd.print(MenuFilettatura[1]);
  lcd.setCursor(0,2);
  lcd.print(MenuFilettatura[2]);
  lcd.setCursor(0,3);
  lcd.print(MenuFilettatura[3]);  

  lcd.setCursor(0,1);
  if(DirVite == LOW)lcd.print("DX");
  else lcd.print("SX");
  lcd.setCursor(5,1);
  lcd.print(LenghtVite,2);
  lcd.setCursor(13,1);
  lcd.print(PassoVite,2); 

  outer= true;
  nupar = 1;  // inizializzo da che parametri partire con il setup
  while (outer)
  {

    stay = true ;
    while (stay)
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
        lcd.setCursor(2,1);
        lcd.print("*");
        lcd.setCursor(4,1);
        lcd.print(" ");
        lcd.setCursor(12,1);
        lcd.print(" ");
      } 
      else if ( nupar ==2)
      {  
        lcd.setCursor(2,1);
        lcd.print(" ");
        lcd.setCursor(4,1);
        lcd.print("*");
        lcd.setCursor(12,1);
        lcd.print(" ");
      } 
      else
      {  
        lcd.setCursor(2,1);
        lcd.print(" ");
        lcd.setCursor(4,1);
        lcd.print(" ");
        lcd.setCursor(12,1);
        lcd.print("*");
      }
// fine print del menu secondo il parametro in modifica

// setup parametro attivo
      JoyY=analogRead(ANALOG_Y); //leggo il joystick in Y per la variazione del parametro attivo
      if(vsjy == -1)JoyY=1024-JoyY;

      if ( nupar == 1)  // setup senso della vite
      {
        if ((JoyY <= LOW_TOL) || ( JoyY > HIGH_TOL))
        {      
          if(DirVite == LOW)DirVite=HIGH;
          else DirVite=LOW;
        }
        lcd.setCursor(0,1);
        if(DirVite == LOW)lcd.print("DX");
        else lcd.print("SX");

      }

      if ( nupar == 2)  // setup lunghezza vite
      {
        if ((JoyY <= LOW_TOL) && ( JoyY > LOW_1)) LenghtVite -= 0.01;
        if (( JoyY <= LOW_1) && ( JoyY > LOW_2))  LenghtVite -= 0.01;
        if (( JoyY <= LOW_2) && ( JoyY > LOW_3))  LenghtVite -= 0.05;
        if ( JoyY <= LOW_3) LenghtVite -= 0.1;

        if ((JoyY >= HIGH_TOL) && (JoyY < HIGH_1)) LenghtVite += 0.01;
        if ((JoyY >= HIGH_1) && (JoyY < HIGH_2))   LenghtVite += 0.01;
        if ((JoyY >= HIGH_2) && (JoyY < HIGH_3))   LenghtVite += 0.05;
        if (JoyY >= HIGH_3) LenghtVite += 0.1;

        if(LenghtVite < 0.0 )LenghtVite=0.0;
        
        lcd.setCursor(5,1);
        lcd.print("      "); 
        lcd.setCursor(5,1);
        lcd.print(LenghtVite,2); 
      }

      if ( nupar == 3)  // setup passo della vite
      {

        if ((JoyY <= LOW_TOL) && ( JoyY > LOW_1)) PassoVite -= 0.01;
        if (( JoyY <= LOW_1) && ( JoyY > LOW_2))  PassoVite -= 0.01;
        if (( JoyY <= LOW_2) && ( JoyY > LOW_3))  PassoVite -= 0.05;
        if ( JoyY <= LOW_3) PassoVite -= 0.1;

        if ((JoyY >= HIGH_TOL) && (JoyY < HIGH_1)) PassoVite += 0.01;
        if ((JoyY >= HIGH_1) && (JoyY < HIGH_2))   PassoVite += 0.01;
        if ((JoyY >= HIGH_2) && (JoyY < HIGH_3))   PassoVite += 0.05;
        if (JoyY >= HIGH_3) PassoVite += 0.1;

        if(PassoVite < 0.0 )PassoVite=0.0;
        
        lcd.setCursor(13,1);
        lcd.print("      "); 
        lcd.setCursor(13,1);
        lcd.print(PassoVite,2); 
  
      }
// fine setup parametro attivo

      if (digitalRead(SEL)  == PRESSED)stay=false;  // fine setup parametri
      if (digitalRead(ESC) == PRESSED)return current; // ritorno al menu principale
      delay(200);  // per evitare rimbalzi
    }

    lcd.setCursor(2,1);
    lcd.print(" ");
    lcd.setCursor(4,1);
    lcd.print(" ");
    lcd.setCursor(12,1);
    lcd.print(" ");


// -----------------------------------fine zona di sviluppo nuova gestione menu-----------------------------------------------------
  

// riparte vecchia parte menu


    RpmMan=GetSpeed(duve); //controllo mandrino in moto


    lcd.setCursor(0,2);
    lcd.print(MenuFilettatura[10]);
    lcd.setCursor(0,3);
    lcd.print(MenuFilettatura[8]);
    lcd.setCursor(10,2);
    lcd.print(RpmMan);
      
  
    while(RpmMan == 0 )
    {
      lcd.setCursor(0,3);  
      lcd.print(MenuFilettatura[7]); 
      delay(500);
      RpmMan=GetSpeed(duve);
      if (digitalRead(ESC) == PRESSED)return current;
      if(RpmMan != 0)  // inserisco un ritardo per far andare a regime il Mandrino
      {
        delay(AccMan);
        RpmMan=GetSpeed(duve);
      }
    }

    lcd.setCursor(10,2);
    lcd.print(RpmMan);
      
    PassoLimite=(float(MaxStepperSpeed)*screw_pitch)/float(RpmMan);

    if(PassoLimite >= PassoVite)outer=false;
    else
    {
       lcd.setCursor(0,3);  
       lcd.print(MenuFilettatura[6]);
       PassoVite=PassoLimite;
       nupar=3;
       delay(10000);
    }
    
  }
// -------------------- fine outer ------------------------------------------


// -----------------------------------------inizio gestione movimenti -------------------------------------------------------------------


  current_value = long((PassoVite*float(RpmMan)*float(one_turn_screw_steps))/(screw_pitch *60.0)); //converto in step/s per routine di movimento no pwm


// ---------------------------------fattore correzione per UNOR4 da prove ( da 250 a 25000 ste/sec ) -----------------------------------
  current_value=long( double(current_value)*(1.0444611634472 + 3.93611004794524E-06 * double(current_value)));    // fattore di compensazione da prove
//---------------------------------------------------------------------------------------------------------------------------------------

/*
// ---------------------------------fattore correzione per MEGA2560 da prove ( da 250 a 25000 ste/sec ) -----------------------------------
  current_value=long( double(current_value)*(1.00602593710343 + 9.59599084241954E-06 * double(current_value)));    // fattore di compensazione da prove
//---------------------------------------------------------------------------------------------------------------------------------------
*/
  LenghtVite=LenghtVite+gioco;   // aggiungo il gioco per avere la lunghezza corretta ( bisogna posizionarsi sul punto di  partenza sempre dall'interno della zona da filettare senza invertire)
  
// per la partenza in lavorazione parto a remainder zero  ( in modo da essere sempre sincronizzato ) e fermo dopoil numero di step motore necessari per la lunghezza
// per il ritorno non aspetto di essere sincronizzato in quanto l'utensile non lavora, per il resto faccio come all'andata 
// ripeto il ciclo per tutte le passate necessarie ( con il pulsante SEL per ogni corsa di andata o ritorno e poi esco con ESC


// vite DX e SX

  contLe=SetParPos(LenghtVite,current_value);    // setto i parametri per la routine di movimento
  
  if( contLe != 0 )
  {

    lcd.setCursor(0,3);  
    lcd.print(MenuFilettatura[9]);
    delay(10000);
    return current;
    
  }
  
  VeLim = long(float(MaxStepperSpeed)*float(one_turn_screw_steps)/60.0);      // per ritorno veloce alla massima velocita
  
  outer=true;
  StepperON();
  delay(200);

  if( IfZA == LOW )   //    strategie alternative in funzione dell'encoder disponibile
  {

//    --------------------------------- inizializzazione interrupt filettatura se presente canale Z ----------------------------------------
    attachInterrupt(digitalPinToInterrupt(ENCZ), InterruptEncoderFile, RISING);   // uso il solo fronte di salita del canale Z
                                                                                  // La precisione dell'encoder non ha nessuna influenza 
                                                                                  // sulla precisione della vite ( basta una tacca al giro )

  }
  else 
  {   
// ------------------------------------ inizializzo interrupt filettatura se uso canale A --------------------------------------------------

    absolute_encoder_steps = 0;           // inizializzo il conteggio con il canale A   quando l'encoder ha solo  i canali A e B 
    attachInterrupt(digitalPinToInterrupt(ENCA), InterruptEncoderFile, RISING);

// ---------------------------------------   fine inizializzazione interrupt ---------------------------------------------------------------
  }
  
  while(outer)
  {
  
// lavorazione

    lcd.setCursor(0,3);
    lcd.print(MenuFilettatura[4]);
    stay=true;



    while(stay)
    {
      if (digitalRead(SEL) == PRESSED)
      {
         stay=false;

         if(DirVite)Direction=CW;
         else Direction=CCW;

         digitalWrite(DIR, Direction); 
         delay(10);

         if( IfZA == LOW )   //    strategie alternative in funzione dell'encoder disponibile
         {
// -------------------------------   partenza filettatura se presente il canale Z ------------------------------------------------------

           absolute_encoder_steps = 0;           // inizializzo il conteggio con il canale Z e poi parto appena passo la tacca
           while(absolute_encoder_steps == 0);      //routine di attesa sul canale Z che fa partire la filettatura appena passa la tacca
           StepToFil(current_value);

// ----------------------------------------------------------------------------------------------------------------------------------------
         
         }
         else
         {
// -------------------------------   partenza filettatura se presente solo canale A e B ------------------------------------------------------

           while( (absolute_encoder_steps % tapafi) != 0 );  // routine di attesa per avere il remainder 0 per partire
           StepToFil(current_value);
// ----------------------------------------------------------------------------------------------------------------------------------------

         }
       
      }
      if (digitalRead(ESC) == PRESSED)
      { 
        StepperOFF();
        return current; 
      }
      delay(100);
    } 
    
// ritorno

    lcd.setCursor(0,3);
    lcd.print(MenuFilettatura[5]);
    stay=true;
    while(stay)
    {
      if (digitalRead(SEL) == PRESSED)
      {
         stay=false;

         if(DirVite)Direction=CCW;
         else Direction=CW;

         digitalWrite(DIR, Direction); 
         delay(10);


// per il ritorno non aspetto il sincronismo tanto l'utensile non lavora e torno invece alla massima velocita

         StepToPos(LenghtVite,VeLim);

      }
      if (digitalRead(ESC) == PRESSED)
      { 
        StepperOFF();
        return current; 
      }
      delay(100);
      
    }    

  } // fine lavorazione vite 


  if( IfZA == LOW )   //    strategie alternative in funzione dell'encoder disponibile
  {
// --------------------------------------------- caso con canale Z ---------------------------------------------------------
    detachInterrupt(digitalPinToInterrupt(ENCZ));  
  }
  else
  {
// --------------------------------------------- caso con encoder con solo canali A e B ------------------------------------
    detachInterrupt(digitalPinToInterrupt(ENCA));

//---------------------------------------------- fine casi detach interupt -------------------------------------------------
  }

  StepperOFF();

  return current;
}

// divido la routine StepToPos in due in modo da avere i movimenti con il minor  ritardo possibile rispetto al segnale di partenza
// nella prima parte calcolo i parammetri per la routine che comanda i motori

byte SetParPos(float dist,long Vel) {

// float dist      // lunghezza della filettatura
// long Vel       // velocita massima richiesta in step/s a cui percorrere il tragitto usando la massima accelerazione possibile

  float deltt; // durata totale del transitorio
  long npd;      // numero di step motore per percorrere la distanza richiesta
  int ite; // iterazione in corso per transitorio acc/dec
  long velp; // velocita nel passo in corso
  long npl;      // numero di step minimi con andamento a triangolo per raggiungere la velocita richiesta
  long nia;     // numero di step motore nella rampa di accelerazione
  long nid;     // numero di step nella rampa di decellerazione
  long nip;     // iterazioni nel macro passo di accelerazione / decelerazione

  byte current;  // valore di ritorno che permette di controllare la fattibilita della lunghezza minima di filettatura

  current=0;

  npd= long((dist/screw_pitch)*float(one_turn_screw_steps));

  npl=long(0.5*float(sq(Vel))/float(Acceleration)+0.5*float(sq(Vel))/float(Deceleration));  //calcolo numero passi minimi necessari per raggiungere la velocita Vel

  if(npl > npd) 
  {

    current=1;
    return current;

  }
  
  deltpa=0;
  deltpd=0;
  nsrf=int((float(nsa)/float(MaxStepperSpeed))*float(Vel));  // numero di macro passi per raggiungere la velocita richiesta o calcolata ( in proporzione di nsa richiesti per la velocita massima )

  // calcolo iterativo degli step reali per la salita e discesa in modo da settare correttamente quelli a velocita costante

  nia=0;
  nid=0;
  if(nsrf >0)
  {
    delvf= int(float(Vel)/float(nsrf));
    deltt= float(Vel)/float(Acceleration);      // parte accelerazione
    deltpa= deltt/float(nsrf);
  
    for( ite= 1; ite <= nsrf; ite++)      
    {
      velp=delvf*ite;
      nip=int(float(velp)*deltpa);
      if(nip < 1)nip=1;
      nia=nia+nip;
    }
  
    deltt= float(Vel)/float(Deceleration);      // parte decelerazione
    deltpd= deltt/float(nsrf);

    for( ite= 1; ite <= nsrf; ite++)     
    {
      velp=Vel-delvf*ite;  // velocita durante la rampa di decelerazione
      nip=int(float(velp)*deltpd);
      if(nip < 1)nip=1;
      nid=nid+nip;
    }
  }
  nicf=npd-nia-nid;
  
  // altri parametri per il movimento
  
  percf=int(1000000.0/float(Vel))-impulso; //semiperiodo della velocita  (corretto con la durata dell'impulso ) in microsecondi per la parte a velocita costante 

  return current;
  
}


// routine per i movimenti di filettatura che utilizza i parametri settati in SetToPar che quindi deve sempre precedere questa routine

void StepToFil(long Vel) {

//  long Vel       // velocita massima richiesta in step/s a cui percorrere il tragitto usando la massima accelerazione possibile

  int ite; // iterazione in corso per transitorio acc/dec
  int nip; // numero di iterazioni all'interno del macro passo di transitorio
  int iti; // iterazione interna del transitorio
  long velp; // velocita nel passo in corso
  int peri;  // periodo per il transitorio in microsecondi corretto della durata dell'impulso



  for( ite= 1; ite <= nsrf; ite++)   // parte accelerazione
  {
    velp=delvf*ite;
    nip=int(float(velp)*deltpa);
    if(nip < 1)nip=1;    
    peri=int(1000000.0/float(velp))-impulso; // in microsecondi corretto della durata dell'impulso
    for( iti= 1; iti <= nip; iti++)
    {
       digitalWrite(STEP, HIGH);
       delayMicroseconds(impulso); // pulse per attivare il driver di uno step
       digitalWrite(STEP, LOW);
       delayMicroseconds(peri); 
    }
  }

// parte velocita costante


  for( ite= 1; ite <= nicf ; ite++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(impulso); // pulse per attivare il driver di uno step
    digitalWrite(STEP, LOW);
    delayMicroseconds(percf); // 
  } 


// fase di decellerazione


  for( ite= 1; ite <= nsrf; ite++)
  {
    velp=Vel-delvf*ite;  // velocita durante la rampa di decellerazione
    nip=int(float(velp)*deltpd);
    if(nip < 1)nip=1;
    peri=int(1000000.0/float(velp))-impulso; // in microsecondi
    for( iti= 1; iti <= nip; iti++)
    {
       digitalWrite(STEP, HIGH);
       delayMicroseconds(impulso); // pulse per attivare il driver di uno step
       digitalWrite(STEP, LOW);
       delayMicroseconds(peri); 
    }
  }



  return;
}

void InterruptEncoderFile()  // interupt per leggere il canale assegnato per far partire la filettatura
                              
{
  absolute_encoder_steps ++;
}
