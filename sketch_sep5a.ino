const int relePin = 2;  // Pino D4
const int minSensorPin = 14;  // Pino D5
const int maxSensorPin = 12;  // Pino D6
int state = 0;

void setup() {
  Serial.begin(9600);

  pinMode(minSensorPin, INPUT_PULLUP);  // Usando resistor pull-up interno
  pinMode(maxSensorPin, INPUT_PULLUP);  // Usando resistor pull-up interno
  
  digitalWrite(relePin, LOW);  // Começa DESLIGADO
  Serial.println("Teste com NO - Deve LIGAR em LOW");
  pinMode(LED_BUILTIN, OUTPUT);
}

void testeSensores() {
  Serial.println("=== TESTE DE SENSORES ===");
  Serial.printf("Sensor Mínimo (D5): %d\n", digitalRead(minSensorPin));
  Serial.printf("Sensor Máximo (D6): %d\n", digitalRead(maxSensorPin));
  
  if (digitalRead(minSensorPin) == LOW) {
    Serial.println("✓ Sensor Mínimo detectando água");
  } else {
    Serial.println("✗ Sensor Mínimo SEM água");
  }
  
  if (digitalRead(maxSensorPin) == LOW) {
    Serial.println("✓ Sensor Máximo detectando água");
  } else {
    Serial.println("✗ Sensor Máximo SEM água");
  }

  if (digitalRead(minSensorPin) == LOW && digitalRead(maxSensorPin) == LOW) {
    digitalWrite(relePin, HIGH);
  } else if (digitalRead(minSensorPin) != LOW) {
    digitalWrite(relePin, LOW);
  }

  Serial.println("=========================\n");
}

void loop() {

  testeSensores();

  // int minSensorPinRead = digitalRead(minSensorPin);
  // int maxSensorPinRead = digitalRead(maxSensorPin);

  // Serial.println(">>> SYSTEM ON");
  // Serial.printf(">>> 1 minSensorPinRead %d\n", minSensorPinRead);
  // Serial.printf(">>> 2 maxSensorPinRead %d\n", maxSensorPinRead);

  // if (minSensorPinRead == LOW && maxSensorPinRead == LOW) {
  //   // Ativar
  //   state = 0;
  //   Serial.println(">>> BOMBA ATIVADA");
  //   digitalWrite(relePin, LOW);
  //   digitalWrite(LED_BUILTIN, LOW);
  // } else {
  //   Serial.println(">>> BOMBA DESATIVADA");
  //   digitalWrite(relePin, HIGH);
  //   digitalWrite(LED_BUILTIN, HIGH);
  // }
  // if (minSensorPinRead == HIGH && maxSensorPinRead == HIGH) {
  //   Serial.println(">>> BOMBA DESATIVADA");
  //   digitalWrite(relePin, HIGH);
  //   digitalWrite(LED_BUILTIN, LOW);
  // }
  
  // else if ((minSensorPinRead == LOW && maxSensorPinRead == LOW) && state != 1) {
  //   // Ativar
  //   state = 1;
  //   Serial.println(">>> BOMBA ATIVADA");
  // }

  // if (maxSensorPinRead == LOW && minSensorPinRead == HIGH) {
  //   Serial.println(">>> SENSOR DE AGUA ATIVO");
  //   digitalWrite(relePin, LOW);
  // } else {
  //   Serial.println(">>> SENSOR DE AGUA DESATIVADO");
  //   digitalWrite(relePin, HIGH);
  // }

  delay(1000);

  // Serial.println(">>> RELÉ LIGADO - Celular DEVE carregar");
  // digitalWrite(relePin, HIGH);    // Ativa relé → fecha contato NO
  // delay(10000);
  
  // Serial.println(">>> RELÉ DESLIGADO - Celular NÃO deve carregar");
  // digitalWrite(relePin, LOW);   // Desativa relé → abre contato NO  
  // delay(10000);
}