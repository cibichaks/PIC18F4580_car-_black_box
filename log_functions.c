#include <xc.h>
#include "car_blackbox.h"
#include "CLCD/clcd.h"
#include "m_keypad/matrix_keypad.h"
#include "UART/uart.h"
#include "rtc_and_Ex_eeprome/Ex_EE24Co2.h"
#include "rtc_and_Ex_eeprome/ds1307.h"
extern unsigned char key, impact_count, crash_alart,time[9];
unsigned char fetched_count = 0, fetched_arr[10], temp_log[10][19], fetch_once = 1;
unsigned char fetch_add = 0;
void here_is_view_log()
{
	if (impact_count > 0)
	{
		if (fetch_once)
		{
			fetch_once = 0;
			fetch_log();
		}

		clcd_putch('0' + (fetched_count), LINE1(0));
		clcd_print("the logs are", LINE1(2));
		clcd_putch(fetched_arr[0], LINE2(2));
		clcd_putch(fetched_arr[1], LINE2(3));
		clcd_putch(fetched_arr[2], LINE2(4));
		clcd_putch(fetched_arr[3], LINE2(5));
		clcd_putch(fetched_arr[4], LINE2(6));
		clcd_putch(fetched_arr[5], LINE2(7));
		clcd_putch(fetched_arr[6], LINE2(8));
		clcd_putch(fetched_arr[7], LINE2(9));
		if (fetched_arr[8] != 'C')
		{
			clcd_putch('G', LINE2(11));
		}else{
			clcd_putch(' ',LINE2(11));
		}
		clcd_putch(fetched_arr[8], LINE2(12));
		clcd_putch('0' + (fetched_arr[9] / 10), LINE2(14));
		clcd_putch('0' + (fetched_arr[9] % 10), LINE2(15));

		if (key == MK_SW2)
		{
			if (fetched_count < impact_count-1)
			{
				fetched_count++;
				fetch_log();
				CLEAR_DISP_SCREEN;
			}
		}
		else if (key == MK_SW1)
		{
			if (fetched_count != 0)
			{
				{
					fetch_add -= 0x14;
					fetched_count--;
					fetch_log();
					CLEAR_DISP_SCREEN;
				}
			}
		}
	}
	else
	{
		clcd_print("no Events!!", LINE2(0));
	}
}
void fetch_log(void)
{
	for (unsigned char i = 0; i < 10; i++)
	{
		fetched_arr[i] = read_Ex_EE24Co2(fetch_add);
		fetch_add++;
	}
}

void here_is_Download_log()
{
	init_uart();
	unsigned char itr = 0;//, user_responce = getch();
		puts(" Time   Ev Sp\n\r");
		modify_for_send();
		while (itr < impact_count)
		{
			puts(temp_log[itr++]);
		}
fetch_add = 0;
}
static void modify_for_send(void)
{
	unsigned char adress = 0x000;
	for (unsigned char i = 0; i < impact_count; i++)
	{
		fetch_log();
		temp_log[i][0] = fetched_arr[0];
		temp_log[i][1] = fetched_arr[1];
		temp_log[i][2] = fetched_arr[2];
		temp_log[i][3] = fetched_arr[3];
		temp_log[i][4] = fetched_arr[4];
		temp_log[i][5] = fetched_arr[5];
		temp_log[i][6] = fetched_arr[6];
		temp_log[i][7] = fetched_arr[7];
		temp_log[i][8] = ' ';
		temp_log[i][9] = ' ';
		if(fetched_arr[8] == 'C'){
			temp_log[i][10] = ' ';
		}else{
			temp_log[i][10] = 'G';
		}

		temp_log[i][11] = fetched_arr[8];
		temp_log[i][11] = fetched_arr[8];
		temp_log[i][12] = ' ';
		temp_log[i][13] = ' ';
		temp_log[i][14] = ('0' + (fetched_arr[9] / 10));
		temp_log[i][15] = ('0' + (fetched_arr[9] % 10));
		temp_log[i][16] = '\n';
		temp_log[i][17] = '\r';
		temp_log[i][18] = '\0';
	}
}
void here_is_set_time(void)
{
	unsigned char hour_hand = 0, minute_hand = 0, seconds_hand = 0, which_hand = 0, complete_or_not = 1;
	unsigned char blink = 1, key,flag = 1;
	unsigned long int blink_delay = 0;
	clcd_print("set_time", LINE1(0));
	clcd_print(time, LINE2(0));

	while (complete_or_not)
	{
		key = read_switches(STATE_CHANGE);
		if (++blink_delay > 2000)
		{
			blink_delay = 0;
			blink = !blink;
		}

		if (key == MK_SW2)
		{
			which_hand = (which_hand + 1) % 3;
		}
		switch (which_hand)
		{
			case 0:
				if (blink)
				{
					clcd_putch('0' + (hour_hand / 10), LINE2(0));
					clcd_putch('0' + (hour_hand % 10), LINE2(1));
				}
				else
				{
					clcd_putch(' ', LINE2(0));
					clcd_putch(' ', LINE2(1));
				}

				if (key == MK_SW1)
				{
					hour_hand = (hour_hand + 1) % 24;
				}
				break;

			case 1:
				if (blink)
				{
					clcd_putch('0' + (minute_hand / 10), LINE2(3));
					clcd_putch('0' + (minute_hand % 10), LINE2(4));
				}
				else
				{
					clcd_putch(' ', LINE2(3));
					clcd_putch(' ', LINE2(4));
				}

				if (key == MK_SW1)
				{
					minute_hand = (minute_hand + 1) % 60;
				}
				break;

			case 2:
				if (blink)
				{
					clcd_putch('0' + (seconds_hand / 10), LINE2(6));
					clcd_putch('0' + (seconds_hand % 10), LINE2(7));
				}
				else
				{
					clcd_putch(' ', LINE2(6));
					clcd_putch(' ', LINE2(7));
				}

				if (key == MK_SW1)
				{
					seconds_hand = (seconds_hand + 1) % 60;
				}
				break;
		}
		if (key == MK_SW5)
		{
			complete_or_not = 0;
		}else if(key == MK_SW6){
			flag = 0;
			complete_or_not = 0;
		}

	}
	if(flag){
	write_ds1307(SEC_ADDR, (((seconds_hand / 10) << 4) | (seconds_hand % 10)) & 0x7F);
	write_ds1307(MIN_ADDR, ((minute_hand / 10) << 4) | (minute_hand % 10));
	write_ds1307(HOUR_ADDR, (((hour_hand / 10) << 4) | (hour_hand % 10)) & 0x3F);
	clcd_print("Time Set!", LINE1(0));
	for (long int i = 500000; i--;)
		;
	}else{
		CLEAR_DISP_SCREEN;
		clcd_print("TIME NOT SET",LINE1(0));
		for(long int i = 500000;i--;);

	}
}
