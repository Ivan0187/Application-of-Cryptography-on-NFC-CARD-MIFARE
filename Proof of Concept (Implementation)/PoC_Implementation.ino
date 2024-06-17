#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <AES.h>
#include <Base64.h>

#define PN532_IRQ   (2)
#define PN532_RESET (3)
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

#define KEY_LENGTH 32  // 32 bytes = 256 bits for AES-256
#define BLOCK_SIZE 16  // Block size for AES is 16 bytes

byte aes_key[KEY_LENGTH] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};

AES256 aes;

void setup(void) {
  Serial.begin(9600);
  Serial.println("This is an NFC Scanner");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("PN532 Board Not Found"); 
    while (1); // halt if the board is not found
  }

  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  nfc.SAMConfig();
  Serial.println("Hover the Card near the NFC reader...");

  aes.setKey(aes_key, KEY_LENGTH);  // Set the key for AES-256
}

void storeDatabase(uint8_t *uid, uint8_t *originData, uint8_t *data){
  delay(1000);
  //store database
  Serial.print("{\"UID\":\"");
  for (int i = 0; i < 7; i++) {
    Serial.print((uint8_t)uid[i]);
  }
  Serial.print("\",\"Data\":\""); 
  for (int i = 0; i < 16; i++) {
    if (originData[i] == 0){
      Serial.print('.');
      continue;
    } 
    Serial.print((char)originData[i]);
  }
  Serial.print("\",\"Shuffle\":\"");
  for (int i = 0; i < 16; i++) {
    if (data[i] == 0){
      Serial.print('.');
      continue;
    } 
    Serial.print((char)data[i]);
  }
  Serial.println("\"}");
  delay(2000);
}

void updateData(){
    Serial.println("Perbaharui data pada database");
    Serial.print("{\"Update\":");
    Serial.print(1);
    Serial.println("}");
}


void initializationData(uint8_t *uid, uint8_t *originData, uint8_t *data, uint8_t success, int update){
// Apply GifShuffle to data before writing
  Serial.println("\nGIFShuffle Data :");
  for (int i = 0; i < 16; i++) {
    if (data[i] == 0){
      Serial.print('.');
      continue;
    } 
    Serial.print((char)data[i]);
  }
  Serial.println("\n");

  // Encrypt data before writing
  byte encrypted[16];
  aes.encryptBlock(encrypted, data);

  for (int i = 0; i < 16; i++) {
    if (encrypted[i] == 0){
      Serial.print('.');
      continue;
    } 
    Serial.print((char)encrypted[i]);
  }
  Serial.println();

  Serial.println("Writing encrypted data to Block 8:");
  success = nfc.mifareclassic_WriteDataBlock(8, encrypted);
        
  if (success) {
    Serial.println("Data is being written, keep hovering the card...");
  } else {
    Serial.println("Failed to write data to Block 8");
  }
  Serial.println("");

  if(update == 1){
    updateData();
  }
  storeDatabase(uid,originData,data);
}

String decryptedBase(uint8_t success){
  String decryptedStr = "";
  byte readDataCard[16];
  byte decrypted[16];

  // Test read data
  success = nfc.mifareclassic_ReadDataBlock(8, readDataCard);
  
  if (success) {
    // Decrypt the data to verify
    aes.decryptBlock(decrypted, readDataCard);

    Serial.println((char*)decrypted);
    Serial.println("Decrypted data:");
    for (int i = 0; i < 16; i++) {
      if (decrypted[i] == 0){
          Serial.print('.');
          continue;
      } 
      Serial.print((char)decrypted[i]);
    }
    Serial.println();
  }

  Serial.println("Decrypted data dan Isi Gifshuffle pada kartu:");
  for (int i = 0; i < 16; i++) {
    if (decrypted[i] == 0){
      decryptedStr += String('.');
      continue;
    } 
    decryptedStr += String((char)decrypted[i]);
  } 
  return decryptedStr;
}

void gifShuffle(uint8_t *data, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    uint8_t j = random(0, len);
    uint8_t temp = data[i];
    data[i] = data[j];
    data[j] = temp;
  }
}

