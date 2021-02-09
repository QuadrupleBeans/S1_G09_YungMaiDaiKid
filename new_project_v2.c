/*Base register address header file */
#include "stm32l1xx.h"

/*Library related header files */
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_usart.h"
#include "stm32l1xx_ll_exti.h"
#include "string.h"
#include "stm32l1xx_ll_lcd.h"
#include "stdio.h"
#include "stm32l1xx_ll_rtc.h"
#include "stdlib.h"
#include "stdbool.h"
#include "time.h"
#include "dwt_delay.h"
#include "time.h"
#include "stm32l1xx_ll_tim.h"
//7segment
#define Num_0	(uint32_t) LL_GPIO_PIN_2  | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 
#define Num_1	(uint32_t) LL_GPIO_PIN_10 | LL_GPIO_PIN_11 
#define Num_2	(uint32_t) LL_GPIO_PIN_2  | LL_GPIO_PIN_10 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_15 
#define Num_3	(uint32_t) LL_GPIO_PIN_2  | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_15 
#define Num_4	(uint32_t) LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15 
#define Num_5 (uint32_t) LL_GPIO_PIN_2 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12
#define Num_6	(uint32_t) LL_GPIO_PIN_2  | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15 
#define Num_7	(uint32_t) LL_GPIO_PIN_2  | LL_GPIO_PIN_10 | LL_GPIO_PIN_11  
#define Num_8	(uint32_t) LL_GPIO_PIN_2  | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15 
#define Num_9	(uint32_t) LL_GPIO_PIN_2  | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15
uint32_t seg0[4] = {Num_0,	Num_0, Num_0, Num_0};
uint32_t seg1[4] = {Num_0,	Num_0, Num_0, Num_1};	
uint32_t seg2[4] = {Num_0,	Num_0, Num_0, Num_2};
uint32_t seg3[4] = {Num_0,	Num_0, Num_0, Num_3};
uint32_t seg4[4] = {Num_0,	Num_0, Num_0, Num_4};
uint32_t seg5[4] = {Num_0,	Num_0, Num_0, Num_5};
uint32_t seg6[4] = {Num_0,	Num_0, Num_0, Num_6};
uint32_t seg7[4] = {Num_0,	Num_0, Num_0, Num_7};	
uint32_t seg8[4] = {Num_0,	Num_0, Num_0, Num_8};
uint32_t seg9[4] = {Num_0,	Num_0, Num_0, Num_9};
uint32_t seg10[4] = {Num_0,	Num_0, Num_1, Num_0};
uint32_t digit[4] = {LL_GPIO_PIN_0 , LL_GPIO_PIN_1 , LL_GPIO_PIN_2 , LL_GPIO_PIN_3};
//function
void SystemClock_Config(void); // config
void setBomb ();
void checkingIfAlive(uint8_t ,uint8_t);
void EXTI_Configure(); //?
void GPIO_Configure();
void Sink_Configure();
void _7Segment_Configure();
void sinkLineToggle();
void togglePlayerLight();
void TIMx_IC_Config();
void _7segment_progress();
bool checkGameEnded();
//variable
uint8_t arrayBomb[2][5];
uint8_t count = 0;
uint16_t counterval = 0;
uint8_t i=10;
uint16_t j = 0;
uint8_t a=0, b=0;
uint8_t c = 0;
uint8_t sw_state_player_1 = 0;
uint8_t sw_state_player_2 = 0;
uint8_t loser = 0;
//Player check
bool isPlayer1Turn = true;
bool isPlayer2Turn = false;
bool isPlayer1Alive = true;
bool isPlayer2Alive = true;
bool isGameEnded = false;
bool isBombSet = false;
//usart 
uint8_t recv_buffer[] ={0};
uint16_t w_index = 0;

int main(){	
	SystemClock_Config();

	GPIO_Configure();
	EXTI_Configure();
	Sink_Configure();
	TIMx_IC_Config();	
	_7Segment_Configure();
	
	while(!isGameEnded){		
		if(isBombSet){
			 togglePlayerLight();
			_7segment_progress();
			}			
	}
}


//Process after pressed button
//return type should be boolean to fucntion that will print out result???
void checkingIfAlive(uint8_t column, uint8_t row){
	
	if(arrayBomb[row][column] == 1){
		if(isPlayer1Turn){
			isPlayer1Alive = false;
		}
		else{
			isPlayer2Alive = false;
		}
		arrayBomb[row][column] = 2; //So that player can't pressed the same button and repeat loser process
	}
	else if (arrayBomb[row][column] == 2){
		//Do nothing, you pressed the same button lmao
	}
	else{ //Not a bomb
		arrayBomb[row][column] = 2;
		i = 10; //reset the 7 segment
		
		if(isPlayer1Turn){
			isPlayer2Turn = true;
			isPlayer1Turn = false;
		}
		else{
			isPlayer2Turn = false;
			isPlayer1Turn = true;
		}
			togglePlayerLight(); //Toggling light
	}
}

