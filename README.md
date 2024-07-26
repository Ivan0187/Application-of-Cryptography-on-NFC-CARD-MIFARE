# Application-of-Cryptography-on-NFC-CARD-MIFARE

Application of cryptography and gifshuffle security implementation designed as additional security of NFC card with type of MIFARE Classic. The design of security can prevent duplication attack on duplicated NFC card and the manager of NFC card can know if the user's card is indicated to be duplicated or not.

## Key Features:
1. **Read NFC Data:** The program design used in simulating to read of the content data from the Mifare Classic NFC card into the Arduino IDE.
2. **Write NFC Data:** The program design used in simulating to write of the content data into the Mifare Classic NFC card through the Arduino IDE.
3. **Gifshuffle Algorithm:** The design of the security algorithm program implemented in randomizing of the content data to be saved into the NFC card and database.
4. **AES 256 Algorithm:** The design of the security cryptography algorithm program that is applied in encrypting and decrypting of the content data to be stored into or read from the NFC card and database.
5. **Store Data to Database:** The design of the program scheme applied in storing the processed data from the Arduino IDE into the database through the node-red module in JSON format.
6. **Read Data From Database:** The design of the program scheme applied in retrieving the content data stored from the database to the Arduino IDE through the node-red module.

## Proof of Concept:
**Video Proof of Concept** : https://youtu.be/3uLGtTppmt0
1. **Data Initialization on NFC access cards that have not been registered:** The implementation step in initializing data on NFC access card that has not been registered into the database through the Arduino IDE and node-red.
2. **Data Validation Process on NFC access cards that have been registered:**
The proving step is conducted through the Arduino IDE in a validating process of the content data from the NFC card registered in the database. There are two condition result get from the process of validating the data on the NFC card registered in the database:
   1. **NFC access card that are indicated to be duplicated** if the content of the gifshuffle data stored on the NFC access card is different from the gifshuffle data stored in the database then the NFC access card is indicated to be duplicated and the card cannot be used and must report to the manager.
   2. **NFC access card that are not duplicated** if the content of the gifshuffle data stored on the NFC access card are same with the gifshuffle data stored in the database then the access card is not duplicated and the content data of the NFC access card will be reset again.  

## Requirement Setup:
### Prerequisites:
1. XAMPP v3.3.0 or the latest version installed on the machine.
2. Nodejs v22.2.0 or the latest version installed on the machine.
3. Node-red v3.1.9 or the latest version installed on the machine.
4. Arduino IDE v2.3.3 or the latest version installed on the machine.

### Libraries used:
1. Node-red:
	1. node-red-node-base64
	2. node-red-node-mysql
	3. node-red-node-serialport
2. Arduino IDE:
	1. SPI.h
	2. AdaFruit_PN532.h
	3. AES.h
	4. Base64

### Steps
1. **Clone the repository:**
	Open Terminal on the operating system used such as Git Bash, Powershell or Command Prompt and set the storage directory used in the clone project. Example:
	```bash
	git clone https://github.com/Ivan0187/Application-of-Cryptography-on-NFC-CARD-MIFARE
	cd Application-of-Cryptography-on-NFC-CARD-MIFARE
	```
 2. **You can access the Installation Manual through the following link: TBA**
