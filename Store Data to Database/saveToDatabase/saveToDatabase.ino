void setup() {
  Serial.begin(9600); // Initialize serial communication at a baud rate of 9600
}

void loop() {
    // Must Running service node-red
    // Initialize variable and Value
    uint8_t uid[] = { 1, 2, 3, 4, 0, 0, 0 };
    uint8_t originData[16];
    memcpy(originData, (const uint8_t[]){ 'B','I','N','U','S','2','0','2','4', 0, 0, 0, 0, 0, 0, 0}, sizeof originData);
    uint8_t data[16];
    memcpy(data, (const uint8_t[]){ 'N','I','B','U','S','2','0','4',0, 0, '2', 0, 0, 0, 0, 0}, sizeof data);
    
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
    delay(10000);
  }