
#ifndef LCD16X2_H_
#define LCD16X2_H_

#include "stm32f1xx_hal.h"

typedef struct
{
	GPIO_TypeDef * LCD_GPIO;
	uint16_t D4_PIN;
	uint16_t D5_PIN;
	uint16_t D6_PIN;
	uint16_t D7_PIN;
	uint16_t EN_PIN;
	uint16_t RS_PIN;
	uint16_t LCD_EN_Delay;
}LCD16x2_CfgType;


//-----[ Prototipos ]-----

void LCD_Init();                  					// Intlnicializa LCD para interfaz 4-bits
void LCD_Clear();                 					// Borra el display
void LCD_ShiftLeft();             					// Shift de display a izquierda
void LCD_ShiftRight();            					// Shift de display a derecha
void LCD_CMD(unsigned char);      					// Enviar comando a LCD
void LCD_DATA(unsigned char);   					// Enviar dato de 4 bits a display
void LCD_SetCursor(unsigned char, unsigned char);  	// Setear posicion de cursor
void LCD_WriteChar(char);        					// Escribir caracter en la posicion actual
void LCD_WriteString(char*);     					// Escribir string a LCD
void LCD_EN_PULSE();

#endif /* LCD16X2_H_ */
