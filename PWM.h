/*
 * PWM.h
 *
 *  Created on: Dec 1, 2021
 *      Author: richo
 */

#ifndef PWM_H_
#define PWM_H_

void PWM_Init(){						// INIT CLOCK

	MCG->C1 |= (1<<6) + (1<<1);
	MCG->C2 |= 1;
	MCG->SC = 0;

	SIM->CLKDIV1 = 0;
	SIM->SOPT2 |= 15<<24;

}

void PWM_Set(int periodo, int duty_cycle){					// INIT TIMER

	SIM->SCGC5 |= (1<<9);				// Habilitamos el clock del port A.
	PORTA->PCR[12] = (3<<8);			// Pin PTA12 Posible error! (3<<8)

	SIM->SCGC6 |= (1<<25);				// TPM1 Clock activated.
	TPM1->SC = (1<<3) + 2;
	TPM1->CONTROLS[0].CnSC = (9<<2);

	TPM1->CONTROLS[0].CnV = periodo - duty_cycle * 10;
	TPM1->MOD = periodo;				// Hasta donde se desborda el LPTPM

	while( (TPM1->SC & 0x80) != 0x80);

}

#endif /* PWM_H_ */
