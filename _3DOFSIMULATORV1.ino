/////////////////////////////////////
////3 DOF SIMULATOR ///
///Arduino Mega 2560 ///
///Güncelleme: 01/06/2017 16:40///
///Motorlar 1/16 mikrostep te caliscak 1 tur icin 1600 adim  //
//////////////////////////////////////
///step motor baglantilari ///
#define ON_MOTSTEP 4
#define ON_MOTDIR 5
#define SOL_MOTSTEP 6
#define SOL_MOTDIR 7
#define SAG_MOTSTEP 8
#define SAG_MOTDIR 9

/// Butonlar ///
#define SAG_BUTON 22
#define SOL_BUTON 24
#define ONE_YAT_BUTON 26
#define ARKAYA_YAT_BUTON 28
#define YUKSEL_BUTON 30
//////////Role//////
#define ROLE1 32
#define ROLE2 34
///////////////////////////
#define MOT_DINLENME 36 //Tüm motorların EN+ ucları buraya bağlı
#define MOT_DINLENME_LED 39 //
#define AKTIF_LED 37 // Platform aktif çalıstıgını gosteren led
///////////////////////////
#define ASTOP_LED 41 // Acil stop a basıldığında yanacak
//////////////////////////////////////
///sonlandırma switchleri ///
#define ON_MOT_SON_UST 42 // 21 //interrupt2
#define ON_MOT_SON_ALT 44 // 21 //interrupt2
#define SOL_MOT_SON_UST 46 // 20 // interrupt3
#define SOL_MOT_SON_ALT 48 // 20 // interrupt3
#define SAG_MOT_SON_UST 50 // 3 // interrupt1
#define SAG_MOT_SON_ALT 52 // 3 // interrupt1
#define ACIL_STOP 2 //interrupt0



//////////////////////////////
#include <EEPROM.h>
int m1adres=10;
int m2adres=20;
int m3adres=30;
////////////////////////
////////////////////////
long max_m=12000;
long orta_m=8000;
long min_m=4000;

long max_m1=12000;
long orta_m1=8000;
long min_m1=4000;

long max_m2=12000;
long orta_m2=8000;
long min_m2=4000;

long max_m3=12000;
long orta_m3=8000;
long min_m3=4000;

//////adim zaman degiskeni//////
unsigned long zm1=0; 
unsigned long zm2=0;
unsigned long zm3=0;
/////////////////////////
////////////////////////
boolean yuksel_butona_basildi=false;
boolean as=1; //acil stop
boolean ms=1; //motor stop
/////////////////////////////
///degiskenler///
///HIZ AYARI////
int hyavas=700;
int horta=300;
int hhizli=200;
int hiz_degeri=512;
//////////////
long m1=orta_m,m2=orta_m,m3=orta_m;//adımsal konum m1 on motor, m2 sol motor, m3 sag motor
//long m1deger=8000;
//Her motor 1600 adımda bir tur atıyor, reduktor orani 1/20 
// kol 20 turda 1 tur donuyor
// kol 10 turda yarım tur donuyor
// kol 5 turda 90 derece hareket ediyor. 1600*5=8000 m1=8000 ise kol 90 derece de demektir 8000/90=> yaklaşık 88 adımda 1 derece dönüyür kol.
// 180 derece için m1=16000 adımda, 0 derece için m1=0 adimda
////////////////
int pot_degeri=0;
////////////////////////
void adimat_ON(){
  while((micros()-zm1)<=horta);
  digitalWrite(ON_MOTSTEP,HIGH); 
     delayMicroseconds(horta);
    digitalWrite(ON_MOTSTEP,LOW);
     zm1=micros();
}
void adimat_SAG(){
   while((micros()-zm3)<=horta);
  digitalWrite(SAG_MOTSTEP,HIGH); 
     delayMicroseconds(horta);
    digitalWrite(SAG_MOTSTEP,LOW);
      zm3=micros();
}
void adimat_SOL(){
  while((micros()-zm2)<=horta);
  digitalWrite(SOL_MOTSTEP,HIGH); 
     delayMicroseconds(horta);
    digitalWrite(SOL_MOTSTEP,LOW);
      zm2=micros();
}
//////////////////////////////////
////////////////////////
void adimat_ON1(){
  digitalWrite(ON_MOTSTEP,HIGH); 
     delayMicroseconds(horta);
    digitalWrite(ON_MOTSTEP,LOW);
     delayMicroseconds(horta);
}
void adimat_SAG1(){
  digitalWrite(SAG_MOTSTEP,HIGH); 
     delayMicroseconds(horta);
    digitalWrite(SAG_MOTSTEP,LOW);
     delayMicroseconds(horta);
}
void adimat_SOL1(){
  digitalWrite(SOL_MOTSTEP,HIGH); 
     delayMicroseconds(horta);
    digitalWrite(SOL_MOTSTEP,LOW);
     delayMicroseconds(horta);
}
//////////////////////////////////
void EEPROMyaz(int address, long value)
      {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, four);
      EEPROM.write(address + 1, three);
      EEPROM.write(address + 2, two);
      EEPROM.write(address + 3, one);
      }
 ///////////////////////////////////////
 long EEPROMoku(long address)
      {
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(address);
      long three = EEPROM.read(address + 1);
      long two = EEPROM.read(address + 2);
      long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
      }
