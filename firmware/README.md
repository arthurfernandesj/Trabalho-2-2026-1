# Simulação SmartLockBLE no Wokwi

Este diretório contém um projeto PlatformIO + Wokwi que simula a máquina de
estados descrita no README principal (TRANCADA → VALIDANDO → AUTORIZADO /
NEGADO → ABERTA → FECHANDO, mais os alarmes de porta aberta e tentativa de
invasão), sem precisar de nenhum ESP32 físico.

## Como rodar

**Opção A — extensão Wokwi no VSCode (recomendado)**
1. Instale a extensão "Wokwi for VS Code".
2. Abra esta pasta (`firmware/`) no VSCode.
3. Rode `pio run` (ou deixe a extensão compilar) para gerar o `.pio/build/esp32dev/firmware.bin`.
4. Pressione `F1` → "Wokwi: Start Simulator".

**Opção B — wokwi-cli**
```bash
pio run
wokwi-cli .
```

## Mapa de pinos / componentes simulados

| Sinal                       | GPIO | Componente real                  |
| ---------------------------- | ---- | --------------------------------- |
| Relé (fechadura)              | D26  | Relé 1 canal                      |
| Reed switch (porta)           | D27  | Botão "btnDoor" — cada toque alterna aberta/fechada |
| Sensor de impacto              | D14  | Botão "btnImpact"                 |
| OLED SDA / SCL                 | D21 / D22 | Display SSD1306 I2C          |
| LED RGB (R/G/B)                 | D25 / D33 / D32 | LED RGB catodo comum   |
| Buzzer                          | D13  | Buzzer ativo                      |
| **DEBUG** solicitação autorizada | D4   | Botão "btnAuth"                  |
| **DEBUG** solicitação negada     | D5   | Botão "btnDeny"                  |

## Sobre o BLE

O Wokwi não faz a ponte do rádio BLE simulado com um celular físico — o
firmware sobe um servidor BLE real (`BLEDevice`) que funciona normalmente em
hardware de verdade, mas dentro do simulador ele fica "mudo". Por isso os
botões **btnAuth** e **btnDeny** existem só para testar a lógica da máquina
de estados sem precisar de um app/celular.

Quando o firmware for gravado num ESP32 físico, qualquer app BLE genérico
(ex.: nRF Connect) pode escrever a string `SMARTLOCK-KEY` na characteristic
exposta pelo serviço para liberar o acesso — é só esse último passo (validar
o pareamento com o celular de verdade) que exige hardware real.
