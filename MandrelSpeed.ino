const char VM0[]  = " VELOCITA' MANDRINO ";
const char VM1[]  = "giri al min:        ";
const char VM2[]  ="            ESC:fine ";
const char* const MenuVelocitaMandrino[] = {VM0, VM1, VM2};

 

byte MandrelSpeed() 
{
  byte current;
  bool stay = true;        //boolean to determine if stay or not in the reading cycle
  int ManSpeed = 0;    //speed of the mandrel calculated
  int MedSpe[10];          // vettore per media mobile velocita mandrino
  int imed;                // indice per caricare la velocita nel vettore MedSpe
  int isum;                // indice per fare la media delle velocita
  int SumSpe;              // somma per calcolare la velocita media

  current=1;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(MenuVelocitaMandrino[0]);
  lcd.setCursor(0,2);
  lcd.print(MenuVelocitaMandrino[1]);
  lcd.setCursor(0,3);
  lcd.print(MenuVelocitaMandrino[2]);

  stay = true;

  MedSpe[0] = GetSpeed(duve);
  for ( imed = 1; imed <= 9; imed++) 
  {

    MedSpe[imed]=MedSpe[0];

  }
  
  imed=0;
  
  while (stay)
  {
    if (digitalRead(ESC) == PRESSED) return current;


//    ManSpeed = GetSpeed(duve);

    MedSpe[imed] = GetSpeed(duve);
    imed = imed +1;
    if(imed == 10)imed=0;

    SumSpe=0;
    for ( isum = 0; isum <= 9; isum++) 
    {

    SumSpe=SumSpe+MedSpe[isum];

    }
    
    ManSpeed = SumSpe/10;                    // velocita come media mobile delle ultime 10 letture

    lcd.setCursor (13,2);
    lcd.print("       ");
    lcd.setCursor (13,2);
    lcd.print(ManSpeed);
 
  }

  return current;

}
