#ifndef Ex_EE24Co2_H
#define Ex_EE24Co2_H

#define Ex_SLAVE_READ		0xA1
#define Ex_SLAVE_WRITE		0xA0

void write_Ex_EE24Co2(unsigned char address1,  unsigned char data);
unsigned char read_Ex_EE24Co2(unsigned char address1);
#endif
