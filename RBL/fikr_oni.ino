#include <LiquidCrystal.h>
//define pin PWM
int OUT11 = 11;
int OUT12 = 10;
int OUT21 = 6;
int OUT22 = 5;
//define pin LCD
int D7 = 12;
int D6 = 8;
int D5 = 7;
int D4 = 4;
int EN = 3;
int RS = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
//define pin Input photodiode
int Left1 = A0;
int Left2 = A1;
int Right1 = A3;
int Right2 = A4;

float left1val = 0;
float right1val = 0;
float left2val = 0;
float right2val = 0;

float sensorkiri1;
float sensorkiri2;
float sensorkanan1;
float sensorkanan2;

int RightSum = 0;
int LeftSum = 0;
int RightOut = 0;
int LeftOut = 0;
//define delay and interval
const unsigned long motorInterval = 100;
unsigned long motorTime;

//Rule Base Kiri
float depankiri [3];
float belakangkiri [3];
float rulekiri [3][3];
float rulekiri00, rulekiri01, rulekiri02;
float rulekiri10, rulekiri11, rulekiri12;
float rulekiri20, rulekiri21, rulekiri22;

//Rule Base Kanan
float depankanan [3];
float belakangkanan [3];
float rulekanan [3][3];
float rulekanan00, rulekanan01, rulekanan02;
float rulekanan10, rulekanan11, rulekanan12;
float rulekanan20, rulekanan21, rulekanan22;

float pwmkanan,pwmkiri;
 
 //hacky method of writing tens and hundreds on LCD
 void LCDOut(){
  lcd.clear();
        if (pwmkiri == 50) {
            lcd.setCursor(0, 0);
            lcd.print("lambat");
        } else if (pwmkiri == 150) {
            lcd.setCursor(0, 0);
            lcd.print("sedang");
        } else if (pwmkiri == 250){
            lcd.setCursor(0,0);
            lcd.print("cepat");
        } else{
            lcd.setCursor(0,0);
            lcd.print(pwmkiri);           
        }

        if (pwmkanan == 50) {
            lcd.setCursor(0, 1);
            lcd.print("lambat");
        } else if (pwmkanan == 150) {
            lcd.setCursor(0, 1);
            lcd.print("sedang");
        } else if (pwmkanan == 250){
            lcd.setCursor(0,1);
            lcd.print("cepat");
        } else{
            lcd.setCursor(0,1);
            lcd.print(pwmkanan);         
        }

 }

 
void FuzzyDepanKanan(){
  // untuk suhu dingin
  if (right1val <= 2.5)
  { depankanan [0] = 1;}
  else if (right1val > 2.5 && right1val <= 3)
  {  depankanan [0] = (3 - right1val)/(3 - 2.5); }
  else
  { depankanan [0] = 0;}
 
  // untuk suhu hangat
  if (right1val <= 2.5)
  { depankanan [1] = 0;}
  else if (right1val > 2.5 && right1val <= 3)
  { depankanan [1] = (right1val-3)/(3-2.5);}
  else if (right1val > 3 && right1val <= 3.5)
  { depankanan [1] = (3.5-right1val)/(3.5 - 3);}
  else
 { depankanan [1] = 0;}
 
  // untuk suhu panas
  if (right1val <= 3)
  { depankanan [2] = 0;}
  else if (right1val > 3 && right1val <= 3.5)
  { depankanan [2] = (right1val-3)/(3.5-3);}
  else
  { depankanan [2] = 1;}
}

