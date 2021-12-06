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
 * @file    Reto_Varroc.c
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
#include "math.h"
#include <PWM.h>
#include <ADC.h>
#include <TIMER0.h>
#include <delay.h>

/* TODO: insert other definitions and declarations here. */
float e_0 = 0.0;
float e_1 = 0.0;
float e_2 = 0.0;

float r_1 = 0.0;
float T_1 = 0.0;

float u_0 = 0.0;
float u_1 = 0.0;

float q_0 = 0.0;
float q_1 = 0.0;
float q_2 = 0.0;

void PID();
void PORTB_Init();

void LPTMR0_IRQHandler(){

	LPTMR0->CSR |= (1 << 7);			// Flag of Timer 0

	//GPIOB->PTOR |= (1 << 18);

	PID();

}

/*
 * @brief   Application entry point.
 */
int main(void) {

	unsigned int ms = 200;			//Tiempo de interrupciones

	float VoltajeTer = 0.0;
	float ResistenciaTer = 0.0;
	float TempCelcius = 0.0;
	float VCC = 3.3;				// CAMBIAR SI SE USAN 5V

	int TempAmbiente = 23;
	int R = 5500;					// CAMBIAR DEPENDIENDO DE LA RESISTENCIA
	int Rz = 47000;					// VERIFICAR ANTES DE CONECTAR
	int B = 4101;
	int Ts = 10;

	int kp = 8;
	int ti = 5;
	int td = 0;

	r_1 = 35.0; 					//Temperatura referencia

	q_0 = kp * (1+Ts/(2.0 * ti)+td/Ts);
	q_1 = -kp * (1-Ts/(2.0 * ti)+(2.0 * td)/Ts);
	q_2 = (kp*td)/Ts;

	PWM_Init();
	ADC_Init();
	PORTB_Init();
	LPTMR_Init(ms);

	while(1){

		GPIOB->PSOR |= (3 << 18);


		float aux = 0.0;

		for(int i = 0; i < 10; i++){

			/*
			TempC = float(analogRead(Termistor) * (500.0 / 1023.0) );
			*/

			VoltajeTer = ADC_value();
			VoltajeTer = 3.3 * (VoltajeTer/65535);					//CAMBIAR SI SE USAN 5V

			ResistenciaTer = (-R * VoltajeTer) / (VoltajeTer - VCC);
			TempCelcius =  1/((log(ResistenciaTer / Rz) / B) + pow((TempAmbiente + 273), -1));
			TempCelcius = TempCelcius - 273.15;

			aux = aux + TempCelcius;

		}

		T_1 = aux/10.0;

	}

	return 0;
}

void PID(){

	e_0 = (r_1 - T_1);


	u_0 = u_1+q_0 * e_0+q_1 * e_1+q_2 * e_2;

	if (u_0 >= 100.0){
		u_0 = 100.0;
	}

	if (u_0 <= 0.0 || r_1 == T_1){
		u_0 = 0.0;
		GPIOB->PCOR |= (1 << 19);
	}

	e_2 = e_1;
	e_1 = e_0;
	u_1 = u_0;

	PWM_Set(1000, u_0);

}

void PORTB_Init(){
	SIM->SCGC5 |= (1 << 10);
	PORTB->PCR[18] |= (1 << 8);
	PORTB->PCR[19] |= (1 << 8);
	GPIOB->PSOR |= (3 << 18);
	GPIOB->PDDR |= (3 << 18);
}
