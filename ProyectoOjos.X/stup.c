/*
 * File:   stup.c
 * Author: WMGWW
 *
 * Created on 21 de mayo de 2022, 06:10 AM
 */


#include <xc.h>

#define _XTAL_FREQ 8000000      //configuracion 8MHz

void resettmr0(void){
   // TMR0 = 5;              //para 15.1 ms
    INTCONbits.T0IF=0;     // ; Limpiamos bandera de TMR0
}

void setuptmr0(void){
    OPTION_REGbits.T0CS=0; // SELECT INTERNAL SOURCE TMR0
    OPTION_REGbits.PSA=0;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS2=1;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS1=1;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS0=1;  // PRESCALER ASSIGNED TO TIMER0 256
    resettmr0();
    
}

void configint(void){
    
    INTCONbits.T0IE = 1;     // Habilitamos interrupcion TMR0
    INTCONbits.T0IF = 0;     // ; Limpiamos bandera de TMR0
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;      //se habilitan las interrupciones por adc
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;      // ENABLE peripherial INTERRUPT
    INTCONbits.RBIE = 1;        //interrupciones en PORTB y TMR0
    INTCONbits.RBIF = 0;        //Apagamos banderas
    
       
    //Configuracion push button
    TRISBbits.TRISB0 = 1;       //RB0 y RB1
    TRISBbits.TRISB1 = 1;       //RB0 y RB1
    OPTION_REGbits.nRBPU = 0;   // Habilitamos resistencias de pull-up del PORTB
    WPUBbits.WPUB = 0b00000011;       //0011 RB0 and RB1 
    IOCBbits.IOCB = 0b00000011;       //RB0 y RB1 pull ups e interrupciones

}
void setup(void){
    
//    //configuraciones
//    //configuracion reloj
//    CCP1CONbits.CCP1M = 0b1100;
    //Single output; P1A modulated; P1B, P1C, P1D assigned as port pins
        
    OSCCONbits.IRCF = 0b0111;//0111, Frecuencia de reloj 8 MHz
    OSCCONbits.SCS   = 1;//reloj interno
    configint();
    //configuracion in out
    ANSELH = 0; //Pines digitales
    ANSELbits.ANS0  = 1;//AN0, AN1 y AN2 como pines analogicos
    ANSELbits.ANS1  = 1;
    ANSELbits.ANS2  = 1;
    ANSELbits.ANS3  = 1;
    TRISA  = 0b00001111; //AN0, AN1 y AN2 como inputs y los demas como outputs
    TRISC  = 0;
    PORTA  = 0;//se limpian los puertos
    PORTC  = 0;
    TRISD = 0; 
    PORTD = 0;
    PORTB  = 0;
    //configuracion adc
    ADCON0bits.ADCS = 2;    //10 se selecciona Fosc/32 para conversion 4us full TAD
    ADCON0bits.CHS0 = 0;    //se selecciona el canal AN0
    ADCON1bits.VCFG1 = 0;   //se ponen los voltajes de referencia internos del PIC
    ADCON1bits.VCFG0 = 0;   //0V -> 5V
    ADCON1bits.ADFM = 0;    //pushed to the left, + significativos
    ADCON0bits.ADON = 1;    //ADC ON
    __delay_us(50);         //delay de 50 us
    //configuracion pwm
    //CCP1
    TRISCbits.TRISC2 = 1;      //CCP1 como entrada;
    PR2 = 250;                 //valor para que el periodo pwm sea 2 ms 
    CCP1CONbits.P1M = 0;       //config pwm
    CCP1CONbits.CCP1M = 0b1100;
    CCPR1L = 0x0f;             //ciclo de trabajo inicial
    CCP1CONbits.DC1B = 0;
    
    //CCP2
    TRISCbits.TRISC1 = 1;       //CCP2 como entrada;
    CCP2CONbits.CCP2M = 0b1100; //config pwm
    CCPR2L = 0x0f;              //ciclo de trabajo inicial
    CCP2CONbits.DC2B1 = 0;
    
    //CONDIG TMR2
    PIR1bits.TMR2IF = 0;        //TMR2 interrrupt flag
    T2CONbits.T2CKPS = 0b11;    //prescaler 1:16
    T2CONbits.TMR2ON = 1;       //se enciende el tmr2
    while(PIR1bits.TMR2IF == 0);//esperar un ciclo de tmr2
    PIR1bits.TMR2IF = 0;
    
    TRISCbits.TRISC3 = 0;       //out manual pwm3
    TRISCbits.TRISC2 = 0;       //out pwm2
    TRISCbits.TRISC1 = 0;       //out pwm1
    
    
    ADCON0bits.GO = 1;      //ADC conversion
    
}