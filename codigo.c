/************************************************************************* 
* Nombre: MARIO ALBERTO LUNA LARA * 
* Dispositivo: RELOJ ALARMA * 
* Lenguaje: C * * Rev: 3.2 * 
* Fecha: 11/06/23 * 
************************************************************************/ 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#define F_CPU 16000000UL
//Definiciones #define SWITCHES PIND 
#define SW0 PIND4 
//Boton INC 
#define SW1 PIND5 
//Boton DEC 
#define SW2 PIND3 
//Boton SET 
#define SW3 PIND2 
//Boton RESET 
#define SW4 PIND1 
//Boton STOP 
#define DISPLAY_DDRX_1 DDRB 
#define DISPLAY_PORTX_1 PORTB 
#define DISPLAY_DDRX_2 DDRD 
#define DISPLAY_PORTX_2 PORTD
//SEGMENTO A 
#define SEG_A PORTB0 
#define SEG_A_ON DISPLAY_PORTX_1 |= (1<<SEG_A) 
#define SEG_A_OFF DISPLAY_PORTX_1 &=~(1<<SEG_A)
//SEGMENTO B 
#define SEG_B PORTD6 
#define SEG_B_ON DISPLAY_PORTX_2 |= (1<<SEG_B) 
#define SEG_B_OFF DISPLAY_PORTX_2 &=~(1<<SEG_B) 
//SEGMENTO C 
#define SEG_C PORTB2 
#define SEG_C_ON DISPLAY_PORTX_1 |= (1<<SEG_C) 
#define SEG_C_OFF DISPLAY_PORTX_1 &=~(1<<SEG_C) 
//SEGMENTO D 
#define SEG_D PORTB3 
#define SEG_D_ON DISPLAY_PORTX_1 |= (1<<SEG_D) 
#define SEG_D_OFF DISPLAY_PORTX_1 &=~(1<<SEG_D) 
//SEGMENTO E 
#define SEG_E PORTB4 
#define SEG_E_ON DISPLAY_PORTX_1 |= (1<<SEG_E) 
#define SEG_E_OFF DISPLAY_PORTX_1 &=~(1<<SEG_E) 
//SEGMENTO F 
#define SEG_F PORTB5 
#define SEG_F_ON DISPLAY_PORTX_1 |= (1<<SEG_F) 
#define SEG_F_OFF DISPLAY_PORTX_1 &=~(1<<SEG_F) 
//SEGMENTO G 
#define SEG_G PORTD7 
#define SEG_G_ON DISPLAY_PORTX_2 |= (1<<SEG_G) 
#define SEG_G_OFF DISPLAY_PORTX_2 &=~(1<<SEG_G)
#define _BV(bit)(1 << (bit))
//BARRIDO 
#define BARRIDO_DDRX DDRC 
#define BARRIDO_PORTX PORTC 
#define BARRIDO_UNIDADES PORTC4 
#define BARRIDO_DECIMAS PORTC3 
#define BARRIDO_CENTESIMAS PORTC2 
#define BARRIDO_MILESIMAS PORTC1
#define ON_BARRIDO_UNIDADES BARRIDO_PORTX |= (1<<BARRIDO_UNIDADES); 
#define OFF_BARRIDO_UNIDADES BARRIDO_PORTX &=~ (1<<BARRIDO_UNIDADES); 
#define ON_BARRIDO_DECIMAS BARRIDO_PORTX |= (1<<BARRIDO_DECIMAS); 
#define OFF_BARRIDO_DECIMAS BARRIDO_PORTX &=~ (1<<BARRIDO_DECIMAS); 
#define ON_BARRIDO_CENTESIMAS BARRIDO_PORTX |= (1<<BARRIDO_CENTESIMAS); 
#define OFF_BARRIDO_CENTESIMAS BARRIDO_PORTX &=~ (1<<BARRIDO_CENTESIMAS); 
#define ON_BARRIDO_MILESIMAS BARRIDO_PORTX |= (1<<BARRIDO_MILESIMAS); 
#define OFF_BARRIDO_MILESIMAS BARRIDO_PORTX &=~ (1<<BARRIDO_MILESIMAS);

