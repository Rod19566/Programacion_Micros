/*
 * File:   setup.c
 * Author: Dina
 *
 * Created on 4 de abril de 2022, 12:53 AM
 */


#include <xc.h>

/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/
void setup(void){
    
    ANSEL = 0;
    ANSELH = 0b00000000;        // Usaremos solo I/O digitales
    
    TRISA = 0x00;               // PORTA como salida
    PORTA = 0;                  // Limpiamos PORTA
    TRISC = 0x00;               // PORTC como salida
    PORTC = 0;                  // Limpiamos PORTC
    
    //TRISB = 1;                // RB0 como entrada (configurada en decimal)
    //TRISB = 0b00000001;       // RB0 como entrada (configurada con binario)
    TRISBbits.TRISB0 = 1;       // RB0 como entrada (configurada con control de bits)
    TRISBbits.TRISB1 = 1;       // RB1 como entrada (configurada con control de bits)
    
    OPTION_REGbits.nRBPU = 0;   // Habilitamos resistencias de pull-up del PORTB
    WPUBbits.WPUB0 = 1;         // Habilitamos resistencia de pull-up de RB0
    WPUBbits.WPUB1 = 1;         // Habilitamos resistencia de pull-up de RB1
    
    INTCONbits.RBIE = 1;        // Habilitamos interrupciones del PORTB
    IOCBbits.IOCB0 = 1;         // Habilitamos interrupci n por cambio de estado para RB0
    IOCBbits.IOCB1 = 1;         // Habilitamos interrupci n por cambio de estado para RB1
    INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupci n�
    
    //configuracion del reloj
    OSCCONbits.IRCF = 0b0110; // 4MHz
    OSCCONbits.SCS = 1;       // Oscilador interno

}
 
 // Td = Pre * TMR1*Ti
 // N = 65536-(Td/Pre*Ti)
 
 //Ttmr1if = Prescaler * PR2 * Postscaler * (1/(Fosc/4))
 //PR2 = Ttmr2if/Prescaler * Postscaler * (1/(Fosc/4))


void resettmr0(void){
    TMR0 = 60;              //para 100 ms
    INTCONbits.T0IF=0;     // ; Limpiamos bandera de TMR0
}
void resettmr1(void){
    TMR1H = 11;              //para 500 ms
    TMR1L = 184;
    PIR1bits.TMR1IF=0;     // ; Limpiamos bandera de TMR0
}

void configint(void){
    
    INTCONbits.T0IE = 1;     // Habilitamos interrupcion TMR0
    INTCONbits.T0IF = 0;     // ; Limpiamos bandera de TMR0
    
    PIE1bits.TMR1IE = 1;	    //tmr1 interrupt
    PIR1bits.TMR1IF = 0;	    //tmr1 interrupt
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;      // ENABLE peripherial INTERRUPT
    
}


void setuptmr0(void){
    //OSCCON|=0x70;      // SELECT 8MHz Internal Oscillator
    OPTION_REGbits.T0CS=0; // SELECT INTERNAL SOURCE TMR0
    OPTION_REGbits.PSA=0;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS2=1;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS1=1;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS0=1;  // PRESCALER ASSIGNED TO TIMER0 256
    //OPTION_REG&=0xF8;  // SELECT 1:2 PRESCALER
    
    resettmr0();
    
}

void setuptmr1(void){
    T1CONbits.T1OSCEN = 1; // SELECT INTERNAL SOURCE TMR1
    T1CONbits.TMR1CS = 0; // SELECT INTERNAL SOURCE TMR1
    T1CONbits.TMR1ON = 1; // SELECT INTERNAL SOURCE TMR1
    
    T1CONbits.T1CKPS = 0b11;      //PRESCALER 1:8
    resettmr1();
    
}



