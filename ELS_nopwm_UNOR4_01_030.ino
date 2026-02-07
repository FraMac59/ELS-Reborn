// ELS_reborn prende spunto da ELS by McMax di meccanica e dintorni ma sia i menu che la logica di 
// funzionamentocon e le routine di controllo dei motori sono molto diverse

// ELS Reborn
// Firmware
// il Firmware è riscritto da Fra&Mac dal 02/02/2024 come menu diversi e funzioni implementate in modo diverso oltre ad essere per Arduino Mega2560 e schermo  I2C 
// inserito il rapporto tra mandrino ed encoder ( deve comunque dare un numero di step encoder intero)
// inserita velocita mandrino come media mobile di 10 letture
// cambiata routine per fare la filettatura dalla 00_13 alla 00_14
// inserita routine per controllo interferenze "sull'encoder" con motori stepper in funzione
// eliminata la gestione degli step con pwm e fatta direttamente con digitalwrite
// porting da arduino mega a unoR4 Minima
// modificate routine di lettura encoder  per posizione angolare ora eseguita senza usare direttamente i registri del MC
// inserito controllo start filettatura su canale Z encoder (deve essere 1:1 con giri mandrino)
// aggiornata EEPROM per diversa lunghezza int ( R3 2bite, R4 4bite )
// inserita la distanza minima da percorrere in movimento e avanzamento libero ( alla quale va aggiunto quanto percorso  in acc e decelerazione )
// aggiunto Movimento lento per spostamenti piccoli lenti ma con il massimo controllo sulla posizione
// modificata funzione alla partenza attivo in numero di giro mandrino e non il menu principale
// corretti diversi piccoli bug dalla R400-006 alla R400-010
// cambiata gestione menu filettatura fatta come per avanzamento vincolato
// cambiato menu avanzamento libero 
// aggiornato fattore di correzione per MEGA2560 e UNOR4 con prove da 250 a 25000 step/sec
// aggiunti parametri per cambiare verso agli assi del joystick
// aggiunta la gestione di due fine corsa uno a SX e uno DX
// aggiunta la possibilita' di scegliere la modalita' di comando di inizio filettatura ( canale Z o A )

#include <LiquidCrystal_I2C.h>  // libreria per la gestione I2C dello schermo
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include <EEPROM.h>           //include la libreria per il controllo della EEPROM

#define firmware_version "fw UNOR4 v01.030"


// ---------------------------------dichiarazioni per R4--------------------------------------------------------
// Pin assignement
#define ANALOG_X A1    //ingresso analogico joystick asse X 
#define ANALOG_Y A0    //ingresso analogico joystick asse Y 
#define STEP     5     //uscita segnale "STEP" per motore stepper 
#define DIR      4     //uscita segnale "DIR" per motore stepper
#define ENABLE   6     //uscita segnale "ENABLE" per motore stepper
#define ENCA     2     //ingresso ancoder A - gestito con interrupt
#define ENCB     3     //ingresso ancoder B - gestito con interrupt 
#define ENCZ     8     //ingresso encoder Z - gestito con interrupt
#define ESC      9     //bottone ESC 
#define SEL      11    //bottone SELECT ( su joystick )
#define RESET    10    //bottone RESET 
#define FC1      0     //fine corsa SX (1) - gestito con interrupt
#define FC2      1     //fine corsa DX (2) - gestito con interrupt
//SDA e SCL non assegnati perche uso i pin di default per unoR4
// ------------------------------------------------------------------------------------------------------------


/*
// --------------------------------- dichiarazioni per Mega2560 --------------------------------------------------
// Pin assignement
#define ANALOG_X A14    //ingresso analogico joystick asse X su shield mega 2560 
#define ANALOG_Y A15    //ingresso analogico joystick asse Y su shield mega 2560
#define STEP     11     //uscita segnale "STEP" per motore stepper su shield mega 2560  ( necessario PWM )
#define DIR      25     //uscita segnale "DIR" per motore stepper su shield mega 2560
#define ENCA     2      //ingresso ancoder A - gestito con interrupt
#define ENCB     3      //ingresso ancoder B - gestito con interrupt 
#define ENABLE   23     //uscita segnale "ENABLE" per motore stepper su shield mega 2560
#define ESC      51     //bottone ESC -  su shield mega 2560
#define SEL      29     //bottone SELECT -  su shield mega 2560
#define RESET    53     //bottone RESET -  su shield mega 2560
//SDA e SCL non assegnati perche uso i pin di default per Mega2560
//----------------------------------------------------------------------------------------------------------------
*/

