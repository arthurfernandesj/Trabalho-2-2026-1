# SmartLockBLE - Sistema Inteligente de Controle de Acesso via Bluetooth com ESP32

## Integrantes

* Arthur Fernandes Silva de Jesus - 221022471
* Nome Completo — Matrícula
* Nome Completo — Matrícula
* Nome Completo — Matrícula

---

# 1. Descrição do Produto Selecionado

## Produto estudado

O produto selecionado para estudo é uma **fechadura inteligente (Smart Lock)** com autenticação via Bluetooth Low Energy (BLE).

As Smart Locks são sistemas embarcados que substituem as chaves tradicionais por mecanismos eletrônicos de autenticação, permitindo maior controle sobre os acessos e integração com sistemas inteligentes de automação residencial e predial.

Esses sistemas são amplamente utilizados em:

* Residências;
* Condomínios;
* Escritórios;
* Laboratórios;
* Salas de servidores;
* Ambientes industriais.

---

## Funções principais

* Controle de acesso sem utilização de chaves físicas;
* Destravamento remoto via smartphone;
* Monitoramento do estado da porta;
* Registro de tentativas de acesso;
* Detecção de acessos não autorizados;
* Sinalização local através de LEDs e buzzer.

---

## Público-alvo

* Residências;
* Empresas;
* Universidades;
* Laboratórios;
* Condomínios;
* Data Centers.

---

## Contexto de utilização

As Smart Locks modernas oferecem maior praticidade e segurança em comparação aos sistemas mecânicos tradicionais, permitindo o gerenciamento digital dos acessos e a integração com sistemas inteligentes de monitoramento e automação.

---

# 2. Componentes e Sensores Utilizados no Produto

Os principais componentes utilizados em fechaduras inteligentes comerciais incluem:

| Componente                 | Função                     |
| -------------------------- | -------------------------- |
| Microcontrolador embarcado | Controle do sistema        |
| Bluetooth BLE              | Comunicação com smartphone |
| Fechadura eletromecânica   | Travamento da porta        |
| Sensor magnético           | Detecção de abertura       |
| LEDs indicadores           | Interface visual           |
| Buzzer                     | Alarme sonoro              |
| Aplicativo móvel           | Interface do usuário       |

---

# 3. Tecnologias de Comunicação e Controle

As principais tecnologias embarcadas presentes nesse tipo de produto são:

* Bluetooth Low Energy (BLE);
* GPIO;
* PWM;
* Máquinas de estados;
* Controle em tempo real;
* Técnicas de economia de energia.

---

# 4. Análise Técnica do Funcionamento

## Sensores

### Chave Magnética (Reed Switch)

Responsável por identificar se a porta encontra-se aberta ou fechada.

### Sensor de Impacto

Permite detectar tentativas de arrombamento ou impactos na estrutura da porta.

---

## Atuadores

### Relé de 1 Canal

Utilizado para simular o acionamento da fechadura elétrica.

### LED RGB

Responsável pela sinalização visual do estado do sistema.

* Verde → acesso liberado;
* Vermelho → acesso negado;
* Amarelo → porta aberta ou aguardando fechamento.

### Buzzer Ativo

Responsável pela sinalização sonora em situações de alerta ou tentativa de invasão.

### Display OLED I2C

Exibe localmente informações sobre o estado do sistema.

Exemplos:

```text
PORTA TRANCADA
```

```text
ACESSO LIBERADO
```

```text
ACESSO NEGADO
```

---

## Unidade de Controle

### ESP32 DevKit

O ESP32 é responsável por:

* Gerenciar as conexões Bluetooth;
* Validar as solicitações de acesso;
* Controlar os atuadores;
* Monitorar os sensores;
* Executar a lógica do sistema.

---

## Comunicação

### Bluetooth Low Energy (BLE)

A comunicação entre o smartphone e a fechadura será realizada através de BLE.

As principais vantagens dessa tecnologia são:

* Baixo consumo energético;
* Baixa latência;
* Maior simplicidade de implementação;
* Segurança adequada para comunicação de curta distância.

