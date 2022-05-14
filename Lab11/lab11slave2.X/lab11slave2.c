/*
 * File:   lab11slave2.c
 * Author: WMGWW
 *
 * Created on 14 de mayo de 2022, 05:17 AM
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
#include "setup2.h"

unsigned short map(uint8_t val, uint8_t inmin, uint8_t inmax, unsigned short outmin, unsigned short outmax);

uint8_t PWM;
unsigned short CCP1;

void __interrupt() isr (void){
    if (PIR1bits.SSPIF){
        __delay_ms(1);
        while (!SSPSTATbits.BF){}   //Recivimos potenciometro desde el maestro
        PWM = SSPBUF;               //lo guardamos en una variable
        __delay_ms(10);
        
        CCP1 = map(PWM, 0, 255, 62, 125); //mapeamos valor de potenciometro
        CCPR1L = (uint8_t)(CCP1>>2);    
        CCP1CONbits.DC1B = CCP1 & 0b11; 
        
        PIR1bits.SSPIF = 0;
    }
}

void main(void) {
    setup();
    while(1){
    }
    return;
}

// y = y0 + [(y1 - y0)/(x1-x0)]*(x-x0)

unsigned short map(uint8_t x0, uint8_t x1, uint8_t x2, unsigned short y0, unsigned short y1){
    return (unsigned short)(y0 +((float)(y1-y0)/(x2-x1))*(x0-x1));
}