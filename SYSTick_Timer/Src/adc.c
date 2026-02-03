#include "adc.h"
#include "stm32f4xx.h"
#define ADC1EN  (1U<<8)


#define GPIOAEN (1U<<0)
#define ADC_SEQ_LEN1  0x00
#define ADC_CH1 (1U<<0)
#define ADC_CR2 (1U<<0)
#define CR_2 (1U<<30)
#define SR_EOC (1U<<1)
#define CR2_CONT (1U<<1)
/**ADC configure with 3 channels
ch2,ch3,ch5
first ch5
second ch2
third ch3
**/


void pa1_adc_init(void){

	//Enable clock access to ADC pin port ie PA1
	RCC->AHB1ENR |= GPIOAEN;
	//Set the mode of PA1 to analog mode
	GPIOA->MODER |=(1U<<2);
	GPIOA->MODER |=(1U<<3);

	/** COnfiguring the ADC Module **/
	//Enable clock access to adc module

	RCC->APB2ENR |=ADC1EN;

	//conversion sequence start
	ADC1->SQR3=ADC_CH1;
	//Conversion sequence length
	ADC1->SQR1 =ADC_SEQ_LEN1;
	//ENable the ADC module
	ADC1->CR2 |=ADC_CR2;

}

void start_conversion(void){
	//Enable cotinuous conversion
	ADC1->CR2 |=CR2_CONT;
	/*Start ADC Conversion*/

	ADC1->CR2 |= CR_2;
}

uint32_t adc_read (void){
	/*wait for conversion to complete*/
	while(! (ADC1->SR & SR_EOC)){

	}
	/*Read Converted data*/
	return (ADC1->DR);
}
