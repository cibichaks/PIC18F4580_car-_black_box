#include <xc.h>
#include "car_blackbox.h"
#include "CLCD/clcd.h"
#include "m_keypad/matrix_keypad.h"
#include "ADC/adc.h"
#include "rtc_and_Ex_eeprome/Ex_EE24Co2.h"
#include "rtc_and_Ex_eeprome/ds1307.h"

unsigned char gear = 0, impact_count = 0, gear_arr[] = " N12345R", clock_reg[3], time[9], power = 1;

unsigned short speed = 0,address = 0x000;
void here_is_dashboard(void)
{
    speed = read_adc(CHANNEL4);
    speed /= 10;
    clcd_print("Time", LINE1(1));
    clcd_print("Ev", LINE1(9));
    clcd_print("Sp", LINE1(13));
    display_time();
    clcd_putch('G', LINE2(9));
    if (power)
    {
        clcd_print("ON", LINE2(9));
    }
    else
    {
        clcd_putch(gear_arr[gear], LINE2(10));
    }
    if (speed < 100)
    {
        clcd_putch('0' + (speed / 10), LINE2(13));
        clcd_putch('0' + (speed % 10), LINE2(14));
    }
    if (key == MK_SW1)
    {
        power = 0;
        if (gear < 7)
        {
            gear++;
        }
        if (impact_count < 10)
        {
            Store_impact(time, gear_arr[gear], speed);
        }
        else
        {
            delete_impact();
            Store_impact(time,gear_arr[gear], speed);
            impact_count -- ;
        }
    }
    else if (key == MK_SW2)
    {

        power = 0;
        if (gear > 0)
        {
            gear--;
        }
        if (impact_count < 10)
        {
            Store_impact(time, gear_arr[gear], speed);
        }
        else
        {
            delete_impact();
            Store_impact(time, gear_arr[gear], speed);
            impact_count -- ;
        }
    }
    else if (key == MK_SW3)
    {
        clcd_print(" C", LINE2(9));
        for (long int i = 200000; i--;)
            ;
        if (impact_count < 10)
        {
            Store_impact(time, 'C', speed);
        }
        else
        {
            delete_impact();
            Store_impact(time, 'C', speed);
            impact_count --;
        }
    }
}
static void delete_impact(void)
{
    unsigned short rewrite = 0x000;
    for (unsigned char i = 0; i < impact_count-1 ; i++)
    {
        for (unsigned char j = 0; j < 10; j++)
        {
            write_Ex_EE24Co2(rewrite, read_Ex_EE24Co2(rewrite + 0x00a));
            rewrite++;
        }
    }
    address = rewrite;
}


static void display_time(void)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
	time[1] = '0' + (clock_reg[0] & 0x0F);
    time[2] = ':';
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
    time[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = ':';
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
    time[7] = '0' + (clock_reg[2] & 0x0F);
    time[8] = '\0';

    clcd_print(time, LINE2(0));
}

static void Store_impact(unsigned char time[], unsigned char gear, unsigned short speed)
{
    impact_count++;
    for (unsigned char i = 0; i < 8; i++)
    {
        write_Ex_EE24Co2(address++, time[i]);
    }
    write_Ex_EE24Co2(address++, gear);
    write_Ex_EE24Co2(address++, (char)speed);
}
