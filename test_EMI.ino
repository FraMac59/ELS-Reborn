
const char TE0[] = "*     Test EMI     *";
const char TE1[] = "<< - < ------ > - >>";
const char TE2[] = "N interferenze      ";
const char TE3[] = " Ritorno Automatico ";
const char* const MTestEMI[] = {TE0, TE1, TE2, TE3};


byte TestEMI()       // routine per controllare le interferenze create dal driver dello stepper mentre muove il motore
{
  byte current = 1;  // valore di ritorno al menu principale

 // variabili per check

  int acce; // step secondo quadro
  int vels; // step al secondo ( coincide con la frequenza degli step )
  int velp; // velocita nel passo in corso
  int delv; // delta velocita per ogni passo
  int nrev; // step a velocita costante
  int ico;  // iterazione a velocita costante
  int perc;  // periodo per la velocita costante in microsecondi corretto della durata dell'impulso
  int peri;  // periodo per il transitorio in microsecondi corretto della durata dell'impulso
 

  int ite; // iterazione in corso per transitorio acc/dec
  float deltt; // durata totale del transitorio
  float deltp; // durata del passo di transitoro

  int nip; // numero di iterazioni all'interno del passo di transitorio
  int iti; // iterazione interna del transitorio

  // fine variabili per check


  current=1;
  
  Speed = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print (MTestEMI[0]);
  lcd.setCursor(0,1);
  lcd.print (MTestEMI[1]);
  lcd.setCursor(0,2);
  lcd.print (MTestEMI[2]);
  lcd.setCursor(0,3);
  lcd.print (MTestEMI[3]);


  absolute_encoder_steps = 0;
  
  attachInterrupt(digitalPinToInterrupt(ENCA), InterruptEncoderSpeed, RISING);

// parte inserita ( testata su arduino 1 ) per check elettronica ELS



  vels = int((float(MaxStepperSpeed)/60.0)*float(one_turn_screw_steps));
  acce=Acceleration;
  nrev=12000;

  // ---------------------------------------   andata --------------------------------------------------------------------

  digitalWrite(DIR,LOW); //attivo una direzione
  StepperON();
  delayMicroseconds(10); // pulse per far leggere al driver le impostazioni dir ed enable

// parte accellerazione

  
  deltt= float(vels)/float(acce);
  deltp= deltt/float(nsa);
  delv= vels/nsa;
  perc=int(1000000.0/float(vels))-impulso; //semiperiodo della velocita costante in microsecondi
  
  for( ite= 1; ite <= nsa; ite++)
  {
    velp=delv*ite;
    nip=int(deltp/float(velp));
    if( nip == 0 )nip=1;
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


  for( ico= 0; ico <= nrev ; ico++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(impulso); // pulse per attivare il driver di uno step
    digitalWrite(STEP, LOW);
    delayMicroseconds(perc); // 
  } 

// fase di decellerazione


  for( ite= 1; ite <= nsa; ite++)
  {
    velp=vels-delv*ite;  // velocita durante la rampa di decellerazione
    nip=int(deltp/float(velp));
    if( nip == 0 )nip=1;
    peri=int(1000000.0/float(velp))-impulso; // in microsecondi
    for( iti= 1; iti <= nip; iti++)
    {
       digitalWrite(STEP, HIGH);
       delayMicroseconds(impulso); // pulse per attivare il driver di uno step
       digitalWrite(STEP, LOW);
       delayMicroseconds(peri); 
    }
  }

// ------------------------------------------- ritorno ---------------------------------------------------------

  digitalWrite(DIR,HIGH); //attivo una direzione

  delayMicroseconds(10); // pulse per far leggere al driver le impostazioni dir ed enable

// parte accellerazione

  
  deltt= float(vels)/float(acce);
  deltp= deltt/float(nsa);
  delv= vels/nsa;
  perc=int(1000000.0/float(vels))-impulso; //semiperiodo della velocita costante in microsecondi
  
  for( ite= 1; ite <= nsa; ite++)
  {
    velp=delv*ite;
    nip=int(deltp/float(velp));
    if( nip == 0 )nip=1;
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


  for( ico= 0; ico <= nrev ; ico++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(impulso); // pulse per attivare il driver di uno step
    digitalWrite(STEP, LOW);
    delayMicroseconds(perc); // 
  } 

// fase di decellerazione


  for( ite= 1; ite <= nsa; ite++)
  {
    velp=vels-delv*ite;  // velocita durante la rampa di decellerazione
    nip=int(deltp/float(velp));
    if( nip == 0 )nip=1;
    peri=int(1000000.0/float(velp))-impulso; // in microsecondi
    for( iti= 1; iti <= nip; iti++)
    {
       digitalWrite(STEP, HIGH);
       delayMicroseconds(impulso); // pulse per attivare il driver di uno step
       digitalWrite(STEP, LOW);
       delayMicroseconds(peri); 
    }
  }


// ------------------------------------------ fine movimento ---------------------------------------------------

  StepperOFF();


  detachInterrupt(digitalPinToInterrupt(ENCA));

  // fine inserimento check


    lcd.setCursor(14,2);
    lcd.print("      ");   
    lcd.setCursor(15,2);
    lcd.print(absolute_encoder_steps,2);

    delay(5000);


  
  
  return current;

}