void sendSignal(){
    Serial.println("Signal Send");
    Serial.print("{\"Signal\":");
    Serial.print(1);
    Serial.println("}");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;                        
    
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    Serial.println("Card Found");
    Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    Serial.print("UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    
    if (uidLength == 4) {
      Serial.println("4 Byte UID card Found");
   
      Serial.println("Trying to authenticate block 8 with default KEYA value");
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 8, 0, keya);
   
      if (success) {
        Serial.println("Sector 2 (Blocks 8..11) has been authenticated");
        uint8_t data[16];
        uint8_t originData[16];
        memcpy(data, (const uint8_t[]){ 'B','I','N','U','S','A','B','C','D', 0, 0, 0, 0, 0, 0, 0}, sizeof data);
        memcpy(originData, (const uint8_t[]){ 'B','I','N','U','S','A','B','C','D', 0, 0, 0, 0, 0, 0, 0}, sizeof originData);

        // Create a buffer to store the string generated from the uint8_t array
        String uidDataStr = ""; 
        for (int i = 0; i < 7; i++) {
            uidDataStr += String(uid[i]);
        }
        // Serial.println(uidDataStr);

        Serial.println("Content Data :"); //initialization data save to Database or retrieved from initialization data in database
        for (int i = 0; i < 16; i++) {
           if (data[i] == 0){
              Serial.print('.');
              continue;
            } 
          Serial.print((char)data[i]);
        }
        Serial.println("\n");

        if (Serial.available() > 0) {
          String uidChecking = "";  
          String gifshuffle = "";
          String input = "";

          int done = 0;
          char receivedChar;
              
          while (Serial.available() > 0) {
            receivedChar = Serial.read();  
            if (receivedChar == '\n') {
                  done = 1;
                  break;  
            }
            input += receivedChar;  
          }

          Serial.println(input);

          int inputLength = input.length();
          int outputLength = Base64.decodedLength(input.c_str(), inputLength);
          char decodedMessage[outputLength + 1]; 

          Base64.decode(decodedMessage, input.c_str(), inputLength);
          decodedMessage[outputLength] = '\0'; 

          input = String(decodedMessage);

          int positionSeparator = input.indexOf(';');
            
          Serial.println("Tunggu kartu anda sedang dibaca !!!");
          if (strcmp (input.c_str(), "Empty") == 0){
            Serial.println("Tahap Inisialisasi Data");
            delay(1000);
            gifShuffle(data, sizeof(data)); 
            initializationData(uid, originData, data, success,0);
            delay(5000);
            sendSignal();
            Serial.println("Kartu didaftarkan pada database");
            delay(10000);
          }
          else{
            if (positionSeparator != -1) {
              uidChecking = input.substring(0, positionSeparator);
              gifshuffle = input.substring(positionSeparator + 1);
            } 
          }

          // Check if the data string is not empty
          if(strcmp (input.c_str(), "Empty") != 0 && strcmp (uidChecking.c_str(), uidDataStr.c_str()) != 0){
            Serial.println("UID kartu Tidak Sama !");
            sendSignal();
          }
          else if (strcmp (uidChecking.c_str(), uidDataStr.c_str()) == 0) {
            String decryptedStr = decryptedBase(success);                    
            if(strcmp(decryptedStr.c_str(),gifshuffle.c_str())!=0){
              sendSignal();
              Serial.println("Kartu Terduplikat!!!");
              Serial.println("Silakan hubungi admin!!!");
              delay(5000);
            }
            else if(strcmp(decryptedStr.c_str(),gifshuffle.c_str())==0){
              Serial.println("Kartu Terdaftar dan data sedang diperbaharui!!!");
              delay(1000);
              gifShuffle(data, sizeof(data)); 
              initializationData(uid, originData, data, success,1);
              delay(1000);
              sendSignal();
              Serial.println("Kartu berhasil direset");
              delay(8000);
            }
          }
          else{
            //waiting for a response from the database
            Serial.println("Silakan mencoba kembali");
          } 
          delay(1000);
        } else {
          //No signal from the database
          Serial.println("Tidak ada signal masuk dari database");
        }
      } else {
        //There is no signal and the card cannot be read
        Serial.println("Kartu tidak dikenali atau tidak ada signal masuk dari database");
      }
    }
  }
}



