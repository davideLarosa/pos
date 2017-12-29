/*
   Sketch used to read personal data on mifare classic 1k
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

void setup() {
  Serial.begin(9600);                                           // Initialize serial communications with the PC
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
}


void loop() {

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Look for new cards
  while ( !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Waiting for card");
    delay(500);
  }

  Serial.println(F("**Card Detected:**"));

  //-------------------------------------------

  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex

  //-------------------------------------------

  Serial.print(F("Surname: "));

  byte buffer1[18];
  byte buffer2[18];
  byte buffer3[18];
  byte buffer4[18];
  byte buffer5[18];
  block = 4;
  len = 18;

  //------------------------------------------- GET FIRST NAME
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  //PRINT FIRST NAME
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer4[i] != 32)
    {
      Serial.write(buffer1[i]);
    }
  }
  Serial.print(" ");



  //----------------------------------------

  Serial.println(F("\n**End Reading**\n"));

  delay(1000); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(5000);
}




