/**
 * @file main.c
 * @author maiky
 * @brief 
 * @version 0.1
 * @date 2021-04-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "em_device.h"
#include "diapasao.h"

/**
 * @brief Função principal do programa
 * @note O programa inicia com a mensagem "HELLO", com a saída de som 
 * desabilitada. É necessário apertar o botão para habilitar o som.
 * 
 * @return int Retorno padrão da função main()
 */
int main(void) {
    // Inicializa o diapasão
    Diapasao_Init();

    while (1) {
        // Coloca em estado de baixo consumo
        __WFI();

    } 
}