void FuzzyBelakangKanan(){
  // untuk suhu dingin
  if (right2val <= 2.5)
  { belakangkanan [0] = 1;}
  else if (right2val > 2.5 && right2val <= 3)
  {  belakangkanan [0] = (3 - right2val)/(3 - 2.5); }
  else
  { belakangkanan [0] = 0;}
 
  // untuk suhu hangat
  if (right2val <= 2.5)
  { belakangkanan [1] = 0;}
  else if (right2val > 2.5 && right2val <= 3)
  { belakangkanan [1] = (right2val-3)/(3-2.5);}
  else if (right2val > 3 && right2val <= 3.5)
  { belakangkanan [1] = (3.5-right2val)/(3.5 - 3);}
  else
 { belakangkanan [1] = 0;}
 
  // untuk suhu panas
  if (right2val <= 3)
  { belakangkanan [2] = 0;}
  else if (right2val > 3 && right2val <= 3.5)
  { belakangkanan [2] = (right2val-3)/(3.5-3);}
  else
  { belakangkanan [2] = 1;}
}

void FuzzyDepanKiri(){
  // untuk suhu dingin
  if (left1val <= 2.5)
  { depankiri [0] = 1;}
  else if (left1val > 2.5 && left1val <= 3)
  {  depankiri [0] = (3 - left1val)/(3 - 2.5); }
  else
  { depankiri [0] = 0;}
 
  // untuk suhu hangat
  if (left1val <= 2.5)
  { depankiri [1] = 0;}
  else if (left1val > 2.5 && left1val <= 3)
  { depankiri [1] = (left1val-3)/(3-2.5);}
  else if (left1val > 3 && left1val <= 3.5)
  { depankiri [1] = (3.5-left1val)/(3.5 - 3);}
  else
 { depankiri [1] = 0;}
 
  // untuk suhu panas
  if (left1val <= 3)
  { depankiri [2] = 0;}
  else if (left1val > 3 && left1val <= 3.5)
  { depankiri [2] = (left1val-3)/(3.5-3);}
  else
  { depankiri [2] = 1;}
}


void FuzzyBelakangKiri(){
  // untuk suhu dingin
  if (left2val <= 2.5)
  { belakangkiri [0] = 1;}
  else if (left2val > 2.5 && left2val <= 3)
  {  belakangkiri [0] = (3 - left2val)/(3 - 2.5); }
  else
  { belakangkiri [0] = 0;}
 
  // untuk suhu hangat
  if (left2val <= 2.5)
  { belakangkiri [1] = 0;}
  else if (left2val > 2.5 && left2val <= 3)
  { belakangkiri [1] = (left2val-3)/(3-2.5);}
  else if (left2val > 3 && left2val <= 3.5)
  { belakangkiri[1] = (3.5-left2val)/(3.5 - 3);}
  else
 { belakangkiri [1] = 0;}
 
  // untuk suhu panas
  if (left2val <= 3)
  { belakangkiri [2] = 0;}
  else if (left2val > 3 && left2val <= 3.5)
  { belakangkiri [2] = (left2val-3)/(3.5-3);}
  else
  { belakangkiri [2] = 1;}
}

void RuleKiri (){
 int i, j;
 float temp;
 for ( i=0; i<=2; i=i+1)
 {
   for ( j=0; j<=2; j=j+1)
   {
     temp = min(depankiri[i], belakangkiri[j]);
     rulekiri [i][j] = temp;
   } 
 } 
 rulekiri00 = rulekiri [0][0]; // (Cepat)
 rulekiri01 = rulekiri [0][1]; // (Cepat)
 rulekiri02 = rulekiri [0][2]; // (Cepat)
 
 rulekiri10 = rulekiri [1][0]; // (Lambat)
 rulekiri11 = rulekiri [1][1]; // (Sedang)
 rulekiri12 = rulekiri [1][2]; // (Cepat)
 
 rulekiri20 = rulekiri [2][0]; // (Lambat)
 rulekiri21 = rulekiri [2][1]; // (Lambat)
 rulekiri22 = rulekiri [2][2]; // (Lambat)
}

