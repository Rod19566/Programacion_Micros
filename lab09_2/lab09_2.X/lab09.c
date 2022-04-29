/*
 * File:   lab09.c
 * Author: WMGWW
 *
 * Created on 24 de abril de 2022, 06:31 PM
 */

#include <xc.h>
#pragma config FOSC=INTRC_NOCLKOUT //Oscilador interno sin salida
#pragma config WDTE=OFF           //Reinicio repetitivo del pic
#pragma config PWRTE=OFF           //no espera de 72 ms al iniciar el pic
#pragma config MCLRE=OFF          //El pin MCLR se utiliza como entrada/salida
#pragma config CP=OFF             //Sin protección de código
#pragma config CPD=OFF            //Sin protección de datos
    
#pragma config BOREN=OFF //Sin reinicio cuando el input voltage es inferior a 4V
#pragma config IESO=OFF  //Reinicio sin cambio de reloj de interno a externo
#pragma config FCMEN=OFF //Cambio de reloj externo a interno en caso de fallas
#pragma config LVP=OFF    //Programación en low voltage apagada
    
//CONFIGURATION WORD 2
#pragma config WRT=OFF //Proteccion de autoescritura por el programa desactivada
#pragma config BOR4V=BOR40V //Reinicio abajo de 4V 
#define _XTAL_FREQ 8000000 //frecuencia de 8 MHz

//variables
int contPOT = 0;
int tempPOT = 0;

void configint(void);
void resettmr0(void);
void setuptmr0(void);

//----------------------interrupciones------------------------------------------
void __interrupt() isr(void){    // only process timer-triggered interrupts
    //ADC INTERRUPT
    if (ADIF == 1) {
        //multiplexacion de canales para el adc
        //canal pwm1
        if(!ADCON0bits.CHS){
            CCPR1L = (ADRESH>>1)+124;//para que el servo1 pueda girar 180 grados
            CCP1CONbits.DC1B1 = ADRESH & 0b01; //añadir precision/resolucion
            CCP1CONbits.DC1B0 = (ADRESL>>7);
            ADCON0bits.CHS = 1; //se cambia al canal del segundo pot
        }
        //canal pwm2
        else if(ADCON0bits.CHS == 1) {
            
            CCPR2L = (ADRESH>>1)+124;//para que el servo0 pueda girar 180 grados
            CCP2CONbits.DC2B1 = ADRESH & 0b01;//añadir precision/resolucion
            CCP2CONbits.DC2B0 = (ADRESL>>7);
            ADCON0bits.CHS = 0b0010;//se cambia a canal del tercer pot
        }
        else if(ADCON0bits.CHS == 2) {
            tempPOT = ADRESH>>1 & 0b01;//añadir precision/resolucion
           // CCP2CONbits.DC2B0 = (ADRESL>>7);
            ADCON0bits.CHS = 0;//se cambia a canal del primer pot
        } 
        
        __delay_us(50);   //delay de 50 us
        PIR1bits.ADIF = 0;//interrupcion de adc
        ADCON0bits.GO = 1;//inicio de la siguiente conversión
        
    }
    
    if(INTCONbits.T0IF){    //se revisa bandera del timer0
        contPOT++; //contador +1
        
        if (contPOT == tempPOT)  {
            contPOT = 0;
            RD5 = 0;
        } 
        else if (contPOT == 0)  {
            RD5 = 1;
        } 
        resettmr0();
    } //tmr0
    
}
void resettmr0(void){
    TMR0 = 1;              //para 15.1 ms
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
    
}
void main(void) {
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
    TRISA  = 0b00000111; //AN0, AN1 y AN2 como inputs y los demas como outputs
    TRISC  = 0;
    PORTA  = 0;//se limpian los puertos
    PORTC  = 0;
    TRISD = 0b00100000;     //RD5 = PWM output
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
    
    TRISDbits.TRISD5 = 0;       //out pwm3
    TRISCbits.TRISC2 = 0;       //out pwm2
    TRISCbits.TRISC1 = 0;       //out pwm1
    
    
    ADCON0bits.GO = 1;      //ADC conversion
    
    //LOOP
    while (1){
    }      
}