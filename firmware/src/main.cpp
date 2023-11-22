#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "../model/machine.cpp"

#define EAP_ANONYMOUS_IDENTITY "anonymous@tuke.sk" // anonymous@example.com, or you can use also nickname@example.com
#define EAP_IDENTITY "178810"                      // login

const char *ssid = "LCI01";
const char *passwd = "up3@wz01";

unsigned long lastTime = 0;
// Ajusta par requisição a cada 5 segundos (5000)
unsigned long timerDelay = 2000;

// Your Domain name with URL path or IP address with path
// Inserção do URI (URL + URN) a acessar
String uri = "http://192.168.1.6:8000/";

void acessaGET(String URN);
void acessaPOST(String URN, Machine machine);
void acessaPUT(String URN, Machine machine);
void acessaPATCH(String URN);
void testarMetodos();
void runCRUD();

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
        if (WiFi.status() == WL_CONNECTED)
        {
            runCRUD();
            // testarRotas();
            //  testarMetodos();
            // testarArgumentos();
            // testarArgumentosQuery();
        }
        else
        {
            Serial.println("WiFi Disconnected");
        }
        lastTime = millis();
    }
}

long long int id1 = 7583587943959;
long long int id2 = 7583582847323;
long long int id3 = 3274589387493;

void runCRUD()
{
    Machine machine1(id1++, "Princesa Top", "Parado", 84.79, 9.42, "2023-10-24T00:00:00", "2023-10-24T00:00:00");
    Machine machine2(id2++, "Estrela G2", "Em movimento", 23.92, -4.29, "2023-10-24T00:00:00", "2023-10-24T00:00:00");
    Machine machine3(id3++, "Hércules", "Trabalhando", 52.522, 4.824, "2023-10-24T00:00:00", "2023-10-24T00:00:00");

    Machine machine2New(7583587943959, "Estrela G3", "Parado", 23.92, -4.29, "2023-10-24T00:00:00", "2023-10-24T00:00:00");

    acessaPOST("machines/", machine1);
    delay(2000);
    acessaPOST("machines/", machine2);
    delay(2000);
    acessaPOST("machines/", machine3);
    delay(2000);
    acessaGET("machines/" + String(id2));
    delay(2000);
    acessaPUT("machines/" + String(id2), machine2New);
    delay(2000);
    acessaGET("machines/" + String(id2));
    delay(2000);
    acessaGET("machines/");
    delay(2000);
    acessaPATCH("machines/" + String(id3));
    delay(2000);
    acessaGET("machines/");
    delay(10000);
}

void acessaPUT(String URN, Machine machine)
{
    HTTPClient http;
    String rota;
    String marshalledJson, prettyMarshalledJson;

    // Configura a conexão com URI via http
    rota = uri.c_str();
    rota.concat(URN.c_str());
    http.begin(rota.c_str());

    StaticJsonDocument<200> unmarshalledJson;

    unmarshalledJson["id"] = machine.id;
    unmarshalledJson["name"] = machine.name;
    unmarshalledJson["status"] = machine.status;
    unmarshalledJson["latitude"] = machine.latitude;
    unmarshalledJson["longitude"] = machine.longitude;
    unmarshalledJson["last_updated"] = machine.last_updated;
    unmarshalledJson["created"] = machine.created;

    serializeJson(unmarshalledJson, marshalledJson);
    serializeJsonPretty(unmarshalledJson, prettyMarshalledJson);

    // Envia uma requisição com o método POST
    int httpResponseCode = http.PUT(marshalledJson);

    if (httpResponseCode > 0)
    {
        Serial.print("Código de resposta HTTP: ");
        Serial.println(httpResponseCode);
        if (httpResponseCode == 200)
        {
            Serial.print("Retorno PUT de ");
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

void acessaPATCH(String URN)
{
    HTTPClient http;
    String rota;

    // Configura a conexão com URI via http
    rota = uri.c_str();
    rota.concat(URN.c_str());
    http.begin(rota.c_str());

    // http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.PATCH("");

    if (httpResponseCode > 0)
    {
        Serial.print("Código de resposta HTTP: ");
        Serial.println(httpResponseCode);
        if (httpResponseCode == 200)
        {
            Serial.print("Retorno PATCH de ");
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

void acessaPOST(String URN, Machine machine)
{
    HTTPClient http;
    String rota;
    String marshalledJson, prettyMarshalledJson;

    // Configura a conexão com URI via http
    rota = uri.c_str();
    rota.concat(URN.c_str());
    http.begin(rota.c_str());

    StaticJsonDocument<200> unmarshalledJson;

    unmarshalledJson["id"] = machine.id;
    unmarshalledJson["name"] = machine.name;
    unmarshalledJson["status"] = machine.status;
    unmarshalledJson["latitude"] = machine.latitude;
    unmarshalledJson["longitude"] = machine.longitude;
    unmarshalledJson["last_updated"] = machine.last_updated;
    unmarshalledJson["created"] = machine.created;

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

void acessaGET(String URN)
{
    HTTPClient http;
    String rota;

    // Configura a conexão com URI via http
    rota = uri.c_str();
    rota.concat(URN.c_str());
    http.begin(rota.c_str());

    // Envia uma requisição com o método GET
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0)
    {
        Serial.print("Código de resposta HTTP: ");
        Serial.println(httpResponseCode);
        if (httpResponseCode == 200)
        {
            Serial.print("Retorno GET de ");
            Serial.println(rota.c_str());
            String payload = http.getString();
            Serial.println(payload);
        }
        else
        {
            String payload = http.getString(); // Para evitar erros na biblioteca
            Serial.print("Erro no processamento da requisição ");
            Serial.println(httpResponseCode);
            Serial.println(http.getString());
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