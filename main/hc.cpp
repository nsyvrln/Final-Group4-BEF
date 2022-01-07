class HC {
public:
  int echoPin;
  int trigPin;
  long duration;
  float distance;

  void begin(int echoPin = 14, int trigPin = 12) {
    this->echoPin = echoPin;
    this->trigPin = trigPin;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }
  float GetDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    this->duration = pulseIn(echoPin, HIGH);

    this->distance = duration * 0.034 / 2;
    return this->distance;
  }
};


HC hc;
void setup() {
  hc.begin();
  Serial.begin(115200);
}

void loop() {
  Serial.println(hc.GetDistance());
  delay(100);
}