/////////////////////////////////////////////
void kapanis(){
   EEPROMyaz(m1adres, m1);
   EEPROMyaz(m2adres, m2);
   EEPROMyaz(m3adres, m3);
}

//////////////////////////////////
void acilis(){
   m1=EEPROMoku(m1adres);
   m2=EEPROMoku(m2adres);
   m3=EEPROMoku(m3adres);
  
}
/////////////////////////////////
void fan_kontrol(){
  if(m1>orta_m1){digitalWrite(ROLE1,LOW);digitalWrite(ROLE2,HIGH);}else{digitalWrite(ROLE1,LOW);digitalWrite(ROLE2,LOW);}
}
void fan_kapa(){
  digitalWrite(ROLE1,HIGH);digitalWrite(ROLE2,HIGH);
}
////////////////////////////////
void Acil_Stop(){
  as=1;
  fan_kapa();
  kapanis();
  digitalWrite(ASTOP_LED,HIGH);
  motorlar_pasif();
  }
void Acil_Stop_off(){
  as=0;
  acilis();
  digitalWrite(ASTOP_LED,LOW);
 motorlar_pasif();
}
//////////////////////////////////////////
void on_kalk(){
  if(m1<max_m1){
    digitalWrite(ON_MOTDIR,HIGH); //kol YUKARI
    if(as==0){
      if(digitalRead(ON_MOT_SON_UST)==HIGH){
      adimat_ON();
    m1=m1+1;
      }else{m1=max_m1+10; }
    }
       }
}
///////////////////////////////////////////
void sol_kalk(){
   if(m2<max_m2){
    digitalWrite(SOL_MOTDIR,HIGH); //kol YUKARI
     if(as==0){
       
      if(digitalRead(SOL_MOT_SON_UST)==HIGH){
      adimat_SOL();
    m2=m2+1;
      }else{m2=max_m2+10; }
     }
       }
}
/////////////////////////////////////
void sag_kalk(){
   if(m3<max_m3){
    digitalWrite(SAG_MOTDIR,HIGH); //kol YUKARI
     if(as==0){
      if(digitalRead(SAG_MOT_SON_UST)==HIGH){
      adimat_SAG();
    m3=m3+1;
      }else{m3=max_m3+10; }
     }
       }
}
/////////////////////////////////////
void on_in(){
   if(m1>min_m1){
    digitalWrite(ON_MOTDIR,LOW); //kol aşagi
     if(as==0){
      if(digitalRead(ON_MOT_SON_ALT)==HIGH){
    adimat_ON();
    m1=m1-1;
      }else{m1=min_m1-150; }
     }
    }
}
/////////////////////////////////////
void sol_in(){
   if(m2>min_m2){
    digitalWrite(SOL_MOTDIR,LOW); //kol aşagi
     if(as==0){
      if(digitalRead(SOL_MOT_SON_ALT)==HIGH){
    adimat_SOL();
    m2=m2-1;
      }else{m2=min_m2-150; }
     }
   }
}
/////////////////////////////////////
void sag_in(){
   if(m3>min_m3){
    digitalWrite(SAG_MOTDIR,LOW); //kol aşagi
     if(as==0){
   if(digitalRead(SAG_MOT_SON_ALT)==HIGH){
    adimat_SAG();
    m3=m3-1;
      }else{m3=min_m3-150; }
     }
   }
}
////////////////////////////////////////
void motorlar_pasif(){
    ms=1;
    digitalWrite(MOT_DINLENME, HIGH); 
    digitalWrite(MOT_DINLENME_LED, HIGH);
    digitalWrite(AKTIF_LED, LOW); 
    delay(100);
}
void motorlar_aktif(){
    ms=0;
    digitalWrite(MOT_DINLENME, LOW); //Tüm motorların EN+ negatif yap akımı ver
    digitalWrite(MOT_DINLENME_LED, LOW);
    digitalWrite(AKTIF_LED, HIGH); //aktif ledini yak
    delay(500);// motorlara akım verildiğinde yarım saniye bekle.
}
void motor_akim_kontrol(){
  if((micros()-zm2)>60000000){ // motorlar 1 dak boyunca hareket etmedi ise akımı kes
    motorlar_pasif();
  }
  while(ms){
    if(digitalRead(YUKSEL_BUTON)==HIGH){ motorlar_aktif();}
     if(digitalRead(SOL_BUTON)==HIGH){ motorlar_aktif();}
      if(digitalRead(SAG_BUTON)==HIGH){ motorlar_aktif();}
       if(digitalRead(ONE_YAT_BUTON)==HIGH){ motorlar_aktif();}
        if(digitalRead(ARKAYA_YAT_BUTON)==HIGH){ motorlar_aktif();}
    
    
    
    delay(30);
    
    } // butonlardan herhangi birine basılana kadar bekle

    
}
///////////////////////////////////////
void setup() {
  Serial.begin(9600);
  
   pinMode(ON_MOTSTEP,OUTPUT);
   pinMode(ON_MOTDIR,OUTPUT);
   pinMode(SOL_MOTSTEP,OUTPUT);
   pinMode(SOL_MOTDIR,OUTPUT);
   pinMode(SAG_MOTSTEP,OUTPUT);
   pinMode(SAG_MOTDIR,OUTPUT);

   pinMode(ROLE1,OUTPUT);
   pinMode(ROLE2,OUTPUT);
   pinMode(MOT_DINLENME,OUTPUT);
   pinMode(MOT_DINLENME_LED,OUTPUT);
   pinMode(AKTIF_LED,OUTPUT);
   pinMode(ASTOP_LED,OUTPUT);
   

   pinMode(SAG_BUTON,INPUT); 
   pinMode(SOL_BUTON,INPUT); 
   pinMode(ONE_YAT_BUTON,INPUT);
   pinMode(ARKAYA_YAT_BUTON,INPUT);
   pinMode(YUKSEL_BUTON,INPUT); 

   pinMode(ON_MOT_SON_UST,INPUT_PULLUP); //DAHİLİ PULLUP DİRENCİNİ KULLANCAK, PİNLER BOŞTAİKEN HIGH DURUMDA
   pinMode(SOL_MOT_SON_UST,INPUT_PULLUP); 
   pinMode(SAG_MOT_SON_UST,INPUT_PULLUP);
   pinMode(ON_MOT_SON_ALT,INPUT_PULLUP); 
   pinMode(SOL_MOT_SON_ALT,INPUT_PULLUP); 
   pinMode(SAG_MOT_SON_ALT,INPUT_PULLUP); 
   pinMode(ACIL_STOP,INPUT); 
  
digitalWrite(ON_MOTSTEP,LOW);
digitalWrite(SOL_MOTSTEP,LOW);
digitalWrite(SAG_MOTSTEP,LOW);

digitalWrite(ON_MOTDIR,LOW);
digitalWrite(SOL_MOTDIR,LOW);
digitalWrite(SAG_MOTDIR,LOW);

digitalWrite(ROLE1,HIGH);
digitalWrite(ROLE2,HIGH);
digitalWrite(AKTIF_LED, LOW); //aktif ledini kapa
digitalWrite(MOT_DINLENME, HIGH); //
digitalWrite(MOT_DINLENME_LED, HIGH);
digitalWrite(ASTOP_LED,LOW);

attachInterrupt(0, Acil_Stop, FALLING);
acilis();
}

