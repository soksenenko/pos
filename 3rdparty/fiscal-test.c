//версия 1.x
//
#include <stdio.h>   /* Стандартные объявления ввода/вывода */
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */
#include "libiks483.h"

int main()
{
    int fd;
    uchar buff[BUFSIZE];
    int errcode, len, i;
    unsigned int sum;
    
    
    fd = FPInit(PORTNUM, B9600, TIMEOUT);
    if (fd == -1) { 
	printf("open port failed\n");	
    	return(1);
    }

/* чтение времени
*/

    len = FPGetDate(buff);
    printf("mm: %x  dd: %x  yy: %x\n",buff[0],buff[1],buff[2]);

    len=FPAvans(100);
    len=FPGive(11);

//    len=FPSetPassword(20,0,0,"PETROV");
//    len=FPSetCashier(0,"",0,1);
//    len=FPSetTaxRate(0,4,2100,2150,2250,2450,0,1,2);
//    len=FPGetTaxRate1();
//    printf("tax= %d\n",len);

//    len=FPResetOrder();
//    len=FPSetString(0,"Mega-SHOP");
    len=FPComment("HELLO,WORLD",1);
    len=FPDiscount(0,1000,1,"");
    len=FPPayment(3,15000,1,"");
//    len=FPPrintCopy();
//    len=FPDayClrReport(0);
    len=FPArtXReport(0);
//    len=FPLineSP(33);

    FPClose();
}
