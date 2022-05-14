/*
 * File:   lab11.c
 * Author: WMGWW
 *
 * Created on 9 de mayo de 2022, 11:56 AM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "setup.h"
/*------------------------------------------------------------------------------
 * CONSTANTES 
 ------------------------------------------------------------------------------*/
#define _XTAL_FREQ 1000000
#define LEN_MSG 9               // Constante para definir largo de mensaje e 
#define UP PORTBbits.RB0     // Asignamos un alias a RB0
#define DOWN PORTBbits.RB1     // Asignamos un alias a RB1
/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
uint8_t cont = 0;           // Contador que env a el maestro al esclavo?
uint8_t potvalue = 0;
uint8_t contvalue = 0;
//char msg[LEN_MSG] = {'D', 'a', 't', 'o', ':', ' ', ' ', 0x0D, 0x0A};
/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() isr (void){
    if(PIR1bits.SSPIF){             //  Recibi  datos el esclavo???
        PORTD = SSPBUF;             // Mostramos valor recibido en el PORTD
        PIR1bits.SSPIF = 0;         // Limpiamos bandera de interrupci n?
    }
    if (ADIF == 1) {
        if(!ADCON0bits.CHS){
            potvalue = ADRESH;
        }
        __delay_us(40);   
        PIR1bits.ADIF = 0;
        ADCON0bits.GO = 1;
    }
        if(INTCONbits.RBIF){            // Fue interrupci n del PORTB?
        if(!UP){                 // Verificamos si fue RB0 quien gener  la ?interrupci n?
            contvalue++;                // Incremento del PORTC (INCF PORTC) 
        }
        if(!DOWN){                 // Verificamos si fue RB0 quien gener  la ?interrupci n?
            contvalue--;                // Incremento del PORTC (INCF PORTC) 
        }
        INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n?
    }
    return;  
}

void main(void) {
    setup();
    while(1){
        if(PORTEbits.RE0){          //  Es maestro?      sip
            __delay_ms(1000);       // Esperamos un segundo para mandar datos
            if(SSPSTATbits.BF){     // Revisamos que no haya comunicaci n en ?
//proces0
                SSPBUF = potvalue;      // Movemos el valor del contador para enviarlo
                PORTD = potvalue;       // Mostramos valor del contador en PORTD
                //cont++;             // Incrementamos contador
            }
        }
        else if(!PORTEbits.RE0){          //  Es maestro?      nope
            __delay_ms(1000);       // Esperamos un segundo para mandar datos
            if(SSPSTATbits.BF){     // Revisamos que no haya comunicaci n en ?
//proceso
                SSPBUF = contvalue;      // Movemos el valor del contador para enviarlo
                PORTD = contvalue;       // Mostramos valor del contador en PORTD
                //cont++;             // Incrementamos contador
            }
        }
    }
    return;
}