//setting EXTI LINE 0; PLAYER ONE's side button
void EXTI0_IRQHandler(void){
	
	EXTI->PR |= (1<<0);
		
		if(isPlayer1Turn&&isPlayer1Alive){
			
			if(((GPIOC->ODR & 1<<8)) == 0){ //PLAYER ONE pressed button 1 is pressed
					sw_state_player_1 = 1; 
					checkingIfAlive(0, 0);
			}
			else if (((GPIOC->ODR & 1<<9)) == 0){ //PLAYER ONE pressed button 2 is pressed
					sw_state_player_1 = 2;
					checkingIfAlive(0, 1);
			}
			else if (((GPIOC->ODR & 1<<10)) == 0){ //PLAYER ONE pressed button 3 is pressed
					sw_state_player_1 = 3;
					checkingIfAlive(0, 2);
			}
			else if (((GPIOC->ODR & 1<<11)) == 0){ //PLAYER ONE pressed button 4 is pressed
					sw_state_player_1 = 4;
					checkingIfAlive(0, 3);
			}
			else if (((GPIOC->ODR & 1<<12)) == 0){ //PLAYER ONE pressed button 5 is pressed
					sw_state_player_1 = 5;
					checkingIfAlive(0, 4);
			}
	}	
}

//setting EXTI LINE 1; PLAYER ONE's side button
void EXTI1_IRQHandler(void){

		EXTI->PR |= (1<<1);
	
		if(isPlayer1Turn&&isPlayer1Alive){

			if(((GPIOC->ODR & 1<<8)) == 0){ //PLAYER ONE pressed button 6 is pressed
					sw_state_player_1 = 6;
					checkingIfAlive(1, 0);
			}
			else if (((GPIOC->ODR & 1<<9)) == 0){ //PLAYER ONE pressed button 7 is pressed
					sw_state_player_1 = 7;
					checkingIfAlive(1, 1);
			}
			else if (((GPIOC->ODR & 1<<10)) == 0){ //PLAYER ONE pressed button 8 is pressed
					sw_state_player_1 = 8;
					checkingIfAlive(1, 2);
			}
			else if (((GPIOC->ODR & 1<<11)) == 0){ //PLAYER ONE pressed button 9 is pressed
					sw_state_player_1 = 9;
					checkingIfAlive(1, 3);
			}
			else if (((GPIOC->ODR & 1<<12)) == 0){ //PLAYER ONE pressed button 10 is pressed
					sw_state_player_1 = 10;
					checkingIfAlive(1, 4);
			}
	}	
}

//setting EXTI LINE 2; PLAYER TWO's side button
void EXTI2_IRQHandler(void){ 
	
		EXTI->PR |= (1<<2);
	
		if(isPlayer2Turn&&isPlayer2Alive){
				
			if(((GPIOC->ODR & 1<<8)) == 0){ //PLAYER TWO pressed button 1 
					sw_state_player_2 = 1; 
					checkingIfAlive(0, 0);
			}
			else if (((GPIOC->ODR & 1<<9)) == 0){ //PLAYER TWO pressed button 2
					sw_state_player_2 = 2;
					checkingIfAlive(0, 1);
			}
			else if (((GPIOC->ODR & 1<<10)) == 0){ //PLAYER TWO pressed button 3 
					sw_state_player_2 = 3;
					checkingIfAlive(0, 2);
			}
			else if (((GPIOC->ODR & 1<<11)) == 0){ //PLAYER TWO pressed button 4
					sw_state_player_2 = 4;
					checkingIfAlive(0, 3);
			}
			else if (((GPIOC->ODR & 1<<12)) == 0){ //PLAYER TWO pressed button 5 
					sw_state_player_2 = 5;
					checkingIfAlive(0, 4);
			}
		}	
}

