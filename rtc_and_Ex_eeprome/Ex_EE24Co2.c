#include "Ex_EE24Co2.h"
#include "i2c.h"
#include <xc.h>

/* 
 * DS1307 Slave address
 * D0  -  Write Mode
 * D1  -  Read Mode
 */

void write_Ex_EE24Co2(unsigned char address, unsigned char data)//is to write the data to the RTC
{
	i2c_start();// these processs are did as a i2c protocol
	i2c_write(Ex_SLAVE_WRITE);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
    for(unsigned int i=3000 ; i--;);
}

unsigned char read_Ex_EE24Co2 (unsigned char address) //these  process are usedto read from the itc
{
	unsigned char data;

	i2c_start();
	i2c_write(Ex_SLAVE_WRITE);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(Ex_SLAVE_READ);
	data = i2c_read();
	i2c_stop();

	return data;

}
