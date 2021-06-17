/**
 * @file diapasao.c
 * @author maiky (maiky.silva@edu.ufes.br)
 * @brief Implementação das funções do diapasão
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdint.h>
#include "diapasao.h"

uint32_t sine_table[40] =  {128,147,167,185,
                            202,218,231,241,
                            249,253,255,253,
                            249,241,231,218,
                            202,185,167,147,
                            128,108,88,70,
                            53,37,24,14,
                            6,2,0,2,
                            6,14,24,37,
                            53,70,88,108,}; // 255*sin(x), onde 
                                            // x = linspace(0,360-9,360/9)

uint8_t sine_pos = 0;                       // Posição na tabela sine_table
uint32_t volatile TickCounter = 0;          // Contador do Systick
uint32_t systick_divider;                   // Divisor do Systick
StateMachine_State state_now = STATE_OFF;   // Valor inicial da máquina de estados

/**
 * @brief Callback do Systick
 * 
 */
void SysTick_Handler(void) 
{
    // Atualiza o contador que percorre a tablea sine_table
    sine_pos++;

    // verifica se extrapolou
    if(sine_pos == 40) sine_pos=0;

    // Atualiza o duty cycle do PWM com o valor da tabela
    PWM_Write(TIMER1, 0, sine_table[sine_pos]);
}

/**
 * @brief Máquina de estados que controla a saída dos periféricos
 * 
 * @param state Estado atual
 */
void stateMachine(StateMachine_State state)
{
    static char *string2;

    // Habilita o LED de Som, indicando que deve haver som na saída
    GPIO_WritePins(LED_PORT, 0, LED2_PIN);

    // Para cada estado são setados o valor do systick_divider, que altera
    // a velocidade com que percorre a tabela sine_table, consequentemente
    // alterando a frequência da senoide após o filtro passa-baixa, e o
    // texto apresentado no display, que mostra a nota musical e sua frequência
    switch(state)
    {
        case STATE_OFF: // CASO ESPECIAL
        // Desliga o PWM
        PWM_Stop(TIMER1);

        // Escreve OFF no display
        string2 = "OFF";

        // Apaga o LED de som
        GPIO_WritePins(LED_PORT, LED2_PIN, 0);
        break;

        case STATE_440:
        systick_divider = 17550;
        string2 = "A4  440";
        break;

        case STATE_466:
        systick_divider = 18600;
        string2 = "A4# 466";
        break;

        case STATE_493:
        systick_divider = 19700;
        string2 = "B4  493";
        break;

        case STATE_523:
        systick_divider = 20880;
        string2 = "C4  523";
        break;

        case STATE_554:
        systick_divider = 22140;
        string2 = "C4# 554";
        break;

        case STATE_587:
        systick_divider = 23460;
        string2 = "D4  587";
        break;

        case STATE_622:
        systick_divider = 24810;
        string2 = "D4# 622";
        break;

        case STATE_659:
        systick_divider = 26270;
        string2 = "E4  659";
        break;

        case STATE_698:
        systick_divider = 27890;
        string2 = "F4  698";
        break;

        case STATE_739:
        systick_divider = 29500;
        string2 = "F4# 739";
        break;

        case STATE_783:
        systick_divider = 31190;
        string2 = "G4  783";
        break;

        case STATE_830:
        systick_divider = 33132;
        string2 = "G4# 830";
        break;

        case STATE_880:
        systick_divider = 34970;
        string2 = "A5  880";
        break;
    }

    // Reconfigura o systick para funcionar com o novo valor de systick_divider
    SysTick_Config(SystemCoreClock/systick_divider);
    
    // Habilita o PWM se o estado não for STATE_OFF
    if(state != STATE_OFF) PWM_Start(TIMER1);

    // Atualiza o texto no display
    char *s2 = string2;
    LCD_WriteAlphanumericDisplay(s2);

}

/**
 * @brief Função que é executada quando há uma interrupção no botão
 * 
 * @param v Parâmetro do protótipo da função de callback de interrupção
 */
void buttoncallback(uint32_t v)
{
    if(Button_ReadReleased())
    {
        // incrementa o estado
        state_now++;

        // volta ao primeiro estado se o contador de estados extrapolar
        if(state_now > STATE_880) state_now = STATE_OFF;

        // chama a máquina de estados
        stateMachine(state_now);
    }
        
}

/**
 * @brief Funcao que inicializa os periféricos utilizados na implementação
 *        do diapasão.
 * 
 */
void Diapasao_Init(void)
{
    // Mensagem inicial do Display
    static char *string = "OFF";
    char *s = string;

    // Configura Clock para 48MHz via Cristal
    SystemCoreClockSet(CLOCK_HFXO,1,1);
    
    // Confiugra os LEDs
    GPIO_Init(LED_PORT,0, LED1_PIN|LED2_PIN);

    // Configura saída do PWM
    GPIO_ConfigPins(OUT_PORT, OUT_PIN, GPIO_MODE_PUSHPULL);

    // Inicia o PWM
    PWM_Init(TIMER1, PWM_LOC4, PWM_LOC_UNUSED, PWM_LOC_UNUSED);

    // Configura o display e reseta
    LCD_Init();
    LCD_SetAll();
    LCD_ClearAll();

    // Escreve a mensagem inicial no display
    LCD_WriteAlphanumericDisplay(s);

    // Configura a interrupção do botão
    Button_Init(BUTTON1);
    Button_SetCallback(buttoncallback);

    // Habilita as interrupções
    __enable_irq();

    // Habilita o LED de Pronto, indicando que o sistema está pronto
    GPIO_WritePins(LED_PORT, 0, LED1_PIN);
}