// ELIMINACIÓN DE REBOTES 
uint8_t RisingEdge(uint8_t pin); 
uint8_t RisingEdge1(uint8_t pin); 
uint8_t RisingEdge2(uint8_t pin); 
uint8_t RisingEdge3(uint8_t pin); 
uint8_t RisingEdge4(uint8_t pin);

// ESTADOS PARA LA ELIMINACIÓN DE REBOTES 
uint8_t EstadoActual = 0; 
uint8_t EstadoAnterior = 0; 
uint8_t EstadoActual1 = 0; 
uint8_t EstadoAnterior1 = 0; 
uint8_t EstadoActual2 = 0; 
uint8_t EstadoAnterior2 = 0; 
uint8_t EstadoActual3 = 0; 
uint8_t EstadoAnterior3 = 0; 
uint8_t EstadoActual4 = 0; 
uint8_t EstadoAnterior4 = 0;

// MAQUINA DE ESTADOS 
enum Estado { SET_TIME, // ESTABLECER HORA 
    SET_ALARM,          // ESTABLECER HORA DE ALARMA 
    RUN,                // INICIA EL RELOJ 
    ALARM               // SUENA LA ALARMA 
};
enum Estado actual_state;
// FUNCIONES 
void ports_init (void); // INICIALIZACIÓN DE PUERTOS 
void init_display(void); // INICIALIZACIÓN DEL DISPLAY 
void init_timer0(void); // INICIALIZACIÓN DEL TIMER 0 
void init_ADC(void); // INICIALIZACIÓN DE LA CONVERSIÓN ANALOGA-DIGITAL 
void ADC_ON(void); // ENCENDIDO DEL PUERTO ANALOGO-DIGITAL 
void ADC_OFF(void); // APAGADO DEL PUERTO ANALOGO-DIGITAL 
void init_timer1(void); // INICIALIZACIÓN DEL TIMER 1 
void on_timer0(void); // ENCENDIDO DEL TIMER 0 
void timer1_on(void); // ENCENDIDO DEL TIMER 1 
void timer1_off(void); // APAGADO DEL TIMER 1 
void mostrar_letra(uint8_t letra); // MUESTREO DE LETRA EN DISPLAY 
void show_hour(void); // INICIA EL ESTADO RUN 
void config_hora(void); // INICIA EL ESTADO SET TIME 
void config_alarm(void); // INICIA EL ESTADO SET ALARM 
void HI(void); // IMPRIME "HI" EN EL DISPLAY 
void alarm(void); // IMPRIME "ALARM" EN EL DISPLAY 
void potenciometro_horas(void); // MODIFICA LAS HORAS POR MEDIO DEL POTENCIOMETRO 
void botones_minutos(void); // MODIFICA LOS MINUTOS CON LOS BOTONES "INC" Y "DEC" 
void buzzer_active(void); // INICIA EL ESTADO DE ALARM

// VARIABLES GLOBALES 
volatile uint8_t time_counter = 0; // INDICE DEL BARRIDO 
volatile uint8_t potenciometro; // VARIABLE DEL ADC 
volatile uint16_t var_minutes = 0; // CALCULADOR DE MINUTOS 
volatile uint16_t var_time_hours = 100; // TIEMPO IMPRESO (HORAS) 
volatile uint16_t var_time_minutes = 100; // TIEMPO IMPRESO (MINUTOS) 
volatile uint16_t timer_hi = 0; // CONTADOR DE TIEMPO DE PALABRA IMPRESA - SET TIME 
volatile uint16_t timer_alarm = 4; // CONTADOR DE TIEMPO DE PALABRA IMPRESA - SET ALARM 
volatile uint16_t timer_run = 0; // STAND BY PARA TIEMPO EN OTROS ESTADOS 
volatile uint16_t state_alarm = 0; // INDICE PARA SONAR LA ALARMA = 1 CUANDO LA HORA Y LA ALARMA SEAN IGUALES 
volatile uint16_t digit2_hour = 0; // HORA EN SEGUNDO DIGITO 
volatile uint16_t digit1_hour = 0; // HORA EN PRIMER DIGITO

