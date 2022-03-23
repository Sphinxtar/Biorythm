/**************************************************************************
 *  FILE            date.c
 *  MODULE OF       biorythm - simple biorythm calculation program
 *  DESCRIPTION     Routines handling dates.
 *  WRITTEN BY      Sverre H. Huseby <sverrehu@online.no>
 **************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "date.h"

/**************************************************************************
 *                    P U B L I C    F U N C T I O N S                    *
 **************************************************************************/

/*-------------------------------------------------------------------------
 *  NAME          dateSet
 *  FUNCTION      Set the elements of a date.
 *  INPUT         year    the year (full year, like 1996). 
 *                month   the month (1-12).
 *                day     the day (1-31).
 *  OUTPUT        date    the filled in date.
 */
void dateSet(Date *date, int year, int month, int day)
{
    date->year = year;
    date->month = month;
    date->day = day;
}

/*-------------------------------------------------------------------------
 *  NAME          dateGet
 *  FUNCTION      Get the elements of a date.
 *  INPUT         date    the filled in date.
 *  OUTPUT        year    the year (full year, like 1996). 
 *                month   the month (1-12).
 *                day     the day (1-31).
 */
void dateGet(Date date, int *year, int *month, int *day)
{
    *year = date.year;
    *month = date.month;
    *day = date.day;
}

/*-------------------------------------------------------------------------
 *  NAME          dateIsLeapYear
 *  FUNCTION      Determine whether a given year is a leapyear.
 *  INPUT         year    the year to test.
 *  RETURNS       boolean value. 1=leapyear.
 */
int dateIsLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

/*-------------------------------------------------------------------------
 *  NAME          dateDaysInMonth
 *  FUNCTION      Get number of days in a month.
 *  INPUT         month   the month; (1..12)
 *                year    the year to test. (full year, like 1996)
 *  RETURNS       boolean value. 1=leapyear.
 */
int dateDaysInMonth(int month, int year)
{
    static int vec[] = { -1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (month == 2)
	return 28 + dateIsLeapYear(year);
    return vec[month];
}

/*-------------------------------------------------------------------------
 *  NAME          dateGetNumDays
 *  FUNCTION      Get number of days from one date to another.
 *  INPUT         d0      the start date.
 *                d1      the end date.
 *  RETURNS       Number of days. Examples:
 *                  days are equal; returns 0.
 *                  d1 is the day after d0; returns 1.
 *                  d1 is the before after d0; returns -1.
 */
int dateGetNumDays(Date d0, Date d1)
{
    int q, negate = 0, ret = 0;
    Date tmp;

    if (d0.year > d1.year
	|| (d0.year == d1.year && d0.month > d1.month)
	|| (d0.year == d1.year && d0.month == d1.month && d0.day > d1.day)) {
        negate = 1;
	tmp = d0;
	d0 = d1;
	d1 = tmp;
    }
    
    /* Count the days in all years _between_ the two given years. */
    for (q = d0.year + 1; q < d1.year; q++)
	ret += 365 + dateIsLeapYear(q);

    /* */
    if (d0.year != d1.year) {
	/* The years are not the same. */
	/* Add number of days left in first year. */
	ret += dateDaysInMonth(d0.month, d0.year) - d0.day;
	for (q = d0.month + 1; q <= 12; q++)
	    ret += dateDaysInMonth(q, d0.year);
	/* Add number of days `used' in second year. */
	for (q = 1; q < d1.month; q++)
	    ret += dateDaysInMonth(q, d1.year);
	ret += d1.day;
    } else {
	/* The years are the same. */
	/* Count the days in all months _between_ the two given months. */
	for (q = d0.month + 1; q < d1.month; q++)
	    ret += dateDaysInMonth(q, d0.year);
	/* Add the days in each month. */
	if (d0.month == d1.month)
	    ret += d1.day - d0.day;
	else {
	    ret += dateDaysInMonth(d0.month, d0.year) - d0.day;
	    ret += d1.day;
	}
    }
    return negate ? -ret : ret;
}

/*-------------------------------------------------------------------------
 *  NAME          dateInc
 *  FUNCTION      Increase a date.
 *  INPUT         date    the date to increase.
 *  OUTPUT        date    the ncreased date.
 */
void dateInc(Date *date)
{
    if (date->day < dateDaysInMonth(date->month, date->year))
	++date->day;
    else {
	date->day = 1;
	if (date->month < 12)
	    ++date->month;
	else {
	    date->month = 1;
	    ++date->year;
	}
    }
}

/*-------------------------------------------------------------------------
 *  NAME          dateStr
 *  FUNCTION      Get a string representation of a date.
 *  INPUT         date    the date to represent as a string.
 *  RETURNS       The date as a string.
 */
char * dateStr(Date date)
{
    static char ret[21];

    sprintf(ret, "%4d-%02d-%02d", date.year, date.month, date.day);
    return ret;
}

/*-------------------------------------------------------------------------
 *  NAME          dateFromStr
 *  FUNCTION      Convert a string to a date.
 *  INPUT         s       a string of the form YYYY-MM-DD
 *  RETURNS       a date or null if the string is poorly formed
 */
Date dateFromStr(const char *s)
{
	Date ret = { 0, '\0', '\0' };
	char *p, *s2;

	if ((s2 = strdup(s)) == NULL) {
		printf("out of memory\n");
	} 
	else 
	{
		p = strtok(s2, "/-:.");
		if (p == NULL) 
			printf("%s is not of the form YYYY-MM-DD\n", s);
		while (p != NULL) {
			if (ret.year == 0)
				ret.year = atoi(p);
			else if (ret.month == 0)
				ret.month = atoi(p);
			else if (ret.day == 0)
				ret.day = atoi(p);
			// printf("p:%s y:%d m:%d d:%d\n", p, ret.year, ret.month, ret.day);
			p = strtok(NULL, "/-:.");
		}	

		if (ret.month < 1 || ret.month > 12)
			printf("illegal month in %s (YYYY-MM-DD)\n", s);
		
		if (ret.day < 1 || ret.day > dateDaysInMonth(ret.month, ret.year)) 
			printf("illegal day in %s (YYYY-MM-DD)\n", s);
	}
	free(s2);
	return ret;
}
/* EOF */
