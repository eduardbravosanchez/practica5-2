#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configuración de la pantalla
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Configuración del sensor
Adafruit_AHTX0 aht;

// Pines I2C definidos previamente
const int SDA_PIN = 9;
const int SCL_PIN = 8;

void setup() {
  Serial.begin(115200);
  
  // Iniciar bus I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  // Iniciar Pantalla
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error: Pantalla no encontrada");
    for(;;);
  }

  // Iniciar Sensor
  if (!aht.begin()) {
    Serial.println("Error: Sensor AHT10 no encontrado");
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.print("Error: Sensor!");
    display.display();
    for(;;);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp); // Obtener datos del sensor

  display.clearDisplay();
  
  // Encabezado
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("MONITOR AMBIENTAL");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  // Mostrar Temperatura
  display.setCursor(0, 20);
  display.setTextSize(2);
  display.print("T: ");
  display.print(temp.temperature, 1);
  display.drawCircle(105, 22, 2, SSD1306_WHITE); // Simbolo de grado
  display.setCursor(110, 20);
  display.print("C");

  // Mostrar Humedad
  display.setCursor(0, 42);
  display.print("H: ");
  display.print(humidity.relative_humidity, 1);
  display.print(" %");

  // Pequeña barra de progreso para la humedad en la parte inferior
  int barWidth = map(humidity.relative_humidity, 0, 100, 0, 128);
  display.fillRect(0, 60, barWidth, 4, SSD1306_WHITE);

  display.display(); // Actualizar pantalla
  
  delay(2000); // Leer cada 2 segundos
}