---

# 5. Proposta de Reprodução Utilizando ESP32

## Componentes Utilizados

| Componente        | Função                     |
| ----------------- | -------------------------- |
| ESP32 DevKit      | Unidade de controle        |
| Bluetooth BLE     | Comunicação com smartphone |
| Relé 1 Canal      | Simulação da fechadura     |
| Reed Switch       | Detecção da porta          |
| Sensor de Impacto | Detecção de arrombamento   |
| LED RGB           | Interface visual           |
| Buzzer Ativo      | Alarme sonoro              |
| OLED I2C          | Interface local            |

---

## Funcionamento Proposto

### Estado Normal

* Porta trancada;
* LED vermelho aceso;
* OLED exibindo:

```text
PORTA TRANCADA
```

---

### Solicitação de Acesso

O usuário aproxima-se da porta e utiliza um aplicativo no smartphone para conectar ao ESP32 via Bluetooth.

---

### Acesso Autorizado

Quando a autenticação é validada:

* O relé é acionado;
* A fechadura é destravada;
* O LED verde é aceso;
* O OLED exibe:

```text
ACESSO LIBERADO
```

Após alguns segundos:

* O relé é desligado;
* A porta retorna ao estado trancado.

---

### Acesso Negado

Caso um dispositivo não autorizado tente se conectar:

* O LED vermelho é acionado;
* O buzzer emite um alerta;
* O OLED exibe:

```text
ACESSO NEGADO
```

---

### Porta Aberta por Muito Tempo

Caso o reed switch detecte a porta aberta por um período superior ao limite configurado:

* O buzzer é acionado;
* O LED amarelo é ativado;
* O OLED exibe:

```text
PORTA ABERTA
```

---

### Tentativa de Arrombamento

Caso o sensor de impacto detecte vibração enquanto a porta permanece trancada:

* O buzzer é acionado continuamente;
* O LED vermelho pisca;
* O OLED exibe:

```text
TENTATIVA DE INVASAO
```

---

# 6. Máquina de Estados

```text
TRANCADA
    │
    ├── Solicitação BLE
    │        │
    │        ▼
    │    VALIDANDO
    │        │
    │   ┌────┴─────┐
    │   │          │
    │   ▼          ▼
    │AUTORIZADO  NEGADO
    │   │          │
    │   ▼          ▼
    │ABERTA     ALARME
    │   │
    │   ▼
    └─FECHANDO
        │
        ▼
     TRANCADA
```

---

# 7. Diagrama Conceitual

```text
                Smartphone
                     │
                Bluetooth BLE
                     │
                     ▼
              ┌───────────┐
              │   ESP32   │
              └─────┬─────┘
                    │
    ┌───────────────┼────────────────┐
    │               │                │
    ▼               ▼                ▼
 Reed Switch      Relé           LED RGB
    │
    ▼
Sensor Impacto
    │
    ▼
 Buzzer Ativo
    │
    ▼
 OLED I2C
```

---

# 8. Limitações e Desafios Esperados

* Alcance limitado do Bluetooth;
* Dependência da alimentação elétrica;
* Ausência de autenticação biométrica;
* Menor robustez em comparação com soluções comerciais;
* Limitação do número de usuários cadastrados localmente.

---

# 9. Tecnologias Críticas Envolvidas

* Bluetooth Low Energy (BLE);
* Sistemas embarcados de tempo real;
* Controle por eventos;
* Comunicação sem fio;
* Máquinas de estado;
* Gerenciamento energético.

---

# 10. Trabalhos Futuros

Possíveis melhorias incluem:

* Integração com Wi-Fi;
* Controle remoto via Internet;
* Aplicativo móvel dedicado;
* Registro persistente de eventos;
* Integração com plataformas IoT;
* Autenticação biométrica;
* Notificações em tempo real.

---

# 11. Produtos Comerciais Relacionados

* August Smart Lock Pro
* Yale Assure Lock 2
* Nuki Smart Lock
* Samsung SHP-DP609
* SwitchBot Lock Pro

---

# 12. Referências Bibliográficas

