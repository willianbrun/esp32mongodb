#define EAP_PASSWORD "Baguvix1"  //password for account
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "../model/log.cpp"

#define ADC_VREF_mV 3300.0 // in millivolt
#define ADC_RESOLUTION 8192.0
#define THIS_IS_A_POWER_PIN 14 // Just to have 3.3V power on the other side of ESP
#define BUZZER_PIN 12          // ESP32 pin GPIO32 that connects to buzzer
#define LED_PIN 26             // ESP32 pin GPIO32 that connects to led
#define PIN_LM35 36            // ESP32 pin GPIO36 (ADC0) connected to LM35
#define SENSOR_ERROR -3        // To compensate bad sensor (only for study, never for prod)

#define MAX_TEMPERATURE 20 // Max temperature to active alarm

#define EAP_ANONYMOUS_IDENTITY "178810@upf.br" //anonymous@example.com, or you can use also nickname@example.com
#define EAP_IDENTITY "178810"  //login

const char* ssid = "Sou UPF";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
float temperature = 0;
bool turnedAlarmOn = false;

String uri = "192.168.5.15";

// Metodos
void acessaPOST(String URN, Log log);
void readSensorDataAndAlarm();
void alarmHighTemperature();

void setup()
{
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(THIS_IS_A_POWER_PIN, HIGH);
    Serial.begin(9600);
    delay(1000);
    Serial.println();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    // WiFi.begin(ssid, passwd);
    WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_ANONYMOUS_IDENTITY, EAP_IDENTITY, EAP_PASSWORD); //WITHOUT CERTIFICATE - WORKING WITH EXCEPTION ON RADIUS SERVER


    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("MAC address: ");
    Serial.println(WiFi.macAddress());
    Serial.println("");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    if ((millis() - lastTime) > timerDelay)
    {
        readSensorDataAndAlarm();
        if (WiFi.status() == WL_CONNECTED)
        {
            Log log = Log(temperature, turnedAlarmOn);
            acessaPOST("logs/", log);
        }
        else
        {
            Serial.println("WiFi Disconnected");
        }
        lastTime = millis();
    }
}

void readSensorDataAndAlarm()
{
    // read the ADC value from the temperature sensor
    int adcVal = analogRead(PIN_LM35);
    // convert the ADC value to voltage in millivolt
    float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
    // convert the voltage to the temperature in °C
    temperature = milliVolt / 10;
    temperature += SENSOR_ERROR;

    if (temperature >= MAX_TEMPERATURE)
    {
        turnedAlarmOn = true;
        alarmHighTemperature();
    }
    else
    {
        turnedAlarmOn = false;
    }
}

void alarmHighTemperature()
{
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_BUILTIN, LOW);
}

void acessaPOST(String URN, Log log)
{
    HTTPClient http;
    String rota;
    String marshalledJson, prettyMarshalledJson;

    // Configura a conexão com URI via http
    rota = uri.c_str();
    rota.concat(URN.c_str());
    http.begin(rota.c_str());

    StaticJsonDocument<200> unmarshalledJson;

    unmarshalledJson["temperature"] = log.temperature;
    unmarshalledJson["turned_alarm_on"] = log.turned_alarm_on;

    serializeJson(unmarshalledJson, marshalledJson);
    serializeJsonPretty(unmarshalledJson, prettyMarshalledJson);

    // Envia uma requisição com o método POST
    int httpResponseCode = http.POST(marshalledJson);

    if (httpResponseCode > 0)
    {
        Serial.print("Código de resposta HTTP: ");
        Serial.println(httpResponseCode);
        if (httpResponseCode == 200)
        {
            Serial.print("Retorno POST de ");
            Serial.println(rota.c_str());
            String payload = http.getString();
            Serial.println(payload);
        }
        else
        {
            String payload = http.getString(); // Para evitar erros na biblioteca
            Serial.print("Erro no processamento da requisição ");
            Serial.println(httpResponseCode);
        }
    }
    else
    {
        Serial.print("Erro de acesso ao Serviço: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
}