/*
 * File:   Lab10.c
 * Author: WMGWW
 *
 * Created on 2 de mayo de 2022, 07:25 AM
 */
// CONFIG1

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits
#pragma config WDTE = OFF       // Watchdog Timer Enable bit 
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit 
#pragma config CP = OFF         // Code Protection bit 
#pragma config CPD = OFF        // Data Code Protection bit
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
#pragma config LVP = OFF        // Low Voltage Programming Enabled 
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit 
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "setup.h"

/*------------------------------------------------------------------------------
 * CONSTANTES 
 ------------------------------------------------------------------------------*/
#define _XTAL_FREQ 1000000
#define LEN_MSG 9               // Constante para definir largo de mensaje e 
#define LEN_MSG1 9               // Constante para definir largo de mensaje e 
/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
char msg[LEN_MSG] = {'D', 'a', 't', 'o', ':', ' ', ' ', 0x0D, 0x0A};
char msg1[LEN_MSG1] = {'H','o','l','i','w','i','s', 0x0D, 0x0A};
uint8_t index = 0;             // Variable para saber que posici n del mensaje 
uint8_t index1 = 1;             // Variable para saber que posici n del mensaje 
uint8_t oldvalue = 0;          // Variable para guardar el valor anterior recibido
/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void send(char[]);
/*------------------------------------------------------------------------------
 * INTERRUPCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() isr (void){
    if(PIR1bits.RCIF){          // Hay datos recibidos?
        
        msg[6] = RCREG;     // Guardamos valor recibido en el arreglo mensaje
        //PORTD = msg[6];     // Mostramos valor recibido en el PORTD
    }
    return;
}
/*------------------------------------------------------------------------------
 * CICLO PRINCIPAL
 ------------------------------------------------------------------------------*/
void main(void) {
    setup();
    while(1){
        index = 0;
        index1 = 0; 
        send("HOLIWIS  \r");
    }
    return;
}



void send(char x[]){
        //size_t size = sizeof(x) / sizeof(x[0]);         //calculate lenght of char array
        int size = strlen(x);
       // #define LEN_MSGx size            // Constante para definir largo de mensaje

       if (oldvalue != msg[6] ){                // Verificamos que el nuevo valor 
            while(x[index1]  !=  0){              // Loop para imprimir el mensaje 
                if (PIR1bits.TXIF){             // Esperamos a que este
                    TXREG = x[index1];        // Cargamos caracter a enviar  
                    index1++;                  // Incrementamos indice para enviar 
                            
           }// if PIR1bits
                oldvalue = msg[6];   //almacena el la posicion 6 de msg el valor ingresado
            }//while                               //
        }//if !=
}
