//#include <ESP_EEPROM.h>
//#define EEPROM_SIZE 18
int eepromDefault[8][5] = { 
{0, 0, 0, 0, 0},
{0, 0, 0, 0, 0},
{0, 0, 0, 0, 0},
{0, 0, 0, 0, 0},
{0, 0, 0, 0, 0},
{0, 0, 0, 0, 0},
{0, 0, 0, 0, 0},
{0, 0, 0, 0, 0}
};
int defaultsLoaded;
int eepromVar1[8][5];

//const int COL = 5;
int calcAdd(int stt, int row, int col){
 // int address = (stt + 4*((row+1)*(col+1) + row));
 int address = stt*4 + (row * 5 + col) * sizeof(int);
  return address; 
}
void writeIntArrayToEEPROM(int addr, int (&arr)[8][5]){
  EEPROM.put(addr, arr);
  boolean ok2 = EEPROM.commit();
 // Serial.println((ok2) ? "Int Array Write commit OK" : "Commit failed");
}

//void writeIntArrayToEEPROM(int addr, int arr[][COL], int row, int col) {
//  for (int i = 0; i < row; i++) {
//    for (int ii = 0; ii< col; ii++){
//     EEPROM.write((addr + i * col + ii), arr[i][ii]);
//  }
//  }
//  boolean ok2 = EEPROM.commit();
//  Serial.println((ok2) ? "Int Array Write commit OK" : "Commit failed");
//}

void writeIntToEEPROM(int addr, int val) {
    EEPROM.put(addr, val);
  boolean ok3 = EEPROM.commit();
  Serial.println((ok3) ? "Int Write commit OK" : "Commit failed");
  }
  
void CheckforDefaults(){
Serial.print("Check for Initial commit: ");
defaultsLoaded = EEPROM.read(0);
//defaultsLoaded = readIntFromEEPROM(0);
if (defaultsLoaded == 1){
Serial.println("Initial Commit Done");
}
 else {
  Serial.println("loading Defaults");
  defaultsLoaded = 1;
  writeIntToEEPROM(0, defaultsLoaded);

  delay(50);
 // writeIntArrayToEEPROM(1, eepromDefault, 8, 5);
  writeIntArrayToEEPROM(4, eepromDefault);
 }
}

//void readIntArrayFromEEPROM(int addr, int arr[][COL], int row, int col) {
//  for (int i = 0; i < row; i++) {
//    for (int j = 0; j < col; j++) {
//      unsigned char val = EEPROM.read(addr + i * col + j);
//      arr[i][j] = static_cast<int>(val);
//    }
//  }
//}


  
//void setup() {
////int aNewVar1 = new int[8][5];
//int aNewVar1[8][5];  
//  Serial.begin(74880);
//  EEPROM.begin(EEPROM_SIZE);
//CheckforDefaults();
////readIntArrayFromEEPROM(1, eepromVar1, 8, 5);
//// int confirmDefaults;
//// delay(100);
//// confirmDefaults = EEPROM.read(0);
////Serial.println("initial values");
//// for (int i = 0; i < 8; i++) {
////    for (int ii = 0; ii< 5; ii++){
////    Serial.println(eepromVar1[i][ii]);  
////    }
//// }
// //manipulate data to see if saving...
////  eepromVar1[0][0] = eepromVar1[0][0] + 1;;
////  eepromVar1[3][3] = 33;
////  eepromVar1[3][3] = 2* eepromVar1[0][0];
////  eepromVar1[7][4] = 888;
//
//  writeIntArrayToEEPROM(1, eepromVar1, 8, 5);
//  delay(200);
//  readIntArrayFromEEPROM(1, aNewVar1, 8, 5);
////  Serial.println("Read back the array from EEPROM: ");
// for (int i = 0; i < 8; i++) {
//    for (int ii = 0; ii< 5; ii++){
//  //  Serial.println(aNewVar1[i][ii]);  
//  }
// }
//int chkDefault;
////chkDefault = readIntFromEEPROM(0);
//chkDefault = EEPROM.read(0);
////Serial.print("chk persistant default ");
////Serial.println(chkDefault);
//
// // delete[][] aNewVar1;  // free the memory allocated for the array
//}
//
