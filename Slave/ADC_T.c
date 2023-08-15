
#include "ADC_T.h"

// Configuración del ADC
void ADC_CONF(){
    
    ADCON0bits.ADCS0 = 1; 
    ADCON0bits.ADCS1 = 0; 
    ADCON1 = 0b00110000;
    ADCON0bits.CHS = 0b0000; // Selecciona el canal para conversión
    
    ADCON0bits.ADON = 1; // Habilitar ADC
    PIE1bits.ADIE = 1; // Habilitar ADC
    PIR1bits.ADIF = 0; // Habilitar ADC

}    


// Función para realizar una conversión ADC en modo de barrido continuo
void conf_ch(int sel){
    
    if (sel == 0){
        ADCON0bits.CHS = 0b0000;
    }
    
    if (sel == 1){ 
        ADCON0bits.CHS = 0b0001;
    }
}



