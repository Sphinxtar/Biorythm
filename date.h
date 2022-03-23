/* $Id: date.h,v 1.1.1.1 1996/10/09 19:14:55 sverrehu Exp $ */
#ifndef DATE_H
#define DATE_H

#ifdef __cplusplus
  extern "C" {
#endif

typedef struct {
    short year;   /* Full year, like 1996 */
    char  month;  /* 1 - 12 */
    char  day;    /* 1 - 31 */
} Date;

void dateSet(Date *date, int year, int month, int day);
int  dateIsLeapYear(int year);
int  dateDaysInMonth(int month, int year);
int  dateGetNumDays(Date d0, Date d1);
void dateInc(Date *date);
char *dateStr(Date date);
Date dateFromStr(const char *s);

#ifdef __cplusplus
  }
#endif

#endif
