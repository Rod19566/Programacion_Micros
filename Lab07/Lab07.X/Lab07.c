/*
 * File:   Lab07.c
 * Author: Dina Rodríguez
 *
 * Created on 3 de abril de 2022, 11:03 PM
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
#include <stdio.h>
#include "setup.h"

#define _XTAL_FREQ 2000000      //configuracion 2MHz

/*------------------------------------------------------------------------------
 * CONSTANTES 
 ------------------------------------------------------------------------------*/
#define INCREMENTAR PORTBbits.RB0     // Asignamos un alias a RB0
#define DECREMENTAR PORTBbits.RB1     // Asignamos un alias a RB1

//----------VARIABLES----------------------------
// Ejemplos:
uint8_t ban = 0;        // declarada inicializada bandera de los 7 segments
uint8_t segment = 0;    // valor que tiene el segment
//------------------------------------------------/

// -----------PROTOTIPO DE FUNCIONES 
void displays(void);
//

/*-------INTERRUPCIONES -----------------*/
void __interrupt() isr (void){
    if(INTCONbits.RBIF){            // Fue interrupci n del PORTB�
        if(!INCREMENTAR){                 // Verificamos si fue RB0 quien gener  la �interrupci n�
            PORTA++;                // Incremento del PORTC (INCF PORTC) 
        }
        if(!DECREMENTAR){                 // Verificamos si fue RB0 quien gener  la �interrupci n�
            PORTA--;                // Incremento del PORTC (INCF PORTC) 
        }
        INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n�
    }
    
    if(INTCONbits.T0IF){    //se revisa bandera del timer
        resettmr0();
        PORTC++; 
        PORTD = 0;
        displays();
    }
    
    return;
}
void displays(void){
    RE0 = 0;
    RE1 = 0;
    RE2 = 0;
    switch(ban){
        case 0:
            RE0 = 1;
            segment = cen((int)PORTA);
            ban++;
            break;
        case 1:
            RE1 = 1;
            segment = dec((int)PORTA);
            ban++;
            break;
        case 2:
            RE2 = 1;
            segment = uni((int)PORTA);
            ban = 0;
            break;
        default:
            break;   
    }
    
    PORTD = display_table(segment);     //PORTD se le asigna segment 
    return;                             //por medio de la funcion display_table
}

void main(void) {
    setup();                        // Llamamos a la funcin de configuraciones
    setuptmr0();                    // funcion de configuracion  Timer0
    configint();                    //funcion de configuracion de interrupciones
    while(1){   //LOOP
        
    }
    return;
}

