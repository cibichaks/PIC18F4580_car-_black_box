#include <xc.h>
#include "car_blackbox.h"
#include "m_keypad/matrix_keypad.h"
#include "CLCD/clcd.h"
#include "ADC/adc.h"
#include "rtc_and_Ex_eeprome/ds1307.h"
#include "rtc_and_Ex_eeprome/i2c.h"
#include "UART/uart.h"
void init_config(void)
{
	init_matrix_keypad();
	init_clcd();
	init_i2c();
	init_adc();
	init_ds1307();
}
extern unsigned char sign,itr,arrow;
unsigned char clcd_function = 1,display=1,not_in=1,fetch_add,fetch_once;
void main(void)
{
	
	init_config();
	while (1)
	{

		key = read_switches(STATE_CHANGE);

		if (clcd_function)
		{
			here_is_dashboard();
		}
		else
		{
			here_is_menu();
		}
		if (key == MK_SW5 )
		{
			CLEAR_DISP_SCREEN;
			clcd_function = 0;
		}
		if (key == MK_SW6 && not_in)
		{
			CLEAR_DISP_SCREEN;
			clcd_function = 1;

		}else if( key == MK_SW6 && not_in == 0){
			CLEAR_DISP_SCREEN;
			sign = 1;
			arrow = 0;
			itr = 0;
			not_in = 1;
			fetch_add = 0;
			fetch_once = 1;
		}
	}
}
