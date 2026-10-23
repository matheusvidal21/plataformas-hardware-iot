# Plataformas de Hardware para IoT

**Portfólio acadêmico de Matheus Costa Vidal · IMD0904 · UFRN · 2026.2**

Da análise de circuitos à programação do ESP32: este repositório reúne minhas atividades na disciplina **Plataformas de Hardware para Internet das Coisas**, ministrada pelo professor **Daniel Lopes Martins**. Os trabalhos registram a integração entre sensores, atuadores e software, com códigos, documentação técnica e evidências das montagens.

[Experimentos](#experimentos-em-destaque) · [Roteiros](#roteiros-de-laboratório) · [Projeto final](#projeto-final) · [Materiais](#organização-e-consulta)

## Experimentos em destaque

### Controle de LEDs com entradas analógicas

Aplicação com ESP32 que alterna entre a leitura de um potenciômetro e de um LDR por meio de um botão. A leitura do ADC controla o brilho de um LED por PWM e o intervalo de alternância de outro LED, reunindo entradas digitais, entradas analógicas e temporização com `millis()`.

[Relatório e evidências](reports/report-1/README.md) · [Código-fonte](reports/report-1/pratica1.ino) · [Enunciado](reports/report-1/HIoT_06.pdf)

### Medição de distância com sensor ultrassônico

Aplicação com ESP32 e HC-SR04 que calcula a distância pelo tempo de retorno do eco e exibe o resultado na porta serial. O limite máximo de detecção é configurável pela serial; o sinal ECHO utiliza um divisor de tensão para adequação à entrada do ESP32.

[Relatório e evidências](reports/report-2/README.md) · [Código-fonte](reports/report-2/source.ino) · [Simulação](reports/report-2/wokwi/diagram.json) · [Enunciado](reports/report-2/HIoT_08.pdf)

### Medição de distância com HC-SR04 por interrupção

Aplicação com ESP32 que usa interrupção nas bordas do sinal ECHO para medir a distância e a informa pela porta serial.

[Relatório e evidências](reports/report-3/README.md) · [Código-fonte](reports/report-3/source.ino) · [Enunciado](reports/report-3/HIoT_11.pdf)

## Roteiros de laboratório

Os registros abaixo documentam as práticas desenvolvidas ao longo da disciplina.

| Tema | Conceitos explorados |
|---|---|
| [Resistores e circuitos RC](lab-guides/lab-guide-1/) | Lei de Ohm, constante de tempo e carga/descarga de capacitores |
| [Diodos e portas lógicas](lab-guides/lab-guide-2/) | Polarização direta e reversa, portas OR e AND |
| [GPIO e resistores de pull](lab-guides/lab-guide-1-gpio/) | Entrada flutuante, pull-up e pull-down |
| [Conversão analógico-digital](lab-guides/lab-guide-2-adc/) | Leitura digital e analógica, milivolts e atenuação |
| [Controle digital e PWM](lab-guides/lab-guide-3-pwm/) | Duty cycle, frequência e acionamento com transistor |

## Projeto final

**Monitoramento de Bebedouro IoT — em fase de proposta.**

A proposta prevê um ESP32 com HC-SR04 e sensor de vibração SW-420 para registrar utilizações do bebedouro e detectar eventos associados à troca do galão. A contagem serviria como estimativa de uso, com alerta por LED e envio dos dados via Wi-Fi/MQTT para um dashboard no Adafruit IO.

[Consultar a proposta](final-project/projeto-final-proposta.pdf)

## Tecnologias e práticas

- **Hardware:** ESP32, protoboard, resistores, potenciômetro, LDR, botão, LEDs e HC-SR04.
- **Programação:** C/C++ e Arduino para ESP32.
- **Conceitos:** GPIO, pull-up/pull-down, ADC, PWM, comunicação serial, divisores de tensão e temporização com `millis()`.
- **Ferramentas:** Arduino IDE, Monitor Serial, Serial Plotter, [Wokwi](websites/wokwi.md) e [Falstad](websites/falstad.md).

## Organização e consulta

```text
├── final-project/  # Proposta e materiais do projeto final
├── lab-guides/     # Enunciados, códigos e materiais de cada roteiro
├── reports/        # Relatos dos experimentos, códigos e evidências
├── slides/         # Apresentações da disciplina
└── websites/       # Links para simuladores e ferramentas
```

Os relatórios podem ser lidos diretamente no GitHub. Para consultar os códigos localmente:

```bash
git clone https://github.com/matheusvidal21/plataformas-hardware-iot.git
cd plataformas-hardware-iot
```

Consulte o relatório e as ligações de cada atividade antes de reproduzir a montagem. Os arquivos `.ino` são destinados ao ambiente Arduino com suporte ao ESP32. As versões das ferramentas ainda não estão fixadas no repositório.

As implementações e evidências registram o desenvolvimento das práticas ao longo da disciplina.
