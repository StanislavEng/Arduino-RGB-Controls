#include <EEPROM.h>

#define RED_LED 7
#define GREEN_LED 4
#define BLUE_LED 2
#define DELAYTIME 1000

#define REDPWM 6
#define GREENPWM 5
#define BLUEPWM 3

#define hi 252
#define lo 0

#define MAXSIZE 12

struct coltyp{
  char colName[MAXSIZE];
  byte Rval=lo;
  byte Gval=lo;
  byte Bval=lo;
  coltyp* fptr = NULL;
  coltyp* bptr = NULL;
  int len;
};
typedef coltyp* colptr;


coltyp base;
colptr head = &base;
char menu[] = "What color do you want to access?";
char blnk[] = "0. None";
char chg[] = "What RGB Values do you want to change? Red, Green, or Blue?";
char sav[] = "Do you want to save this colour?";
char nam[] = "What is the name of this colour?";
char debug[] = "Entering debug mode";
char invalid[] = "Not a valid entry";
char myTmp[] = "Last known state";
char col[MAXSIZE];

byte stater = 0;
byte stateg = 0;
byte stateb = 0;
bool dirr = true;
bool dirg = true;
bool dirb = true;
byte incrm = 63;
int j=0;
int selsize;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(REDPWM,OUTPUT);
  pinMode(GREENPWM,OUTPUT);
  pinMode(BLUEPWM,OUTPUT);
  while (!Serial) {}
  initRGB();
  checkEEPROM();
  //myEEPROM();
}

void loop() {
  int eeAddress = 0;
  bool rep = false;
  // put your main code here, to run repeatedly:
  j = 1;
  Serial.println(menu); // Main Menu Message basically 
  colptr iter = head;
  Serial.println(blnk); // Skip option
  while(iter != NULL){
    Serial.print(j);
    Serial.print(F(". "));
    Serial.println(iter->colName);
    iter = iter->fptr;
    j++;
  }
  iter = head;

  while(rep == false){
    while(Serial.available()==0){}
    delay(50);
    rep = readinput();
  }
  rep = false;

  while (iter != NULL && !(strcmp(iter->colName,col)==0)){
    iter = iter->fptr;
    delay(200);
  }
  
  if((strcmp(iter->colName,col)==0)){
    stater = iter->Rval;
    stateg = iter->Gval;    
    stateb = iter->Bval;
    analogWrite(REDPWM,stater);
    analogWrite(GREENPWM,stateg);
    analogWrite(BLUEPWM,stateb);
  }
  else{
    do {
      clr();
      Serial.println(chg); // What RGB Value do you want
      while(rep == false){
        while(Serial.available()==0){}
        delay(50);
        rep = readinput();
      }
      Serial.println(col);
      rep = false;
      if ((strcmp(col,"Red")==0) || (strcmp(col,"red")==0)){
        if ((stater == lo || stater < hi) && dirr == true){
          stater = stater + incrm;
        }
        else if (stater > lo && dirr == false){
          stater = stater - incrm;
        }
        else if (stater == hi && dirr == true){
          dirr = false;
          stater = stater - incrm;
        }
        else if (stater == lo && dirr == false){
          dirr = true;
          stater = stater + incrm;
        }
        analogWrite(REDPWM,stater);
      }
      else if ((strcmp(col,"Green")==0) || (strcmp(col,"green")==0)){
        if (stateg < hi && dirg == true){
          stateg = stateg + incrm;
        }
        else if (stateg > lo && dirg == false){
          stateg = stateg - incrm;
        }
        else if (stateg == hi && dirg == true){
          dirg = false;
          stateg = stateg - incrm;
        }
        else if (stateg == lo && dirg == false){
          dirg = true;
          stateg = stateg + incrm;
        }
        analogWrite(GREENPWM,stateg);
       }
      else if ((strcmp(col,"Blue")==0) || (strcmp(col,"blue")==0)){
        if (stateb < hi && dirb == true){
          stateb = stateb + incrm;
        }
        else if (stateb > lo && dirb == false){
          stateb = stateb - incrm;
        }
        else if (stateb == hi && dirb == true){
          dirb = false;
          stateb = stateb - incrm;
        }
        else if (stateb == lo && dirb == false){
          dirb = true;
          stateb = stateb + incrm;
        }
        analogWrite(BLUEPWM,stateb);
      }
    else {
      Serial.println(F("That was not a valid choice"));
      rep = true;
    }
      clr();
      Serial.println(F("Are you happy with this color?"));
      while(rep == false){
        while(Serial.available()==0){}
        delay(50);
        rep = readinput();
      }
      rep = false;
    } while (!(strcmp(col,"Yes")==0) && !(strcmp(col,"yes")==0));
      Serial.print("Red is : ");
      Serial.print(stater);
      Serial.print(". Green is : ");
      Serial.print(stateg);
      Serial.print(". Blue is : ");
      Serial.println(stateb);
      do{
        clr();
        Serial.println(sav); // DO you want to save this color
        while(rep == false){
          while(Serial.available()==0){}
          delay(100);
          rep = readinput();
        }
        rep = false;
      }while(!(strcmp(col,"Yes")==0) && !(strcmp(col,"yes")==0) && !(strcmp(col,"No")==0) && !(strcmp(col,"no")==0));
      if((strcmp(col,"Yes")==0) || (strcmp(col,"yes")==0)){
        colptr temp = new coltyp;
        coltyp tosave;
        colptr iter = head;
        while (iter->fptr != NULL){
          iter = iter->fptr;
        }
        delay(200);
        Serial.println(nam); // What is the name of this colour
        while(rep == false){
          clr();
          while(Serial.available()==0){}
          delay(50);
          rep = readinput();
        }
        strcpy(temp->colName,col);
        delay(200);
        temp->len = selsize;
        temp->Rval = stater;
        temp->Gval = stateg;
        temp->Bval = stateb;
        iter->fptr = temp;
        temp->bptr = iter;
        iter = temp;
        tosave = *temp;
        ////////////////// EEPROM //////////////////
        EEPROM.put(eeAddress, *temp);
        
      }
      else{
        eeAddress = 0;
        EEPROM.put(eeAddress,myTmp);
        eeAddress += sizeof myTmp;
        delay(50);
        EEPROM.put(eeAddress,stater);
        eeAddress += sizeof(byte);
        delay(50);
        EEPROM.put(eeAddress,stateg);
        eeAddress += sizeof(byte);        
        EEPROM.put(eeAddress,stateb);
        // literally wipes the list if I don't add this 
        colptr iter = head; 
        while (iter->fptr != NULL){
        iter = iter->fptr;
        ////////////////////////////////////////////
        }
      }
      delay(2*DELAYTIME);
  }
    clr();
}
void debugMode(){
  colptr temp = head;
  while (temp != NULL){
    Serial.println(temp->colName);
    Serial.println(temp->Rval);
    Serial.println(temp->Gval);
    Serial.println(temp->Bval);
    delay(1000);
    temp = temp->fptr;
  }
}

