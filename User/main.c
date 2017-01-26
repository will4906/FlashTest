#include "stm32f10x.h"
#include "delay.h"
#include "TotalTimer.h"
#include "led.h"
#include "Uart.h"
#include "flash.h"
#include <stdio.h>
#include <string.h>
#include "UartCom2.h"


int main()
{
	u8 data[50];
	u16 flashData[20];
	u16 readFlash[20];
	u8 sendData[20];
	int i = 20;
	
	delay_init();
	//定时器时间设置为10ms
	InitTimerCon();
	InitUartCon();
	LED_Init();
	for(i = 0; i < 20; i ++)
	{
		flashData[i] = 0x70 + i;
	}
	putHalfWordData(USER_FLASH_START_ADDR + 0xdc00 + 1024 - 84, flashData, 20);
	while(1)
	{
		EnableAllLed();
		delay_ms(1000);
		if(ReadUartData(UART_PORT_COM2,data))
		{
			getHalfWordData(USER_FLASH_START_ADDR + 0xdc00 + 1024 - 84, readFlash, 20);
			for(i = 0; i < 20; i ++)
			{
				sendData[i] = (u8)readFlash[i];
			}
			SendUartData(UART_PORT_COM2,sendData,20);
		}
		DisableAllLed();
		delay_ms(500);
	}
}
