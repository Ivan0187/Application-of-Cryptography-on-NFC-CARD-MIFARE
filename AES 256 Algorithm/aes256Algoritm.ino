#include <Crypto.h>
#include <AES.h>

#define KEY_LENGTH 32  // 32 bytes = 256 bits for AES-256
#define BLOCK_SIZE 16  // Block size for AES is 16 bytes

byte aes_key[KEY_LENGTH] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};

AES256 aes;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  aes.setKey(aes_key, KEY_LENGTH);  // Set the key for AES-256
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t data[16];
  memcpy(data, (const uint8_t[]){ 'B','I','N','U','S','2','0','2','4', 0, 0, 0, 0, 0, 0, 0}, sizeof data);
        
  // Encrypt data before writing
  byte encrypted[16];
  aes.encryptBlock(encrypted, data);
  

  Serial.print("Encrypted Data: ");
  for (int i = 0; i < 16; i++) {
    if (encrypted[i] == 0){
      Serial.print('.');
      continue;
    } 
    Serial.print((char)encrypted[i]);
  }
  Serial.println();
     
  byte decrypted[16];
  aes.decryptBlock(decrypted, encrypted);
  Serial.print("Decrypted data:");
  for (int i = 0; i < 16; i++) {
    if (decrypted[i] == 0){
      Serial.print('.');
      continue;
    } 
    Serial.print((char)decrypted[i]);
  }
  Serial.println();
  Serial.println();
  delay(5000);
}