//setting EXTI LINE 3; PLAYER TWO's side button
void EXTI3_IRQHandler(void){ 
	
	EXTI->PR |= (1<<3);
	
	if(isPlayer2Turn&&isPlayer2Alive){
			
			if(((GPIOC->ODR & 1<<8)) == 0){ //PLAYER TWO pressed button 6
					sw_state_player_2 = 6; 
					checkingIfAlive(1, 0);
			}
			else if (((GPIOC->ODR & 1<<9)) == 0){ //PLAYER TWO pressed button 7
					sw_state_player_2 = 7;	
					checkingIfAlive(1, 1);
			}
			else if (((GPIOC->ODR & 1<<10)) == 0){ ////PLAYER TWO pressed button 8 
					sw_state_player_2 = 8;
					checkingIfAlive(1, 2);
			}
			else if (((GPIOC->ODR & 1<<11)) == 0){ ////PLAYER TWO pressed button 9 
					sw_state_player_2 = 9;
					checkingIfAlive(1, 3);
			}
			else if (((GPIOC->ODR & 1<<12)) == 0){ ////PLAYER TWO pressed button 10 
					sw_state_player_2 = 10;
					checkingIfAlive(1, 4);
			}
	}	
}

void EXTI4_IRQHandler(void){ 
	EXTI->PR |= (1<<4);
	setBomb();
}

//This function used to set 1 bomb into arrayBomb
void setBomb(){
		memset(arrayBomb,0,10); //clear arrayBomb to 0 											
				
			a = LL_TIM_GetCounter(TIM2)%2;
			b = LL_TIM_GetCounter(TIM2)%5;
	
			if(arrayBomb[a][b] == 0){
					arrayBomb[a][b] = 1;
			}	
			
			isBombSet = true;		
}

void TIMx_IC_Config(void){
	LL_TIM_IC_InitTypeDef timic;
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	
	//TIM_IC Configure CH1
	timic.ICActiveInput		=	LL_TIM_ACTIVEINPUT_DIRECTTI;
	timic.ICFilter				=	LL_TIM_IC_FILTER_FDIV1_N2;
	timic.ICPolarity			=	LL_TIM_IC_POLARITY_BOTHEDGE;
	timic.ICPrescaler			=	LL_TIM_ICPSC_DIV1;
	LL_TIM_IC_Init(TIM2, LL_TIM_CHANNEL_CH1, &timic);
	
	NVIC_SetPriority(TIM2_IRQn, 0);
	
	NVIC_EnableIRQ(TIM2_IRQn);
	LL_TIM_EnableIT_CC1(TIM2);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
	
	LL_TIM_EnableCounter(TIM2);
}


//GPIO Configure
void GPIO_Configure(void){

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); //Enable GPIOA Clock
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB); //Enable GPIOB Clock
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC); //Enable GPIOC Clock
	
	//Setting input line button
	LL_GPIO_InitTypeDef GPIO_init;
	GPIO_init.Mode = LL_GPIO_MODE_INPUT;
	GPIO_init.Pull = LL_GPIO_PULL_NO;
	GPIO_init.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	
	//Player 1 use PIN 0 and PIN 1, Player 2 use PIN 2 and PIN 3
	GPIO_init.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
	LL_GPIO_Init(GPIOA,	&GPIO_init); //Initializing inputs
	
	//Setting 2 Green LEDs and 2 Red LEDs
	GPIO_init.Mode = LL_GPIO_MODE_OUTPUT;
	
	//Configure PB9 as Red of Player 1
	GPIO_init.Pin = LL_GPIO_PIN_9;
	LL_GPIO_Init(GPIOB,	&GPIO_init); //Initializing inputs
	
	//Configure PA8 as player 2's green and PA15 as player 1's green
	GPIO_init.Pin = LL_GPIO_PIN_8 | LL_GPIO_PIN_15;
	LL_GPIO_Init(GPIOA,	&GPIO_init); //Initializing inputs
	
	//Configure PC7 as Red of Player 2
	GPIO_init.Pin = LL_GPIO_PIN_7;
	LL_GPIO_Init(GPIOC, &GPIO_init);
	
	//Configure PB4 as Start button
	GPIO_init.Pin = LL_GPIO_PIN_4;
	GPIO_init.Mode = LL_GPIO_MODE_INPUT;
	LL_GPIO_Init(GPIOB, &GPIO_init);
}
//Sink Line Configure
void Sink_Configure(){

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC); //Enable GPIOC Clock
	LL_GPIO_InitTypeDef GPIO_init;
	
	//Setting sink line PC8, PC9, PC10, PC11, PC12
	GPIO_init.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_init.Pull = LL_GPIO_PULL_NO;
	GPIO_init.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_init.Pin = LL_GPIO_PIN_8 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12;
	
	LL_GPIO_Init(GPIOC,	&GPIO_init); //set GPIOC PIN 8, 9, 10, 11, 12 to SINK LINE
}
//7 Segment_Configure
void _7Segment_Configure(){
	//Setting 7 Segment
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB); //Enable GPIOB Clock
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC); //Enable GPIOC Clock
	LL_GPIO_InitTypeDef ltc4727_initstruct;
	
	ltc4727_initstruct.Mode 				= LL_GPIO_MODE_OUTPUT;
	ltc4727_initstruct.OutputType 	= LL_GPIO_OUTPUT_PUSHPULL;
	ltc4727_initstruct.Pull 				= LL_GPIO_PULL_NO;
	ltc4727_initstruct.Speed 				= LL_GPIO_SPEED_FREQ_VERY_HIGH;
	ltc4727_initstruct.Pin 					= LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | 
																		LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
	LL_GPIO_Init(GPIOB, &ltc4727_initstruct);
	
	ltc4727_initstruct.Pin 					= LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
	LL_GPIO_Init(GPIOC, &ltc4727_initstruct);
}

