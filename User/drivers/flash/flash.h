#ifndef _FLASH_H_
#define _FLASH_H_

#include "stm32f10x.h"

#define SYSTEM_FLASH_START_ADDR			0x08000000
#define SYSTEM_FLASH_END_ADDR			0x0807FFFF
//用户可使用的flash地址范围
#define	USER_FLASH_START_ADDR			0x08032000								//经试验只能从这开始(200k)
#define USER_FLASH_END_ADDR				USER_FLASH_START_ADDR + 0xdfc4			//经试验不能超过这个地址(255.941k，估计是误差，所以尽量不要到这么深)

#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
    #define FLASH_PAGE_SIZE    ((u16)0x0800)				//2k
#else
    #define FLASH_PAGE_SIZE    ((u16)0x0400)				//1k
#endif


u8 putWordData(u32 startAddr, u32 *pBuf, u32 len);
void getWordData(u32 startAddr, u32* pBuf, u32 len);

u8 putHalfWordData(u32 startAddr, u16 *pBuf, u32 len);
void getHalfWordData(u32 startAddr, u16* pBuf, u32 len);



#endif
