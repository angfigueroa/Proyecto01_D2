/*
 * File:   MAIN1.c
 * Author: ANGELA
 *
 * Created on 31 de julio de 2023, 22:11
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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

//Librerías
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "SPI.h"
#include "LCD.h"
#define S_Term PORTBbits.RB2

//Definición de variables
#define _XTAL_FREQ (8000000)


//Variables
uint8_t termometro;

//Almacenan los chars utilizados para imprimir en el LCD
uint8_t uniTV;
uint8_t decTV;
uint8_t cenTV;
uint8_t temp;
int contador;
int ADC;
int in;
//Prototipos
void setup(void);
void main(void);

//Loop
void main(void) {
    setup();
    while(1){
        S_Term = 0; //Slave select del termometro
        __delay_ms(5);
        spiWrite(1);
        termometro = spiRead();
        __delay_ms(5);
        S_Term = 1;//Slave deselect
        
        Lcd_Set_Cursor(1, 13);
        Lcd_Write_String("TEMP");
        
        if (termometro < 69){
            temp = termometro * 0.81;
            temp = 55 - temp;
            temp = temp * 10;
            uniTV = temp / 100;
            temp = temp - (uniTV * 100);
            decTV = temp / 10;
            temp = temp - (decTV * 10);
            cenTV = temp;
            
            Lcd_Set_Cursor(2,12);
            Lcd_Write_String("-");
            
            Lcd_Set_Cursor(2,13);
            uniTV = uniTV + 48;
            Lcd_Write_Char(uniTV);//Pongo decena
            
            Lcd_Set_Cursor(2,14);
            decTV = decTV + 48;
            Lcd_Write_Char(decTV);//Pongo unidad
            
            Lcd_Set_Cursor(2,15);
            Lcd_Write_String(".");
            
            Lcd_Set_Cursor(2,16);
            cenTV = cenTV + 48;
            Lcd_Write_Char(cenTV);//Pongo decimal  
        }
        else {
            temp = termometro * 0.81;
            temp = temp - 55;//Son positivos el desfase de 55 se quita
            temp = temp * 10;
            uniTV = temp / 100;
            temp = temp - (uniTV * 100);
            decTV = temp / 10;
            temp = temp - (decTV * 10);
            cenTV = temp;
            
            Lcd_Set_Cursor(2,12);
            Lcd_Write_String("+");//Grado positivo
            
            Lcd_Set_Cursor(2,13);
            uniTV = uniTV + 48;
            Lcd_Write_Char(uniTV);//Pongo decena
            
            Lcd_Set_Cursor(2,14);
            decTV = decTV + 48;
            Lcd_Write_Char(decTV);//Pongo unidad
            
            Lcd_Set_Cursor(2,15);
            Lcd_Write_String(".");
            
            Lcd_Set_Cursor(2,16);
            cenTV = cenTV + 48;
            Lcd_Write_Char(cenTV);//Pongo decimal
        } 
    }
    
}

//Funciones
//Setup General
void setup(void){
    
    //Oscilador
    OSCCON = 0B01110001;               //Oscilador a 8Mhz
    //ADC
    //conf_ch(0);
    //Entradas y salidas
    ANSEL = 0;
    ANSELH = 0;
    TRISC = 0;
    TRISC4 = 1;
    TRISB = 0;
    TRISE = 0x00;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    S_Term = 1;
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1, 13);
    Lcd_Write_String("HOLA");
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
      
}

//Interrupcion
void __interrupt() isr(void) {
    /*if (PIR1bits.ADIF){
        ADC = conf_ch();
    }*/
    if (SSPIF == 1){// Verifica si la interrupción es del módulo SPI
        in = spiRead();// Lee los datos recibidos por SPI
        if (in == 0){
            spiWrite(ADC);// Envía el valor del ADC por SPI si se recibe un 0
        }
        else if (in == 1){
            spiWrite(contador);// Envía el valor del contador por SPI si se recibe un 1
        }
    }
    INTCONbits.RBIF = 0;   //Limpia la bandera de interrupción
    PIR1bits.ADIF = 0;// Limpia la bandera de interrupción del ADC
    SSPIF = 0;// Limpia la bandera de interrupción del módulo SPI
}

  

