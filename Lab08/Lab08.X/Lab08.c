/*
 * File:   Lab08.c
 * Author: Dina Rodríguez
 *
 * Created on 12 de abril de 2022, 11:03 PM
 */


#include <xc.h>
#include <stdio.h>
#include "setup.h"

#define _XTAL_FREQ 4000000      //configuracion 4MHz

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
 
//prototipos
//

void main(void) {
    setup();
    setuptmr0();
    configint();
     while(1){
        if(ADCON0bits.GO == 0){   
            if(ADCON0bits.CHS == 0b0000){
                ADCON0bits.CHS = 0b0001;    // Cambio de canal
            }
            
            else if(ADCON0bits.CHS == 0b0001){
                ADCON0bits.CHS = 0b0000;    // Cambio de canal  
            }
            __delay_us(40);                 // Tiempo de adquisici n
            ADCON0bits.GO = 1;              // Iniciamos proceso de conversi n�
        } 
    }
    return;
}

void __interrupt() isr(void){
    if (PIR1bits.ADIF){
        if (ADCON0bits.CHS == 0){       //INTERRUPCION AN0
            PORTC   = ADRESH;
        } else if (ADCON0bits.CHS == 1){
            PORTD   = ADRESH;
            
        }
        PIR1bits.ADIF = 0;
    }
   return;         //INTERRUPT RETURN  
}

