/* 
 * File:   main.c
 * Author: ANGELA
 *
 * Created on 3 de agosto de 2023, 16:20
 */

#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//***************************
// Definici n e importaci n de librer as
//***************************
#include <xc.h>
#include <stdint.h>
#include "LCD.h"
#include "stdio.h"
#include "I2C.h"
#include "ADC.h"
#include <stdlib.h>
//***************************
// Definici n de variables
//***************************
#define _XTAL_FREQ 8000000
char buffer_SEG[5];
char buffer_ADC[5];
int SEG;
char uni, dec,cen,mil;
char buffer_MIN[5];
int MIN;
char buffer_HOR[5];
int HOR;
char buffer_DAY[5];
int DAY;
char buffer_MOTH[5];
int MOTH;
char buffer_YEAR[5];
int YEAR;

int adc_val;


void Division(char y){
    dec = ((y % 100) / 10);
    uni = ((y % 100) % 10);
}
char uni, dec,cen,mil;

void Division3(char y){
    cen = (y / 100);
    dec = ((y % 100) / 10);
    uni = ((y % 100) % 10);
}
void __interrupt() isr(void){
    if (PIR1bits.ADIF){
        if (ADCON0bits.CHS == 5){
            adc_val = ADRESH; 
        }
        PIR1bits.ADIF = 0; 
    }
}


