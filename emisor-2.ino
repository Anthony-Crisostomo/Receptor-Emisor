const int receptorPin = 4; 
const int emisorPin = 2; // Pin del modulo emisor
volatile unsigned long startTime = 0; 
volatile unsigned long alto1 = 0; 
volatile unsigned long alto2 = 0;
volatile int contador = 0; 
volatile bool nuevo = false; 


unsigned long pulsoAlto1 = 0;
unsigned long pulsoAlto2 = 0;

void IRAM_ATTR medida_pulso() { //funcion que sera llamada en la interrupcion
  unsigned long currentTime = micros();
  if (digitalRead(receptorPin) == HIGH) {
    startTime = currentTime; 
  } else {
    if (contador == 0) {
      alto1 = currentTime - startTime; 
      contador = 1;
    } else if (contador == 1) {
      alto2 = currentTime - startTime; 
      contador = 2;
      nuevo = true; 
      detachInterrupt(digitalPinToInterrupt(receptorPin)); 
    }
  }
}

void setup() {
  pinMode(receptorPin, INPUT);
  pinMode(emisorPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(receptorPin), medida_pulso, CHANGE); //en cada flanco de subida y bajada se hara la interrupcion
  Serial.begin(9600);
}

void loop() {
  if (nuevo) {
    noInterrupts();
    pulsoAlto1 = alto1;
    pulsoAlto2 = alto2;
    nuevo = false;
    contador = 0;
    interrupts();
    
    Serial.print("Pulso Alto 1: ");
    Serial.print(pulsoAlto1);
    Serial.println(" us");

    Serial.print("Pulso Alto 2: ");
    Serial.print(pulsoAlto2);
    Serial.println(" us");

    attachInterrupt(digitalPinToInterrupt(receptorPin), medida_pulso, CHANGE);
    emitirPulsos();
  }
}

void emitirPulsos() {
  digitalWrite(emisorPin, HIGH);
  delayMicroseconds(pulsoAlto1);//tiempo del primer pulso
  digitalWrite(emisorPin, LOW);
  delayMicroseconds(2000); // tiempo entre pulsos 
  digitalWrite(emisorPin, HIGH);
  delayMicroseconds(pulsoAlto2);//tiempo del segundo pulso
  digitalWrite(emisorPin, LOW);
}
