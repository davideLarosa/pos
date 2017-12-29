/*
 * TEST FOR COMMIT
   Sketch used to wirte personal data on mifare classic 1k
   to communicate with Arduiner Pos.
    -----------------------------------------------------------------------------------------
                MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
                Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
    Signal      Pin          Pin           Pin       Pin        Pin              Pin
    -----------------------------------------------------------------------------------------
    RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
    SPI SS      SDA(SS)      10            53        D10        10               10
    SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
    SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
    SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15

*/


#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 5;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 53;     // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance


byte name[34]; // block 1
byte surname[34];// block 4
byte value[34];// block 5
byte freeBuffer[34];// block 2 3
byte block;

const int nameBlock = 1;
const int freeBlock2 = 2;
const int freeBlock3 = 3;
const int surnameBlock = 4;
const int valueBlock = 5;

MFRC522::StatusCode status;
byte len;



void setup() {
  Serial.begin(9600);        // Initialize serial communications with the PC
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
  Serial.println(F("Write personal data on a MIFARE PICC "));
}

void loop() {
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  readPersonalData();

  //TODO remove data writing on value
  value[0] = 1;
  value[1] = 2;

  // Look for new cards
  while ( !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Now waiting for card!");
    delay(500);
  }

  Serial.print(F("Card UID:"));    //Dump UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.print(F(" PICC type: "));   // Dump PICC type
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));



  /****** Block 1 - Name ******/
  //Authenticating with given key
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, nameBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("PCD_Authenticate() success: "));


  // Write block
  status = mfrc522.MIFARE_Write(nameBlock, name, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));
  /****** Block 1 - Name ******/


  /****** Block 2 - FREE ******/

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, freeBlock2, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(freeBlock2, freeBuffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));
  /****** Block 2 - FREE ******/

  /****** Block 3 - FREE ******/
    //second authentication
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, freeBlock3, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(freeBlock3, freeBuffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));
  /****** Block 3 - FREE ******/

  /****** Block 4 - Surname ******/
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, surnameBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(surnameBlock, surname, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));
  /****** Block 4 - Surname ******/



  /****** Block 5 - Value ******/
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, valueBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(valueBlock, value, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));
  /****** Block 5 - Value ******/


  Serial.println(" ");
  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

}//end loop


void readPersonalData() {
  Serial.setTimeout(20000L) ;     // wait until 20 seconds for input from serial
  // Ask personal data: Surname
  Serial.println(F("Type Surname, ending with #"));
  len = Serial.readBytesUntil('#', (char *) surname, 30) ; // read surname name from serial
  for (byte i = len; i < 30; i++) {
    surname[i] = ' ';     // pad with spaces
  }

  // Ask personal data: First name
  Serial.println(F("Type First name, ending with #"));
  len = Serial.readBytesUntil('#', (char *) name, 20) ; // read first name from serial
  for (byte i = len; i < 20; i++) {
    name[i] = ' ';     // pad with spaces
  }

  //fill free buffer with empty spaces
  for (int i = 0; i < sizeof(freeBuffer); i++) {
    freeBuffer[i] = ' ';
    value[i] = ' ';
  }

}

