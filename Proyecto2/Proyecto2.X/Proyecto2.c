/*
 * File:   Proyecto2.c
 * Author: WMGWW
 *
 * Created on 16 de mayo de 2022, 11:03 AM
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

#define LEN_MSG 20               // Constante para definir largo de mensaje e 
/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
char msg[LEN_MSG] = {'C', 'h', 'o', 'o', 's', 'e', ' ', 'P', 'o', 's', 'i', 't', 'i', 'o', 'n', ':', ' ', ' ', 0x0D, 0x0A};
uint8_t potvalue = 0;
uint8_t index = 0;             // Variable para saber que posici n del mensaje 
uint8_t mode = 0;       //variable for mode
uint8_t oldvalue = 0;          // Variable para guardar el valor anterior recibido

//Interruptions
void __interrupt() isr (void){
    
    if(PIR1bits.RCIF){          // Hay datos recibidos?
        
        msg[17] = RCREG;     // Guardamos valor recibido en el arreglo mensaje
        //PORTD = msg[6];     // Mostramos valor recibido en el PORTD
    }
    
    //ADC Interrupt
     if (ADIF == 1) {
        if(!ADCON0bits.CHS){ //POT Eye 1
            ADCON0bits.CHS = 1; 
        }
        else if(ADCON0bits.CHS == 1) { //POT Eye 2
            ADCON0bits.CHS = 2;//se cambia a canal del tercer pot
        } 
        else if(ADCON0bits.CHS == 2) {      //POT Eyebrow   1
            ADCON0bits.CHS = 3;//se cambia a canal del tercer pot
        } 
        else if(ADCON0bits.CHS == 3) {      //POT Eyebrow   2
            ADCON0bits.CHS = 0;//se cambia a canal del tercer pot
        } 
        __delay_us(40);   
        PIR1bits.ADIF = 0;
        ADCON0bits.GO = 1;
        
    }
}
void
send(char x[]){

       if (oldvalue != msg[17] ){                // Verificamos que el nuevo valor 
            while(x[index]  !=  0){              // Loop para imprimir el mensaje 
                if (PIR1bits.TXIF){             // Esperamos a que este
                    TXREG = x[index];        // Cargamos caracter a enviar  
                    index++;                  // Incrementamos indice para enviar 

           }// if PIR1bits
                oldvalue = msg[17];   //almacena el la posicion 6 de msg el valor ingresado
            }//while                               //
        }//if !=
}

void main(void) {
    if (mode == 3){
        send("Eliga \r 1. Leer Potenciometro \r 2. Enviar Ascii \r");
    }
    return;
}
