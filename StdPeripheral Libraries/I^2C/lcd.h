#include "stm32f10x.h"
void lcd_init(void);
void sendCMD(uint8_t c);
void lcd_putc(char c);
void lcd_print(char *s);
void lcd_clear(void);
void lcd_line1(void);
void lcd_line2(void);
void Delay(uint32_t nCount);
