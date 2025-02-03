# Projeto de Controle de Matriz de LEDs com Raspberry Pi Pico com interrupções

Este projeto foi desenvolvido como parte de uma atividade prática para consolidar o entendimento de interrupções, debouncing, controle de LEDs e uso de resistores de pull-up internos no microcontrolador RP2040, utilizando a placa de desenvolvimento BitDogLab.

## Descrição do Projeto

O projeto consiste em um sistema que controla uma matriz de LEDs WS2812 (5x5) e um LED RGB, utilizando botões para interação. As funcionalidades incluem:

1. **Piscar LED Vermelho**: O LED vermelho pisca continuamente a uma taxa de 5 vezes por segundo.
2. **Incrementar Número**: O botão A incrementa o número exibido na matriz de LEDs.
3. **Decrementar Número**: O botão B decrementa o número exibido na matriz de LEDs.
4. **Exibição de Números**: A matriz de LEDs exibe números de 0 a 9 em um formato fixo (estilo digital).

## Componentes Utilizados

- **Raspberry Pi Pico**
- **Matriz de LEDs WS2812 (5x5)**
- **LED RGB**
- **Botões A e B**
- **Resistores de pull-up internos**

## Funcionalidades Implementadas

- **Interrupções**: Uso de interrupções para detectar pressionamentos dos botões.
- **Debouncing**: Implementação de debouncing via software para evitar leituras falsas dos botões.
- **Controle de LEDs**: Controle de LEDs comuns (RGB) e LEDs endereçáveis (WS2812).
- **Organização do Código**: Código estruturado e comentado para facilitar o entendimento.

## Estrutura do Código

O código está organizado da seguinte forma:

- **Bibliotecas**: Inclusão das bibliotecas necessárias.
- **Definições de Pinos**: Definição dos pinos utilizados para LEDs e botões.
- **Variáveis Globais**: Armazenamento do estado dos botões e do número atual.
- **Funções**:
  - `piscar_led_vermelho`: Faz o LED vermelho piscar.
  - `gpio_irq_handler`: Manipula interrupções dos botões.
  - `debounce_button`: Implementa debounce para os botões.
  - `display_number`: Exibe números na matriz de LEDs.
- **Função `main`**: Configura o hardware e executa o loop principal.

## Como Usar

### Configuração

Clone o Repositório

Configure o Ambiente de Desenvolvimento no VSCode:

Utilize a bibiloteca da Raspberry Pi Pico para criar o arquivo .build .

Compile o Projeto:
Carregue o Firmware no Raspberry Pi Pico:

Execução
Após carregar o firmware, o projeto estará em execução na placa BitDogLab.

Pressione os botões A e B para incrementar e decrementar o número exibido na matriz de LEDs.

 **Vídeo Demonstrativo**:
Vídeo de Demonstração
https://youtube.com/shorts/cE7nyK1iTgU