//Analog Joystick reading tolerance definition
#define LOW_TOL   450   //minimum tolerance for joystick movement LOW (2.2V on joystic pot cursor)  ATTENZIONE DEVE ESSERE SIMMETRICA A HIGH_TOL
#define HIGH_TOL  573   //minimum tolerance for joystick movement HIGH (2.8V on joystic pot cursor) ATTENZIONE DEVE ESSERE SIMMETRICA A LOW_TOL
#define LOW_1     348   //first LOW pos tolerance for joystick movement (1.7V on joystic pot cursor)
#define LOW_2     246   //second LOW pos tolerance for joystick movement (1.2V on joystic pot cursor)
#define LOW_3     143   //third LOW pos tolerance for joystick movement (0.7V on joystic pot cursor)
#define HIGH_1    675   //first HIGH pos tolerance for joystick movement (3.3V on joystic pot cursor)
#define HIGH_2    777   //second HIGH pos tolerance for joystick movement (3.8V on joystic pot cursor)
#define HIGH_3    880   //third HIGH pos tolerance for joystick movement (4.3V on joystic pot cursor)
//END Analog Joystick reading tolerance definition

#define PRESSED LOW     //costante per determinare la pressione dei pulsanti


long AccMan;             //variabile che controlla il ritardo per lasciare andare a regime il mandrino dopo l'accensione, Variabile letta da EEPROM

// variabili relative a motore stepper e cinematica tornio
long one_turn_screw_steps;          //numero di step/giro del motore stepper sulla vite (valore caricato da EEPROM)
float screw_pitch;                 //passo della vite madre in mm (valore caricato da EEPROM)
float gioco;                       // gioco tra vite e carro nell'inversione di moto in mm (valore caricato da EEPROM)

//variabili relative alla lettura dell'encoder

volatile long absolute_encoder_steps ;           //passi assoluti encoder
long one_turn_enc_steps;        //numero di step/giro dell'encoder mandrino (valore caricato da EEPROM)
float tau_man_enc;                 // rapporto di trasmissione tra mandrino ed encoder  (valore caricato da EEPROM)

// stepper motor variables
bool CW;         //TRUE  se la rotazione standard della vite e' in senso orario (orario trascina il carro verso il mandrino). variabile letta da EEPROM
bool CCW;        //FALSE se la rotazione standard della vite e' in senso anti-orario (anti-orario trascina il carro verso il madnrino) variabile letta da EEPROM
bool Direction;  //variabile usata per determinare il senso di rotazione momentaneo del motore stepper e incrementare o decrementare i passi
int passi_stepper = 0;
int Speed=0;           //memorizza la velocità corrente del motore stepper ( prima di raggiungere la nuova velocità ) in giri al minuto - se = 0 lo stepper e' fermo
long MaxStepperSpeed; //massima velocità di rotazione ammessa per il motore stepper ( rpm ). Variabile letta da EEPROM
int MaxStepSec;      // massima velocit' dello stepper in step/s
const int VeLe = 6;            // velocita di spostamento (in mm/min) lento che non necessita di routine di accellerazione/decellerazione per massimo controllo sulla posizione

volatile long absolute_steps = 0;         //passi stepper assoluti

unsigned long Acceleration;                //accelerazione del motore stepper - varibile letta da EEPROM
unsigned long Deceleration;                //decelerazione del motore stepper - varibile letta da EEPROM
const int impulso = 20;                   // durata dell'impulso che viene inviato allo stepper - variabile letta da EEPROM nella versione finale ora costante
const int nsa = 30;                       // costante numero di macro-passi in cui si vuole accelerare o decelerare da zero a vmax o viceversa

// parametri per routine movimento della filettatura
int nsrf;     // numero di macro passi ridotti in accelerazione e decellerazione per velocita inferiori alla massima possibile ( impostata come nsa ) 
int percf;    // periodo per la velocita costante in microsecondi corretto della durata dell'impulso
int delvf;    // delta velocita per ogni macro passo
long nicf;    // numero di step a velocita costante
float deltpa; // durata del passo di transitoro  in accelerazione
float deltpd; // durata del passo di transitoro in decelerazione 


// default filettatura
float  DefLenghtVite;   //default lunghezza vite
bool   DefDirVite ;     //default direzione vite
float  DefPassoVite ;   //default passo vite

const int duve = 1000;    //interval in milliseconds to measure the speed
float DefMinDist;         // distanza minima a velocita costante da fare nelle routine movimento libero e avanzamento libero ( da aggiungere a quanto percorso in acc e decelerazione)
int freou ;              // frequenza per definire ogni quanti step avere il check nella routine avanzamento libero ( deriva da DefMinDist )
                         // viene caricato alla partenza dalla routine LoadFromEEPROM
long vsjx;                 // controlla il senso di uso dell'asse X del joystick
long vsjy;                 // controlla il senso di uso dell'asse Y del joystick
bool CheckMotor;          // variabile che autorizza l'accensione dei motori ( true: si possono accendere false: non si possono accendere ) gestita nei fine corsa
                          // i fine corsa sono montati NC e quindi su FC1 e FC2 si ha in normale funzionamento un segnale alto
