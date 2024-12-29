#ifndef SCI_H
#define SCI_H

#define RX_PIN					TRISC7
#define TX_PIN					TRISC6

void init_uart(void);
void putch(unsigned char byte);//to transmit 1 byte 
int puts(const char *s);//to transmit string
unsigned char getch(void);//to recive 1 byte
unsigned char getche(void);//recive 1 byte ,recive 1 byte

#endif
