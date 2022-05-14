/*
 * File:   lab11master.c
 * Author: WMGWW
 *
 * Created on 11 de mayo de 2022, 07:19 PM
 */

//MAESTRO
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

#define SPIflag 0xFF

uint8_t potvalue = 0;

void __interrupt() isr (void){
    if(PIR1bits.ADIF){              //ADC en RA2 donde guardamos el valor del potenciometro
        if(ADCON0bits.CHS == 2)     //en una variable
            potvalue = ADRESH;        
        PIR1bits.ADIF = 0;
    }
}

void main(void) {
    setup();
    while(1){
        
        if(ADCON0bits.GO == 0){    //Solo usamos un canal          
            ADCON0bits.GO = 1;              
        }
        
        PORTAbits.RA7 = 1;          //Desactivamos y activamos el esclavo que 
        PORTAbits.RA6 = 0;          //queremos usar
        __delay_ms(1);
        SSPBUF = potvalue;        //Para este enviamos el dato del POT
        while(!SSPSTATbits.BF){}    //esperamos a que se envie
        __delay_ms(10);
        
        PORTAbits.RA7 = 0;          //cambiamos de esclavo
        PORTAbits.RA6 = 1;
        __delay_ms(1);
        
        SSPBUF = SPIflag;          //enviamos dato un dato cualquiera para activar 
        while(!SSPSTATbits.BF){}    //los pulsos de reloj para que asi el esclavo
        PORTD = SSPBUF;             //nos envie un dato y lo guardamos en PORTD
        __delay_ms(10);
    }
    return;
}