//-------------------------MAIN--------------------------// 
int main (void) { 
    // APAGAR INTERRUPTORES 
    cli(); 
    ports_init(); 
    init_display(); 
    init_timer0(); 
    init_timer1(); 
    init_ADC(); // ACTIVAR INTERRUPTORES 
    sei(); 
    ADC_ON(); 
    on_timer0(); 
    timer1_on();
    actual_state = SET_TIME; // ESTABLECEMOS EL INICIO DE LA MAQUINA DE ESTADOS
    while (1) { 
        switch(actual_state) { 
            case SET_TIME: 
                onfig_hora(); 
            break; 
            
            case SET_ALARM: 
            config_alarm(); 
            break; 
            
            case RUN: 
            show_hour(); 
            break; 
            
            case ALARM: 
            buzzer_active(); 
            break; 
        } 
    }//fin while 
}//fin main

//----------------------------------------------------------------ESTADOS----------------------------------------------------------------//

// CONFIGURACIÓN DE HORA INICIAL 
void config_hora(void) { 
    timer_run = 0; 
    //------- MENSAJE "HI" --------- 
    if(timer_hi < 3) { 
        HI(); 
    }else{ 
        //-------POTENCIOMETRO------------- 
        potenciometro_horas(); 
        //-------BOTONES.................... 
        botones_minutos(); 
        if (RisingEdge2(SW2)) { 
            timer_alarm = 0; 
            state_alarm = 0; 
            var_time_hours = (((digit1_hour*1000)+(digit2_hour*100)) + var_time_minutes)-100; 
            actual_state = SET_ALARM; 
        } 
        
        switch (time_counter) { 
            //MILLARES 
            case 0: 
            BARRIDO_PORTX = 0b00011101; 
            mostrar_letra(digit1_hour); 
            break; 

            //CENTENAS 
            case 1: 
            BARRIDO_PORTX = 0b00011011; 
            mostrar_letra(digit2_hour); 
            break; 
            
            //DECENAS 
            case 2: 
            BARRIDO_PORTX = 0b00010111; 
            mostrar_letra((var_time_minutes%100)/10); 
            break;

            //UNIDADAES 
            case 3: 
            BARRIDO_PORTX = 0b00001111; 
            mostrar_letra(var_time_minutes%10); 
            break; 
        }//fin switch 
        
        if(((var_time_minutes%100)/10) == 6) {
            var_time_minutes = var_time_minutes + 40; 
        } if(var_time_minutes >= 1200) { 
            var_time_minutes = 100; 
        } if(var_time_minutes < 1) { 
            var_time_minutes = 0; 
        } if(var_time_minutes == 99) { 
            var_time_minutes = 59 ; 
        } 
    } 
}

