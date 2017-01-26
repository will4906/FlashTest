/*********************************************************************************************************
* 文件名称: Timer3.c
* 文件作者: 苍海一粟
* 当前版本: 01.00.00
* 所属模块: Timer
* 创建日期: 2017年01月20日
* 文件摘要: 
* 注意事项: 
* 更改说明: 初始创建
**********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "Timer3.h"
#include "Uart.h"

/*********************************************************************************************************
*                                              静态函数定义
*********************************************************************************************************/
static void InitTimer3Mode( void );
static void InitTimer3Nvic( void );

/*********************************************************************************************************
* 函数名称: InitTimer3Con
* 函数功能: 
* 创建日期: 2017年01月20日
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 注意事项: 
**********************************************************************************************************/
void InitTimer3Con( void )
{
	InitTimer3Mode();
	InitTimer3Nvic();
}


/*********************************************************************************************************
* 函数名称: TIM3_IRQHandler
* 函数功能: 
* 创建日期: 2017年01月20日
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 注意事项: 
**********************************************************************************************************/
void TIM3_IRQHandler( void )
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		//请在下方添加代码
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update );
	}
}


/*********************************************************************************************************
* 函数名称: InitTimer3Mode
* 函数功能: 
* 创建日期: 2017年01月20日
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 注意事项: 
**********************************************************************************************************/
static void InitTimer3Mode( void )
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 99;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );
	TIM_Cmd(TIM3, ENABLE);
}


/*********************************************************************************************************
* 函数名称: InitTimer3Nvic
* 函数功能: 
* 创建日期: 2017年01月20日
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 注意事项: 
**********************************************************************************************************/
static void InitTimer3Nvic( void )
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