void clr(){
  //for (int ii = 0; ii < MAXSIZE; ii++){
    //col[ii] = '\0';
  //}
  memset(col,'\0',MAXSIZE);
}

bool readinput(){
  selsize = Serial.available();
  if (selsize <= MAXSIZE){
    for (int ii = 0; ii < selsize; ii++) {
        col[ii] = Serial.read();
    }
    return true;
  }
  else{
    Serial.println(invalid);
    for (int ii = 0; ii <= selsize; ii++){
      Serial.read();
    }
    return false;
  }
}

void initRGB(){
  char colo[3][10] = {"RED","GREEN","BLUE"};
  for (int jj = 0; jj < 3; jj++){
    colptr iter = head;
    while(iter->fptr != NULL){
      iter = iter->fptr;
      delay(50);
    }
    colptr temp = new coltyp;
    //delay(50);
    if(jj == 0){
      temp->Rval = hi;
      temp->len  = 3;
    }
    else if (jj == 1){
      temp->Gval = hi;
      temp->len  = 5;
    }
    else {
      temp->Bval = hi;
      temp->len  = 4;
    }
    for (int kk = 0; kk <= temp->len; kk++){ 
      temp->colName[kk] = colo[jj][kk];
    }
    if(jj > 0){
      iter->fptr = temp;
      temp->bptr = iter;
      iter = temp;
    }
  else{
      head = temp;
      iter = head;
    }
  }
}
void checkEEPROM(){
  coltyp test;
  EEPROM.get(0,test);
  delay(500);
  if (strcmp(test.colName,"") == 0){
    Serial.println("Nothing Saved");
  }
  else if((strcmp(test.colName, myTmp))==0){
    int eeAddress = 0;
    EEPROM.get(eeAddress,test.colName);
    eeAddress += sizeof myTmp; 
    EEPROM.get(eeAddress,test.Rval);
    eeAddress += sizeof(byte);
    EEPROM.get(eeAddress,test.Gval);
    eeAddress += sizeof(byte);
    EEPROM.get(eeAddress,test.Bval); 
    analogWrite(REDPWM   , test.Rval);
    analogWrite(GREENPWM , test.Gval);
    analogWrite(BLUEPWM  , test.Bval);
    Serial.println(myTmp);
  }
  else {
    colptr iter = head;
    colptr temp = new coltyp;
    temp = &test;
    while (iter->fptr != NULL){
      iter = iter->fptr;
    }
    iter->fptr = temp;
    temp->bptr = iter;
    analogWrite(REDPWM   ,temp->Rval);
    analogWrite(GREENPWM ,temp->Gval);
    analogWrite(BLUEPWM  ,temp->Bval);
    iter = temp;
    delay(100);
    iter = head;
    while(iter != NULL){
      iter = iter->fptr;
    }
  }
}

void myEEPROM(){
  coltyp test;
  //EEPROM.get(0,test);
  int eeAddress = 0;
  Serial.println(F("This should skip"));
  EEPROM.get(eeAddress,test.colName);
  eeAddress += sizeof myTmp; 
  EEPROM.get(eeAddress,test.Rval);
  eeAddress += sizeof(byte);
  EEPROM.get(eeAddress,test.Gval);
  eeAddress += sizeof(byte);
  EEPROM.get(eeAddress,test.Bval); 
  delay(500);
  Serial.println(F("Hello"));
  Serial.println(test.colName);
  Serial.println(test.Rval);
  Serial.println(test.Gval);
  Serial.println(test.Bval);
  
  //return;
}