// CONFIGURACIÓN DE LA HORA DE LA ALARMA 
void config_alarm() { 
    timer_run = 0; 
    
    //-------MENSAJE "ALARM"--------- 
    if(timer_alarm < 3) { 
        alarm(); 
    } else { 
    //-------POTENCIOMETRO------------- 
        potenciometro_horas(); 
        //-------BOTONES.................... 
        botones_minutos(); 
        if (RisingEdge2(SW2)) { 
            state_alarm = (((digit1_hour*1000)+(digit2_hour*100)) + var_time_minutes)-100; 
            actual_state = RUN; 
        } if (RisingEdge3(SW3)) { 
            timer_hi = 0; 
            state_alarm = 0; 
            actual_state = SET_TIME; 
        } switch (time_counter) { 
            //MILLARES 
            case 0: 
            BARRIDO_PORTX = 0b00011101; 
            mostrar_letra(digit1_hour); 
            break; 
            
            //CENTENAS 
            case 1: 
            BARRIDO_PORTX = 0b00011011;
            mostrar_letra(digit2_hour); 
            break; 
            
            //DECENAS 
            case 2: 
            BARRIDO_PORTX = 0b00010111;
            mostrar_letra((var_time_minutes%100)/10); 
            break; 
            
            //UNIDADAES 
            case 3: 
            BARRIDO_PORTX = 0b00001111;
            mostrar_letra(var_time_minutes%10); 
            break; 
        }//fin switch 
        if(((var_time_minutes%100)/10) == 6) { 
            var_time_minutes = var_time_minutes + 40; 
        } if(var_time_minutes >= 1200) { 
            var_time_minutes = 0; 
        } if(var_time_minutes == 99) { 
            var_time_minutes = 59; 
        } 
    } 
}
// ESTADO RUN 
void show_hour(void) { 
    timer_run = 1; 
    if(state_alarm == var_time_hours) { 
        timer_run = 0; actual_state = ALARM; }
    else{ 
        // var_time_hours = 1234; 
        switch (time_counter) { 
            //MILLARES 
            case 0: 
            BARRIDO_PORTX = 0b00011101; 
            mostrar_letra(var_time_hours/1000); 
            break; 
            
            //CENTENAS 
            case 1: 
            BARRIDO_PORTX = 0b00011011; 
            mostrar_letra((var_time_hours%1000)/100); 
            break; 
            
            //DECENAS 
            case 2: 
            BARRIDO_PORTX = 0b00010111; 
            mostrar_letra((var_time_hours%100)/10); 
            break; 
            
            //UNIDADAES 
            case 3: BARRIDO_PORTX = 0b00001111;
            mostrar_letra(var_time_hours%10); 
            break; 
        }//fin switch 
        
        if(((var_time_hours%100)/10) == 6) {
            var_time_hours = var_time_hours + 40; 
        } if(var_time_hours >= 1200) { 
            var_time_hours = 100;
        } if (RisingEdge3(SW3)) { 
            timer_hi = 0; actual_state = SET_TIME; 
        } 
    } 
}

// ESTADO DE ALARMA 
void buzzer_active(void) { 
    timer_run = 0; 
    
    switch (time_counter) { 
        //MILLARES 
        case 0: 
        BARRIDO_PORTX = 0b00011101; 
        mostrar_letra('H'); 
        break; 
        
        //CENTENAS 
        case 1: 
        BARRIDO_PORTX = 0b00011011; 
        mostrar_letra('O'); 
        break; 
        
        //DECENAS 
        case 2: 
        BARRIDO_PORTX = 0b00010111; 
        mostrar_letra('L'); 
        break; 
        
        //UNIDADAES 
        case 3: 
        BARRIDO_PORTX = 0b00001111; 
        mostrar_letra('A'); 
        break; 
    }
        
    //fin 
    switch if (RisingEdge3(SW3)) { 
        timer_hi = 0; actual_state = SET_TIME; state_alarm = 0; 
    } if (RisingEdge4(SW4)) { 
        timer_hi = 0; state_alarm = 0; actual_state = RUN; 
    } 
}

