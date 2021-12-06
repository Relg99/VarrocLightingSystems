/*
 * TIMER0.h
 *
 *  Created on: Dec 1, 2021
 *      Author: richo
 */

#ifndef TIMER0_H_
#define TIMER0_H_

void LPTMR_Init(unsigned int ms){
	SIM->SCGC5 |= 0x01;

	LPTMR0->PSR = 5;
	LPTMR0->CMR = ms - 1;
	LPTMR0->CSR = (1 << 6) + 1;

	NVIC->ISER[0] = (1 << 28);
}

#endif /* TIMER0_H_ */
