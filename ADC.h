/*
 * ADC.h
 *
 *  Created on: Dec 1, 2021
 *      Author: richo
 */

#ifndef ADC_H_
#define ADC_H_

void ADC_Init(){
	SIM->SCGC6 |= (1 << 27);
	ADC0->CFG1 |= 0x0C;
}

int ADC_value(){

	ADC0->SC1[0] = 0x08;

	while( (ADC0->SC1[0] & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);

	return ADC0->R[0];

}

#endif /* ADC_H_ */
