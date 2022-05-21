/*
 * File:   ProyectoOjosMASTER.c
 * Author: WMGWW
 *
 * Created on 21 de mayo de 2022, 06:03 AM
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
#include "stup.h"

#define delayTMR0 250

//variables
unsigned char contLED = 0;
unsigned char temppwm = 0;

uint8_t potvalue = 0;
uint8_t index = 0;             // Variable para saber que posici n del mensaje 
uint8_t mode = 0;       //variable for mode
uint8_t oldvalue = 0;          // Variable para guardar el valor anterior recibido

//prototipos
void configint(void);
void resettmr0(void);
void setuptmr0(void);

//constantes
#define MODEbut PORTBbits.RB0     // Asignamos un alias a RB0
#define SAVEbut PORTBbits.RB1     // Asignamos un alias a RB1
#define _XTAL_FREQ 8000000      //configuracion 8MHz
//----------------------interrupciones------------------------------------------
void __interrupt() isr(void){   
    //ADC INTERRUPT
    if (ADIF == 1) {
        if(!ADCON0bits.CHS){
            CCPR1L = (ADRESH>>1)+124;
            CCP1CONbits.DC1B1 = ADRESH & 0b01; 
            CCP1CONbits.DC1B0 = (ADRESL>>7);
            ADCON0bits.CHS = 1; 
        }
        //canal pwm2
        else if(ADCON0bits.CHS == 1) {
            
            CCPR2L = (ADRESH>>1)+124;
            CCP2CONbits.DC2B1 = ADRESH & 0b01;
            CCP2CONbits.DC2B0 = (ADRESL>>7);
            ADCON0bits.CHS = 0b0010;//se cambia a canal del tercer pot
        }
        else if(ADCON0bits.CHS == 2) {
            temppwm = ADRESH;
            PIR1bits.ADIF = 0;
            ADCON0bits.CHS = 3;//se cambia a canal del primer pot
           // CCP2CONbits.DC2B0 = (ADRESL>>7);
            
        } 
        else if(ADCON0bits.CHS == 3) {
            temppwm = ADRESH;
            PIR1bits.ADIF = 0;
            ADCON0bits.CHS = 0;//se cambia a canal del primer pot
           // CCP2CONbits.DC2B0 = (ADRESL>>7);
            
        } 
        
        __delay_us(40);   
        PIR1bits.ADIF = 0;
        ADCON0bits.GO = 1;
        
    }
    if(INTCONbits.T0IF){    //se revisa bandera del timer0
       
        resettmr0();  
        contLED++;
        TMR0 = 250;
        if (contLED < temppwm) RC3 = 1;
        
        else RC3 = 0;
//        
//        if (contLED == temppwm) {
//            contLED = 0;
//        }
//      
        resettmr0();  
//        if (ledActualBrightness > contLED) RD5 = 1;
//        else  RD5 = 0;
//        contLED++;      
//        
//        if (contLED > 19) contLED = 0;
//        
//        if    b(ledTargetBrightness >= ledActualBrightness)
//			ledActualBrightness = ledTargetBrightness;
//		else
//		{
//			fadeCounter++;
//			if (fadeCounter == 24)
//			{
//				ledActualBrightness--;
//				fadeCounter = 0;
//			}	
//		}        
    }//tmr0
    
    if(INTCONbits.RBIF){            // Fue interrupci n del PORTB�
        if(!MODEbut){                 // Verificamos si fue RB0 quien gener  la �interrupci n�
            mode++;
        }
        if(!SAVEbut){                 // Verificamos si fue RB0 quien gener  la �interrupci n�
            //PORTD = read_EEPROM(1);
            mode--;
        }
        INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n�
    } //RBIF
    
    } //interrupciones
    

void main(void) {
    setup();
    //LOOP
    while (1){ 
    
    if (mode == 4) mode = 0; //mode reseter
    PORTD = mode; //SHOWS MODE
   }      
}