//-----------------------------------------------------------------DISPLAYS----------------------------------------------------------------------//
// PALABRA "HI" 
void HI(void) { 
    switch (time_counter) {
        //MILLARES 
        case 0: 
        BARRIDO_PORTX = 0b00011101; 
        mostrar_letra('H'); 
        break; 
        
        //CENTENAS 
        case 1: 
        BARRIDO_PORTX = 0b00011011; 
        mostrar_letra('I'); 
        break; 
        
        //DECENAS 
        case 2: 
        BARRIDO_PORTX = 0b00010111; 
        mostrar_letra('*'); //Este caso es para que no se muestre nada en ese display 
        break; 
        
        //UNIDADAES 
        case 3: 
        BARRIDO_PORTX = 0b00001111; 
        mostrar_letra('*'); 
        break; 
    } 
}
    // PALABRA "ALARM" 
    void alarm(void) { 
        switch (time_counter) { 
            //MILLARES 
            case 0: 
            BARRIDO_PORTX = 0b00011101; 
            mostrar_letra('A'); 
            break; 
            
            //CENTENAS 
            case 1: 
            BARRIDO_PORTX = 0b00011011; 
            mostrar_letra('L'); 
            break; 
            
            //DECENAS 
            case 2: 
            BARRIDO_PORTX = 0b00010111; 
            mostrar_letra('R'); 
            break; 
            
            //UNIDADAES 
            case 3: 
            BARRIDO_PORTX = 0b00001111; 
            mostrar_letra('M'); 
            break; 
        } 
    }
    
    // SEGMENTOS (LETRAS Y NUMEROS) DEL DISPLAY 
    void mostrar_letra(uint8_t letra) { 
        switch (letra) { 
            // NUMEROS 
            case 0: 
                SEG_A_ON; 
                SEG_B_ON; 
                SEG_C_ON; 
                SEG_D_ON;
                SEG_E_ON; 
                SEG_F_ON; 
                SEG_G_OFF; 
            break; 
            
            case 1: 
                SEG_A_OFF; 
                SEG_B_ON; 
                SEG_C_ON; 
                SEG_D_OFF; 
                SEG_E_OFF; 
                SEG_F_OFF; 
                SEG_G_OFF; 
            break; 
            
            case 2: 
                SEG_A_ON; 
                SEG_B_ON; 
                SEG_C_OFF; 
                SEG_D_ON; 
                SEG_E_ON; 
                SEG_F_OFF; 
                SEG_G_ON; 
            break; 
            
            case 3: 
                SEG_A_ON; 
                SEG_B_ON; 
                SEG_C_ON; 
                SEG_D_ON; 
                SEG_E_OFF; 
                SEG_F_OFF; 
                SEG_G_ON; 
            break; 
            
            case 4: 
                SEG_A_OFF; 
                SEG_B_ON; 
                SEG_C_ON; 
                SEG_D_OFF; 
                SEG_E_OFF; 
                SEG_F_ON; 
                SEG_G_ON; 
            break; 
            
            case 5: 
                SEG_A_ON; 
                SEG_B_OFF; 
                SEG_C_ON; 
                SEG_D_ON; 
                SEG_E_OFF; 
                SEG_F_ON; 
                SEG_G_ON; 
            break; 
            case 6: 
                SEG_A_ON; 
                SEG_B_OFF; 
                SEG_C_ON; 
                SEG_D_ON; 
                SEG_E_ON; 
                SEG_F_ON; 
                SEG_G_ON; 
            break; 
            case 7: 
                SEG_A_ON; 
                SEG_B_ON; 
                SEG_C_ON; 
                SEG_D_OFF;
                SEG_E_OFF; 
                SEG_F_OFF; 
                SEG_G_OFF; 
            break; 
            
            case 8: 
                SEG_A_ON; 
                SEG_B_ON; 
                SEG_C_ON; 
                SEG_D_ON; 
                SEG_E_ON; 
                SEG_F_ON; 
                SEG_G_ON; 
            break; 
            
            case 9: 
                SEG_A_ON; 
                SEG_B_ON; 
                SEG_C_ON; 
                SEG_D_ON; 
                SEG_E_OFF; 
                SEG_F_ON; 
                SEG_G_ON; 
            break; 
            
            // LETRAS 
            case '*': 
                SEG_A_OFF; 
                SEG_B_OFF; 
                SEG_C_OFF; 
                SEG_D_OFF; 
                SEG_E_OFF; 
                SEG_F_OFF; 
                SEG_G_OFF; 
            break; 
            
            case 'H': 
                SEG_A_OFF; 
                SEG_B_ON; 
                SEG_C_ON; 
                SEG_D_OFF; 
                SEG_E_ON; 
                SEG_F_ON; 
                SEG_G_ON; 
            break; 

            case 'I': 
                SEG_A_OFF; 
                SEG_B_ON; 
                SEG_C_ON; 
                SEG_D_OFF; 
                SEG_E_OFF; 
                SEG_F_OFF; 
                SEG_G_OFF; 
            break; 
            
            case 'A': 
                SEG_A_ON; 
                SEG_B_ON; 
                SEG_C_ON; 
                SEG_D_OFF; 
                SEG_E_ON; 
                SEG_F_ON; 
                SEG_G_ON; 
            break; 
            
            case 'L': 
                SEG_A_OFF; 
                SEG_B_OFF; 
                SEG_C_OFF;
                SEG_D_ON; 
                SEG_E_ON; 
                SEG_F_ON; 
                SEG_G_OFF; 
            break; 
            
            case 'R': 
                SEG_A_ON; 
                SEG_B_ON; 
                SEG_C_ON; 
                SEG_D_OFF; 
                SEG_E_ON; 
                SEG_F_ON; 
                SEG_G_ON; 
            break; 
            
            case 'M': 
                SEG_A_ON; 
                SEG_B_ON; 
                SEG_C_ON; 
                SEG_D_OFF; 
                SEG_E_ON; 
                SEG_F_ON; 
                SEG_G_OFF; 
            break; 
            
            case 'O': 
                SEG_A_ON; 
                SEG_B_ON; 
                SEG_C_ON; 
                SEG_D_ON; 
                SEG_E_ON; 
                SEG_F_ON; 
                SEG_G_OFF; 
            break; 
        } 
    }

    // AJUSTE DE HORAS POR MEDIO DEL POTENCIOMETRO

    void potenciometro_horas(void) { 
        if(potenciometro <= 21) { 
            digit2_hour = 1; 
            digit1_hour = 0; 
        } else if (potenciometro >= 22 && potenciometro <= 42) { 
            digit2_hour = 2; 
            digit1_hour = 0; 
        }else if (potenciometro >= 43 && potenciometro <= 64) { 
            digit2_hour = 3; 
            digit1_hour = 0; 
        } else if (potenciometro >= 65 && potenciometro <= 86) { 
            digit2_hour = 4; 
            digit1_hour = 0; 
        } else if (potenciometro >= 87 && potenciometro <= 108) { 
            digit2_hour = 5; 
            digit1_hour = 0;
        } else if (potenciometro >= 109 && potenciometro <= 129) { 
            digit2_hour = 6; digit1_hour = 0; 
        } else if (potenciometro >= 130 && potenciometro <= 150) { 
            digit2_hour = 7; 
            digit1_hour = 0; 
        }else if (potenciometro >= 151 && potenciometro <= 171) { 
            digit2_hour = 8; 
            digit1_hour = 0; 
        } else if (potenciometro >= 172 && potenciometro <= 192) { 
            digit2_hour = 9; digit1_hour = 0; 
        }else if (potenciometro >= 193 && potenciometro <= 213) { 
            digit2_hour = 0; digit1_hour = 1; 
        } else if (potenciometro >= 214 && potenciometro <= 234) { 
            digit2_hour = 1; digit1_hour = 1; 
        } else if (potenciometro >= 235) { 
            digit2_hour = 2; digit1_hour = 1; 
        } 
    }
    // BOTONES
    // AJUSTE DE MINUTOS CON BOTONES 
    void botones_minutos() { 
        if (RisingEdge(SW0)) { // BOTON INC 
            var_time_minutes++; 
        } if (RisingEdge1(SW1)) { // BOTON DEC  
            var_time_minutes--; 
        } 
    
    }
    // RISINGEDGE
    uint8_t RisingEdge(uint8_t pin) {
        // SET_TIME 
        EstadoAnterior = EstadoActual; 
        EstadoActual = bit_is_clear(SWITCHES,pin); 
        if (EstadoAnterior == 0 && EstadoActual== 1) { 
            return 1; 
        } else {
            return 0; 
        } 
    }
    uint8_t RisingEdge1(uint8_t pin) { 
        // SET_TIME 
        EstadoAnterior1 = EstadoActual1; 
        EstadoActual1 = bit_is_clear(SWITCHES,pin); 
        if (EstadoAnterior1 == 0 && EstadoActual1== 1) { 
            return 1; 
        } else { 
            return 0; 
        } 
    }
    
    uint8_t RisingEdge2(uint8_t pin) {
        // SET 
        EstadoAnterior2 = EstadoActual2; 
        EstadoActual2 = bit_is_clear(SWITCHES,pin); 
        if (EstadoAnterior2 == 0 && EstadoActual2== 1) { 
            return 1; 
        } else { 
            return 0; 
        } 
    }
    uint8_t RisingEdge3(uint8_t pin) {
        // SET 
        EstadoAnterior3 = EstadoActual3; 
        EstadoActual3 = bit_is_clear(SWITCHES,pin); 
        if (EstadoAnterior3 == 0 && EstadoActual3== 1) { 
            return 1; 
        } else{ 
            return 0; 
        } 
    }
    uint8_t RisingEdge4(uint8_t pin) {
        // STOP 
        EstadoAnterior4 = EstadoActual4; 
        EstadoActual4 = bit_is_clear(SWITCHES,pin); 
        if (EstadoAnterior4 == 0 && EstadoActual4== 1) { 
            return 1; 
        } else { 
            return 0; 
        }
    }
    // INICIALIZACION DE PUERTOS, TIMER Y ADC
    void ports_init (void) { 
        // ENTRADAS 
        DDRD &= ~(_BV(SW0) | _BV(SW1) | _BV(SW2) | _BV(SW3) | _BV(SW4)); 
        // PULL-UP ACTIVADA 
        PORTD |= (_BV(SW0) | _BV(SW1) | _BV(SW2) | _BV(SW3) | _BV(SW4)); 
        // SALIDAS 
        BARRIDO_DDRX |=(1<<BARRIDO_UNIDADES); BARRIDO_DDRX |=(1<<BARRIDO_DECIMAS); 
        BARRIDO_DDRX |=(1<<BARRIDO_CENTESIMAS); BARRIDO_DDRX |=(1<<BARRIDO_MILESIMAS); 
    }
    void init_display(void) { 
        // SALIDAS 
        DISPLAY_DDRX_1 |= (1<<SEG_A); DISPLAY_DDRX_2 |= (1<<SEG_B); 
        DISPLAY_DDRX_1 |= (1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F); 
        DISPLAY_DDRX_2 |= (1<<SEG_G); 
        // INICIALIZACIÓN 
        DISPLAY_PORTX_1 &= ~(1<<SEG_A); 
        DISPLAY_PORTX_2 &= ~(1<<SEG_B); 
        DISPLAY_PORTX_1 &= ~(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F); 
        DISPLAY_PORTX_2 &= ~(1<<SEG_G); 
    }
    // TIMER 0 
    void init_timer0(void) { 
        // CTC mode 
        TCCR0B &= ~ (1<<WGM02); 
        TCCR0A |= (1<<WGM01); 
        TCCR0A &= ~ (1<<WGM00); 
        // TOPE 
        OCR0A = 15625; 
        //1s 
        // ACT INTERRUPCION 
        TIMSK0 |= (1<<OCIE0A); 
    } 
    void on_timer0(void) { 
        // PRESCALER 1024 
        TCCR0B |= (1<<CS02); 
        TCCR0B &= ~ (1<<CS01); 
        TCCR0B |= (1<<CS00); 
    } void off_timer0(void) { 
        // STOP 
        TCCR0B &= ~ (1<<CS02); 
        TCCR0B &= ~ (1<<CS01); 
        TCCR0B &= ~ (1<<CS00); 
    }

    ISR(TIMER0_COMPA_vect) { 
        time_counter++; 
        if (time_counter>4) {
            time_counter=0; 
        } 
    }
    // ADC 
    void init_ADC(void) { 
        //REFS# CONFIGURA EL MODO DE VOLTAJE DE REFERENCIA 
        //En este caso es para poner capacitor en AREF (01) // y el AVCC es el pin de referencia 
        ADMUX &=~ (1<<REFS1); 
        ADMUX |= (1<<REFS0); //Configurar los bits 
        //Si ADLAR es 1 es para 8 bits 
        //Si ADLAR es 0 es para 10 bits 
        ADMUX |= (1<<ADLAR); //ESCOGER EL PIN ANALOGO PARA EL ADC 
        //Configurado para ADC0 (se coloca 0000) 
        ADMUX &=~ (1<<MUX3); 
        ADMUX &=~ (1<<MUX2); 
        ADMUX &=~ (1<<MUX1); 
        ADMUX &=~ (1<<MUX0); //Configurado para Freeruning 
        ADCSRA |= (1<<ADATE); //Activar interrupcion 
        ADCSRA |= (1<<ADIE); //Velocidad de muestreo 
        //Configuracion de PREESCALER a 8 (011), frecuencia debe estar entre 50kHz-200kHz 
        ADCSRA &=~(1<<ADPS2); 
        ADCSRA |=(1<<ADPS1); 
        ADCSRA |=(1<<ADPS0); 
    } 

    void ADC_ON(void) { 
        //ACTIVAR ADC 
        ADCSRA |= (1<<ADEN); 
        //INICIAR LA CONVERSION 
        ADCSRA |= (1<<ADSC); 
    }

    void ADC_OFF(void) { 
        //DESACTIVAR ADC 
        ADCSRA &=~ (1<<ADEN); 
    }

    ISR(ADC_vect) { 
        //Convierte los valores en numeros binarios 0-255 
        potenciometro=ADCH; 
    }
    //-----------TIMER1 
    void init_timer1(void) {
        //Modo 4 de configuracion CTC (poner 0010) 
        TCCR1A &= ~ _BV(WGM10);     //0 en el bit WGM10 
        TCCR1A &= ~ _BV(WGM11);     //0 en el bit WGM11 
        TCCR1B |= _BV(WGM12);       //1 en el bit WGM12 
        TCCR1B &= ~ _BV(WGM13);     //0 en el bit WGM13 //Tiempo para 1s 
        OCR1A = 15625;              //Registro de 16 bits 
        TIMSK1 |= (1<<OCIE0A);      //TIMSK1 habilita interrupcion 
    }
    void timer1_on(void) { 
        TCNT1 = 0;              //Registro de 16 bits 
        //PRESCALER configurado a 1024 (010) 
        TCCR1B |= (1<<CS10);    //1 en el bit CS10 
        TCCR1B &=~ (1<<CS11);   //1 en el bit CS11 
        TCCR1B |= (1<<CS12);    //1 en el bit CS12 
    }
    void timer1_off(void) { 
        //Modo timer detenido 
        TCCR1B &=~ (1<<CS10); //0 en el bit CS10 
        TCCR1B &=~ (1<<CS11); //0 en el bit CS11 
        TCCR1B &=~ (1<<CS12); //0 en el bit CS12 
    }
    ISR(TIMER1_COMPA_vect) { 
        var_minutes++; 
        if (var_minutes == 81) { 
            if(timer_run == 1 && var_time_hours!=state_alarm) {//cuando run esta activo y la alarma no esta activa, hace la cuenta en var_time_hours 
                var_time_hours ++; 
            } var_minutes = 0; 
        } if(timer_hi < 4) { //Contador de la palabra HI  
            timer_hi++; 
        }else { 
            timer_hi = 4; 
        } if(timer_alarm < 4) {//Contador de la palabra ALARM  
            timer_alarm++; 
        }else { 
            timer_alarm = 4; 
        } 
    }