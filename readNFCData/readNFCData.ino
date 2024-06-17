#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_IRQ   (2)
#define PN532_RESET (3)

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup(void) {
  Serial.begin(9600);
  Serial.println("This is an NFC Scanner");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("PN532 Board Not Found");
    while (1); //Program Stop
  }
  //Version Data not Null
  Serial.print("Found chip PN532"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware version "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  nfc.SAMConfig();
  
  Serial.println("Hover the Card near the NFC reader...");
}


void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  //A placeholder for the 4 or 7 bytes UID card
  uint8_t uidLength;                      
    
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    Serial.println("Card Found");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    
    if (uidLength == 4){
      Serial.println("4 Byte UID card Found");
   
      Serial.println("Trying to authenticate block 4 with default KEYA value");
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; // 255 255 255 255 255 255
   
     success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 8, 0, keya);
   
      if (success){
        Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
        uint8_t data[16];
  
        success = nfc.mifareclassic_ReadDataBlock(8, data);
  
        if (success){
          Serial.println("Reading Block 4 ");
          nfc.PrintHexChar(data, 16);
          Serial.println("");
        delay(1000);
        }
        else{
          //block 4 is authenticated but can't be read
          Serial.println("Block 4 unable to be read, please try again with another key");
        }
      }
      else{
        //block 4 failed to be authenticated by the generic key
        Serial.println("Authentication Failed, please try again with another key");
      }
    }
    
    if (uidLength == 7){
      Serial.println("7 byte UID card found");
   
      //page 4 is a general purpose page, usually for card information
      Serial.println("Reading page 4");
      uint8_t data[32];
      success = nfc.mifareultralight_ReadPage (4, data);
      if (success)
      {
        // Data is sucessfully read
        nfc.PrintHexChar(data, 4);
        Serial.println("");
  
        //Give delay before reading the card
        delay(1000);
      }
      else{
        Serial.println("Unable to read the page, please try again");
      }
    }
  }
}