bool IfZA;                 // variabile che controlla il canale dell'encoder usato per far partire la filettatura ( LOW canale Z HIGH canale A )

void StepperON()          //accende il motore stepper
{
  if(CheckMotor)
  {
    attachInterrupt(digitalPinToInterrupt(FC1), InterruptFC1, FALLING); //prima di abilitare lo stepper inserisco il fine corsa lato SX
    attachInterrupt(digitalPinToInterrupt(FC2), InterruptFC2, FALLING); //prima di abilitare lo stepper inserisco il fine corsa lato DX
    digitalWrite(ENABLE, LOW);        // il driver DM556 di default ha l'enable on a livello low
    delay(10);                        // ritardo per essere sicuro che il comando venga recepito dal driver
  }
  else
  {
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
}

void StepperOFF()        //spegne il motore stepper
{
  
  digitalWrite(ENABLE, HIGH);
  detachInterrupt(digitalPinToInterrupt(FC1));  // dopo aver spento lo stepper disinserisco il fine corsa
  detachInterrupt(digitalPinToInterrupt(FC2));  // dopo aver spento lo stepper disinserisco il fine corsa

}



void setup() {

/*
//------------------- serial monitor per debug -------
  Serial.begin(9600);
  while (!Serial);
  Serial.println("test menu");
//----------------------------------------------------
*/

//------------------------------------------------------
  lcd.init();                      // initialize the lcd  I2C 
  lcd.backlight();
//----------------------------------------------------

  lcd.setCursor(0,0);
  lcd.print("     ELS REBORN     ");
  lcd.setCursor(9,1);
  lcd.print("by");
  lcd.setCursor(0,2);
  lcd.print(" Macheda  Francesco ");
  lcd.setCursor(0,3);
  lcd.print(firmware_version);
  delay(2500);
  pinMode(ENCA, INPUT_PULLUP);        //Econder A input - INTERRUPT
  pinMode(ENCB, INPUT_PULLUP);        //Encoder B input - INTERRUPT

//---------------------------------------------------------- solo per unoR4 ---------------------------------------
  pinMode(ENCZ, INPUT_PULLUP);        //Encoder Z input - INTERRUPT
  pinMode(FC1, INPUT_PULLUP);         //Encoder fine corsa 1 (SX) input - INTERRUPT
  pinMode(FC2, INPUT_PULLUP);         //Encoder fine corsa 2 (DX) input - INTERRUPT 
// --------------------------------------------------------------------------------------------------------------

  pinMode(ESC, INPUT_PULLUP);   //ESC BUTTON (D4 of the sparkfun Joystick shield)
  pinMode(SEL, INPUT_PULLUP);   //SELECTION BUTTON (D5 of the sparkfun Joystick shield)
  pinMode(RESET, INPUT_PULLUP); //RESET BUTTON (D6 of the sparkfun Joystick shield)
  pinMode(STEP, OUTPUT);        //STEP OUTPUT to stepper driver
  pinMode(DIR, OUTPUT);         //DIRECTION OUTPUT to stepper driver
  pinMode(ENABLE, OUTPUT);      //ENABLE OUTPUT to stepper driver

    if (digitalRead(SEL) == PRESSED)
  {
    lcd.setCursor(0,2);
    lcd.print("INIZIO RIPRISTINO?");
    delay(500);
    RipristinaImpostazioni();
  }


  LoadFromEEPROM();     // carica i valori attualmenti salvati nella EEPROM

  CCW = !CW;

  StepperOFF();    // setto i motore come spenti verranno accesi ( e poi spenti) solo quando serve

  
  CheckMotor = true;     // controllo se il carro e' contro i fine corsa
  if(digitalRead(FC1) == LOW)CheckMotor = false;
  if(digitalRead(FC2) == LOW)CheckMotor = false; 

  MandrelSpeed();  // faccio partire la funzione per la velocita' mandrino


}

void loop() 
{

  byte scelta;

  while (true)
  {
    scelta = Principale();
    switch (scelta) {
      case  1: lcd.clear(); MandrelSpeed(); break;
      case  2: lcd.clear(); MovimentoLibero(); break;
      case  3: lcd.clear(); MovimentoLento(); break;      
      case  4: lcd.clear(); AvanzamentoLibero(); break;
      case  5: lcd.clear(); AvanzamentoVincolato(); break;
      case  6: lcd.clear(); Filettatura(); break;
      case  7: lcd.clear(); PosizioneAngolare(); break;
      case  8: lcd.clear(); Impostazioni(); break;
      case  9: lcd.clear(); SalvaImpostazioni();break;
      case 10: lcd.clear(); RipristinaImpostazioni();break;
      case 11: lcd.clear(); TestEMI();break;
    }  
  }
}
