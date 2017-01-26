# stm32 flash读写例程

## 宏定义

* `USER_FLASH_START_ADDR`：

  用户可操作的flash起始地址，可能会根据不同的设备变换，请用户自行查询

* `USER_FLASH_END_ADDR`：

  用户可操作的flash最末地址，同上，用户自行查询

## 接口函数

* `u8 putWordData(u32 startAddr, u32 *pBuf, u32 len)`：

  向flash中写入32位数据类型的数据

* `u8 putHalfWordData(u32 startAddr, u16 *pBuf, u32 len)`：

  向flash中写入16位数据类型的数据

* `void getWordData(u32 startAddr, u32* pBuf, u32 len)`：

  从flash中读取32位数据类型的数据

* `void getHalfWordData(u32 startAddr, u16* pBuf, u32 len)`：

  从flash中读取16位数据类型的数据

## 注

flash读写无需使能驱动