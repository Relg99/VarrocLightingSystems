/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    Test_1.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
void PORT_INIT(void);

unsigned int GET_ADC(void);


void LEDR(void);
void LEDG(void);
void LEDB(void);
void LEDS_OFF(void);


void __delay(unsigned int ms);
/*
 * @brief   Application entry point.
 */
int main(void) {

	unsigned int adc_value;

    PORT_INIT();

	while(1){

	    adc_value = GET_ADC();

	    if ( adc_value < 0x3333 && adc_value >= 0x0000 ){
	    	LEDR();
	    }
	    if ( adc_value < 0x6666 && adc_value > 0x3333 ){
	    	LEDR();
	    	LEDG();
	    }
	    if ( adc_value < 0x9999 && adc_value > 0x6666 ){
	    	LEDG();
	    }
	    if ( adc_value < 0xCCCC && adc_value > 0x9999 ){
	    	LEDG();
	    	LEDB();
	    }
	    if ( adc_value <= 0xFFFF && adc_value > 0xCCCC ){
	    	LEDB();
	    }

	    LEDS_OFF();

	}

    return 0 ;
}

void PORT_INIT(void){

	SIM->SCGC6 |= 1 << 27;			//RELOJ PARA ADC

	SIM->SCGC5 |= 0x00001400;		//PORTD AND PORTB

	for(int i = 0; i < 31; i++){

		if (i){
			PORTB->PCR[i] |= 0x00000100;
			PORTB->PCR[i] &= 0xFFFFF9FF;
		}


		PORTD->PCR[i] |= 0x00000100;
		PORTD->PCR[i] &= 0xFFFFF9FF;

	}

	GPIOB->PSOR |= 0xFFFFFFFE;
	GPIOD->PSOR |= 0xFFFFFFFF;

	GPIOB->PDDR |= 0xFFFFFFFE;
	GPIOD->PDDR |= 0xFFFFFFFF;

}

unsigned int GET_ADC(){

	ADC0->CFG1 |= 0x0C;				//CONFIG ADC

	ADC0->SC1[0] = 0x08;			//CHANNEL ADC

	while((ADC0->SC1[0] & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);

	return ADC0->R[0];
}

void __delay(unsigned int ms){
	for(int i = 0; i < ms; i++){
		__asm volatile ("nop");
	}
}


void LEDR(void){
	GPIOB->PCOR |= 0x00040000;
}
void LEDG(void){
	GPIOB->PCOR |= 0x00080000;
}
void LEDB(void){
	GPIOD->PCOR |= 0x00000002;
}

void LEDS_OFF(){
	GPIOB->PSOR |= 0x00040000;
	GPIOB->PSOR |= 0x00080000;
	GPIOD->PSOR |= 0x00000002;
}
