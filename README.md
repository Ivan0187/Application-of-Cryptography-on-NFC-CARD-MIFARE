# Application-of-Cryptography-on-NFC-CARD-MIFARE

Application of cryptography and gifshuffle security implementation designed as additional security of NFC card with type of MIFARE Classic. The design of security can prevent duplication attack on duplicated NFC card and the manager of NFC card can know if the user's card is indicated to be duplicated or not.

##Key Features:
1. Read NFC Data: The program design used in simulating to read of the content data from the Mifare Classic NFC card into the Arduino IDE.
2. Write NFC Data: The program design used in simulating to write of the content data into the Mifare Classic NFC card through the Arduino IDE.
3. Gifshuffle Algorithm: The design of the security algorithm program implemented in randomizing of the content data to be saved into the NFC card and database.
4. AES 256 Algorithm: The design of the security cryptography algorithm program that is applied in encrypting and decrypting of the content data to be stored into or read from the NFC card and database.
5. Store Data to Database: The design of the program scheme applied in storing the processed data from the Arduino IDE into the database through the node-red module in JSON format.
6. Read Data From Database: The design of the program scheme applied in retrieving the content data stored from the database to the Arduino IDE through the node-red module.
