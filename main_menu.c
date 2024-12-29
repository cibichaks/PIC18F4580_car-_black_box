#include <xc.h>
#include "car_blackbox.h"
#include "CLCD/clcd.h"
#include "m_keypad/matrix_keypad.h"
#include "rtc_and_Ex_eeprome/Ex_EE24Co2.h"

const unsigned char *list[] = {
    "VIEW LOG",
    "DOWNLOAD LOG",
    "CLEAR LOG",
    "SET TIME"};

unsigned char arrow = 0, itr = 0, sign = 1;
extern unsigned char impact_count,not_in,fetched_count;
extern unsigned short address;

void here_is_menu(void)
{
    if (sign)
    {
        fetched_count = 0;
        if (arrow == 0)
        {
            clcd_putch('*', LINE1(0));
        }
        if (key == MK_SW2)
        {
            if (arrow < 3)
            {
                CLEAR_DISP_SCREEN;
                arrow++;
                if (arrow != 1)
                {
                    itr++;
                }
            }
            if (arrow == 0)
            {
                clcd_putch('*', LINE1(0));
            }
            else
            {
                clcd_putch('*', LINE2(0));
            }
        }
        else if (key == MK_SW1)
        {
            if (arrow > 0)
            {
                CLEAR_DISP_SCREEN;
                arrow--;
                if (arrow != 2)
                {
                    itr--;
                }
                clcd_putch('*', LINE1(0));
            }
        }
        print_main_menu(itr);
    }
    else
    {
        not_in = 0;
        switch (arrow)
        {
        case 0:
            here_is_view_log();
            break;
        case 1:
            clcd_print("Logs are ",LINE1(0));
            clcd_print("Downloaded", LINE2(0));
            here_is_Download_log();
            for(unsigned long int i = 200000; i--;);
            CLEAR_DISP_SCREEN;
            arrow = 0;
            itr = 0;
	        sign=1;
            break;
        case 2:
            impact_count = 0;
            address = 0x000;
            clcd_print("Cleard logs ", LINE1(0));
            clcd_print("Succesfully :)", LINE2(0));
            for(long int i = 500000 ; i--;);
            CLEAR_DISP_SCREEN;
            not_in = 1;
            sign = 1;
            arrow = 0;
            itr=0;
            break;
        case 3:
            here_is_set_time();
            CLEAR_DISP_SCREEN;
            not_in = 1;
            arrow = 0;
            itr = 0;
            sign = 1;

            break;
        }
    }
    if (key == MK_SW5)
    {
        CLEAR_DISP_SCREEN;
        sign = 0;
    }
}
static void print_main_menu(unsigned char itr)
{
    clcd_print(list[itr], LINE1(2));
    clcd_print(list[itr + 1], LINE2(2));
}
