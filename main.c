#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <locale.h>
#include "xfifo.h"
#include "xfifo_test.h"

int main()
{
	setlocale(LC_ALL, "rus");
		
	if (xfifo_test_run() == 0)
	{
		printf("\n********* ��� ����� �������� �������! **********\n");
	}
	else printf("\n********* ������! �� ��� ����� ��������. *************\n");
	system("pause");
	return 0;
}