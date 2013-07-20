#ifdef __cplusplus
extern "C" {
#endif

//версия 1.1
//
#include <stdio.h>   /* Стандартные объявления ввода/вывода */
#include <string.h>  /* Объявления строковых функций */
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */

typedef unsigned char uchar;
#define PORTNUM "/dev/ttyS0" /* номер порта, к которому подключен аппарат */
#define PORTSPEED B9600    /* скорость порта по умолчанию  */
#define TIMEOUT 200
#define BUFSIZE 128 /* размер входного буфера */

#define DLE '\x10'	/* управляющие коды для аппарата */
#define STX '\x2'
#define ETX '\x3'
#define ACK '\x6'
#define NAK '\x15'
#define SYN '\x16'
#define ENQ '\x5'

/* Открывает последовательный порт
 * Возвращает файловый дескриптор или -1 при ошибке.*/
extern int FPInit ( uchar * port, unsigned int BaudRate, int TimeOut);

// закрывает порт
extern int FPClose();

// читает состояние регистратора
extern int GetStatus(uchar * buff);

//  чтение блока памяти из регистратора
extern int GetMemory(unsigned short addr, uchar page, uchar amount, uchar * buff);

/* возвращает дату, <0 если была ошибка, или длину полученной строки
 * данные в buff */
extern int FPGetDate(uchar * buff);

/* Установка даты. 0 - успешно, <0 - ошибка */
extern int FPSetDate(uchar * buff);

// чтение времени из кассы
extern int FPGetTime(uchar * buff);

//установка времени
extern int FPSetTime(uchar * buff);

//смена пароля кассира, а также его имени
extern int FPSetPassword(unsigned short old_p, unsigned short new_p, 
		    uchar n_cashier, uchar * name);

//регистрация кассира
extern int FPSetCashier(uchar n_cashier, uchar * name, 
		unsigned short passwd, uchar prog_name);

//
extern int FPPayMoney(int amount, uchar status, int price, 
		uchar naloggroup, uchar * goods, unsigned long art);

extern int FPComment(uchar * comment, uchar opencheck);

//перевод строки
extern int FPLineFeed();

//сброс чека
extern int FPResetOrder();

//служебный внос денег
extern int FPAvans(unsigned long avans);


extern int FPSale(int amount, uchar status, int price, 
	    uchar naloggroup, uchar * goods, unsigned long art);

extern int FPPayment(uchar status, unsigned long payment, 
		uchar checkclose, uchar * information);

extern int FPSetString(uchar last, uchar * message);

//служебная выдача денег
extern int FPGive(unsigned long give);

// пересылает строку на индикатор клиента
extern int FPSendCustomer(uchar last, uchar * message);

// открыть денежный ящик
extern int FPOpenBox();

// печать копии чека
extern int FPPrintCopy();

//печать версии
extern int FPPrintVer();

//возвращает количество денег в кассе
extern unsigned long FPGetBox();

//устанавливает скидку
extern int FPDiscount(uchar type, unsigned int sum, 
		    uchar isdiscount, uchar * comment);

extern int FPCplOnLine();

extern int FPCplInd();

//смена скорости обмена данными
extern int FPChangeRate(uchar rate);

//задает межстрочный интервал, начальное значение=30
extern int FPLineSP(uchar range);

//производит фискализацию
extern int FPFiscalization(unsigned short passwd, uchar * fiscalnumber);

// регистрация атрибутов налогоплательщика
extern int FPSetHeadLine(unsigned short passwd, uchar * str1,
		    uchar * str2, uchar * str3, uchar * str4);

// программирует описание товара
extern int FPProgArt(unsigned short passwd, uchar status, int price, 
		uchar naloggroup, uchar * goods, unsigned long art);

// печать отчета по артикулам
extern int FPArtReport(unsigned short passwd);

// печать дневного отчета по операциям
extern int FPDayReport(unsigned short passwd);

// печать Х-отчета по артикулам
extern int FPArtXReport(unsigned short passwd);

// печать и регистрация дневного отчета по операциям с обнулением 
// дневных регистров
extern int FPDayClrReport(unsigned short passwd);

// печать отчета из фиск.памяти за период
extern int FPPeriodicReport(unsigned short passwd,unsigned long firstdate, 
			unsigned long lastdate);

// печать короткого отчета за период
extern int FPPeriodicReportShort(unsigned short passwd, unsigned long firstdate, 
			unsigned long lastdate);

// печать периодического отчета по номерам
extern int FPPeriodicNumberReport(unsigned short passwd, unsigned short firstnum, 
			unsigned short lastnum);

// устанавливает налоговые ставки
extern int FPSetTaxRate(unsigned short passwd, uchar amount, unsigned short tax1, 
		unsigned short tax2, unsigned short tax3, 
		unsigned short tax4, unsigned short tax5,
		uchar taxtype, uchar decimal);

// определяет состояние денежного ящика, 0-открыт, <0 - ошибка
extern int GetBoxStatus();

// определяет тип чека, 0 - чек продажи, 1 - чек выплаты
extern int GetCheckStatus();

// определяет тип запрограммированного налога, 0 - вложенный, 1 - добавляемый
extern int GetNalogStatus();

// определяет состояние смены, 0 - закрыта
extern int GetSmenaStatus();

// определяет состояние текущего чека, 0 - закрыт
extern int GetCheckOpenedStatus();

// определяет режим работы индикатора клиента, 0 - разрешен вывод
extern int GetDisplayStatus();

// определяет были ли изменены ставки налоговых груп, 0 - не изменялись
extern int GetNalogChangeStatus();

// определяет режим работы, 0 - тренировочный, 1 - фискальный
extern int GetFiscalizationStatus();

// определяет результат завершения последней команды, 0 - успешно
extern int GetErrorCommandStatus();

// определяет режим регистраций товаров в чеке, 0 - режим OffLine регистраций
extern int GetRegistrationRegim();

// возвращает заводской номер ЭККР и дату выпуска
extern int GetSerialNumber(uchar * buff);

// возвращает фискальный номер ЭККР и дату выпуска
extern int GetFiscalNumber(uchar * buff);

// возвращает 1-ю строку атрибутов налогоплательщика
extern int GetHeadString1(uchar * buff);

// возвращает 2-ю строку атрибутов налогоплательщика
extern int GetHeadString2(uchar * buff);

// возвращает 3-ю строку атрибутов налогоплательщика
extern int GetHeadString3(uchar * buff);

// возвращает 4-ю строку атрибутов налогоплательщика
extern int GetHeadString4(uchar * buff);

// возвращает версию ПО ЭККР
extern int GetHardwareVersion(uchar * buff);

// возвращает дату регистрации ЭККР
extern int GetRegistrationDate(uchar * buff);

// возвращает время регистрации ЭККР
extern int GetRegistrationTime(uchar * buff);

// возвращает бай статуса последней команды, посланной в ЭККР
extern uchar GetByteStatus();

// возвращает сообщение об ошибке принтера
extern uchar * FPErrorMessage();

// чтение номера последнего чека продаж
extern int FPGetCheckSale();

// возвращает значение счетчика дневных ооборотов с продаж
// naloggroup = (0..6) 0=A, 5=E
extern int FPGetDaySumSale(uchar naloggroup);

// чтение номера последнего чека выплат
extern int FPGetCheckPay();
    
// возвращает значение счетчика дневных ооборотов по выплатам
extern int FPGetDaySumPay(uchar naloggroup);

// возвращает номер последнего Z отчета
extern int FPGetNumZReport();

// возвращает байт результата последней команды
extern int GetByteResult();

// возвращает значение ставки налоговой группы А
extern int FPGetTaxRate1();

// возвращает значение ставки налоговой группы Б
extern int FPGetTaxRate2();

// возвращает значение ставки налоговой группы В
extern int FPGetTaxRate3();

// возвращает значение ставки налоговой группы Г
extern int FPGetTaxRate4();

// возвращает значение ставки налоговой группы Д
extern int FPGetTaxRate5();

// возвращает зколичество запрограммированных налоговых групп
extern int FPGetTaxCount();

#ifdef __cplusplus
}
#endif
