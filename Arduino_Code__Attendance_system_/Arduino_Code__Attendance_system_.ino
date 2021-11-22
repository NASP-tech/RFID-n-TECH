#include <SPI.h>      //  Libreria SPI
#include <MFRC522.h>      // Libreria  MFRC522
#include <Servo.h>     // Libreria  SERVO
#include "SoftwareSerial.h"
#define RST_PIN  9      // Pin de reset
#define SS_PIN  10      // Pin de slave select
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
SoftwareSerial ser(2,3); // RX, TX 

byte LecturaUID[4];         // Array para almacenar el UID leido
int analogo5 = 0;                               // Variable de lectura del Analogo5 para sensor de obstaculos
int redLed = 6;
int greenLed = 5;
int buzzer = 7;
Servo servoPT;                                 // Asocia la libreria servo a  servoPT


void setup() 
{
  pinMode(buzzer, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(greenLed, OUTPUT); // Green Led
  pinMode(redLed, OUTPUT); // Red Led;
  
  Serial.begin(9600);   // Initiate a serial communication
  
  ser.begin (115200);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  servoPT.attach(4);      // Sevo asociado al pin 4 y lleva a 170 grados
  // servoPT.write(170);
  Serial.println("Put RFID Card to Scan...");
  Serial.println("Sistema Activado");    // Muestra texto Listo

  Serial.println();
 
}
void loop() 
{  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())  // si no hay una tarjeta presente
  {
    return;  // retorna al loop esperando por una tarjeta
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) // si no puede obtener datos de la tarjeta
  {
    return; // retorna al loop esperando por una tarjeta
  }
  //Show UID on serial monitor
  
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("User No./Name:    ");
  content.toUpperCase();

  if (content.substring(1) == "03 6F E8 14" ) //change here the UID of the card/cards that you want to give access
  {
    Serial.println();
    Serial.println("Bienvenid@ a Tech-n-RFID"); // si retorna verdadero muestra texto bienvenida
    Serial.println("1-Natalia Solórzano");
    ser.write(1);
    // ser.write("Access by Natalia");
    Serial.println();
    Serial.println("Access Granted!");
    
    abrirPuerta(); 
    delay(3000);
  }
  else if (content.substring(1) == "" ) //change here the UID of the card/cards that you want to give access
  {
    Serial.println();
    Serial.println("Bienvenid@ a Tech-n-RFID"); // si retorna verdadero muestra texto bienvenida
    Serial.println("1-Nat Solórzano");
    ser.write(2);
    Serial.println();
    Serial.println("Access Granted!");
    
    abrirPuerta(); 
    delay(3000);
  }
  else {
    Serial.println("No Registrado");    // muestra texto equivalente a acceso denegado
    Mal_Registro();                    // funcion sonido de targeta no identificada
  }
}

void puertaSensor(){
  servoPT.write(170);       // Abrir la puerta
  delay(7000);             // Tiempo de la puerta abierta
  servoPT.write(90);       // Cierra puerta
}

void abrirPuerta() {  
  digitalWrite(buzzer, HIGH);   // Suena el buzzer para indicar que se va a cerrar la puerta
  digitalWrite(greenLed, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);     // apaga el buzzer
  digitalWrite(greenLed, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);   // Suena el buzzer para indicar que se va a cerrar la puerta
  digitalWrite(greenLed, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);     // apaga el buzzer
  digitalWrite(greenLed, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);   // Suena el buzzer para indicar que se va a cerrar la puerta
  digitalWrite(greenLed, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);     // apaga el buzzer
  digitalWrite(greenLed, LOW);
  delay(500);
  analogo5 = analogRead(A5);
  if ( analogo5 <= 400) {                           // Si el sensor de obtaculos fue obstruido abre la puerta
    Serial.println("Se percibe persona a ingresar");
    puertaSensor();
  }
  else {
    Serial.println("No se percibe persona a ingresar");
  }  
}

void Mal_Registro() {      // Activa el Buzzer 2 veces por tarjeta no autorizada
  digitalWrite(buzzer, HIGH);
  digitalWrite(redLed, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  digitalWrite(redLed, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  digitalWrite(redLed, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  digitalWrite(redLed, LOW);
}

boolean comparaUID(byte lectura[], byte usuario[]) // funcion comparaUID
{
  for (byte i = 0; i < 4; i++) { // bucle recorre de a un byte por vez el UID
    if (lectura[i] != usuario[i])       // si byte de UID leido es distinto a usuario
      return (false);         // retorna falso
  }
  return (true);          // si los 4 bytes coinciden retorna verdadero
}
