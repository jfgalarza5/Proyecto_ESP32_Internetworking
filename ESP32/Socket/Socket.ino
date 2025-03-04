#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

const char* ssid = "HP_LAPTOP 4294";           // Reemplaza con el nombre de tu red WiFi
const char* password = "12345678";   // Reemplaza con tu contraseña WiFi

int redPin = 2;
int bluePin = 4;
int greenPin = 16;

WebSocketsServer webSocket = WebSocketsServer(8080);  // Puerto 81 para WebSocket

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);           // Inicia la conexión WiFi

  // Espera hasta que se conecte
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a WiFi!");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());        // Muestra la dirección IP obtenida

  webSocket.begin();  // Inicia el servidor WebSocket
  webSocket.onEvent(webSocketEvent);  // Establece el evento cuando se recibe un mensaje
}

void loop() {
  webSocket.loop();  // Escucha los mensajes entrantes
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  String message;
  DynamicJsonDocument doc(1024);
  int red = 0, green = 0, blue = 0;  // Inicializar las variables fuera del switch

  switch (type) {
    case WStype_TEXT:
      // Deserializar el JSON recibido
      message = String((char*)payload);
      deserializeJson(doc, message);

      red = doc["red"];
      green = doc["green"];
      blue = doc["blue"];

      setRGB(red, green, blue);

      // Responder al cliente
      webSocket.sendTXT(num, "Mensaje recibido");
      break;
      
    case WStype_DISCONNECTED:
      Serial.println("Cliente desconectado");
      break;

    case WStype_CONNECTED:
      Serial.println("Cliente conectado");
      break;

    default:
      break;
  }
}

void setRGB(int red, int green, int blue) {
  // Usamos analogWrite en los pines para ajustar el brillo de cada color
  analogWrite(redPin, red);     // Controlar el color rojo
  analogWrite(greenPin, green); // Controlar el color verde
  analogWrite(bluePin, blue);   // Controlar el color azul
}