//External Interupt Configuration
void EXTI_Configure(){
	
	//Enable Clock Bus SYSTEMCONFIG
	RCC->APB2ENR |= (1<<0);
	
	//EXTICR0[0:15] - clear bit to reset state
	SYSCFG->EXTICR[0] &= ~(65535<<0);
	
	//EXTCR2[0:3] - clear bit to reset state
	SYSCFG->EXTICR[1] |= ~(15<<0);
	
	//Set EXTI LINE TO PA0, PA1, PA2, PA3, PA4
	SYSCFG->EXTICR[0] &= ~((15<<0)|(15<<4)|(15<<8)|(15<<12));
	SYSCFG->EXTICR[1] |= (1<<0);
	
	//Enable Interupt line to EXTI0, EXTI1, EXTI2, EXTI3, EXTI4
	EXTI->IMR |= ((1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4));
	
	//Enable Falling edge to EXTI0, EXTI1, EXTI2, EXTI3, EXTI4
	EXTI->FTSR |= (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4);
	
	//Enable IRQ 
	NVIC_EnableIRQ((IRQn_Type)6);
	NVIC_SetPriority((IRQn_Type)6,0);
	NVIC_EnableIRQ((IRQn_Type)7);
	NVIC_SetPriority((IRQn_Type)7,0);
	NVIC_EnableIRQ((IRQn_Type)8);
	NVIC_SetPriority((IRQn_Type)8,0);
	NVIC_EnableIRQ((IRQn_Type)9);
	NVIC_SetPriority((IRQn_Type)9,0);
	NVIC_EnableIRQ((IRQn_Type)10);
	NVIC_SetPriority((IRQn_Type)10,0);
}

