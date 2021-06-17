# Projeto de Sistemas Embarcados 2
#### Aluno: Maiky Barreto da Silva
#### Período: 2021/1

## Introdução

O projeto consiste da construção de um diapasão eletrônico utilizando a placa de desenvolvimento EFM32 Giant Gecko, disponibilizada pelo professor Camilo, e todo o hardware necessário para a implementação, como alto falante, amplificador, filtro, etc. 

O protótipo deve tocar uma nota musical, que é mostrada no display junto com sua frequência, e ao apertar o botão da placa deve alterar para a nota seguinte da escala musical, conforme a tablea abaixo:

| Nota musical  | Frequência (Hz)| Texto do display  |
| ------------- | -------------- | ----------------- |
| LA 4 (A)      | 440.000        | `A4  440`         |
| LA# 4 (A#)    | 466.164        | `A4# 440`         |
| SI 4 (B)      | 493.883        | `B4  440`         |
| DO 4 (C)      | 523.251        | `C4  440`         |
| DO# 4 (C#)    | 554.365        | `C4# 440`         |
| RE 4 (D)      | 587.330        | `D4  440`         |
| RE# 4 (D#)    | 622.254        | `D4# 440`         |
| MI 4 (E)      | 659.255        | `E4  440`         |
| FA 4 (F)      | 698.456        | `F4  440`         |
| FA# 4 (F#)    | 739.989        | `F4# 440`         |
| SOL 4 (G)     | 783.991        | `G4  440`         |
| SOL# 4 (G#)   | 783.991        | `G4# 440`         |
| LA 5 (A)      | 783.991        | `A5  440`         |

## Hardware

#### EFM32 Giant Gecko

Para gerar o som foi utilizado o periférico PWM, com saída na porta PD6. A frequência de operação do PWM foi 187.5 kHz, ou seja, o Timer utilizou o clock do sistema (48 MHz) e o valor de TOP foi de 255 (0xFF). 

#### Amplificador de Som

Foi utilizado uma placa amplificadora de som baseada no chip PAM8610 ([placa](https://www.tecnotronics.com.br/pam8610-placa-mini-amplificador.html), [datasheet](https://pdf1.alldatasheet.com/datasheet-pdf/view/246508/PAM/PAM8610/+Q_552-VMvLaRDPwCtt+/datasheet.pdf)) e um alto falante genérico de 8 ohms. Também foi utilizado um filtro passa-baixa de segunda ordem, com frequência de corte de aproximadamente 4.3 kHz, suficiente para não atenuar a maior frequência utilizada no projeto (880 Hz). Um potenciômetro foi utilizado para controlar o volume no alto falante. O circuito do filtro e o circuito do microcontrolador foi isolado utilizando um capacitor. A figura abaixo mostra as ligações do circuito.

![circuito](Documentação/circuito.png)

## Software

#### Bibliotecas Livres

Foram utilizadas as bibliotecas `button.c`, `clock_efm32gg2.c`, `gpio.c`, `lcd.c` e `pwm.c`, disponibilizadas na [página do GitHub do professor Hans Jorg](https://github.com/hans-jorg/efm32gg-stk3700-gcc-cmsis). 

#### Bibliotecas do Aluno

Foi criada a biblioteca `diapasao.c`, que possui as funções e variáveis necessárias para a implementação do programa. Para detalhes da funcionalidade de cada uma, execute `make docs` em um terminal.

#### Funcionamento Básico

A função `Diapasao_Init` ajusta o clock para 48 MHz, configura os periféricos GPIO, LCD e PWM, além de habilitar as interrupções para o botão e para o SysTick. Logo, o programa passa a funcionar via interrupção ou do SysTick ou do botão.

Em caso de interrupção do Systick, incrementa o contador `sine_pos`, responsável por percorrer a tabela `sine_table`, onde estão armazenados os duty cycles do PWM. Estes valores de duty cycle, após serem filtrados para uma baixa frequência, são transformados de pulsos para um sinal senoidal contínuo. 

Em caso de interrupção do botão, incrementa o contador `state_now`, que representa qual estado da Máquina de Estados principal está ativo no momento, e chama a função da Máquina de Estados. Cada estado da Máquina possuem dois parâmetros que são setados: o `systick_divider` e o texto do LCD. Após a alteração destes parâmetros são chamadas as funções que atualiza os caracteres no display e reconfigura o SysTick.

Para gerar uma senoide em uma frequência específica nesta implementação deve-se ajustar a velocidade com que a interrupção do SysTick ocorre. Em exemplos de implementações realizadas nas aulas, a função `SysTick_Config` era ajustada para gerar um tick a cada 1ms e utilizava-se outra técnica para ajustar o tempo desejado, desde que múltiplo de 1ms. Aqui a geração do tick é configurada para casar com 40 * frequência da senoide (pois a tabela `sine_table` possui 40 elementos), e cada vez que o tick é gerado, atualiza-se o duty cycle do PWM de acordo com a posição da tabela atual. 