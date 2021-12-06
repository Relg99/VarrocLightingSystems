/*
 * delay.h
 *
 *  Created on: Dec 1, 2021
 *      Author: richo
 */

#ifndef DELAY_H_
#define DELAY_H_

void __delay(unsigned int ms){
	for(int i = 0; i < ms; i++){
		__asm volatile ("NOP");
	}
}

#endif /* DELAY_H_ */
