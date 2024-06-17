void setup() {
  Serial.begin(9600); // Initialize serial communication at a baud rate of 9600
}

void loop() {
  //Must Running Service node-red
  //Check if there is an input buffer incoming to the Arduino IDE
   if (Serial.available() > 0) {
      //Check if the UID has been registered or not?
      String uidChecking = "";  // Initialize an empty string to hold incoming data
      String gifshuffle = "";
      String input = "";

      int done = 0;
      char receivedChar;

      // Continue reading characters until a newline is received
      while (Serial.available() > 0) {
        receivedChar = Serial.read();  // Read the next character
        if (receivedChar == '\n') {
          done = 1;
          break;  // Exit the loop if newline character is received
        }
        input += receivedChar;  // Append the character to the data string
      }          

      int positionSeparator = input.indexOf(';');
      if (positionSeparator != -1){
        uidChecking = input.substring(0, positionSeparator);
        gifshuffle = input.substring(positionSeparator + 1);
      }
      else{
        Serial.println("Not Valid Data");
      }

      Serial.println("Hasilnya adalah: ");
      Serial.println("Data Input: ");
      Serial.println(input);
      Serial.println("UID Read: ");
      Serial.println(uidChecking);
      Serial.println("Giffshuffle Read: ");
      Serial.println(gifshuffle);     
    }
    delay(1000);
  }