//***************************
// Definici n de funciones para que se puedan colocar despu s del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//***************************
void setup(void);
uint8_t Bcd_to_Dec(uint8_t bcd);
uint8_t Dec_to_Bcd(uint8_t dec);
//***************************
// C digo Principal
//***************************
void main(void) {
   setup();
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x01);
        I2C_Master_Write(Dec_to_Bcd(20));
        I2C_Master_Stop();
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x00);
        I2C_Master_Write(Dec_to_Bcd(55));
        I2C_Master_Stop();
        
         I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x02);
        I2C_Master_Write(Dec_to_Bcd(11));
        I2C_Master_Stop();
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x04);
        I2C_Master_Write(Dec_to_Bcd(8));
        I2C_Master_Stop();
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x05);
        I2C_Master_Write(Dec_to_Bcd(8));
        I2C_Master_Stop();
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x06);
        I2C_Master_Write(Dec_to_Bcd(23));
        I2C_Master_Stop();
        
        __delay_ms(10);
    while(1){
        ADC_IF();
        // Dentro de la condición de la alarma
        if (HOR == 11 && MIN == 21) {
            PORTCbits.RC1 = 1; // Encender el LED conectado al pin RC1
        }
        if (HOR == 11 && MIN == 22) {
            PORTCbits.RC1 = 0; // apagar el LED conectado al pin RC1
        }
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x00);
        I2C_Master_Stop();
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x00);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        SEG = Bcd_to_Dec(I2C_Master_Read(0));
        I2C_Master_Stop();
        __delay_ms(10);
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x01);
        I2C_Master_Stop();
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x01);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        MIN = Bcd_to_Dec(I2C_Master_Read(0));
        I2C_Master_Stop();
        __delay_ms(10);
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x02);
        I2C_Master_Stop();
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x02);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        HOR = Bcd_to_Dec(I2C_Master_Read(0));
        I2C_Master_Stop();
        __delay_ms(10);
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x04);
        I2C_Master_Stop();
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x04);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        DAY = Bcd_to_Dec(I2C_Master_Read(0));
        I2C_Master_Stop();
        __delay_ms(10);
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x05);
        I2C_Master_Stop();
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x05);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        MOTH = Bcd_to_Dec(I2C_Master_Read(0));
        I2C_Master_Stop();
        __delay_ms(10);
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x06);
        I2C_Master_Stop();
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x06);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        YEAR = Bcd_to_Dec(I2C_Master_Read(0));
        I2C_Master_Stop();
        __delay_ms(10);
        
        
        
        Division(SEG); 
        Lcd_Set(1, 13); 
        sprintf(buffer_SEG, "%d", dec);
        Lcd_Write_Str(buffer_SEG);
        sprintf(buffer_SEG, "%d", uni);
        Lcd_Write_Str(buffer_SEG);
         __delay_ms(10);
        Lcd_Set(1, 12); 
        Lcd_Write_Str(":");
        Division(MIN); 
        Lcd_Set(1, 10); 
        sprintf(buffer_MIN, "%d", dec);
        Lcd_Write_Str(buffer_MIN);
        sprintf(buffer_MIN, "%d", uni);
        Lcd_Write_Str(buffer_MIN);
         __delay_ms(10);
        Lcd_Set(1, 9); 
        Lcd_Write_Str(":");
        Division(HOR); 
        Lcd_Set(1, 7); 
        sprintf(buffer_HOR, "%d", dec);
        Lcd_Write_Str(buffer_HOR);
        sprintf(buffer_HOR, "%d", uni);
        Lcd_Write_Str(buffer_HOR);
         __delay_ms(10);
         
        Division(DAY); 
        Lcd_Set(2, 7); 
        sprintf(buffer_DAY, "%d", dec);
        Lcd_Write_Str(buffer_DAY);
        sprintf(buffer_DAY, "%d", uni);
        Lcd_Write_Str(buffer_DAY);
         __delay_ms(10);
        Lcd_Set(2, 9); 
        Lcd_Write_Str("/");
        Division(MOTH); 
        Lcd_Set(2, 10); 
        sprintf(buffer_MOTH, "%d", dec);
        Lcd_Write_Str(buffer_MOTH);
        sprintf(buffer_MOTH, "%d", uni);
        Lcd_Write_Str(buffer_MOTH);
         __delay_ms(10);
         Lcd_Set(2, 12); 
        Lcd_Write_Str("/");
        Division(YEAR); 
        Lcd_Set(2, 13); 
        sprintf(buffer_YEAR, "%d", dec);
        Lcd_Write_Str(buffer_YEAR);
        sprintf(buffer_YEAR, "%d", uni);
        Lcd_Write_Str(buffer_YEAR);
         __delay_ms(10);
        
       
        Division3(adc_val); 
        Lcd_Set(2, 0); 
        sprintf(buffer_ADC, "%d", cen);
        Lcd_Write_Str(buffer_ADC);
        sprintf(buffer_ADC, "%d", dec);
        Lcd_Write_Str(buffer_ADC);
        sprintf(buffer_ADC, "%d", uni);
        Lcd_Write_Str(buffer_ADC);
         __delay_ms(10);
        
         
      
    }
}
//***************************
// Funci n de Inicializaci n
//***************************
void setup(void){
    // Configuraci n del oscilador
    OSCCONbits.IRCF =   0b0111; //8MHz
    OSCCONbits.SCS = 1;
    ADC_CONF(8); 
    ANSEL = 0;
    ANSELH = 0;
    TRISC2 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;
    TRISD = 0; 
    TRISB = 0; 
    PORTD = 0;
    PORTCbits.RC4 = 1;  
    PORTCbits.RC3 = 1;
    I2C_Master_Init(100000);
    Lcd_Init(); 
    Lcd_Clear();
    Lcd_Set(1,1);
    Lcd_Write_Str("S1       ");
    
    INTCONbits.PEIE = 1; 
    PIE1bits.ADIE = 1; 
    ADCON0bits.GO = 0; 
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    TRISC1 = 0; // Configurar el pin RC1 como salida para el LED
    PORTCbits.RC1 = 0; // Asegurarse de que el LED esté apagado al inicio
}
//Funciones
uint8_t Bcd_to_Dec(uint8_t bcd){                            //Funci n para pasar n meros de bcd a decimal
    uint8_t dec;                                            //Variable para guardar valor
    dec = ((bcd>>4)*10)+(bcd & 0b00001111);                 //Hacer un corrimiento de bits y sumar con la unidad
    return dec;                                             //Retornar valor
}

uint8_t Dec_to_Bcd(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}
