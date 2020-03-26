#include <stdint.h>
#include "xfifo.h"
#include "xfifo_defs.h"

#define INCREMENT(X)	X = ++X % XFIFO_QUEUE_SIZE	

static uint16_t head[XFIFO_QUEUES_COUNT] = { 0 };						//������ �������
static uint16_t tail[XFIFO_QUEUES_COUNT] = { 0 };						//����� �������
static uint16_t queFree[XFIFO_QUEUES_COUNT] = { 0 };					//���-�� ��������� �����
static uint8_t que[XFIFO_QUEUES_COUNT][XFIFO_QUEUE_SIZE];				//������ ��������

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
	//�������� �� �� ������� ���������
	if (buff == NULL)
	{
		return QUE_ERR_NULLPTR;
	}
	//�������� �� ���������� ������ �������
	if ((fifo_num > XFIFO_QUEUES_COUNT) || (fifo_num < 0))
	{
		return QUE_ERR_FIFO_NUM;
	}
	//�������� �� ������� ������ ������������ ������
	if (dataSize == 0)
	{
		return QUE_ERR_NODATA;
	}
	//��������, �� ������� �� ������ ������������ ������ �� ������ ��������� ����� � �������
	if ((dataSize + 1) > queFree[fifo_num])
	{
		return QUE_ERR_NOMEMORY;
	}
	
	//������ � ������� ������� ���������
	que[fifo_num][tail[fifo_num]] = dataSize;
	INCREMENT(tail[fifo_num]);

	//������ � ������� ���������
	for (int i = 0; i < dataSize; i++)
	{
		que[fifo_num][tail[fifo_num]] = buff[i];
		INCREMENT(tail[fifo_num]);
	}

	queFree[fifo_num] -= (dataSize + 1);		//��������� ���-�� ��������� �����
	return QUE_ERR_NO;
}

QUE_ERR xfifo_get(int fifo_num, uint8_t* buff, uint8_t buffSize, uint8_t* dataSize)
{
	//�������� �� �� ������� ���������
	if(buff == NULL || dataSize == NULL)
	{
		return QUE_ERR_NULLPTR;
	}
	//�������� �� ������� ������ � �������
	if (queFree[fifo_num] == XFIFO_QUEUE_SIZE)
	{
		return QUE_ERR_EMPTY;					//������ � ������� �����������
	}
	//�������� �� ���������� ������ �������
	if ((fifo_num > XFIFO_QUEUES_COUNT) || (fifo_num < 0))
	{
		return QUE_ERR_FIFO_NUM;
	}
	//������ ����������� ������ ��������� ������ ������
	*dataSize = que[fifo_num][head[fifo_num]];
	if (*dataSize > buffSize)
	{
		return 	QUE_ERR_LOBUFF_SIZE;			//������ ��������� ��������� ������ ������ ��� ������
	}
	INCREMENT(head[fifo_num]);
	//���������� ��������� �� ������� � �����
	for (int i = 0; i < *dataSize; i++)
	{
		buff[i] = que[fifo_num][head[fifo_num]];
		INCREMENT(head[fifo_num]);
	}
	//��������� ���-�� ��������� �����
	queFree[fifo_num] += (*dataSize + 1);
	return QUE_ERR_NO;
}