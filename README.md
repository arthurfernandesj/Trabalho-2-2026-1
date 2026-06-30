# SmartLockBLE - Sistema Inteligente de Controle de Acesso via Bluetooth com ESP32

## Integrantes

* Arthur Fernandes Silva de Jesus - 221022471
* Anne de Capdeville - 221031111
* Vitor Carvalho Pereira - 211062615


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

## 12. Comparativo com Produtos Similares do Mercado

Para balizar o desenvolvimento do protótipo, analisamos 5 produtos comerciais líderes no segmento de Smart Locks. Esta análise evidencia que o padrão ouro da indústria utiliza BLE para interações de proximidade combinadas com baixo consumo energético.

As especificações e características técnicas detalhadas na tabela abaixo foram coletadas a partir dos manuais, documentações oficiais e fichas técnicas disponibilizadas pelos respectivos fabricantes.

| Produto | Conectividade Principal | Alimentação | Acesso de Backup (Emergência) | Integração Smart Home | Diferencial Técnico | Referência |
| :--- | :--- | :--- | :--- | :--- | :--- | :---: |
| **August Smart Lock Pro** | BLE / Wi-Fi (via Bridge) | 4 Pilhas AA | Chave física original | HomeKit, Alexa, Google | *Retrofit* (Instala-se sobre a fechadura existente mantendo a parte exterior intocada). | [[11]](#ref-august) |
| **Yale Assure Lock 2** | BLE (Wi-Fi Opcional) | 4 Pilhas AA | Teclado Numérico ou Chave | Alexa, Google | Módulos de rede intercambiáveis, permitindo upgrade futuro de hardware. | [[12]](#ref-yale) |
| **Nuki Smart Lock 3.0 Pro** | BLE / Wi-Fi | Bateria Recarregável | Chave física original | Apple, Google, MQTT local | Funcionalidade *Auto-unlock* georreferenciada altamente otimizada (via app do celular). | [[13]](#ref-nuki) |
| **Samsung SHP-DP609** | Wi-Fi / BLE / NFC | 8 Pilhas AA | Biometria, Tag NFC, Senha | App Samsung SmartThings | Mecanismo *Push-Pull* mecânico aliado a multiplas redundâncias eletrônicas. | [[14]](#ref-samsung) |
| **SwitchBot Lock Pro** | BLE | 4 Pilhas AA | Chave física original | Matter via Hub Externo | Uso de adaptadores não destrutivos via fita 3M VHB para instalação universal. | [[15]](#ref-switchbot) |

---

# 13. Pesquisa Bibliográfica e Tecnológica

Abaixo apresentamos o levantamento bibliográfico de artigos científicos (Journals) que fundamentam as tecnologias e as aplicações deste produto embarcado:

### Tecnologias Base (Protocolos, Arquitetura, Segurança)

1. **Design of a smart hydroponics monitoring system using an ESP32 microcontroller and the Internet of Things** 
* **Resumo e Contribuição:** Embora focado na agricultura inteligente, este estudo detalha a arquitetura de *hardware* e *software* do microcontrolador ESP32 atuando no centro de um ecossistema IoT. O artigo demonstra a eficácia da ESP32 em coletar dados de sensores ambientais, processá-los e acionar atuadores (relés/bombas) automaticamente, além de estabelecer comunicação bidirecional com um aplicativo de smartphone. Isso fundamenta e valida tecnicamente a viabilidade da nossa proposta de usar a ESP32 para monitorar *reed switches* e sensores de impacto, enquanto controla o relé da fechadura e o aplicativo do usuário de forma simultânea.
   * **Link do Artigo:** [Acessar artigo](https://doi.org/10.1016/j.mex.2023.102401)
2. **IoT-Enabled Smart Drip Irrigation System Using ESP32** 
* **Resumo e Contribuição:** Este artigo apresenta o desenvolvimento de um sistema IoT utilizando o microcontrolador ESP32 para automação de tarefas baseadas em leitura de sensores. O trabalho demonstra como a ESP32 gerencia a leitura contínua de variáveis físicas, toma decisões lógicas e aciona relés (no caso, válvulas eletromecânicas) enquanto mantém comunicação em tempo real com um aplicativo móvel. Para o nosso projeto de fechadura inteligente, este estudo serve como validação arquitetônica fundamental, provando que a ESP32 possui capacidade de processamento e conectividade suficientes para monitorar nossos sensores de segurança (porta e impacto), acionar a trava eletromecânica via relé e trocar dados com o smartphone sem travamentos (*multitasking*).
   * **Link do Artigo:** [Acessar artigo](https://www.mdpi.com/2624-831X/4/3/12)

3. **Design and Implementation RESTful API for IoT Based Smart Home Systems** 
* **Resumo e Contribuição:** O artigo discute o desenvolvimento e a implementação de APIs RESTful para a integração de dispositivos embarcados em sistemas de automação residencial (*Smart Home*). Embora o protótipo inicial da nossa fechadura inteligente utilize comunicação local via BLE, este estudo fundamenta a nossa seção de trabalhos futuros. Ele demonstra como a ESP32 possui capacidade nativa para gerenciar requisições HTTP via Wi-Fi, permitindo que a fechadura seja facilmente integrada a ecossistemas complexos, servidores locais e assistentes virtuais para controle remoto e registro de logs em nuvem.
   * **Link do Artigo:** [Acessar artigo](https://doi.org/10.1051/e3sconf/202346502061)

4. **Electric Drive with an Adaptive Controller and Wireless Communication System** 
* **Resumo e Contribuição:** Este artigo explora o controle remoto de acionamentos elétricos utilizando o microcontrolador ESP32 como ponte de comunicação sem fio e interface de controle. No contexto da nossa fechadura inteligente, o sistema depende de um atuador eletromecânico (solenoide ou motor operado via relé) para realizar o destravamento físico da porta. O estudo valida a confiabilidade do ESP32 para gerenciar o acionamento de cargas elétricas/mecânicas de forma assíncrona, garantindo que o comando de liberação enviado pelo smartphone seja executado pelo hardware da fechadura de forma segura.
   * **Link do Artigo:** [Acessar artigo](https://doi.org/10.3390/fi15020049)

5. **Security and Privacy Threats for Bluetooth Low Energy in IoT and Wearable Devices: A Comprehensive Survey** 
* **Resumo e Contribuição:** Este artigo apresenta um mapeamento abrangente das vulnerabilidades de segurança e privacidade inerentes ao protocolo Bluetooth Low Energy (BLE) na Internet das Coisas, citando explicitamente as fechaduras inteligentes (*smart locks*) como alvos críticos. Para o desenvolvimento do nosso projeto, o estudo reforça a necessidade de não dependermos apenas do pareamento padrão do BLE na ESP32, justificando a implementação de camadas adicionais de autenticação de dispositivos, criptografia e proteção de privacidade do usuário na troca de dados com o smartphone.
   * **Link do Artigo:** [Acessar artigo](https://ieeexplore.ieee.org/document/9706334)

6. **Inside Bluetooth Low Energy [Book Review]** 
* **Resumo e Contribuição:** Publicada em um periódico da IEEE, esta análise detalha uma obra focada exclusivamente na arquitetura e nas camadas do protocolo Bluetooth Low Energy (BLE). Para a nossa fechadura inteligente, a compreensão profunda das pilhas do protocolo (*protocol stacks*) é essencial para equilibrar duas variáveis críticas na engenharia de sistemas embarcados: o baixo consumo de energia e a segurança. O texto fornece a base teórica para justificar a adoção do BLE no ecossistema da ESP32, garantindo comunicação confiável com o smartphone em cenários de automação e segurança.
   * **Link do Artigo:** [Acessar artigo](https://ieeexplore.ieee.org/document/8014283)

7. **Security Vulnerabilities in Bluetooth Technology as Used in IoT**
* **Resumo e Contribuição:** Este artigo realiza uma análise profunda das vulnerabilidades de segurança inerentes à tecnologia Bluetooth (especialmente o BLE) aplicada à Internet das Coisas. O estudo cita expressamente as fechaduras automatizadas (*door locks*) como dispositivos críticos que carecem de infraestrutura de segurança centralizada. Para o desenvolvimento do protótipo com a ESP32, este texto funciona como o principal balizador de risco, ressaltando a necessidade de programar contramedidas e técnicas de mitigação diretamente no *firmware* para evitar a exploração de brechas conhecidas durante o destravamento da porta.
   * **Link do Artigo:** [Acessar artigo](https://doi.org/10.3390/jsan7030028)

8. **Open-Source MQTT-Based End-to-End IoT System for Smart City Scenarios**
* **Resumo e Contribuição:** Este artigo detalha a implementação de um sistema IoT utilizando o microcontrolador ESP32 para receber dados de sensores via Bluetooth Low Energy (BLE) e retransmiti-los para a nuvem através do protocolo MQTT. Para o nosso projeto de fechadura inteligente, este estudo é de extrema relevância, pois valida a estabilidade do ESP32 no gerenciamento de conexões BLE locais e fundamenta tecnicamente a nossa proposta de trabalhos futuros: a integração da fechadura com a rede Wi-Fi para enviar *logs* de acesso em tempo real a um servidor remoto.
   * **Link do Artigo:** [Acessar artigo](https://doi.org/10.3390/fi14020057)

9. **IoT-Based Smart Remote Door Lock and Monitoring System Using an Android Application**
   * **Resumo e Contribuição:** Este artigo apresenta o projeto e a implementação de uma fechadura inteligente baseada em ESP32, controlada remotamente por um aplicativo Android e integrada a sensores de monitoramento da porta, atuador de travamento e notificações ao usuário. Diferentemente dos demais trabalhos levantados — que validam a arquitetura do ESP32 em domínios correlatos (irrigação, hidroponia, automação) —, este estudo aborda exatamente o mesmo caso de uso da nossa proposta: o controle de acesso eletrônico de uma porta. Ele fundamenta diretamente as decisões de *hardware* e a lógica de estados do nosso protótipo, comprovando a viabilidade de o ESP32 gerenciar o destravamento, monitorar o estado da porta e comunicar-se com o dispositivo móvel do usuário em tempo real.
   * **Link do Artigo:** [Acessar artigo](https://doi.org/10.3390/engproc2024076085)

10. **An improved authentication scheme for BLE devices with no I/O capabilities**
   * **Resumo e Contribuição:** Este artigo propõe um esquema aprimorado de autenticação para dispositivos BLE desprovidos de capacidade de entrada/saída (sem teclado ou tela), cenário em que o modelo de pareamento *Just Works* fica exposto a ataques *Man-in-the-Middle*. Como a nossa fechadura inteligente é justamente um dispositivo BLE sem interface física de digitação, este estudo é diretamente aplicável: oferece uma alternativa concreta para reforçar a autenticação no pareamento entre o smartphone e o ESP32, mitigando as vulnerabilidades apontadas pelos *surveys* de segurança já citados ([5] e [7]) e embasando tecnicamente as contramedidas a serem implementadas no *firmware*.
   * **Link do Artigo:** [Acessar artigo](https://doi.org/10.1016/j.comcom.2023.01.001)

---

# 14. Referências Bibliográficas
### Artigos Científicos e Acadêmicos 

* [1] SNEINEH, A. A.; SHABANEH, A. A. Design of a smart hydroponics monitoring system using an ESP32 microcontroller and the Internet of Things. *MethodsX*, Elsevier, v. 11, p. 102401, dez. 2023. Disponível em: <https://doi.org/10.1016/j.mex.2023.102401>.
* [2] PEREIRA, G. P.; CHAARI, M. Z.; DAROGE, F. IoT-Enabled Smart Drip Irrigation System Using ESP32. *IoT*, MDPI, v. 4, n. 3, p. 221-243, jul. 2023. Disponível em: <https://www.mdpi.com/2624-831X/4/3/12>.
* [3] RAMELAN, A. et al. Design and Implementation RESTful API for IoT Based Smart Home Systems. *E3S Web of Conferences*, EDP Sciences, v. 465, p. 02061, dez. 2023. Disponível em: <https://doi.org/10.1051/e3sconf/202346502061>.
* [4] MALARCZYK, M. et al. Electric Drive with an Adaptive Controller and Wireless Communication System. *Future Internet*, MDPI, v. 15, n. 2, p. 49, jan. 2023. Disponível em: <https://www.mdpi.com/1999-5903/15/2/49>.
* [5] BARUA, A. et al. Security and Privacy Threats for Bluetooth Low Energy in IoT and Wearable Devices: A Comprehensive Survey. *IEEE Open Journal of the Communications Society*, IEEE, v. 3, 2022. Disponível em: <https://ieeexplore.ieee.org/document/9706334>.
* [6] PANWAR, G.; MISRA, S. Inside Bluetooth Low Energy (Gupta, N.) [Book Review]. *IEEE Wireless Communications*, IEEE, v. 24, n. 4, p. 2-3, ago. 2017. Disponível em: <https://ieeexplore.ieee.org/document/8014283>.
* [7] LONZETTA, A. M. et al. Security Vulnerabilities in Bluetooth Technology as Used in IoT. *Journal of Sensor and Actuator Networks*, MDPI, v. 7, n. 3, p. 28, jul. 2018. Disponível em: <https://www.mdpi.com/2224-2708/7/3/28>.
* [8] D'ORTONA, C.; TARCHI, D.; RAFFAELLI, C. Open-Source MQTT-Based End-to-End IoT System for Smart City Scenarios. *Future Internet*, MDPI, v. 14, n. 2, p. 57, fev. 2022. Disponível em: <https://www.mdpi.com/1999-5903/14/2/57>.
* [9] SONAMONI, J. S. et al. IoT-Based Smart Remote Door Lock and Monitoring System Using an Android Application. *Engineering Proceedings*, MDPI, v. 76, n. 1, p. 85, 2024. Disponível em: <https://doi.org/10.3390/engproc2024076085>.
* [10] GUPTA, C.; VARSHNEY, G. An improved authentication scheme for BLE devices with no I/O capabilities. *Computer Communications*, Elsevier, v. 200, p. 42-53, fev. 2023. Disponível em: <https://doi.org/10.1016/j.comcom.2023.01.001>.

### Documentação Técnica de Produtos Comerciais 

* <a id="ref-august"></a>[11] AUGUST HOME INC. **August Smart Lock Pro Installation and Technical Specifications Guide**. San Francisco, CA, 2022. Disponível em: <https://support.august.com/>.
* <a id="ref-yale"></a>[12] YALE HOME. **Yale Assure Lock 2 Product Manual & Smart Home Integrations**. Berlin, CT: ASSA ABLOY, 2023. Disponível em: <https://www.yalehome.com/>.
* <a id="ref-nuki"></a>[13] NUKI HOME SOLUTIONS GMBH. **Nuki Smart Lock 3.0 Technical Data Sheet & Local MQTT API Specifications**. Graz, Austria, 2023. Disponível em: <https://nuki.io/en/service/>.
* <a id="ref-samsung"></a>[14] SAMSUNG SDS. **SHP-DP609 Smart Door Lock User Manual and Digital Interfaces**. Seul, Coreia do Sul, 2021. Disponível em: <https://www.samsungsmartlock.com/>.
* <a id="ref-switchbot"></a>[15] SWITCHBOT. **SwitchBot Lock Pro Specifications, Matter Support and Installation Guide**. Tóquio, Japão, 2024. Disponível em: <https://support.switch-bot.com/>.