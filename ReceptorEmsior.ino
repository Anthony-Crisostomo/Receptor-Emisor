const int sensorPin = 4; 
volatile unsigned long startTime = 0; 
volatile unsigned long alto1 = 0; //highTime1
volatile unsigned long alto2 = 0; //highTime2
volatile int contador = 0; //pulseCount
volatile bool  nuevo= false; //newMeasurement

void IRAM_ATTR medida_pulso() {//funcion que sera llamada en la interrupcion
  unsigned long currentTime = micros();
  if (digitalRead(sensorPin) == HIGH) {
    startTime = currentTime; 
  } else {
    if (contador == 0) {
      alto1 = currentTime - startTime; 
      contador = 1;
    } else if (contador == 1) {
      alto2 = currentTime - startTime; 
      contador = 2;
      nuevo = true; 
      detachInterrupt(digitalPinToInterrupt(sensorPin)); 
    }
  }
}

void setup() {
  pinMode(sensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), medida_pulso, CHANGE);//en cada flanco de subida y bajada se hara la interrupcion
  Serial.begin(9600);
}

void loop() {
  if (nuevo) {
    noInterrupts();
    unsigned long pulsoalto1 = alto1;
    unsigned long pulsoalto2 = alto2;
    nuevo = false;
    contador = 0;
    interrupts();
    
    Serial.print("Tiempo en alto-1: ");
    Serial.print(pulsoalto1 / 1000.0, 3); 
    Serial.println(" ms");

    Serial.print("Tiempo en alto-2: ");
    Serial.print(pulsoalto2 / 1000.0, 3); 
    Serial.println(" ms");

    attachInterrupt(digitalPinToInterrupt(sensorPin), medida_pulso, CHANGE);
  }
}
