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
 * @file    PWM_Act.c
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

void clk_init();

void TPM1_CH0_init(unsigned short, unsigned char);

void __delay(unsigned int);

/*
 * @brief   Application entry point.
 */
int main(void) {

	unsigned short periodo;
	unsigned char duty_cycle;

	clk_init();

	periodo = 1000;
	duty_cycle = 0;
	TPM1_CH0_init(periodo, duty_cycle);


	while (1){


	}

    return 0 ;
}

void clk_init(){						// INIT CLOCK

	MCG->C1 |= (1<<6) + (1<<1);
	MCG->C2 |= 1;
	MCG->SC = 0;

	SIM->CLKDIV1 = 0;
	SIM->SOPT2 |= 15<<24;

}

void TPM1_CH0_init(unsigned short periodo, unsigned char duty_cycle){					// INIT TIMER

	SIM->SCGC5 |= (1<<9);				// Habilitamos el clock del port A.
	PORTA->PCR[2] = (3<<8);			// Pin PTA12 Posible error! (3<<8)

	SIM->SCGC6 |= (1<<25);				// TPM1 Clock activated.
	TPM1->SC = (1<<3) + 2;
	TPM1->CONTROLS[0].CnSC = (9<<2);

	TPM1->CONTROLS[0].CnV = periodo - duty_cycle * 10;
	TPM1->MOD = periodo;				// Hasta donde se desborda el LPTPM

}

void __delay(unsigned int ms){
	for	(int i = 0; i < ms; i++){
		__asm volatile ("NOP");
	}
}
