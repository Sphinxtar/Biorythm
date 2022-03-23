/* $Id: biorythm.c,v 1.2 1998/07/05 15:55:11 sverrehu Exp $ */
/**************************************************************************
 *
 *  FILE            biorythm.c
 *  MODULE OF       biorythm - simple biorythm calculation program
 *
 *  DESCRIPTION     
 *
 *  WRITTEN BY      Sverre H. Huseby <sverrehu@online.no>
 *
 **************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "date.h"

/* Periods: Physical, Emotional and Intellectual */
#define PHY_PERIOD 23
#define EMO_PERIOD 28
#define INT_PERIOD 33

#define PLOT_PHY 1
#define PLOT_EMO 2
#define PLOT_INT 4
#define PLOT_AVG 8

/**************************************************************************
 *                   P R I V A T E    F U N C T I O N S                   *
 **************************************************************************/

/*-------------------------------------------------------------------------
 *  NAME          f
 *  FUNCTION      Calculate a curve sample value.
 *  INPUT         birth   date of birth.
 *                day     the day to get sample from.
 *                period  the period of the curve in days.
 *  RETURNS       A value in [-1, 1].
 */
static double f(Date birth, Date day, int period)
{
    int n;

    n = dateGetNumDays(birth, day) % period;
    return sin((double) n * 2.0 * M_PI / period);
}

/*-------------------------------------------------------------------------
 *  NAME          bioTextPlot
 *  FUNCTION      Make a biorythm graph using text characters.
 *  INPUT         birth   date of birth.
 *                from    date of first day of plot.
 *                ndays   number of days to plot.
 *                what    what to plot. (PLOT_PHY | PLOT_EMO ...)
 */
static void bioTextPlot(Date birth, Date from, int ndays, int what)
{
#define WIDTH 53
    int q;
    char s[WIDTH + 1];
    double val, sum;

    printf("Day of birth: %s\n", dateStr(birth));
    if (what & PLOT_PHY)
	printf("[P]hysical   ");
    if (what & PLOT_EMO)
	printf("[E]motional   ");
    if (what & PLOT_INT)
	printf("[I]ntellectual   ");
    if (what & PLOT_AVG)
	printf("[A]verage");
    printf("\n");
    
    for (q = 0; q < 73; q++)
	printf("-");
    printf("\n");
    printf("YYYY-MM-DD  down%57s\n", "up    age");
    for (q = 0; q < 73; q++)
	printf("-");
    printf("\n");
    
    while (--ndays >= 0) {
	for (q = 0; q <= WIDTH; q++)
	    s[q] = ' ';
	s[q] = '\0';
	s[0] = s[WIDTH] = '|';
	s[WIDTH / 2] = '.';
	
	printf("%-10.10s  ", dateStr(from));
	
	sum = val = f(birth, from, PHY_PERIOD);
	if (what & PLOT_PHY)
	    s[(int)round(((val + 1.0) / 2.0) * WIDTH)] = 'P';

	val = f(birth, from, EMO_PERIOD);
	sum += val;
	if (what & PLOT_EMO)
	    s[(int)round(((val + 1.0) / 2.0) * WIDTH)] = 'E';

	val = f(birth, from, INT_PERIOD);
	sum += val;
	if (what & PLOT_INT)
	    s[(int)round(((val + 1.0) / 2.0) * WIDTH)] = 'I';

	if (what & PLOT_AVG)
	    s[(int)round((((sum / 3.0) + 1.0) / 2.0) * WIDTH)] = 'A';
	
	printf("%s  %5d\n", s, dateGetNumDays(birth, from));

	dateInc(&from);
    }
}

/*-------------------------------------------------------------------------
 *  NAME          version
 *  FUNCTION      Show the version of this program, and exit.
 */
static void version(void)
{
	puts("\nbiorythm by Sverre H. Huseby badly hacked to remove shhmsg and shhopt");
}

/*-------------------------------------------------------------------------
 *
 *  NAME          usage
 *  FUNCTION      Show the usage of this program, and exit.
 */
static void usage(void)
{
	version();
	puts("\nUsage: biorythm [Option] Date-of-birth First-date-to-plot Number-of-days\n");
	puts("Option is not optional:");
	puts("\t-e plot the emotional curve");
	puts("\t-i plot the intellectual curve");
	puts("\t-p plot the physical curve");
	puts("\t-a plot all three curves");
	puts("\tDates must be specified as YYYY-MM-DD (ISO date format).\n");
	exit(0);
}

/**************************************************************************
 *                    P U B L I C    F U N C T I O N S                    *
 **************************************************************************/

int main(int argc, char *argv[])
{
	int  ndays = 0, what = 0;
	Date birth, from;
     
	if (argc < 4) 
	{
		usage();
	}
	else
	{
		for (int i = 1; i < argc; ++i) {
			// printf("  %d. %s\n", i, argv[i]);
			if (argv[i][0] == '-') {
				switch(toupper(argv[i][1])) {
					case 'A':
						what = PLOT_EMO | PLOT_INT | PLOT_PHY;
						break;
					case 'E':
						what |= PLOT_EMO;
						continue;
					case 'I':
						what |= PLOT_INT;
						continue;
					case 'P':
						what |= PLOT_PHY;
						continue;
					default:
						puts("No such option available.");
						exit(1);
				}
			}
                }
    
		birth = dateFromStr(argv[2]);
		from = dateFromStr(argv[3]);
		ndays = atoi(argv[4]);
		if (ndays < 1 || ndays > 500)
			puts("number of days must be from 1 to 500\n");
		else
			bioTextPlot(birth, from, ndays, what);
	}
	return 0;
}
