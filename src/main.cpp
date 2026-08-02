#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const short sensorPin = 9;   // Пін датчика руху/присутності (HC-SR501 / RCWL-0516)
int presenceCount = 0;       // Лічильник присутньостей (int замість short для більших значень)
short lastSensorState = LOW; // Попередній стан датчика

unsigned long last_trigger_time = 0;
const unsigned long DEBOUNCE_TIME = 1000; // Мінімальна пауза між зарахуванням подій (1 сек)

void setup()
{
  pinMode(sensorPin, INPUT);

  lcd.init();
  lcd.backlight();

  // Статичний текст
  lcd.setCursor(0, 0);
  lcd.print("Presence Count:");

  lcd.setCursor(0, 1);
  lcd.print(presenceCount);
}

void loop()
{
  unsigned long cur_time = millis();
  short currentSensorState = digitalRead(sensorPin);

  // Фіксуємо ТІЛЬКИ момент появи людини (перехід з LOW в HIGH)
  if (currentSensorState == HIGH && lastSensorState == LOW)
  {
    // Зараховуємо подій лише якщо з моменту останнього заліку минула затримка
    if (cur_time - last_trigger_time >= DEBOUNCE_TIME)
    {
      last_trigger_time = cur_time;
      presenceCount++;

      // Оновлюємо екран без повного стирання (щоб не було мерехтіння)
      lcd.setCursor(0, 1);
      lcd.print(presenceCount);
      lcd.print("        "); // Затираємо залишки старих довгих чисел
    }
  }

  // Оновлюємо стан датчика
  lastSensorState = currentSensorState;
}