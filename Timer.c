/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
void LEDR_TOGLE(void);
void PORTB_Init(void);
void LPTimer_IRQHandler()
{
	// Función de interrupción del timer
	LPTMR0_CSR |= (1<<7);
	LEDR_TOGLE();
	
}
void LPTIMER0_init()
{
	SIM_SCGC5 |= 1;
	LPTMR0_PSR = 5; // 1 bypas al prescaler; 01: selección de reloj (LP0 1kHz)
	LPTMR0_CMR = 500-1; // Cada medio segundo
	LPTMR0_CSR = (1<<6)+1; // Se habilitan las interrupciones
	NVIC_ISER = (1<<28);
}

int main(void)
{
	int counter = 0;
	
	
	LPTIMER0_init();
	PORTB_Init();
	for(;;) {	   
	   	counter++;
	}
	
	return 0;
}
void PORTB_Init(void)
{
	SIM_SCGC5 |= (1<<10);
	PORTB_PCR18 |= (1<<8);
	GPIOB_PDDR |= (1<<18);
}
void LEDR_TOGLE(void)
{
	GPIOB_PTOR |= 0x00040000;
}
