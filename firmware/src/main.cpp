#include <Arduino.h>
#include <string.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

// ---- Pinos ----
const int PIN_RELAY      = 26; // aciona a fechadura eletromecanica
const int PIN_DOOR_BTN   = 27; // simula o reed switch (alterna aberta/fechada a cada toque)
const int PIN_IMPACT_BTN = 14; // simula o sensor de impacto
const int PIN_BTN_AUTH   = 4;  // DEBUG: simula uma solicitacao BLE autorizada
const int PIN_BTN_DENY   = 5;  // DEBUG: simula uma solicitacao BLE negada
const int PIN_LED_R      = 25;
const int PIN_LED_G      = 33;
const int PIN_LED_B      = 32;
const int PIN_BUZZER     = 13;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---- BLE ----
// O Wokwi nao faz ponte do radio BLE simulado com um celular real, entao em
// simulacao o acesso e disparado pelos botoes AUTH/DENY. Em hardware real,
// qualquer app BLE generico (ex.: nRF Connect) pode escrever ACCESS_KEY na
// characteristic abaixo para liberar o acesso.
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "12345678-1234-1234-1234-1234567890ac"
const char *ACCESS_KEY = "SMARTLOCK-KEY";

enum State {
  LOCKED,
  VALIDATING,
  UNLOCKED,
  DOOR_OPEN,
  DOOR_OPEN_ALERT,
  CLOSING,
  DENIED_ALERT,
  BREACH_ALERT
};

State state = LOCKED;
unsigned long stateEnteredAt = 0;
bool pendingGrant = false;
bool doorIsOpen = false;

const unsigned long VALIDATE_MS    = 800;
const unsigned long UNLOCK_HOLD_MS = 3000;
const unsigned long DOOR_LIMIT_MS  = 8000;
const unsigned long DENIED_MS      = 2000;
const unsigned long CLOSING_MS     = 500;

void enterState(State s) {
  state = s;
  stateEnteredAt = millis();
}

void requestAccess(bool granted) {
  if (state != LOCKED) return;
  pendingGrant = granted;
  enterState(VALIDATING);
}

class AccessCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *c) override {
    String value = c->getValue().c_str();
    requestAccess(value == ACCESS_KEY);
  }
};

class ServerCallback : public BLEServerCallbacks {
  void onDisconnect(BLEServer *s) override {
    s->getAdvertising()->start();
  }
};

void setupBLE() {
  BLEDevice::init("SmartLockBLE");
  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallback());
  BLEService *service = server->createService(SERVICE_UUID);
  BLECharacteristic *characteristic = service->createCharacteristic(
      CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
  characteristic->setCallbacks(new AccessCallback());
  service->start();
  server->getAdvertising()->start();
}

bool pressed(int pin) {
  static bool lastLow[40] = {false};
  static unsigned long lastChange[40] = {0};
  bool isLow = digitalRead(pin) == LOW;
  bool edge = false;
  if (isLow != lastLow[pin] && millis() - lastChange[pin] > 50) {
    lastChange[pin] = millis();
    lastLow[pin] = isLow;
    edge = isLow; // dispara só na transicao HIGH->LOW (clique), nao enquanto segura
  }
  return edge;
}

void setColor(bool r, bool g, bool b) {
  digitalWrite(PIN_LED_R, r);
  digitalWrite(PIN_LED_G, g);
  digitalWrite(PIN_LED_B, b);
}

void showMessage(const char *line1, const char *line2 = "") {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 16);
  display.println(line1);
  if (strlen(line2)) {
    display.setTextSize(1);
    display.setCursor(0, 48);
    display.println(line2);
  }
  display.display();
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_DOOR_BTN, INPUT_PULLUP);
  pinMode(PIN_IMPACT_BTN, INPUT_PULLUP);
  pinMode(PIN_BTN_AUTH, INPUT_PULLUP);
  pinMode(PIN_BTN_DENY, INPUT_PULLUP);

  digitalWrite(PIN_RELAY, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  showMessage("SmartLock", "iniciando...");

  setupBLE();
  enterState(LOCKED);
}

void loop() {
  unsigned long elapsed = millis() - stateEnteredAt;

  if (pressed(PIN_BTN_AUTH)) requestAccess(true);
  if (pressed(PIN_BTN_DENY)) requestAccess(false);
  if (pressed(PIN_DOOR_BTN)) doorIsOpen = !doorIsOpen;
  if (pressed(PIN_IMPACT_BTN)) {
    if (state == LOCKED) enterState(BREACH_ALERT);
    else if (state == BREACH_ALERT) enterState(LOCKED); // reconhecimento manual do alarme
  }

  switch (state) {
    case LOCKED:
      digitalWrite(PIN_RELAY, LOW);
      digitalWrite(PIN_BUZZER, LOW);
      setColor(true, false, false);
      showMessage("PORTA", "TRANCADA");
      break;

    case VALIDATING:
      setColor(true, true, false);
      showMessage("VALIDANDO", "...");
      if (elapsed > VALIDATE_MS) {
        enterState(pendingGrant ? UNLOCKED : DENIED_ALERT);
      }
      break;

    case UNLOCKED:
      digitalWrite(PIN_RELAY, HIGH);
      setColor(false, true, false);
      showMessage("ACESSO", "LIBERADO");
      if (elapsed > UNLOCK_HOLD_MS) {
        digitalWrite(PIN_RELAY, LOW);
        enterState(doorIsOpen ? DOOR_OPEN : CLOSING);
      }
      break;

    case DOOR_OPEN:
      setColor(false, false, false);
      showMessage("PORTA", "ABERTA");
      if (!doorIsOpen) {
        enterState(CLOSING);
      } else if (elapsed > DOOR_LIMIT_MS) {
        enterState(DOOR_OPEN_ALERT);
      }
      break;

    case DOOR_OPEN_ALERT:
      setColor(true, true, false);
      digitalWrite(PIN_BUZZER, (millis() / 300) % 2);
      showMessage("PORTA!", "ABERTA HA MUITO TEMPO");
      if (!doorIsOpen) {
        digitalWrite(PIN_BUZZER, LOW);
        enterState(CLOSING);
      }
      break;

    case CLOSING:
      digitalWrite(PIN_BUZZER, LOW);
      setColor(true, false, false);
      showMessage("FECHANDO", "...");
      if (elapsed > CLOSING_MS) enterState(LOCKED);
      break;

    case DENIED_ALERT:
      setColor(true, false, false);
      digitalWrite(PIN_BUZZER, HIGH);
      showMessage("ACESSO", "NEGADO");
      if (elapsed > DENIED_MS) {
        digitalWrite(PIN_BUZZER, LOW);
        enterState(LOCKED);
      }
      break;

    case BREACH_ALERT:
      digitalWrite(PIN_BUZZER, HIGH);
      setColor((millis() / 200) % 2, false, false);
      showMessage("INVASAO!", "TENTATIVA DETECTADA");
      break;
  }
}