//Show 7 segment 
void _7segment_progress(){
		for(i=10; i >= 0;){
				sinkLineToggle();
			 if (i ==0){
				 for(c=0;c<4;c++){
									LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
									LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15); 
									LL_GPIO_SetOutputPin(GPIOB, seg0[c]);
									LL_GPIO_SetOutputPin(GPIOC, digit[c]);
									LL_mDelay(1);}
			 }
			 else if(i ==1)
			 {
				 for(c=0;c<4;c++){
									LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
									LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15); 
									LL_GPIO_SetOutputPin(GPIOB, seg1[c]);
									LL_GPIO_SetOutputPin(GPIOC, digit[c]);
									LL_mDelay(1);	}
			 }
			 else if(i ==2)
			 {
				 for(c=0;c<4;c++){
									LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
									LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15); 
									LL_GPIO_SetOutputPin(GPIOB, seg2[c]);
									LL_GPIO_SetOutputPin(GPIOC, digit[c]);
									LL_mDelay(1);}
			 }
			 else if(i ==3)
			 {
				 for(c=0;c<4;c++){
									LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
									LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15); 
									LL_GPIO_SetOutputPin(GPIOB, seg3[c]);
									LL_GPIO_SetOutputPin(GPIOC, digit[c]);
									LL_mDelay(1);}
			 }
			 else if(i ==4)
			 {
				 for(c=0;c<4;c++){
									LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
									LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15); 
									LL_GPIO_SetOutputPin(GPIOB, seg4[c]);
									LL_GPIO_SetOutputPin(GPIOC, digit[c]);
									LL_mDelay(1);}
			 }
			 else if(i ==5)
			 {
				 for(c=0;c<4;c++){
									LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
									LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15); 
									LL_GPIO_SetOutputPin(GPIOB, seg5[c]);
									LL_GPIO_SetOutputPin(GPIOC, digit[c]);
									LL_mDelay(1);}
			 }
			 else if(i ==6)
			 {
				 for(c=0;c<4;c++){
									LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
									LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15); 
									LL_GPIO_SetOutputPin(GPIOB, seg6[c]);
									LL_GPIO_SetOutputPin(GPIOC, digit[c]);
									LL_mDelay(1);}
			 }
			 else if(i ==7)
			 {
				 for(c=0;c<4;c++){
									LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
									LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15); 
									LL_GPIO_SetOutputPin(GPIOB, seg7[c]);
									LL_GPIO_SetOutputPin(GPIOC, digit[c]);
									LL_mDelay(1);}
			 }
			 else if(i ==8)
			 {
				 for(c=0;c<4;c++){
									LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
									LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15); 
									LL_GPIO_SetOutputPin(GPIOB, seg8[c]);
									LL_GPIO_SetOutputPin(GPIOC, digit[c]);
									LL_mDelay(1);}
			 }
			 else if(i ==9)
			 {
				 for(c=0;c<4;c++){
									LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
									LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15); 
									LL_GPIO_SetOutputPin(GPIOB, seg9[c]);
									LL_GPIO_SetOutputPin(GPIOC, digit[c]);
									LL_mDelay(1);}
			 }
			 else if(i ==10)
			 {for(c=0;c<4;c++){
									LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
									LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15); 
									LL_GPIO_SetOutputPin(GPIOB, seg10[c]);
									LL_GPIO_SetOutputPin(GPIOC, digit[c]);
									LL_mDelay(1);}
			 }
			 
				
				//LL_mDelay(1);
				j++;
				
				if(j == 125){
					j = 0;
					i--;
				}
				isGameEnded = checkGameEnded();
			}
}	
//Check Game is ended
bool checkGameEnded(){
	if(i==0){
					if(isPlayer1Turn){
						//send data: player 1 lost
					}
					else{
						//send data: player 2 lost
					}
					return true; 
				}
					
				if(!isPlayer1Alive){
					//send data: Player 1 has lost
					return true; 
				}
				
				if(!isPlayer2Alive){
					//send data: Player 2 has lost
				  return true; 
				}
}
//Toggling sinking line between PC8, 9, 10, 11, 12
void sinkLineToggle(){
	
	//SINK LINE PC8
	GPIOC->ODR &= ~(1<<8);
	GPIOC->ODR |= (1<<9);
	GPIOC->ODR |= (1<<10);
	GPIOC->ODR |= (1<<11);
	GPIOC->ODR |= (1<<12);
	
	//SINK LINE PC9
	GPIOC->ODR |= (1<<8);
	GPIOC->ODR &= ~(1<<9);
	GPIOC->ODR |= (1<<10);
	GPIOC->ODR |= (1<<11);
	GPIOC->ODR |= (1<<12);
	
	//SINK LINE PC10
	GPIOC->ODR |= (1<<8);
	GPIOC->ODR |= (1<<9);
	GPIOC->ODR &= ~(1<<10);
	GPIOC->ODR |= (1<<11);
	GPIOC->ODR |= (1<<12);
	
	//SINK LINE PC11
	GPIOC->ODR |= (1<<8);
	GPIOC->ODR |= (1<<9);
	GPIOC->ODR |= (1<<10);
	GPIOC->ODR &= ~(1<<11);
	GPIOC->ODR |= (1<<12);
	
	//SINK LINE PC12
	GPIOC->ODR |= (1<<8);
	GPIOC->ODR |= (1<<9);
	GPIOC->ODR |= (1<<10);
	GPIOC->ODR |= (1<<11);
	GPIOC->ODR &= ~(1<<12);
}

void togglePlayerLight(){
	if(isPlayer1Turn){
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_15); //turns green on on player 1 side
		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9); //turns red off on player 1 side
		
		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8);//turns green off on player 2 side
		LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7 ); //turns red on on player 2 side	
	}
	else{
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8); //turns green on on player 2 side
		LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7); //turns red off on player 2 side
	
		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_15);//turns green off on player 1 side
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9); //turns red on on player 1 side
	}
}

void SystemClock_Config(void){
  /* Enable ACC64 access and set FLASH latency */ 
  LL_FLASH_Enable64bitAccess();; 
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  /* Set Voltage scale1 as MCU will run at 32MHz */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  
  /* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
  while (LL_PWR_IsActiveFlag_VOSF() != 0)
  {
  };
  
  /* Enable HSI if not already activated*/
  if (LL_RCC_HSI_IsReady() == 0)
  {
    /* HSI configuration and activation */
    LL_RCC_HSI_Enable();
    while(LL_RCC_HSI_IsReady() != 1)
    {
    };
  }
  
	
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 32MHz                             */
  /* This frequency can be calculated through LL RCC macro                          */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ (HSI_VALUE, LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3); */
  LL_Init1msTick(32000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(32000000);
}


