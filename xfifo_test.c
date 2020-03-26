#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "xfifo.h"
#include "xfifo_test.h"
#include "xfifo_defs.h"

#define MAX_SIZE_QUE 255

static uint8_t test[XFIFO_QUEUE_SIZE * 2];		//������ ��� ������ � �������
static uint8_t read[XFIFO_QUEUE_SIZE * 2];		//������ ��� ������ �� �������
static int testSize = XFIFO_QUEUE_SIZE * 2;
static int readSize = XFIFO_QUEUE_SIZE * 2;

//���������� ������� ���������� �������
static int arrFill(uint8_t* arr, int arrSize);
//����� �� ����� ��������� QUE_ERR
static void printErr(QUE_ERR err);

int xfifo_test_init(void)
{
	arrFill(test, testSize);	//���������� ������� ���������� �������
	return 0;
}


int xfifo_test_run(void)
{
	srand((unsigned int)time(NULL));
	int test_num = 1;									//����� �����
	QUE_ERR	err = QUE_ERR_NO;							//������ ������ �������

	uint8_t msgSizeW = 255;								//������ ����������� ��������� � ������
	uint8_t msgSizeR = 0;								//������ ����������� ��������� � ������
	int msgCountW = 0;									//���-�� ���������� ���������
	int msgCountR = 0;									//���-�� ����������� ���������
	
	printf("��������� ������� : \n");
	printf("%d - ���-�� ��������, \n", XFIFO_QUEUES_COUNT);
	printf("%d - ������ ������ (����) ���������� ��� ���� �������\n\n", XFIFO_QUEUE_SIZE);
	
	//********************* xfifo_init *****************************
	printf("============ xfifo_init ============\n");
	//������������� ������������� �������
		printf("����_%d - ������������� -1 �������: ", test_num++);
		err = xfifo_init(-1);
		if (err == QUE_ERR_FIFO_NUM)
		{
			printf("��\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//������������� �� ������������ �������
		printf("����_%d - ������������� �� ������������ �������: ", test_num++);
		err = xfifo_init(XFIFO_QUEUES_COUNT);
		if (err == QUE_ERR_FIFO_NUM)
		{
			printf("��\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//������������� ������������ �������
		printf("����_%d - ������������� ������������ �������: ", test_num++);
		err = xfifo_init(0);
		if (err == QUE_ERR_NO)
		{
			printf("��\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	
		//********************* xfifo_get *****************************
		printf("\n============ xfifo_get ============\n");
	//���� �� �� ������� ���������
		printf("����_%d - �� �� ������� ���������: ", test_num++);
		err = xfifo_get(0, (uint8_t*)0, MAX_SIZE_QUE, &msgSizeR);
		if (err == QUE_ERR_NULLPTR)
		{
			printf("��\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//���� �� ���������� ������ �������
		printf("����_%d - �� ���������� ������ �������: ", test_num++);
		err = xfifo_get(-1, read, MAX_SIZE_QUE, &msgSizeR);
		if (err == QUE_ERR_FIFO_NUM)
		{
			printf("��\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//���� �� ���������� ������ ������
		printf("����_%d - �� ���������� ������ ������: ", test_num++);
		if (xfifo_put(0, test, 11) != QUE_ERR_NO) return 1;
		err = xfifo_get(0, read, 10, &msgSizeR);
		if (err == QUE_ERR_LOBUFF_SIZE)
		{
			printf("��\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//���� �� ���������� ������ �� ������ �������
		printf("����_%d - ���������� ������ �� ������ �������: ", test_num++);
		//������� ������� ����� ������ ������
		xfifo_init(0);
		err = xfifo_get(0, read, MAX_SIZE_QUE, &msgSizeR);
		if (err == QUE_ERR_EMPTY)
		{
			printf("��\n");
		}
		else if (err == QUE_ERR_NO)
		{
			printf("������ - ������� �� �����!\n");
			return 1;
		}
		else
		{
			printErr(err);
			return 1;
		}
		
	//********************* xfifo_put *****************************
		printf("\n============ xfifo_put ============\n");
	//���� �� �� ������� ���������
		printf("����_%d - �� �� ������� ���������: ", test_num++);
		err = xfifo_put(0, (uint8_t*)0, msgSizeW);
		if (err == QUE_ERR_NULLPTR)
		{
			printf("��\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//���� �� ���������� ������ �������
		printf("����_%d - �� ���������� ������ �������: ", test_num++);
		err = xfifo_put(-1, test, msgSizeW);
		if (err == QUE_ERR_FIFO_NUM)
		{
			printf("��\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//���� �� ������� ������ ������������ ������
		printf("����_%d - �� ������� ������ ������������ ������: ", test_num++);
		err = xfifo_put(0, test, 0);
		if (err == QUE_ERR_NODATA)
		{
			printf("��\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//���� �� ������������ �������
		printf("����_%d - �� ������������ �������: ", test_num++);
		//������ ������ � ������� �� ������������
		for (int i = 0; i < sizeof(test); i++)
		{
			err = xfifo_put(0, test + i, 255);
			if (err != QUE_ERR_NO) break;
		}
		if (err == QUE_ERR_NOMEMORY)
		{
			printf("��\n");
		}
		else
		{
			printErr(err);
			return 1;
		}

	//**************** ���� ������-������ � ��������� ������ *******************
		uint8_t* pt;
		int freeSize;		//��������� ����� � ������� ��� ������ ������

		for (int que_no = 0; que_no < XFIFO_QUEUES_COUNT; que_no++)
		{
			printf("\n===== ���� ������-������ ������ ������� � %d =====\n", que_no);
			//���������� ������� ���������� �������
			arrFill(test, testSize);	

			//������� ������� ������� � ���������� ����� ������
			xfifo_init(que_no);	
			pt = test;
			freeSize = readSize;
			msgSizeW = 0;
			msgSizeR = 0;
			msgCountW = 0;
			msgCountR = 0;

			//���������� ������� ����������� ��������� ����� ������� �� ��������������� ������
			while (true)
			{
				msgSizeW = rand() % 255 + 1;
				err = xfifo_put(que_no, pt, msgSizeW);
				if (err == QUE_ERR_NO)
				{
					pt += msgSizeW;
					msgCountW++;
				}
				else if (err == QUE_ERR_NOMEMORY)
				{
					printf("��������: %d ����, %d ���������\n", pt - test, msgCountW);
					break;
				}
				else
				{
					printf("������ ������ � �������: ");
					printErr(err);
					return 1;
				}

			}
			
			pt = read;
			//������ ���������� ������ �� �������
			while (true)
			{
				err = xfifo_get(que_no, pt, (freeSize > 255 ? 255 : freeSize), &msgSizeR);
				if (err == QUE_ERR_NO)
				{
					pt += msgSizeR;
					freeSize -= msgSizeR;
					msgCountR++;
				}
				else if (err == QUE_ERR_EMPTY)
				{
					printf("���������: %d ����, %d ���������\n", pt - read, msgCountR);
					break;
				}
				else
				{
					printf("������ ������ �� �������: ");
					printErr(err);
					return 1;
				}
			}
			//��������� ������
			printf("��������� ������ : ");
			if (memcmp(test, read, pt - read) == 0)
			{
				printf("������ ���������\n");
			}
			else
			{
				printf("������! ������ �� ���������\n");
				return 1;
			}
		}
		return 0;
}

//���������� ������� ���������� �������
static int arrFill(uint8_t* arr, int arrSize)
{
	for (int i = 0; i < arrSize; i++)
	{
		arr[i] = rand();
	}
	return 0;
}

//����� �� ����� ��������� QUE_ERR
static void printErr(QUE_ERR err)
{
	switch (err)
	{
	case QUE_ERR_NO:
		printf("OK!");
		break;
	case QUE_ERR_EMPTY:
		printf("������� �����");
		break;
	case QUE_ERR_NODATA:
		printf("������� ������ ������������ ������ � �������");
		break;
	case QUE_ERR_NOMEMORY:
		printf("������������ ������ ��� ������ ������");
		break;
	case QUE_ERR_LOBUFF_SIZE:
		printf("������ ������ ������������");
		break;
	case QUE_ERR_FIFO_NUM:
		printf("�������� ����� �������");
		break;
	case QUE_ERR_NULLPTR:
		printf("������� ������� ���������");
		break;
	default:
		printf("����������� ������");
	}
	printf("\n");
}