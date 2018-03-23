/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read new NUID from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to the read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the type, and the NUID if a new card has been detected. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>

#define STATE_0 0 //Idle
#define STATE_1 8 //ID Accepted
#define STATE_2 9 //ID No Discount
#define STATE_3 10 //ID Yes Discount
#define STATE_4 11 //Soap Ready to Dispense
#define STATE_5 12 //Discount Recieved 
#define STATE_6 13 //Error

#define SOAPBUTTON 6

constexpr uint8_t RST_PIN = 5;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 53;     // Configurable, see typical pin layout above
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];
int choice = 0;

// State Variables
int state = STATE_0;

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  //Serial.println(F("This code scan the MIFARE Classsic NUID."));
  //Serial.print(F("Using the following key:"));
  //printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

  pinMode(STATE_1, OUTPUT);
  pinMode(STATE_2, OUTPUT);
  pinMode(STATE_3, OUTPUT);
  pinMode(STATE_4, OUTPUT);
  pinMode(STATE_5, OUTPUT);
  pinMode(STATE_6, OUTPUT);

  pinMode(SOAPBUTTON,INPUT);
}
 
void loop() {
  readCard();

  if(Serial.available() > 0){
    choice = Serial.parseInt();
    ledArray(choice);
  }
  //Serial.print(choice);

  
}

void readCard(){
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  //Serial.print(F("PICC type: "));
  //MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  //Serial.println(rfid.PICC_GetTypeName(piccType));

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    //Serial.println(F("A new card has been detected."));

    // Move to card detected state
    state = STATE_1;

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    //Serial.println(F("The NUID tag is:"));
    //Serial.print(F("In hex: "));
    Serial.print("Card: ");
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    //Serial.print(F("In dec: "));
    //printDec(rfid.uid.uidByte, rfid.uid.size);
    //Serial.println();
  }
  else{
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "0" : "");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void ledArray(int state){
  switch(state){
    case 1:
      digitalWrite(STATE_1, HIGH);
      digitalWrite(STATE_2, LOW);
      digitalWrite(STATE_3, LOW);
      digitalWrite(STATE_4, LOW);
      digitalWrite(STATE_5, LOW);
      digitalWrite(STATE_6, LOW);
      break;
    case 2:
      digitalWrite(STATE_1, LOW);
      digitalWrite(STATE_2, HIGH);
      digitalWrite(STATE_3, LOW);
      digitalWrite(STATE_4, LOW);
      digitalWrite(STATE_5, LOW);
      digitalWrite(STATE_6, LOW);
      break;
    case 3:
      digitalWrite(STATE_1, LOW);
      digitalWrite(STATE_2, LOW);
      digitalWrite(STATE_3, HIGH);
      digitalWrite(STATE_4, LOW);
      digitalWrite(STATE_5, LOW);
      digitalWrite(STATE_6, LOW);
      break;
    case 4:
      digitalWrite(STATE_1, LOW);
      digitalWrite(STATE_2, LOW);
      digitalWrite(STATE_3, LOW);
      digitalWrite(STATE_4, HIGH);
      digitalWrite(STATE_5, LOW);
      digitalWrite(STATE_6, LOW);
      break;
    case 5:
      digitalWrite(STATE_1, LOW);
      digitalWrite(STATE_2, LOW);
      digitalWrite(STATE_3, LOW);
      digitalWrite(STATE_4, LOW);
      digitalWrite(STATE_5, HIGH);
      digitalWrite(STATE_6, LOW);
      break;
    case 6:
      digitalWrite(STATE_1, LOW);
      digitalWrite(STATE_2, LOW);
      digitalWrite(STATE_3, LOW);
      digitalWrite(STATE_4, LOW);
      digitalWrite(STATE_5, LOW);
      digitalWrite(STATE_6, HIGH);
      break;
    default:
      digitalWrite(STATE_1, HIGH);
      digitalWrite(STATE_2, HIGH);
      digitalWrite(STATE_3, HIGH);
      digitalWrite(STATE_4, HIGH);
      digitalWrite(STATE_5, HIGH);
      digitalWrite(STATE_6, HIGH);
  }
}
