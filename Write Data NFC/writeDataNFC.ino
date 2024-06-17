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
    while (1); // program stop
  }
  //Data found
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  nfc.SAMConfig();
  
  Serial.println("Hover the Card near the NFC reader...");
}


void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;                        
    
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    Serial.println("Card Found");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    
    if (uidLength == 4){
      // 4 byte card (mifare classic) 
      Serial.println("4 Byte UID card Found");
   
      Serial.println("Trying to authenticate block 8 with default KEYA value"); //block 8 usually is the 1st block to be written on 
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
     success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 8, 0, keya);
   
      if (success)
      {
        Serial.println("Sector 2 (Blocks 8..11) has been authenticated"); //data usually inserted from sector 2 onwards
        uint8_t data[16];

        Serial.println("Writing Block 8:...");
        //Writing Data into block 8
        memcpy(data, (const uint8_t[]){ 'B','I','N','U','S','2','0','2','4', 0, 0, 0, 0, 0, 0 ,0}, sizeof data);
        success = nfc.mifareclassic_WriteDataBlock (8, data); //blok 8
        Serial.println("Data is being written, keep hovering the card...");
        Serial.println("");
        
        // test write data
        success = nfc.mifareclassic_ReadDataBlock(8, data);
  
        if (success){
          //sucessful read
          Serial.println("This is what's written in block 8 : ");
          nfc.PrintHexChar(data, 16);
          Serial.println("");

          delay(1000);
        }
        else{
          Serial.println("Block 8 unable to be read, please try again with another key");

        }
      }
      else{
        Serial.println("Authentication Failed, please try again with another key");
      }
    }
  }
}