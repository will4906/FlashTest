#include "flash.h"
#include "uart.h"
#include "UartCom2.h"

static u8 checkTargetAddr(u32 startAddr, u32 len);
static u8 computeForEraseCnt(u32 startAddr, u32 len, u8 size);
static u8 putFlashData(u32 startAddr, void *pBuf, u32 len, u8 size);
static u8 putFlashHalfWord(u32 startAddr, u16 *pBuf, u32 len);
static u8 putFlashWord(u32 startAddr, u32 *pBuf, u32 len);
/***************************************************************************************************
* 函数名：向flash写入32位数据类型的数据
* 参数：  起始地址，数据，数据长度
* 返回值：是否写入成功，0：失败，1：成功
* 注：	 1、起始地址必须大于USER_FLASH_START_ADDR，小于USER_FLASH_END_ADDR，而且不能为奇数
*		 2、Flash写入数据时会把同一扇区的启动东西清空，所以尽量同时写入或者写进其他扇区
****************************************************************************************************/
u8 putWordData(u32 startAddr, u32 *pBuf, u32 len)				
{
	return putFlashData(startAddr, pBuf, len, 4);
}

/***************************************************************************************************
* 函数名：向flash写入16位数据类型的数据
* 参数：  起始地址，数据，数据长度
* 返回值：是否写入成功，0：失败，1：成功
* 注：	 1、起始地址必须大于USER_FLASH_START_ADDR，小于USER_FLASH_END_ADDR，而且不能为奇数
*		 2、Flash写入数据时会把同一扇区的启动东西清空，所以尽量同时写入或者写进其他扇区
****************************************************************************************************/
u8 putHalfWordData(u32 startAddr, u16 *pBuf, u32 len)
{
	return putFlashData(startAddr, pBuf, len, 2);
}

/***************************************************************************************************
* 函数名：从flash中读取数据类型为32位的数据
* 参数：  起始地址，数据指针，数据长度
* 返回值：无
****************************************************************************************************/
void getWordData(u32 startAddr, u32* pBuf, u32 len)
{
	u32 i;
	for (i = 0; i < len; i++)
	{
		pBuf[i] = *((u32*)startAddr + i);
	}
}

/***************************************************************************************************
* 函数名：从flash中读取数据类型为16位的数据
* 参数：  起始地址，数据指针，数据长度
* 返回值：无
****************************************************************************************************/
void getHalfWordData(u32 startAddr, u16* pBuf, u32 len)
{
	u32 i;
	for (i = 0; i < len; i++)
	{
		pBuf[i] = *((u16*)startAddr + i);
	}
}

/***************************************************************************************************
* 函数名：向flash中写入指定类型的数据
* 参数：  起始地址，数据指针，数据长度，数据类型
* 返回值：是否写入成功
****************************************************************************************************/
static u8 putFlashData(u32 startAddr, void *pBuf, u32 len, u8 size)
{
	u32 i;
	u8 eraseCnt;
	
	if (!checkTargetAddr(startAddr, len))
	{
		return 0;
	}
	eraseCnt = computeForEraseCnt(startAddr, len, size);
	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_BSY | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

	for (i = 0; i < eraseCnt; i++)
	{
		if(FLASH_ErasePage(startAddr + FLASH_PAGE_SIZE * i) != FLASH_COMPLETE)	//对齐擦除，即使不是首地址也会转成对应的首地址进行擦除
		{
			FLASH_Lock();
			return 0;
		}
	}

	switch(size)
	{
		case 2:
			if (!putFlashHalfWord(startAddr, pBuf, len))
			{
				return 0;
			}
			break;
		case 4:
			if (!putFlashWord(startAddr, pBuf, len))
			{
				return 0;
			}
			break;
		default:
			break;
	}

	FLASH_Lock();
	
	return 1;
}

static u8 putFlashHalfWord(u32 startAddr, u16 *pBuf, u32 len)
{
	int i;
	for (i = 0; i < len; i ++)
	{
		if (FLASH_ProgramHalfWord(startAddr + i * 2, pBuf[i]) != FLASH_COMPLETE)
		{
			FLASH_Lock();
			return 0;
		}
	}
	return 1;
}

static u8 putFlashWord(u32 startAddr, u32 *pBuf, u32 len)
{
	int i;
	for (i = 0; i < len; i ++)
	{
		if (FLASH_ProgramWord(startAddr + i * 4, pBuf[i]) != FLASH_COMPLETE)
		{
			FLASH_Lock();
			return 0;
		}
	}
	return 1;
}
/***************************************************************************************************
* 函数：  判断目标写入地址是否符合规则
* 参数：  目标地址，数据长度
* 返回值：返回是否符合规则，1：符合，0：不符合
****************************************************************************************************/
static u8 checkTargetAddr(u32 startAddr, u32 len)
{
	if (len <= 0)
	{
		return 0;
	}
	if (startAddr < SYSTEM_FLASH_START_ADDR || startAddr >= USER_FLASH_END_ADDR)		//防止写入的地址不在范围内
	{
		return 0;
	}
	
	if (startAddr + len >= USER_FLASH_END_ADDR)
	{
		return 0;
	}
	if (startAddr % 2 == 1)		//防止写入的地址为奇数
	{
		return 0;
	}
	return 1;
}

/***************************************************************************************************
* 函数：  计算要擦除的页数
* 参数：  目标起始地址，数据长度
* 返回值：返回应该擦除的页数
****************************************************************************************************/
static u8 computeForEraseCnt(u32 startAddr, u32 len, u8 size)
{
	u32 end;
	u8 eraseCnt;
	end = startAddr + len * size;

	if ((end - SYSTEM_FLASH_START_ADDR) / FLASH_PAGE_SIZE != (startAddr - SYSTEM_FLASH_START_ADDR) / FLASH_PAGE_SIZE)
	{
		eraseCnt = (end - startAddr) / FLASH_PAGE_SIZE + 2;
	}
	else
	{
		eraseCnt = (end - startAddr) / FLASH_PAGE_SIZE + 1;
	}
	return eraseCnt;
}





