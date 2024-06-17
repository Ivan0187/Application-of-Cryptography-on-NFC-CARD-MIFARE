void setup() {
  Serial.begin(9600);
}

void loop() {
  uint8_t data[16];
  memcpy(data, (const uint8_t[]){ 'B','I','N','U','S','2','0','2','4', 0, 0, 0, 0, 0, 0, 0}, sizeof data);
        
  // Apply GifShuffle to data before writing
  gifShuffle(data, sizeof(data));

  for (int i = 0; i < 16; i++) {
      if (data[i] == 0){
          Serial.print('.');
          continue;
      } 
      Serial.print((char)data[i]);
  }
          
  Serial.println("");
  delay(10000);
}

void gifShuffle(uint8_t *data, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    uint8_t j = random(0, len);
    uint8_t temp = data[i];
    data[i] = data[j];
    data[j] = temp;
  }
}