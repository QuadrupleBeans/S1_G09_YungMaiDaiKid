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

void SystemClock_Config(void); // config
void soundLoser();
void soundWinner();
void setBomb ();
void checkingIfAlive(uint8_t);
void EXTI_Configure(); //?
void GPIO_Configure();
void Sink_Configure();
void _7Segment_Configure();
void rtc_config(); //will replace with something else later

uint8_t arrayBomb[2][5];
uint8_t count = 0;
uint8_t i=0;
uint8_t a=0, b=0;
uint8_t sw_state = 0;
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

int main(){	
	SystemClock_Config();
	rtc_config();
	GPIO_Configure();
	EXTI_Configure();
	Sink_Configure();
	
	while(1){
		//SINK LINE PA11
  GPIOA->ODR &= ~(1<<11);
	GPIOA->ODR |= (1<<12);
	
	//SINK LINE PA12
	GPIOA->ODR |= (1<<11);
	GPIOA->ODR &= ~(1<<12);
		
	
	}
}


//Process after pressed button
//return type should be boolean to fucntion that will print out result
void checkingIfAlive(uint8_t state){
	
	//Printing which button is pressed onto 7-segment
	for(i=0; i < 4; ++i){
			LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
			LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | 
																		LL_GPIO_PIN_14 | LL_GPIO_PIN_15);
			LL_GPIO_SetOutputPin(GPIOB, seg[state-1]);
			LL_GPIO_SetOutputPin(GPIOC, digit[i]);
			
			LL_mDelay(1000);  // USE for DEBUG increase delay to see what's happenning when 7-seg is lit
		}

	/*
	if(arrayBomb[column][row] == 1){
		//TODO: Lose process.
		arrayBomb[column][row] = 2; //So that player can't pressed the same button and repeat loser process
		soundLoser();
	}
	else if (arrayBomb[column][row] == 2){
		//Do nothing, you pressed the same button
	}
	else{
		//TODO: Survive process.
	}
	*/
}
//setting EXTI LINE 0
void EXTI0_IRQHandler(void){
	EXTI->PR |= (1<<0);
	if(((GPIOA->ODR & 1<<11)) == 0){ //button 1 is pressed
				sw_state = 1; 
				//checkingIfAlive(1);
		}
	else if (((GPIOA->ODR & 1<<12)) == 0){ //button 6 is pressed
				sw_state = 6;
				//checkingIfAlive(6);
	  }
}

//setting EXTI LINE 1
void EXTI1_IRQHandler(void){
	EXTI->PR |= (1<<1);
	if(((GPIOA->ODR & 1<<11)) == 0){ //button 2 is pressed
			sw_state = 2;
			//checkingIfAlive(2);
	}
	else if (((GPIOA->ODR & 1<<12)) == 0){ //button 7 is pressed
			sw_state = 7;
			//checkingIfAlive(7);
	 }
}

//setting EXTI LINE 2
void EXTI2_IRQHandler(void){ 
	EXTI->PR |= (1<<2);
	if(((GPIOA->ODR & 1<<11)) == 0){ //button 3 is pressed
			sw_state = 3;
			//checkingIfAlive(3);
	}
	else if (((GPIOA->ODR & 1<<12)) == 0){ //button 8 is pressed
			sw_state = 8;
			//checkingIfAlive(8);
	  }
}

//setting EXTI LINE 3
void EXTI3_IRQHandler(void){ 
	EXTI->PR |= (1<<3);
	if(((GPIOA->ODR & 1<<11)) == 0){ //button 4 is pressed
			sw_state = 4;
			//checkingIfAlive(4);
		}
	else if (((GPIOA->ODR & 1<<12)) == 0){ //button 9 is pressed
			sw_state = 9;
			//checkingIfAlive(9);
	  }
}

//setting EXTI LINE 4
void EXTI4_IRQHandler(void){ 
	EXTI->PR |= (1<<4);
	if(((GPIOA->ODR & 1<<11)) == 0){ //button 5 is pressed
			sw_state = 5;
			//checkingIfAlive(5);
		}
	else if (((GPIOA->ODR & 1<<12)) == 0){ //button 10 is pressed
			sw_state = 10;
			//checkingIfAlive(10);
	  }
}

//This function used to set 1 bomb into arrayBomb
void setBomb(){
		memset(arrayBomb,0,10); //clear arrayBomb to 0 											
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
	
	//Setting input line button
	LL_GPIO_InitTypeDef GPIO_init;
	GPIO_init.Mode = LL_GPIO_MODE_INPUT;
	GPIO_init.Pull = LL_GPIO_PULL_NO;
	GPIO_init.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_init.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
	
	LL_GPIO_Init(GPIOA,	&GPIO_init); //set GPIOA PIN 0, 1, 2, 3 and 4 to input mode.
	
	GPIO_init.Pin = LL_GPIO_PIN_4;
	LL_GPIO_Init(GPIOB,	&GPIO_init);
	
	//TODO: configure more?
}
//Sink Line Configure
void Sink_Configure(){

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); //Enable GPIOA Clock
	LL_GPIO_InitTypeDef GPIO_init;
	
	//Setting sink line PA11, PA12
	GPIO_init.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_init.Pull = LL_GPIO_PULL_NO;
	GPIO_init.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_init.Pin = LL_GPIO_PIN_11 | LL_GPIO_PIN_12;
	
	LL_GPIO_Init(GPIOA,	&GPIO_init); //set GPIOA PIN 11, 12 to output mode.

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

