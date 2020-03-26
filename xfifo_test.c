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

static uint8_t test[XFIFO_QUEUE_SIZE * 2];		//Массив для записи в очередь
static uint8_t read[XFIFO_QUEUE_SIZE * 2];		//Массив для чтения из очереди
static int testSize = XFIFO_QUEUE_SIZE * 2;
static int readSize = XFIFO_QUEUE_SIZE * 2;

//Заполнение массива случайными числами
static int arrFill(uint8_t* arr, int arrSize);
//Вывод на экран сообщений QUE_ERR
static void printErr(QUE_ERR err);

int xfifo_test_init(void)
{
	arrFill(test, testSize);	//Заполнение массива случайными числами
	return 0;
}


int xfifo_test_run(void)
{
	srand((unsigned int)time(NULL));
	int test_num = 1;									//Номер теста
	QUE_ERR	err = QUE_ERR_NO;							//Ошибка работы очереди

	uint8_t msgSizeW = 255;								//Размер записанного сообщения в байтах
	uint8_t msgSizeR = 0;								//Размер полученного сообщения в байтах
	int msgCountW = 0;									//Кол-во записанных сообщений
	int msgCountR = 0;									//Кол-во прочитанных сообщений
	
	printf("Параметры очереди : \n");
	printf("%d - кол-во очередей, \n", XFIFO_QUEUES_COUNT);
	printf("%d - размер памяти (байт) выделенный под одну очередь\n\n", XFIFO_QUEUE_SIZE);
	
	//********************* xfifo_init *****************************
	printf("============ xfifo_init ============\n");
	//инициализация отрицательной очереди
		printf("Тест_%d - Инициализация -1 очереди: ", test_num++);
		err = xfifo_init(-1);
		if (err == QUE_ERR_FIFO_NUM)
		{
			printf("ОК\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//инициализация не существующей очереди
		printf("Тест_%d - Инициализация не существующей очереди: ", test_num++);
		err = xfifo_init(XFIFO_QUEUES_COUNT);
		if (err == QUE_ERR_FIFO_NUM)
		{
			printf("ОК\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//инициализация существующей очереди
		printf("Тест_%d - Инициализация существующей очереди: ", test_num++);
		err = xfifo_init(0);
		if (err == QUE_ERR_NO)
		{
			printf("ОК\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	
		//********************* xfifo_get *****************************
		printf("\n============ xfifo_get ============\n");
	//Тест на не нулевой указатель
		printf("Тест_%d - На не нулевой указатель: ", test_num++);
		err = xfifo_get(0, (uint8_t*)0, MAX_SIZE_QUE, &msgSizeR);
		if (err == QUE_ERR_NULLPTR)
		{
			printf("ОК\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//Тест на валидность номера очереди
		printf("Тест_%d - На валидность номера очереди: ", test_num++);
		err = xfifo_get(-1, read, MAX_SIZE_QUE, &msgSizeR);
		if (err == QUE_ERR_FIFO_NUM)
		{
			printf("ОК\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//Тест на превышение размер буфера
		printf("Тест_%d - На превышение размер буфера: ", test_num++);
		if (xfifo_put(0, test, 11) != QUE_ERR_NO) return 1;
		err = xfifo_get(0, read, 10, &msgSizeR);
		if (err == QUE_ERR_LOBUFF_SIZE)
		{
			printf("ОК\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//Тест на извлечение данных из пустой очереди
		printf("Тест_%d - Извлечение данных из пустой очереди: ", test_num++);
		//Очистка очереди перед данным тестом
		xfifo_init(0);
		err = xfifo_get(0, read, MAX_SIZE_QUE, &msgSizeR);
		if (err == QUE_ERR_EMPTY)
		{
			printf("ОК\n");
		}
		else if (err == QUE_ERR_NO)
		{
			printf("Ошибка - очередь не пуста!\n");
			return 1;
		}
		else
		{
			printErr(err);
			return 1;
		}
		
	//********************* xfifo_put *****************************
		printf("\n============ xfifo_put ============\n");
	//Тест на не нулевой указатель
		printf("Тест_%d - На не нулевой указатель: ", test_num++);
		err = xfifo_put(0, (uint8_t*)0, msgSizeW);
		if (err == QUE_ERR_NULLPTR)
		{
			printf("ОК\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//Тест на валидность номера очереди
		printf("Тест_%d - На валидность номера очереди: ", test_num++);
		err = xfifo_put(-1, test, msgSizeW);
		if (err == QUE_ERR_FIFO_NUM)
		{
			printf("ОК\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//Тест на нулевой размер записываемых данных
		printf("Тест_%d - На нулевой размер записываемых данных: ", test_num++);
		err = xfifo_put(0, test, 0);
		if (err == QUE_ERR_NODATA)
		{
			printf("ОК\n");
		}
		else
		{
			printErr(err);
			return 1;
		}
	//Тест на переполнение очереди
		printf("Тест_%d - На переполнение очереди: ", test_num++);
		//Запись данных в очередь до переполнения
		for (int i = 0; i < sizeof(test); i++)
		{
			err = xfifo_put(0, test + i, 255);
			if (err != QUE_ERR_NO) break;
		}
		if (err == QUE_ERR_NOMEMORY)
		{
			printf("ОК\n");
		}
		else
		{
			printErr(err);
			return 1;
		}

	//**************** Тест записи-чтения и сравнения данных *******************
		uint8_t* pt;
		int freeSize;		//Свободное место в массиве для чтения данных

		for (int que_no = 0; que_no < XFIFO_QUEUES_COUNT; que_no++)
		{
			printf("\n===== Тест записи-чтения данных очереди № %d =====\n", que_no);
			//Заполнение массива случайными числами
			arrFill(test, testSize);	

			//Очистка текущей очереди и переменных перед тестом
			xfifo_init(que_no);	
			pt = test;
			freeSize = readSize;
			msgSizeW = 0;
			msgSizeR = 0;
			msgCountW = 0;
			msgCountR = 0;

			//Заполнение очереди сообщениями различной длины данными из подготовленного масива
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
					printf("Записано: %d байт, %d сообщений\n", pt - test, msgCountW);
					break;
				}
				else
				{
					printf("Ошибка записи в очередь: ");
					printErr(err);
					return 1;
				}

			}
			
			pt = read;
			//Чтение записанных данных из очереди
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
					printf("Прочитано: %d байт, %d сообщений\n", pt - read, msgCountR);
					break;
				}
				else
				{
					printf("Ошибка чтения из очереди: ");
					printErr(err);
					return 1;
				}
			}
			//Сравнение данных
			printf("Сравнение данных : ");
			if (memcmp(test, read, pt - read) == 0)
			{
				printf("Данные идентичны\n");
			}
			else
			{
				printf("Ошибка! Данные не совпадают\n");
				return 1;
			}
		}
		return 0;
}

//Заполнение массива случайными числами
static int arrFill(uint8_t* arr, int arrSize)
{
	for (int i = 0; i < arrSize; i++)
	{
		arr[i] = rand();
	}
	return 0;
}

//Вывод на экран сообщений QUE_ERR
static void printErr(QUE_ERR err)
{
	switch (err)
	{
	case QUE_ERR_NO:
		printf("OK!");
		break;
	case QUE_ERR_EMPTY:
		printf("Очередь пуста");
		break;
	case QUE_ERR_NODATA:
		printf("Нулевой размер записываемых данных в очередь");
		break;
	case QUE_ERR_NOMEMORY:
		printf("Недостаточно памяти для записи данных");
		break;
	case QUE_ERR_LOBUFF_SIZE:
		printf("Размер буфера недостаточен");
		break;
	case QUE_ERR_FIFO_NUM:
		printf("Неверный номер очереди");
		break;
	case QUE_ERR_NULLPTR:
		printf("Передан нулевой указатель");
		break;
	default:
		printf("Неизвестная ошибка");
	}
	printf("\n");
}