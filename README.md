# CheckPoint 5 - Edge Computing e Computer Sistems
## Integrantes do Grupo
- **Luan Orlandelli** - 554747
- **Igor Medeiros** - 555337
- **Jorge Luiz** - 554418
- **Arthur Bobadilla** - 555056
  
# Projeto de Monitoramento de Condições Ambientais para Vinícola

Este projeto tem como objetivo monitorar as condições ambientais de uma vinícola, especificamente temperatura, umidade e luminosidade, utilizando sensores DHT11, LDR e um ESP32. Os dados são enviados via MQTT para serem acessados remotamente através do aplicativo MyMQTT.

## Componentes Utilizados

- **ESP32**: Microcontrolador responsável por processar os dados dos sensores e enviá-los via MQTT.
- **DHT11**: Sensor utilizado para medir a **temperatura** e **umidade** do ambiente.
- **LDR (Light Dependent Resistor)**: Sensor que mede a **luminosidade** do ambiente.
- **Resistor 10kΩ**: Usado em conjunto com o LDR para formar um divisor de tensão.
- **Fonte de alimentação**: Para alimentar o ESP32.
- **Cabo USB**: Para programar o ESP32.

## Diagrama de Conexões

### Conexão DHT11:
- VCC → 3.3V no ESP32
- GND → GND no ESP32
- Data → Pino GPIO escolhido (ex: GPIO 4)

### Conexão LDR:
- Um lado do LDR → 3.3V no ESP32
- Outro lado do LDR → Resistor 10kΩ e entrada analógica do ESP32 (ex: GPIO 34)
- O outro lado do resistor vai para o GND.

## Configuração do Broker MQTT

1. **Broker MQTT**: Utilize um serviço de broker MQTT (local ou remoto). Pode ser o Mosquitto ou outro de sua escolha.
2. **Tópicos MQTT**: Configure os tópicos para o envio dos dados:
   - `/TEF/hosp051/attrs/p`
3. **Aplicativo MyMQTT**: Utilize este aplicativo para monitorar os dados enviados. Basta configurar a conexão com o broker inserindo o IP e a porta adequados.

## Código-fonte (ESP32)

Você precisará de uma IDE para programar o ESP32 (ex: Arduino IDE ou VSCode com PlatformIO). Certifique-se de que as bibliotecas `DHT`, `Wifi` e `PubSubClient` estão instaladas.
(Arquivo .ino disponibilizado no repositório)

## Configurando o MyMQTT

1. Instale o aplicativo MyMQTT no seu dispositivo.
2. Configure o servidor MQTT inserindo o IP do broker MQTT e a porta (geralmente 1883).
3. Adicione os tópicos que você configurou no ESP32, no nosso caso: `/TEF/hosp051/attrs/p`.
4. Agora você pode visualizar as condições ambientais em tempo real no aplicativo.

## Instruções para Montagem

1. Monte os sensores conforme descrito no diagrama de conexões.
2. Configure o ESP32 com o código fornecido e ajuste as credenciais WiFi e MQTT.
3. Utilize uma fonte de alimentação confiável para manter o ESP32 funcionando continuamente.

## Funcionamento Geral

- O ESP32 coleta dados dos sensores DHT11 e LDR.
- Esses dados são enviados periodicamente (a cada 2 segundos) para um broker MQTT.
- Qualquer dispositivo conectado ao mesmo broker pode acessar os dados, incluindo o aplicativo MyMQTT.

## Considerações Finais

Este projeto permite o monitoramento remoto das condições ambientais em uma vinícola, garantindo que a temperatura, umidade e luminosidade estejam sob controle, o que é essencial para a qualidade do armazenamento do vinho.
