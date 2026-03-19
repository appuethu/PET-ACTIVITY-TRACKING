#define BLYNK_TEMPLATE_ID "TMPL3mlo089qK"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "bpTFKOW-xzQJAoRSL9vJYaI9Qi6bJnHN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>                     
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "vivo T2x 5G";
char pass[] = "kjgdacbzd";

#define DHTPIN D2
#define DHTTYPE DHT11
#define SOUND_SENSOR D5
#define PULSE_SENSOR A0

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

int smoothPulseRead() {
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(PULSE_SENSOR);
    delay(2);
  }
  return sum / 10; // average
}

void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int soundValue = digitalRead(SOUND_SENSOR);
  int pulseValue = smoothPulseRead();

  if (isnan(h) || isnan(t)) {
    Serial.println("DHT11 reading failed!");
    return;
  }

  Serial.print("Temp: "); Serial.print(t);
  Serial.print(" °C  Hum: "); Serial.print(h);
  Serial.print("%  Sound: "); Serial.print(soundValue);
  Serial.print("  Pulse (raw): "); Serial.println(pulseValue);

  // Send to Blynk app
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, soundValue);
  Blynk.virtualWrite(V3, pulseValue);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(SOUND_SENSOR, INPUT);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