void RuleKanan (){
 int i, j;
 float temp;
 for ( i=0; i<=2; i=i+1)
 {
   for ( j=0; j<=2; j=j+1)
   {
     temp = min(depankanan[i], belakangkanan[j]);
     rulekanan [i][j] = temp;
   } 
 } 
 rulekanan00 = rulekanan [0][0]; // (Cepat)
 rulekanan01 = rulekanan [0][1]; // (Cepat)
 rulekanan02 = rulekanan [0][2]; // (Cepat)
 
 rulekanan10 = rulekanan [1][0]; // (Lambat)
 rulekanan11 = rulekanan [1][1]; // (Sedang)
 rulekanan12 = rulekanan [1][2]; // (Cepat)
 
 rulekanan20 = rulekanan [2][0]; // (Lambat)
 rulekanan21 = rulekanan [2][1]; // (Lambat)
 rulekanan22 = rulekanan [2][2]; // (Lambat)
}


float DefuzzyKiri () {
  // metode sugeno (weighted average)
  float lambat = 50;
  float sedang = 150;
  float cepat = 250;
  float defuz,pwm;
  RuleKiri();
  pwm = (rulekiri00 * cepat) + (rulekiri01 * cepat)+ (rulekiri02 *cepat)+ (rulekiri10 * lambat)+ (rulekiri11 * sedang)+ (rulekiri12 * cepat) + (rulekiri20 * lambat)+ (rulekiri21 * lambat)+ (rulekiri22 * lambat);
 
  defuz = 0;
  int i, j;
  for ( i=0; i<=2; i=i+1)
  {
    for ( j=0; j<=2; j=j+1)
    {
      defuz = defuz + rulekiri [i][j];
    } 
  } 
  if (defuz != 0){ 
  pwm = pwm / defuz;
  } else{
    pwm = sedang;
  }
  return pwm;
}

float DefuzzyKanan() {
  // metode sugeno (weighted average)
  float lambat = 50;
  float sedang = 150;
  float cepat = 250;
  float defuz,pwm;
  RuleKanan();
  pwm = (rulekanan00 * cepat) + (rulekanan01 * cepat)+ (rulekanan02 *cepat)+ (rulekanan10 * lambat)+ (rulekanan11 * sedang)+ (rulekanan12 * cepat) + (rulekanan20 * lambat)+ (rulekanan21 * lambat)+ (rulekanan22 * lambat);
 
  defuz = 0;
  int i, j;
  for ( i=0; i<=2; i=i+1)
  {
    for ( j=0; j<=2; j=j+1)
    {
      defuz = defuz + rulekanan [i][j];
    } 
  }
  if (defuz != 0){ 
  pwm = pwm / defuz;
  } else{
    pwm = sedang;
  }
  return pwm;
}

void setup()
{
    lcd.begin(16, 2);
    pinMode(OUT11, OUTPUT); // sets the pin as output
    pinMode(OUT12, OUTPUT); // sets the pin as output
    pinMode(OUT21, OUTPUT); // sets the pin as output
    pinMode(OUT22, OUTPUT); // sets the pin as output
    
    pinMode(Left1, INPUT);
    pinMode(Right1, INPUT);
    pinMode(Left2, INPUT);
    pinMode(Right2, INPUT);

    Serial.begin(9600);
    analogWrite(OUT12, 0);
    analogWrite(OUT22, 0);
}

void loop()
{
    if (millis() - motorTime > motorInterval) {
        motorTime = millis();
  // baca datanya
  
        sensorkiri1 = analogRead(Left1);
        left1val = map(sensorkiri1,0,1023,0,5);

        sensorkiri2 = analogRead(Left2);
        left2val = map(sensorkiri2,0,1023,0,5);

        sensorkanan1 = analogRead(Right1);
        right1val = map(sensorkanan1,0,1023,0,5);

        sensorkanan2 = analogRead(Right2);
        right2val = map(sensorkanan2,0,1023,0,5);

        FuzzyDepanKanan();
        FuzzyBelakangKanan();
        FuzzyDepanKiri();
        FuzzyBelakangKiri();

        pwmkanan = DefuzzyKanan();
        pwmkiri =  DefuzzyKiri();
        
        analogWrite(OUT11, pwmkanan);
        analogWrite(OUT21, pwmkiri);
        LCDOut();
    }
}
