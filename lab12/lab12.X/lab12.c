/*
 * File:   lab12.c
 * Author: WMGWW
 *
 * Created on 16 de mayo de 2022, 01:04 AM
 */
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

#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ 1000000

#include <xc.h>
#include <stdint.h>
#include "setup.h"
#define SLEEPbut PORTBbits.RB0     // Asignamos un alias a RB0
#define WAKEbut PORTBbits.RB1     // Asignamos un alias a RB1
#define SAVEbut PORTBbits.RB2     // Asignamos un alias a RB2

uint8_t potvalue = 0;

void __interrupt() isr (void){
    if(PIR1bits.ADIF){              //ADC en RA2 donde guardamos el valor del potenciometro
        if(ADCON0bits.CHS == 2)     //en una variable
            potvalue = ADRESH;        
            PIR1bits.ADIF = 0;
    } //ADIF
    
    if(INTCONbits.RBIF){            // Fue interrupci n del PORTB�
        if(!SLEEPbut){                 // Verificamos si fue RB0 quien gener  la �interrupci n�
            SLEEP();
            INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n�
        }
        if(!WAKEbut){                 // Verificamos si fue RB0 quien gener  la �interrupci n�
            
            INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n�
        }
        if(!SAVEbut){                 // Verificamos si fue RB0 quien gener  la �interrupci n�
            
            INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n�
        }
    }
}

void main(void) {
    setup();
    while(1){
        
        if(ADCON0bits.GO == 0){    //Solo usamos un canal          
            ADCON0bits.GO = 1;              
        }
        PORTC = potvalue;
    }
    return;
}