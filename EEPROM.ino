/* EEPROM factory defaults and parameters to be stored
 * 
 * PARAMETER               |TYPE          |SIZE  |STARTING LOCATION |FACTORY DEFAULT
 * one_turn_enc_steps      | long         |4     |0                 |600
 * tau_man_enc             | float        |4     |4                 |2.307692
 * screw_pitch             | float        |4     |8                 |1.40
 * one_turn_screw_steps    | long         |4     |12                |400
 * Acceleration            | long         |4     |16                |20500
 * Deceleration            | long         |4     |20                |20500
 * AccMan                  | long         |4     |24                |5000
 * CW                      | bool         |1     |28                |LOW
 * gioco                   | float        |4     |29                |0.80
 * MaxStepperSpeed         | long         |4     |33                |700 
 * DefLenghtVite           | float        |4     |37                |10.00
 * DefDirVite              | bool         |1     |41                |LOW
 * DefPassoVite            | float        |4     |42                |1.00  
 * DefMinDist              | float        |4     |46                |0.10
 * vsjx                    | long         |4     |50                |1
 * vsjy                    | long         |4     |54                |1
 * IfZA                    | bool         |1     |58                |HIGH
 * FREE                    |              |      |59                | 
*/
#define one_turn_enc_steps_default        600      // variato si inserisce il numero di step/giro dell'encoder usato
#define tau_man_enc_default               2.307692 // rapporto trasmissione tra mandrino  ed encoder inserito per considerare anche rapporti non interi
#define screw_pitch_default               1.40     // passo vite del carro in mm/giro
#define one_turn_screw_steps_default      400      // motore stepper collegato half step direttamente
#define Acceleration_default              20500    // accelerazione motore stepper in steps/s^2
#define Deceleration_default              20500    // decelerazione motore stepper in steps/s^2
#define AccMan_default                    5000     // ritardo per consentire al mandrino di raggiungere la velocità costante
#define CW_default                        LOW      // variabile che controlla il senso di rotazione dello stepper
#define gioco_default                     0.80     // backslash della vite del carro in mm
#define MaxStepperSpeed_default           700      // Max speed allowed to the stepper motor rpm
#define DefLenghtVite_default             10.00    // lunghezza  da filettare in mm
#define DefDirVite_default                LOW      // LOW destrorsa   HIGH sinistrorsa
#define DefPassoVite_default              1.00     // passo filettatura in mm
#define DefMinDist_default                0.10     // distanza minima da percorrere in movimento libero e avanzamento libero ( oltre alla parte di acc e decelerazione )
#define vsjx_default                      1        // verso di utilizzo asse x del joystick
#define vsjy_default                      1        // verso di utilizzo asse y del joystick
#define IfZA_default                      HIGH     // tipo di controllo per inizio filettatura , LOW canale Z , HIGH canale A



byte SalvaImpostazioni()    // salva i valori correnti nella EEPROM
{
  byte current = 1;

  EEPROM.put(0, one_turn_enc_steps);
  EEPROM.put(4, tau_man_enc);                   
  EEPROM.put(8, screw_pitch);                      
  EEPROM.put(12, one_turn_screw_steps);                     
  EEPROM.put(16, Acceleration);                     
  EEPROM.put(20, Deceleration);                    
  EEPROM.put(24, AccMan);                       
  EEPROM.put(28, CW);      
  EEPROM.put(29, gioco);                   
  EEPROM.put(33, MaxStepperSpeed);                  
  EEPROM.put(37, DefLenghtVite);                  
  EEPROM.put(41, DefDirVite);      
  EEPROM.put(42, DefPassoVite);
  EEPROM.put(46, DefMinDist);
  EEPROM.put(50, vsjx); 
  EEPROM.put(54, vsjy);
  EEPROM.put(58, IfZA);               

  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("   Setup attuale    ");
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print("      salvato       ");

  delay(2000);

  return current;
}


byte RipristinaImpostazioni()   // restore the factory default value in the EEPROM
{
  byte current = 1;
  
  long UI;
  float F;
//  byte B;
  bool BOO;
  String C;

  //RESTORES FACTORY DEFAULT PARAMETERS IN EEPROM
  UI = one_turn_enc_steps_default;   
  EEPROM.put(0, UI);                     
  F = tau_man_enc_default;               
  EEPROM.put(4, F);                      
  F = screw_pitch_default;               
  EEPROM.put(8, F);                      
  UI = one_turn_screw_steps_default;     
  EEPROM.put(12, UI);                     
  UI = Acceleration_default;        
  EEPROM.put(16, UI);                     
  UI = Deceleration_default;        
  EEPROM.put(20, UI);                    
  UI = AccMan_default;               
  EEPROM.put(24, UI);                       
  BOO = CW_default;         
  EEPROM.put(28, BOO);      
  F = gioco_default;          
  EEPROM.put(29, F);                   
  UI = MaxStepperSpeed_default;         
  EEPROM.put(33,UI);                  
  F = DefLenghtVite_default;          
  EEPROM.put(37, F);                  
  BOO = DefDirVite_default;         
  EEPROM.put(41, BOO);      
    F = DefPassoVite_default;          
  EEPROM.put(42, F);              
    F = DefMinDist_default;          
  EEPROM.put(46, F);              
  UI = vsjx_default;
  EEPROM.put(50, UI);
  UI = vsjy_default;
  EEPROM.put(54, UI);
  BOO = IfZA_default;
  EEPROM.put(58, BOO);              

  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(" Setup Ripristinato ");
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print(" Valori di default  ");

  delay(2000);

  return current;

}

void LoadFromEEPROM() //Loads the parameters from the EEPROM and put them in the proper variables
{

  EEPROM.get(0, one_turn_enc_steps); 
  EEPROM.get(4, tau_man_enc);                    
  EEPROM.get(8, screw_pitch);                      
  EEPROM.get(12, one_turn_screw_steps);                     
  EEPROM.get(16, Acceleration);                     
  EEPROM.get(20, Deceleration);                    
  EEPROM.get(24, AccMan);                       
  EEPROM.get(28, CW);      
  EEPROM.get(29, gioco);                   
  EEPROM.get(33,MaxStepperSpeed);                  
  EEPROM.get(37, DefLenghtVite);                  
  EEPROM.get(41, DefDirVite);      
  EEPROM.get(42, DefPassoVite);
  EEPROM.get(46, DefMinDist);
  EEPROM.get(50, vsjx);
  EEPROM.get(54, vsjy); 
  EEPROM.get(58, IfZA);
  freou = int(DefMinDist*float(one_turn_screw_steps)/screw_pitch);              // definisce ogni quanti step eseguire il controllo nella routine StepToLibe

}
