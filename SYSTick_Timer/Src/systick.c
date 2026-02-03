#include "stm32f4xx.h"

#define SYSTICK_LOAD_VAL 16000
#define CTRL_ENABLE	(1U<<0)
#define CTRL_CLK_SRC (1U<<2)
#define CTRL_COUNTFLAG (1U<<16)

void systickDelayMs(int delay){
	/*Configure SYSTICK*/
	//reload with no. of clks per milisecond
	SysTick->LOAD = SYSTICK_LOAD_VAL;
	//clear the systick current value
	SysTick->VAL =0;
	//enable systick and select internal clk source
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLK_SRC;
	for (int i=0;i<delay;i++){
		//wait until count flag is set
		while ((SysTick->CTRL & CTRL_COUNTFLAG)==0){

		}
		SysTick->CTRL=0;

	}

}
