#ifndef _XFIFOH_   
#define _XFIFOH_   

#include <stdint.h>   
#include "xfifo_defs.h"   

typedef enum
{
	QUE_ERR_NO,				//��������� �������
	QUE_ERR_FIFO_NUM,		//�������� ����� �������
	QUE_ERR_EMPTY,			//������� �����
	QUE_ERR_NODATA,			//������� ������ ������������ ������ � �������
	QUE_ERR_NOMEMORY,		//������ ������������ ������ ��������� ������ ���������� ����� � �������
	QUE_ERR_LOBUFF_SIZE,	//������ ����������� ������ ��������� ������ ������ ������������
	QUE_ERR_NULLPTR,		//������� ����������������������� ���������
}QUE_ERR;

/**
! ������������� �������
+ 0 ���� ��� ��, ����� �� 0
*/
int xfifo_init(int fifo_num);

/**
! �������� ������ � �������
+ 0 ���� ��� ��, ����� �� 0
*/
int xfifo_put(int fifo_num, uint8_t * buff, uint8_t dataSize);

/**
! ������� ������ �� �������
+ 0 ���� ��� ��, ����� �� 0
*/
int xfifo_get(int fifo_num, uint8_t * buff, uint8_t buffSize, uint8_t * dataSize);

#endif  // _XFIFOH_   