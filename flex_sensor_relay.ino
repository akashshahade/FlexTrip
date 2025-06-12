const int flexPin = A0;       // Flex sensor connected to A0
const int relayPin = 2;       // Relay module IN pin
int baseValue = 0;            // Resting flex value
int tolerance = 10;           // Allow minor fluctuations
unsigned long relayOffTime = 0;
bool relayLocked = false;

void setup() {
  pinMode(relayPin, OUTPUT);
  Serial.begin(9600);

  delay(1000);  // Let sensor settle
  baseValue = analogRead(flexPin);
  Serial.print("Base Flex Value: ");
  Serial.println(baseValue);

  digitalWrite(relayPin, LOW);  // Relay ON (motor ON)
}

void loop() {
  int flexValue = analogRead(flexPin);
  Serial.print("Flex Reading: ");
  Serial.println(flexValue);

  unsigned long currentTime = millis();

  if (!relayLocked && abs(flexValue - baseValue) > tolerance) {
    // Detected change → Turn OFF relay and lock for 10 seconds
    digitalWrite(relayPin, HIGH);     // Relay OFF (motor OFF)
    relayOffTime = currentTime;
    relayLocked = true;
    Serial.println("⚠️ Flex change detected! Motor OFF for 10 sec.");
  }

  if (relayLocked && (currentTime - relayOffTime >= 10000)) {
    // 10 seconds passed → Turn relay back ON
    digitalWrite(relayPin, LOW);      // Relay ON (motor ON)
    relayLocked = false;
    Serial.println("✅ 10 seconds over. Motor ON.");
  }

  delay(100);
}
