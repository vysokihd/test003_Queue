#ifndef _XFIFOH_   
#define _XFIFOH_   

#include <stdint.h>   
#include "xfifo_defs.h"   

typedef enum
{
	QUE_ERR_NO,				//Завершено успешно
	QUE_ERR_FIFO_NUM,		//Неверный номер очереди
	QUE_ERR_EMPTY,			//Очередь пуста
	QUE_ERR_NODATA,			//Нулевой размер записываемых данных в очередь
	QUE_ERR_NOMEMORY,		//Размер записываемых данных превышает размер свободного места в очереди
	QUE_ERR_LOBUFF_SIZE,	//Размер извлекаемых данных превышает размер буфера пользователя
	QUE_ERR_NULLPTR,		//Передан непроинициализированный указатель
}QUE_ERR;

/**
! инициализация очереди
+ 0 если все ок, иначе не 0
*/
int xfifo_init(int fifo_num);

/**
! положить данные в очередь
+ 0 если все ок, иначе не 0
*/
int xfifo_put(int fifo_num, uint8_t * buff, uint8_t dataSize);

/**
! достать данные из очереди
+ 0 если все ок, иначе не 0
*/
int xfifo_get(int fifo_num, uint8_t * buff, uint8_t buffSize, uint8_t * dataSize);

#endif  // _XFIFOH_   