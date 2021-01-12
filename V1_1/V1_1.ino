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

struct coltyp{
  String colName = "";
  byte Rval=lo;
  byte Gval=lo;
  byte Bval=lo;
  coltyp* fptr = NULL;
  coltyp* bptr = NULL;
};
typedef coltyp* colptr;


coltyp base;
colptr head = &base;
String msg  = "What color do you want to access?";
String msg0 = "0. None";
String msgg = "What RGB value do you want to Change? Red, Green or Blue?";
String msg1 = "Do you want to save this colour?";
String msg2 = "What is the name of this colour?";
String msg3 = "Entering debug mode";
String msg4 = "Not a valid entry";
String col;
int stater = 0;

int stateg = 0;
int stateb = 0;
bool dirr = true;
bool dirg = true;
bool dirb = true;
int incrm = 63;
int j=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(REDPWM,OUTPUT);
  pinMode(GREENPWM,OUTPUT);
  pinMode(BLUEPWM,OUTPUT);
  while (!Serial) {}
  initRGB();
  checkEEPROM();
  j=0;
}

void loop() {
  int eeAddress = 0;
  bool rep = false;
  // put your main code here, to run repeatedly:
  j = 1;
  
  Serial.println(msg); // Main Menu Message basically 
  colptr iter = head;
  Serial.println(msg0); // Skip option
  while(iter != NULL){
    Serial.print(j);
    Serial.print(F(". "));
    Serial.println(iter->colName);
    iter = iter->fptr;
    j++;
  }
  iter = head;
  while(Serial.available()==0){
  }
  col = Serial.readString();
 
  while (iter != NULL && iter->colName != col){
    iter = iter->fptr;
  }
  
  if(iter->colName == col){
    stater = iter->Rval;
    stateg = iter->Gval;    
    stateb = iter->Bval;
    analogWrite(REDPWM,stater);
    analogWrite(GREENPWM,stateg);
    analogWrite(BLUEPWM,stateb);
  }
  else{
    do {
      Serial.println(msgg); // This is it
      while(Serial.available()==0){
      }
      col = Serial.readString();
  
      if (col == "Red" || col == "red"){
        if ((stater == 0 || stater < 252) && dirr == true){
          stater = stater + incrm;
        }
        else if (stater > 0 && dirr == false){
          stater = stater - incrm;
        }
        else if (stater == 252 && dirr == true){
          dirr = false;
          stater = stater - incrm;
        }
        else if (stater == 0 && dirr == false){
          dirr = true;
          stater = stater + incrm;
        }
        analogWrite(REDPWM,stater);
      }
      if (col == "Green" || col == "green"){
        if (stateg < 252&& dirg == true){
          stateg = stateg + incrm;
        }
        else if (stateg > 0 && dirg == false){
          stateg = stateg - incrm;
        }
        else if (stateg == 252 && dirg == true){
          dirg = false;
          stateg = stateg - incrm;
        }
        else if (stateg == 0 && dirg == false){
          dirg = true;
          stateg = stateg + incrm;
        }
        analogWrite(GREENPWM,stateg);
      }
      if (col == "Blue" || col == "blue"){
        if (stateb < 252 && dirb == true){
          stateb = stateb + incrm;
        }
        else if (stateb > 0 && dirb == false){
          stateb = stateb - incrm;
        }
        else if (stateb == 252 && dirb == true){
          dirb = false;
          stateb = stateb - incrm;
        }
        else if (stateb == 0 && dirb == false){
          dirb = true;
          stateb = stateb + incrm;
        }
        analogWrite(BLUEPWM,stateb);
      }
      } while (!(col == "Yes") && !(col != "yes"));
      Serial.print("Red is : ");
      Serial.print(stater);
      Serial.print(". Green is : ");
      Serial.print(stateg);
      Serial.print(". Blue is : ");
      Serial.println(stateb);
      Serial.println(msg1);
      do{
        if (rep == true){
          Serial.println(msg4);
        }
        while(Serial.available()==0){
        }
        col = Serial.readString();
        rep = true;
        Serial.println(col);
        if (col == "no"){
          Serial.println("This should work");
        }
      }while(!(col == "yes") && !(col == "YES") && !(col == "no") && !(col != "NO"));
      if(col == "yes" || col == "YES"){
        colptr temp = new coltyp;
        colptr iter = head;
        while (iter->fptr != NULL){
          iter = iter->fptr;
        }
        Serial.println(msg2);
        while(Serial.available() == 0){
        }
        temp->colName = Serial.readString();
        temp->Rval = stater;
        temp->Gval = stateg;
        temp->Bval = stateb;
        temp->bptr = iter;
        iter->fptr = temp;
        iter = temp;
        EEPROM.put(eeAddress,iter);
      }
      else{
        String tn = "Saved RGB";
        EEPROM.put(eeAddress,tn);
        eeAddress += sizeof(String);
        EEPROM.put(eeAddress,stater);
        eeAddress += sizeof(byte);
        EEPROM.put(eeAddress,stateg);
        eeAddress += sizeof(byte);
        EEPROM.put(eeAddress,stateb);
      }
      delay(2*DELAYTIME);
  }
    // Saves last state to EEPROM memory 
    /*
    EEPROM.put(eeAddress,iter->colName);
    eeAddress += sizeof(String);
    EEPROM.put(eeAddress,iter->Rval);
    eeAddress += sizeof(byte);
    EEPROM.put(eeAddress,iter->Gval);
    eeAddress += sizeof(byte);
    EEPROM.put(eeAddress,iter->Bval);*/
    //EEPROM.put(eeAddress,iter);
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

void initRGB(){

  String colo[] = {"RED","GREEN","BLUE"};
  for (int j = 0; j < 3; j++){
    colptr iter = head;
    while(iter->fptr != NULL){
      iter = iter->fptr;
    }
    colptr temp = new coltyp;
    temp->colName = colo[j];
    if(j == 0){
      temp->Rval = hi;
    }
    else if (j == 1){
      temp->Gval = hi;
    }
    else {
      temp->Bval = hi;
    }
    if(j > 0){
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
  int ii = 0;
  //String testname;
  colptr test = new coltyp;
  EEPROM.get(0,test);
  if (test == 0){
    Serial.println("Right");
  }
  else if((test->colName == "RED") || (test->colName == "GREEN") || (test->colName == "BLUE" || (test->colName =="Saved RGB")) ){
    analogWrite(REDPWM,test->Rval);
    analogWrite(GREENPWM,test->Gval);
    analogWrite(BLUEPWM,test->Bval);
    Serial.println("Wrong");
  }
  else {
    colptr iter = head;
    while(iter->fptr != NULL){
      iter = iter->fptr;
    }
    iter->fptr = test;
    test->bptr = iter;
    iter = test;
    analogWrite(REDPWM,test->Rval);
    analogWrite(GREENPWM,test->Gval);
    analogWrite(BLUEPWM,test->Bval);
    Serial.println("def wrong");
  }
}
