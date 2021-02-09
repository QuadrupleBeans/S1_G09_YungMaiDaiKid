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

void SystemClock_Config(void); // config
void setBomb ();
void checkingIfAlive(uint8_t ,uint8_t);
void EXTI_Configure(); //?
void GPIO_Configure();
void Sink_Configure();
void _7Segment_Configure();
void rtc_config(); //will replace with something else later
void sinkLineToggle();
void player1PressedSound();
void player2PressedSound();

uint8_t arrayBomb[2][5];
uint8_t count = 0;
uint8_t i=10;
uint16_t j = 0;
uint8_t a=0, b=0;

uint8_t sw_state_player_1 = 0;
uint8_t sw_state_player_2 = 0;
uint8_t loser = 0;

bool isPlayer1Turn = true;
bool isPlayer2Turn = false;
bool isPlayer1Alive = true;
bool isPlayer2Alive = true;

uint32_t seg[10] = {LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14,
										LL_GPIO_PIN_10 | LL_GPIO_PIN_11,
										LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_15,
										LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_15,
										LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15,
										LL_GPIO_PIN_2 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15,
										LL_GPIO_PIN_2 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15,
										LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11,
										LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15,
										LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15,
										};
uint32_t digit[4] = {LL_GPIO_PIN_0 , LL_GPIO_PIN_1 , LL_GPIO_PIN_2 , LL_GPIO_PIN_3};
uint8_t recv_buffer[] ={0};
uint16_t w_index = 0;

int main(){	
	SystemClock_Config();
	rtc_config();
	GPIO_Configure();
	EXTI_Configure();
	Sink_Configure();
	setBomb();
	_7Segment_Configure();
	
		while(1){	
			
			for(i=10; i > 0;){	
				sinkLineToggle();
				
				LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
				LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | 
																	LL_GPIO_PIN_14 | LL_GPIO_PIN_15);
				LL_GPIO_SetOutputPin(GPIOC, digit[2]);	
				LL_GPIO_SetOutputPin(GPIOB, seg[i]);
				
				LL_mDelay(1);
				j++;
				
				if(j == 1000){
					j = 0;
					i--;
				}
				
				if(!isPlayer1Alive){
					break;
				}
			
			}							
	}
}


//Process after pressed button
//return type should be boolean to fucntion that will print out result???
void checkingIfAlive(uint8_t column, uint8_t row){
	
	if(arrayBomb[row][column] == 1){
		if(isPlayer1Turn){
			isPlayer1Alive = false;
			i = -1;
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
			player1PressedSound(); //play sound when successfully pressed
			isPlayer2Turn = true;
			isPlayer1Turn = false;
		}
		else{
			player2PressedSound(); //play sound when successfully pressed
			isPlayer2Turn = false;
			isPlayer1Turn = true;
		}	
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

		EXTI->PR |= (1<<0);
	
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
	
		EXTI->PR |= (1<<0);
	
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
	
	EXTI->PR |= (1<<0);
	
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

//This function used to set 1 bomb into arrayBomb
void setBomb(){
		memset(arrayBomb,0,10); //clear arrayBomb to 0 											
		
		//TO BE FIXED
		srand(LL_RTC_TIME_GetSecond(RTC));		
				
			a = rand()%2;
			b = rand()%5;
	
			if(arrayBomb[a][b] == 0){
					arrayBomb[a][b] = 1;
			} 
			
}


void soundLoser(){
	//TODO: print loser music
}

void soundWinner(){
	//TODO: print winner music
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

// rtc configure for randoming, will replace with something else later
void rtc_config(){
	LL_RTC_InitTypeDef RTC_InitStruct;
	LL_RTC_TimeTypeDef RTC_TimeStruct;
	

	LL_RTC_DateTypeDef RTC_DateStruct;
	LL_RTC_AlarmTypeDef RTC_AlarmStruct;
	
	LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
	LL_RCC_EnableRTC();
	
	//test = LL_RCC_IsEnabledRTC();
	
	RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
	RTC_InitStruct.AsynchPrescaler = 127;
	RTC_InitStruct.SynchPrescaler = 255;
	
	LL_RTC_Init(RTC, &RTC_InitStruct);

	RTC_TimeStruct.TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
	RTC_TimeStruct.Hours      = 0U;
  RTC_TimeStruct.Minutes    = 0U;
  RTC_TimeStruct.Seconds    = 0U;
	
	LL_RTC_TIME_StructInit(&RTC_TimeStruct);
	
	RTC_DateStruct.WeekDay = LL_RTC_WEEKDAY_MONDAY;
  RTC_DateStruct.Day     = 1U;
  RTC_DateStruct.Month   = LL_RTC_MONTH_JANUARY;
  RTC_DateStruct.Year    = 0U;
	
	LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_DateStruct);
	
	RTC_AlarmStruct.AlarmTime.Hours = 0;
	RTC_AlarmStruct.AlarmTime.Minutes = 0;
	RTC_AlarmStruct.AlarmTime.Seconds = 10;
	RTC_AlarmStruct.AlarmDateWeekDaySel = LL_RTC_ALMA_DATEWEEKDAYSEL_DATE;
	RTC_AlarmStruct.AlarmDateWeekDay = 1;
	
	LL_RTC_ALMA_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_AlarmStruct);
	
	NVIC_SetPriority(RTC_Alarm_IRQn, 0);
	NVIC_EnableIRQ(RTC_Alarm_IRQn);
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

void player1PressedSound(){
	//TODO: play some sound via speaker?
}

void player2PressedSound(){
	//TODO: play some sound via speaker?
}

void RNG_Config(){
	
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


