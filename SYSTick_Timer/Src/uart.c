
#include "uart.h"


#define GPIOAEN (1U<<0)

#define UART2EN (1U<<17)

#define SYS_FREQ 16000000

#define APB1_CLK SYS_FREQ
#define UART_BAUDRATE  115200

#define CR1_TE (1U<<3)
#define CR1_RE (1U<<2)
#define CR1_UE (1U<<13)
#define SR_TXE (1U<<7)
#define SR_RXNE (1U<<5)


static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t periphclk, uint32_t BaudRate );
static uint16_t compute_uart_bd(uint32_t periphclk, uint32_t BaudRate);
void uart2_write(int ch);
void uart2_tx_init(void);


int __io_putchar(int ch){

	uart2_write(ch);
	return ch;

}

void uart2_tx_init(void){
	//configure uart GPIO Pin
	//enable clock access to GPIO A
	RCC->AHB1ENR |= GPIOAEN;

	//set PA2 to alternate mode
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |=(1U<<5);
	//set PA2 alternate fun type to UART_TX (AF07)
	GPIOA->AFR[0] |=(1U<<8);
	GPIOA->AFR[0] |=(1U<<9);
	GPIOA->AFR[0] |=(1U<<10);
	GPIOA->AFR[0] &=~(1U<<11);
	//Configure USART MODULE
	//Enable clock access
	RCC->APB1ENR |=UART2EN;
	//configure uart baudrate
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);

	//configure the transfer direction
	USART2->CR1 =CR1_TE;
	USART2->CR1 |=CR1_UE;
	//enable uart module
}


void uart2_rxtx_init(void){
	//configure uart GPIO Pin
	//enable clock access to GPIO A
	RCC->AHB1ENR |= GPIOAEN;

	//set PA2 to alternate mode
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |=(1U<<5);
	//set PA2 alternate fun type to UART_TX (AF07)
	GPIOA->AFR[0] |=(1U<<8);
	GPIOA->AFR[0] |=(1U<<9);
	GPIOA->AFR[0] |=(1U<<10);
	GPIOA->AFR[0] &=~(1U<<11);



	//set PA3 to alternate mode
	GPIOA->MODER &=~(1U<<6);
	GPIOA->MODER |=(1U<<7);
	//set PA3 alternate function type to UART_RX AF(07)
	GPIOA->AFR[0] |=(1U<<12);
	GPIOA->AFR[0] |=(1U<<13);
	GPIOA->AFR[0] |=(1U<<14);
	GPIOA->AFR[0] &=~(1U<<15);
	//Configure USART MODULE
	//Enable clock access
	RCC->APB1ENR |=UART2EN;
	//configure uart baudrate
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);

	//configure the transfer direction
	USART2->CR1 =(CR1_TE | CR1_RE);
	USART2->CR1 |=CR1_UE;
	//enable uart module
}

char uart2_read(void){

	//make sure received data is not empty (Status Register)
	while(!(USART2->SR & SR_RXNE)){

		}
	//return data
	return USART2->DR;

}


void uart2_write(int ch){

	//make sure transmit data register is empty
	while(!(USART2->SR & SR_TXE)){

	}

	//write to transmit data register

	USART2->DR = (ch&0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t periphclk, uint32_t BaudRate ){
	USARTx->BRR = compute_uart_bd(periphclk,BaudRate);
}

static uint16_t compute_uart_bd(uint32_t periphclk, uint32_t BaudRate){

	return ((periphclk + (BaudRate/2U))/BaudRate);

}