void loop() {
  /////////////////////////////////////////////////////////////
 
  /////////////////////////////////////////////////////////////
  if(as==1){// Acil stopa basılmışsa 
while(!digitalRead(ACIL_STOP)){delay(100);} // buton de aktif yapılana kada bekle

    if(digitalRead(ACIL_STOP)==HIGH){Acil_Stop_off();}
  }
  /////////////////////////Dinlenme modu kontrol//////////////////////////////////////
   motor_akim_kontrol();
   //////////HIZ DEGERINI OKU////////////////////////////////////
hiz_degeri=analogRead(A0);
horta=map(hiz_degeri,90,1023,220,600);
  //////////YUKSEL BUTONU/////////////////////////////////////////////////////////////////////////////////////////////////////
   if(digitalRead(YUKSEL_BUTON)==HIGH){ //butona basıldı ise
    delay(30);
    yuksel_butona_basildi=true;
    for(int i=0;i<4000;i++){
          on_kalk();
         
          sol_kalk();
          
          sag_kalk();
    }
  }//YUKSEL BUTONA BASILDI ISE IF SONU
else{
 
  /////////////orta konuma gel////////////
  if( yuksel_butona_basildi==true){
  digitalWrite(ON_MOTDIR,LOW); //kol aşagi
      digitalWrite(SOL_MOTDIR,LOW); //kol aşagi
       digitalWrite(SAG_MOTDIR,LOW); //kol aşagi
  for(int i=0; i<(max_m-orta_m); i++){
    if(m1>orta_m1){ if(as==0){if(digitalRead(ON_MOT_SON_ALT)==HIGH){  adimat_ON();    m1=m1-1; }else{m1=min_m1-150; }  } }
    if(m2>orta_m2){  if(as==0){if(digitalRead(SOL_MOT_SON_ALT)==HIGH){ adimat_SOL();    m2=m2-1; }else{m2=min_m2-150; } }  }
    if(m3>orta_m3){  if(as==0){if(digitalRead(SAG_MOT_SON_ALT)==HIGH){ adimat_SAG();    m3=m3-1; }else{m3=min_m3-150; } }  }
     if(digitalRead(YUKSEL_BUTON)==HIGH){ yuksel_butona_basildi=false; break;}
  }
   yuksel_butona_basildi=false;
}

  //////////ONE YAT ARKAYA YAT  BUTONU//////////////////////////////////////////////////////////////////////////////////////////////////
  if(digitalRead(ONE_YAT_BUTON)==HIGH){ // butona basıldı ise
    on_in();
    fan_kontrol();
    //////////------Aynı anda sol ve sag motorun hareketi ters yonde olmalı
    // sol ve sag motor aynı anda yukarı hareket etcek //
   if(digitalRead(SAG_BUTON)==LOW){ sag_kalk();}
   if(digitalRead(SOL_BUTON)==LOW){ sol_kalk();}
       
    /////////-------
  }
  else if(digitalRead(ARKAYA_YAT_BUTON)==HIGH){
     on_kalk();
   fan_kontrol();
   ///////////////////------------
    if(digitalRead(SAG_BUTON)==LOW){ sol_in();}
    if(digitalRead(SOL_BUTON)==LOW){ sag_in();}
   //////////////////----------
     
    
  }
else{ //one yat arkaya yat butona basılmamışsa
  if(abs(m1-orta_m1)>3){
//////////
if(m1>orta_m1){
  digitalWrite(ON_MOTDIR,LOW); //kol aşagi
   if(as==0){
    if(digitalRead(ON_MOT_SON_ALT)==HIGH){
  adimat_ON();
    m1=m1-1;
    }
   }
    fan_kontrol();
}else{
   digitalWrite(ON_MOTDIR,HIGH); //kol yukari
    if(as==0){
  if(digitalRead(ON_MOT_SON_UST)==HIGH){
  adimat_ON();
    m1=m1+1;
  }
    }
    fan_kontrol();
}
/////////
 }
  
  ////////////////////..................../////////////////////
  ////////////////SAG SOL BUTON///////////////////////////////////////////////////////////////////////////////////////////////
  if(digitalRead(SAG_BUTON)==HIGH){ //sağ butona basıldı ise
   sag_in();
   sol_kalk();
  } 
  ///////////////////////////////
  //////////***////////////////
  else if(digitalRead(SOL_BUTON)==HIGH){ //sol butona basıldı ise
    sol_in();
    sag_kalk();
  }
  
  ///////////////////////////////
  else{ //sag veya sol butona basilmamis veya birakilmis durumda kendini dengele m2==m3 yap
    if(abs(m2-orta_m2)>3){
if(m2>orta_m2){
   sol_in();
  }else{
     sol_kalk();
      }//else sonu
     }
     /////////////00000
     if(abs(m3-orta_m3)>3){
if(m3<orta_m3){
    sag_kalk();
  }else{
     sag_in();
      }//else sonu
     }
     ///////////0000000
    } // sag sol butonuna basılmamışsa else sonu
    ////////.......................////////////////////////
     }  //one yat arkaya yat else sonu
}//YUKSEL BUTONA BASILDI ISE else sonu
    ////////////////////////////////////////////////////////////////////////////////////////
 /*
Serial.print("m1:");
Serial.print(m1);
Serial.print("......m2:");
Serial.print(m2);
Serial.print("......m3:");
Serial.print(m3);
Serial.print(".....as:");
Serial.println(as);
delay(10);

Serial.println(hiz_degeri);
Serial.println(".........");
Serial.println(horta);
delay(100);
*/
//////////////////////////////////////
//////////////demo////////////////////
 if(digitalRead(ONE_YAT_BUTON)==HIGH && digitalRead(ARKAYA_YAT_BUTON)==HIGH){
  delay(6000);
  for(int i=0; i<4000; i++){
    on_kalk();
    sol_in();
    sag_in();
    
    } 
    delay(10000);
    for(int i=0; i<4000; i++){
    on_in();
    sol_kalk();
    sag_kalk();
    
    } 
    delay(10000);
     for(int i=0; i<4000; i++){  //sagdon
    
    sol_kalk();
    sag_in();
    
    } 
    delay(3000);
    for(int i=0; i<4000; i++){  //
    
    sol_in();
    sag_kalk();
    
    } 
    delay(8000);
    for(int i=0; i<4000; i++){  //soladon
    
    sag_kalk();
    sol_in();
    
    } 
    delay(3000);
    for(int i=0; i<4000; i++){  //
    
    sag_in();
    sol_kalk();
    
    } 
    delay(8000);
 }
///////////demo son///////////////////
}//loop sonu


