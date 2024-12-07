void setup() {
    Serial.begin(9600);
    Serial.flush();
}

void loop() {
  Serial.print("Hello World ;)\n");
  delay(1000);
}
