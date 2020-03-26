#ifndef _XFIFOTESTH_
#define _XFIFOTESTH_

/**
! инициализация тестов
+ 0 если все ок, иначе не 0
*/
int xfifo_test_init(void);

 /*
 ! запуск всех тестов
 + 0 - все тесты пройдены, -1 - не все тесты пройдены */
 int xfifo_test_run(void);

 #endif // _XFIFOTESTH