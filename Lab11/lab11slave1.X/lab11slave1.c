/*
 * File:   lab11slave1.c
 * Author: WMGWW
 *
 * Created on 12 de mayo de 2022, 05:06 AM
 */


#include <xc.h>

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
#include "setup1.h"
uint8_t slave1 = 0;

void __interrupt() isr (void){
    if(INTCONbits.RBIF){        //Chequear si se prendio la bandera
        if (!PORTBbits.RB0)     //Si RB0 = 0, incrementar varible           
            ++slave1;            
        if(!PORTBbits.RB1)      //Si RB1 = 0, decrementar variable
            --slave1;
        INTCONbits.RBIF = 0;    // Limpiamos bandera 
    }
    else if (PIR1bits.SSPIF){
        __delay_ms(1);
        SSPBUF = slave1;        //enviamos el valor de la variable al maestro
        while (!SSPSTATbits.BF){}
        __delay_ms(10);
        
        PIR1bits.SSPIF = 0;
    }
}

void main(void) {
    setup();
    while(1){
        
    }
    return;
}
