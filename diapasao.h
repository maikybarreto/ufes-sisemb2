/**
 * @file diapasao.h
 * @author maiky (maiky.silva@edu.ufes.br)
 * @brief Header das funções do diapasão
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef DIAPASAO_H
#define DIAPASAO_H

#include "gpio.h"
#include "pwm.h"
#include "clock_efm32gg2.h"
#include "lcd.h"
#include "button.h"

/**
 * @brief Enum da máquina de estados para facilitar a leitura do código
 * 
 */
typedef enum 
{
    STATE_OFF = -1, //OFF
    STATE_440 = 0,  // A4
    STATE_466 = 1,  // A4#
    STATE_493 = 2,  // B4
    STATE_523 = 3,  // C4
    STATE_554 = 4,  // C4#
    STATE_587 = 5,  // D4
    STATE_622 = 6,  // D4#
    STATE_659 = 7,  // E4
    STATE_698 = 8,  // F4
    STATE_739 = 9,  // F4#
    STATE_783 = 10, // G4
    STATE_830 = 11, // G4#
    STATE_880 = 12, // A5
} StateMachine_State;

#define BIT(N) (1U<<(N))    // Função da biblioteca para selecionar pinos do uC

#define OUT_PORT    GPIOD   // Porta de saída do PWM
#define OUT_PIN     BIT(6)  // Pino de saída do PWM
#define LED_PORT    GPIOE   // Porta de saída dos LEDs 
#define LED1_PIN    BIT(3)  // Pino de saída do LED de Pronto
#define LED2_PIN    BIT(2)  // Pino de saída do LED de Som

uint32_t sine_table[40];
uint8_t sine_pos;               
uint32_t volatile TickCounter;  
uint32_t systick_divider;           
StateMachine_State state_now; 

void buttoncallback(uint32_t v);
void stateMachine(StateMachine_State state);
void SysTick_Handler(void);
void Diapasao_Init(void);

#endif