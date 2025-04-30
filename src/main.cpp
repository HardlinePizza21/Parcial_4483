#include <WiFi.h>
#include <PubSubClient.h>
#include <LM35.h>

#define lm35Pin 32
#define led 33

LM35 temp(lm35Pin);

const char *ssid = "IoT-B19";
const char *password = "lcontrol2020*";
const char *mqtt_server = "192.168.100.110"; // IP de tu PC (broker)
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado");
}

void reconnect()
{
  while (!client.connected())
  {
    if (client.connect("aasdasdsdfdafgqerf"))
    {
      Serial.println("Conectado a MQTT");
    }
    else
    {
      Serial.print("Error MQTT: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Mensaje recibido en el tema: ");
  Serial.println(topic);

  String msg;
  for (unsigned int i = 0; i < length; i++)
  {
    msg += (char)payload[i];
  }

  Serial.print("Mensaje: ");
  Serial.println(msg);

  if (msg == "encender")
  {
    digitalWrite(led, HIGH);
    Serial.println("LED encendido");
  }
  else if (msg == "apagar")
  {
    digitalWrite(led, LOW);
    Serial.println("LED apagado");
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  // Enviar un mensaje cada 5 segundos
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 3000)
  {

    int lectura = analogRead(lm35Pin);

    // Conversión para ADC de 12 bits (0–4095) y 3.3V referencia
    float voltaje = lectura * (5 / 4095.0);

    // LM35 da 10 mV por grado Celsius → 0.01 V/°C
    float temperaturaC = voltaje / 0.01;

    Serial.println(temperaturaC);
    char payload[10];
    dtostrf(temperaturaC, 6, 2, payload);
    client.publish("temperatura", payload);
    client.subscribe("led");
    lastSend = millis();
  }
}
