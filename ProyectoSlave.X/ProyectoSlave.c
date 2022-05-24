/*
 * File:   ProtectoSlave.c
 * Author: WMGWW
 *
 * Created on 16 de mayo de 2022, 11:18 AM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include <stdint.h>
#include "setup.h"
/*------------------------------------------------------------------------------
 * CONSTANTES 
 ------------------------------------------------------------------------------*/
#define _XTAL_FREQ 8000000
/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
uint8_t cont = 0xFF, potselect = 1;                // Valor inicial del contador
/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * INTERRUPCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() isr (void){
    
    if (PIR1bits.SSPIF){
        SSPCONbits.CKP = 0;         // Mantenemos el reloj en 0 para que se configure el esclavo
        
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            uint8_t var = SSPBUF;   // Limpiamos el buffer
            SSPCONbits.SSPOV = 0;   // Limpiamos bandera de overflow
            SSPCONbits.WCOL = 0;    // Limpiamos indicador de colisi n�
            SSPCONbits.CKP = 1;     // Habilitamos reloj para recibir datos
        }
        
        // Verificamos lo recibido fue un dato y no una direcci n�
        // Verificamos si el esclavo tiene que recibir datos del maestro
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW){
            SSPSTATbits.BF = 0;     // Limpiamos bandera para saber cuando se reciben los datos
            while(!SSPSTATbits.BF); // Esperamos a recibir los datos
            if (SSPBUF == 250) potselect = 1;
            if (SSPBUF == 251) potselect = 2;
            
            if (SSPBUF != 250 && SSPBUF != 251 ){
                if (potselect == 1) CCPR1L = SSPBUF; 
                if (potselect == 2) CCPR2L = SSPBUF;  
            }  
                    // Mostramos valor recibido del mestro en PORTD
            SSPCONbits.CKP = 1;     // Habilitamos el reloj
        }
        
        // Verificamos lo recibido fue un dato y no una direcci n�
//        // Verificamos si el esclavo tiene que enviar datos al maestro
//        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
//            SSPBUF = cont ;         // Preparamos dato a enviar
//            SSPCONbits.CKP = 1;     // Habilitamos reloj para el env o�
//            while(SSPSTATbits.BF);  // Esperamos a que se env e el dato�
//            PORTA = cont;           // Mostramos dato enviado en PORTA
//            cont--;                 // Actualizamos valor del contador
//        }
        PIR1bits.SSPIF = 0;         // Limpiamos bandera de interrupci n�
    }
    return;
}
/*------------------------------------------------------------------------------
 * CICLO PRINCIPAL
 ------------------------------------------------------------------------------*/
void main(void) {
    setup();
    while(1){}
    return;
}