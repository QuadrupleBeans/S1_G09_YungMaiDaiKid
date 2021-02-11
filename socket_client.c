#include "stm32l1xx.h"
/*Library related header files*/
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_tim.h"
#include "stm32l1xx_ll_usart.h"
#include <string.h>
#include "ESP8266_lowlevel_conf.h"

#define MAX_RESP_BUFFER_SIZE			200

uint8_t resp[MAX_RESP_BUFFER_SIZE] = {0};
uint8_t idx;


uint8_t ESP8266_SendCmd(uint8_t* cmd)
{
	ESP_USART_LOWLEVEL_Transmit(cmd);
	while(1)
	{
		
		(ESP_USART_LOWLEVEL_Recv(resp, idx) != 1)?(idx = (idx + 1) % MAX_RESP_BUFFER_SIZE):(idx);		
		if(strstr((const char*)resp, "OK"))
		{
			return 0;
		}
	}
}

void ESP_ServerStart()
{
	while(1)
	{
		
		(ESP_USART_LOWLEVEL_Recv(resp, idx) != 1)?(idx = (idx + 1) % MAX_RESP_BUFFER_SIZE):(idx);		
		if(strstr((const char*)resp, "CONNECT"))
		{
			return;
		}
	}
}
void ESP8266_RespBufferReset(void)
{
	//watch resp
	memset(resp, NULL, MAX_RESP_BUFFER_SIZE);//debug here
	idx = 0;
}

//TEST
uint8_t state = 0;
void SystemClock_Config(void);

int main()
{
	SystemClock_Config();
	ESP_USART_LOWLEVEL_Conf();
	ESP_USART_Start();
	
	ESP8266_SendCmd((uint8_t*)"AT\r\n");
	ESP8266_RespBufferReset();
	ESP8266_SendCmd((uint8_t*)"ATE0\r\n");
	ESP8266_RespBufferReset();
	//ESP8266_SendCmd((uint8_t*)"AT+RST\r\n");
	//ESP8266_RespBufferReset();	
	//ESP8266_SendCmd((uint8_t*)"AT+RESTORE\r\n");
	//ESP8266_RespBufferReset();
	LL_mDelay(1000); //Prevent ESP8266 flooding message
	ESP8266_SendCmd((uint8_t*)"AT+CWMODE=1\r\n");
	ESP8266_RespBufferReset();
	//ESP8266_SendCmd((uint8_t*)"AT+RST\r\n");
	//ESP8266_RespBufferReset();	
	LL_mDelay(1000);
	ESP8266_SendCmd((uint8_t*)"AT+CWJAP=\"TNI_CERT_LAB\",\"tnieng406\"\r\n");
	ESP8266_RespBufferReset();	
	ESP8266_SendCmd((uint8_t*)"AT+CWJAP?\r\n");
	ESP8266_RespBufferReset();	
	//LL_mDelay(1000);
	
/*----------------CLIENT------------------------------------------------------*/
	ESP8266_SendCmd((uint8_t*)"AT+CIPMUX=0\r\n");
	ESP8266_RespBufferReset();
	ESP8266_SendCmd((uint8_t*)"AT+CIPSTART=\"TCP\",\"169.254.148.106\",1102\r\n");
	ESP8266_RespBufferReset();
	
	ESP8266_SendCmd((uint8_t*)"AT+CIPSEND=6\r\n");
	ESP8266_RespBufferReset();
	
	ESP_USART_LOWLEVEL_Transmit((uint8_t *)"1");
	
	while(1)
	{
		//Determine what to do with server by using 2 following function
		//use (ESP_USART_LOWLEVEL_Recv(resp, idx) != 1)?(idx = (idx + 1) % MAX_RESP_BUFFER_SIZE):(idx);	 to pop out response from esp8266 and store in 'resp' data
		//use ESP8266_RespBufferReset() to clear response buffer after dealing with current command
		//use strstr function to find substring in 'resp'
		(ESP_USART_LOWLEVEL_Recv(resp, idx) != 1)?(idx = (idx + 1) % MAX_RESP_BUFFER_SIZE):(idx);	
		
	}
}


void SystemClock_Config(void)
{
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