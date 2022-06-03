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

//constantes
#define I2C_SPEED 100000
#define ADDRESS 0x08
#define READ 0b0
#define MODEbut PORTBbits.RB0     // Asignamos un alias a RB0
#define SAVE1but PORTBbits.RB1     // Asignamos un alias a RB1
#define SAVE2but PORTBbits.RB2     // Asignamos un alias a RB2
#define SAVE3but PORTBbits.RB3     // Asignamos un alias a RB3
#define _XTAL_FREQ 8000000      //configuracion 4MHz
#define LEN_MSG 15               // Constante para definir largo de mensaje e iteraciones al enviarlo por el serial
/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
char mensaje[LEN_MSG] = {'P', 'o', 't', '1', ':', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' '};
//char mensaje[LEN_MSG] = {'D', 'a', 't', 'o', ':', ' ', ' ', , 0x0A};
uint8_t indice = 0;             // Variable para saber que posici n del mensaje �enviar al serial
uint8_t valor_old = 0, potsel = 0;          // Variable para guardar el valor anterior recibido
uint8_t potvalue1 = 0, potvalue2 = 0, potvalue3 = 0, potvalue4 = 0, pots = 0, change = 1 ;
uint8_t index = 0;             // Variable para saber que posici n del mensaje 
uint8_t mode = 1, data = 0, response = 0, amount = 0;       //variable for mode
uint8_t oldvalue = 0;          // Variable para guardar el valor anterior recibido

void senddata(uint8_t, uint8_t);

//----------------------interrupciones------------------------------------------
void __interrupt() isr(void){ 
     
    if(RCIF){          // Hay datos recibidos?
        //__delay_ms(50);4        
       if (mode == 3){ 
            if ( RCREG >= 121 ){
            potsel = RCREG-120;
            mensaje[3] = potsel;       // ELECCION de potenciometro
        } else {
            pots = RCREG; 
            mensaje[6] = pots;
//            switch ((int) potsel){
//                case 1:
//                    //potvalue1 = pots;
//                    break;
//                case 2:
//                    //potvalue2 = pots;
//                    break;
//                case 3:
//                    //CCPR1L = pots;
//                    break;
//                case 4:
//                   // CCPR1L = pots;
//                    break;
//                default:
//                    break;
            }//switch  
//            
//            __delay_ms(100);
            PIR1bits.ADIF = 0; 
//            ADCON0bits.GO = 1;
            }//if mode ==3 
       
          //  __delay_us(40);
            
    } // if UART
    
    
    //ADC INTERRUPT
    if (ADIF == 1 ) {
        if (mode == 1){
        //change = 0;        

        switch (ADCON0bits.CHS){
            case 0:
                potvalue1 = (ADRESH>>1)+124;
                //change = 1;
                ADCON0bits.CHS = 1;
                __delay_us(40);   
                PIR1bits.ADIF = 0;
                ADCON0bits.GO = 1;
                
                break;
            case 1:
                potvalue2 = (ADRESH>>1)+124;
                //change = 1;
                ADCON0bits.CHS = 2;
                __delay_us(40);   
                PIR1bits.ADIF = 0;
                ADCON0bits.GO = 1;
                break;
            case 2:
                potvalue3 = (ADRESH>>1)+124;
                //change = 1; 
                ADCON0bits.CHS = 3;
                __delay_us(40);   
                PIR1bits.ADIF = 0;
                ADCON0bits.GO = 1; 
                break;
            case 3:
                potvalue4 = (ADRESH>>1)+124;
                //change = 1;
                ADCON0bits.CHS = 0;
                __delay_us(40);   
                PIR1bits.ADIF = 0;
                ADCON0bits.GO = 1;
                break;
        } //switch
        
        CCPR1L = potvalue1;
        CCPR2L = potvalue2;
        }//mode 1
        
    } //ADIF
    
  
    if(INTCONbits.RBIF){            // Fue interrupci n del PORTB�
        if(!MODEbut){                 // Verificamos si fue RB0 quien gener  la �interrupci n�
            if (mode != 3) mode++;
            else {
                mode = 1;
                ADCON0bits.GO = 1;
            }
        PORTD = mode; //SHOWS MODE
        }
        if (mode == 1){ //saves info
            if(!SAVE1but){  //pos 1
                write_EEPROM(1, potvalue1);
                write_EEPROM(2, potvalue2);
                write_EEPROM(3, potvalue3);
                write_EEPROM(4, potvalue4);
            }
            if(!SAVE2but){  //pos 1
                write_EEPROM(5, potvalue1);
                write_EEPROM(6, potvalue2);
                write_EEPROM(7, potvalue3);
                write_EEPROM(8, potvalue4);             
            }
        }// mode 1
        if (mode == 2){ //reproduces info
            if(!SAVE1but){  //pos 1
                potvalue1 = read_EEPROM(1);
                potvalue2 = read_EEPROM(2);  
                
               // senddata(250, read_EEPROM(3));
               //__delay_us(60); 
               // senddata(251, read_EEPROM(4));
               //__delay_us(60); 
            }
            if(!SAVE2but){  //pos 1
                potvalue1 = read_EEPROM(5);
                potvalue2 = read_EEPROM(6); 
                
               // senddata(250, read_EEPROM(7));
               //__delay_us(60); 
               // senddata(251, read_EEPROM(8));
               //__delay_us(60);                
            }
            
            CCPR1L = potvalue1;
            CCPR2L = potvalue2;
        }// mode 2
        
        INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n�
    } //RBIF
//   
    return;
    } //interrupciones
    

void main(void) {
    setup();
   // setupI2C();
   // mode = 1;
    
    //LOOP
    while (1){
        
//        senddata(250, potvalue3); 
//        senddata(251, potvalue4);
        
        PORTD = mode; //SHOWS MODE 
        if (mode == 3){ 
        indice = 0;                             // Reiniciamos indice para enviar todo el mensaje
            if (valor_old != mensaje[6]){           // Verificamos que el nuevo valor recibido en el serial
                while(indice<LEN_MSG){              // Loop para imprimir el mensaje completo
                    if (PIR1bits.TXIF){             // Esperamos a que est  libre el �TXREG para poder enviar por el serial
                        TXREG = mensaje[indice];    // Cargamos caracter a enviar
                        indice++;                   // Incrementamos indice para enviarsigiente caracter
                }
            }
                valor_old = mensaje[6];             // Guardamos valor recibido para comparar en siguiente iteraci n�
              
            } //if old value  
        
        }//mode 3

   }//WHILE(1))
  
    return;
}
