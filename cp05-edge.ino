#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Configurações - variáveis editáveis
const char* default_SSID = "FIAP-IBM"; // Nome da rede Wi-Fi, importante a sua rede wi-fi ter suporte a dispositivos de 2,4GHz devido ao ESP32 utilizar essa frequência 
const char* default_PASSWORD = "Challenge@24!"; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "18.208.160.16"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/hosp051/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/hosp051/attrs"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/hosp051/attrs/p"; // Tópico MQTT de envio de informações para Broker
const char* default_ID_MQTT = "fiware_051"; // ID MQTT
const int default_D4 = 2; // Pino do LED onboard
// Declaração da variável para o prefixo do tópico
const char* topicPrefix = "hosp051";

// Pinos dos sensores
#define DHTPIN 4  // Pino do DHT11
#define LDRPIN 34 // Pino do LDR

// Inicialização do DHT22
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Variáveis para Wi-Fi e MQTT
WiFiClient espClient;
PubSubClient MQTT(espClient);

void initSerial() {
    Serial.begin(115200);
}

void initWiFi() {
    delay(10);
    Serial.println("------Conexão WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(default_SSID);
    Serial.println("Aguarde...");
    
    WiFi.begin(default_SSID, default_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.println("Conectado com sucesso!");
    Serial.print("IP obtido: ");
    Serial.println(WiFi.localIP());
}

void initMQTT() {
    MQTT.setServer(default_BROKER_MQTT, default_BROKER_PORT);
}

void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("Tentando se conectar ao Broker MQTT: ");
        Serial.println(default_BROKER_MQTT);

        if (MQTT.connect(default_ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
        } else {
            Serial.print("Falha ao conectar, rc=");
            Serial.print(MQTT.state());
            Serial.println(". Tentando novamente em 5 segundos...");
            delay(5000);
        }
    }
}

void sendSensorData() {
    // Lendo o DHT22
    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();

    if (isnan(temperatura) || isnan(umidade)) {
        Serial.println("Falha ao ler o sensor DHT22!");
        return;
    }

    // Lendo o LDR
    int ldrValue = map((analogRead(LDRPIN)), 0, 4095, 0, 100);
    
    // Printando os valores no Serial Monitor
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print(" ºC  | Umidade: ");
    Serial.print(umidade);
    Serial.print(" %  | LDR: ");
    Serial.println(ldrValue);

    // Criando as mensagens para o MQTT
    String dhtPayload = "{\"temperatura\":";
    dhtPayload += String(temperatura);
    dhtPayload += ", \"umidade\":";
    dhtPayload += String(umidade);
    dhtPayload += "}";

    String ldrPayload = "{\"ldr\":";
    ldrPayload += String(ldrValue);
    ldrPayload += "}";

    // Publicando os dados nos tópicos MQTT
    MQTT.publish(default_TOPICO_PUBLISH_2, dhtPayload.c_str());
    MQTT.publish(default_TOPICO_PUBLISH_2, ldrPayload.c_str());
}

void setup() {
    initSerial();
    initWiFi();
    initMQTT();
    
    // Inicializando o sensor DHT22
    dht.begin();
}

void loop() {
    if (!MQTT.connected()) {
        reconnectMQTT();
    }
    
    MQTT.loop();
    
    // Enviar os dados a cada 2 segundos
    sendSensorData();
    delay(2000);
}
