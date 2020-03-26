#include <stdint.h>
#include "xfifo.h"
#include "xfifo_defs.h"

#define INCREMENT(X)	X = ++X % XFIFO_QUEUE_SIZE	

static uint16_t head[XFIFO_QUEUES_COUNT] = { 0 };						//Начало очереди
static uint16_t tail[XFIFO_QUEUES_COUNT] = { 0 };						//Конец очереди
static uint16_t queFree[XFIFO_QUEUES_COUNT] = { 0 };					//Кол-во свободных ячеек
static uint8_t que[XFIFO_QUEUES_COUNT][XFIFO_QUEUE_SIZE];				//Массив очередей

QUE_ERR xfifo_init(int fifo_num)
{
	if ((fifo_num >= XFIFO_QUEUES_COUNT) || (fifo_num < 0))
	{
		return QUE_ERR_FIFO_NUM;
	}
	head[fifo_num] = 0;
	tail[fifo_num] = 0;
	queFree[fifo_num] = XFIFO_QUEUE_SIZE;
	return QUE_ERR_NO;
}

QUE_ERR xfifo_put(int fifo_num, uint8_t* buff, uint8_t dataSize)
{
	//Проверка на не нулевой указатель
	if (buff == NULL)
	{
		return QUE_ERR_NULLPTR;
	}
	//Проверка на валидность номера очереди
	if ((fifo_num > XFIFO_QUEUES_COUNT) || (fifo_num < 0))
	{
		return QUE_ERR_FIFO_NUM;
	}
	//Проверка на нулевой размер записываемых данных
	if (dataSize == 0)
	{
		return QUE_ERR_NODATA;
	}
	//Проверка, не выходит ли размер записываемых данных за размер свободных ячеек в очереди
	if ((dataSize + 1) > queFree[fifo_num])
	{
		return QUE_ERR_NOMEMORY;
	}
	
	//Запись в очередь размера сообщения
	que[fifo_num][tail[fifo_num]] = dataSize;
	INCREMENT(tail[fifo_num]);

	//Запись в очередь сообщения
	for (int i = 0; i < dataSize; i++)
	{
		que[fifo_num][tail[fifo_num]] = buff[i];
		INCREMENT(tail[fifo_num]);
	}

	queFree[fifo_num] -= (dataSize + 1);		//Декремент кол-ва свободных ячеек
	return QUE_ERR_NO;
}

QUE_ERR xfifo_get(int fifo_num, uint8_t* buff, uint8_t buffSize, uint8_t* dataSize)
{
	//Проверка на не нулевой указатель
	if(buff == NULL || dataSize == NULL)
	{
		return QUE_ERR_NULLPTR;
	}
	//Проверка на наличие данных в очереди
	if (queFree[fifo_num] == XFIFO_QUEUE_SIZE)
	{
		return QUE_ERR_EMPTY;					//Данные в очереди отсутствуют
	}
	//Проверка на валидность номера очереди
	if ((fifo_num > XFIFO_QUEUES_COUNT) || (fifo_num < 0))
	{
		return QUE_ERR_FIFO_NUM;
	}
	//Размер извлекаемых данных превышает размер буфера
	*dataSize = que[fifo_num][head[fifo_num]];
	if (*dataSize > buffSize)
	{
		return 	QUE_ERR_LOBUFF_SIZE;			//Размер сообщения превышает размер буфера для записи
	}
	INCREMENT(head[fifo_num]);
	//Извлечение сообщения из очереди в буфер
	for (int i = 0; i < *dataSize; i++)
	{
		buff[i] = que[fifo_num][head[fifo_num]];
		INCREMENT(head[fifo_num]);
	}
	//Инкремент кол-ва свободных ячеек
	queFree[fifo_num] += (*dataSize + 1);
	return QUE_ERR_NO;
}