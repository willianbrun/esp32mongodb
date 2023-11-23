#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "../model/log.cpp"

#define EAP_ANONYMOUS_IDENTITY "anonymous@tuke.sk" // anonymous@example.com, or you can use also nickname@example.com
#define EAP_IDENTITY "178810"

#define ADC_VREF_mV 3300.0 // in millivolt
#define ADC_RESOLUTION 4096.0

#define BUZZER_PIN 32      // ESP32 pin GPIO32 that connects to buzzer
#define PIN_LM35 36        // ESP32 pin GPIO36 (ADC0) connected to LM35
#define MAX_TEMPERATURE 36 // Max temperature to active buzzer

const char *ssid = "iPhone (13)";
const char *passwd = "willian123";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

float temperature = 0;

// Your Domain name with URL path or IP address with path
// Inserção do URI (URL + URN) a acessar
String uri = "http://172.20.10.8:8000/";

void acessaPOST(String URN, Log log);
void readSensorDataAndAlarm();
void alarmHighTemperature();

void setup()
{
    Serial.begin(9600);
    delay(1000);
    Serial.println();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passwd);

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
            Log log = Log(temperature);
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

    if (temperature >= MAX_TEMPERATURE)
    {
        alarmHighTemperature();
    }
}

void alarmHighTemperature()
{
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
    delay(2000);
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

    serializeJson(unmarshalledJson, marshalledJson);
    serializeJsonPretty(unmarshalledJson, prettyMarshalledJson);

    Serial.println(prettyMarshalledJson);

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
        timerDelay = 2000;
    }
    else
    {
        Serial.print("Erro de acesso ao Serviço: ");
        Serial.println(httpResponseCode);
        timerDelay = 100;
    }
    // Free resources
    http.end();
}