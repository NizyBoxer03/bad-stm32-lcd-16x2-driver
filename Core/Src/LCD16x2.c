
#include "LCD16x2.h"
#include "LCD16x2_cfg.h"
#include "DWT_Delay.h"

/* Escribe 1 o 0 en los pines configurados en LCD16x2_CfgParam */

void LCD_DATA(unsigned char Data)
{
	(LCD16x2_CfgParam.LCD_GPIO)->ODR &= ~(0xF<<4);
	(LCD16x2_CfgParam.LCD_GPIO)->ODR |= (0xF0 & (Data<<4)); //Aplico un mask para que no sobrescriba nada de lo que haya en el puerto
	LCD_EN_PULSE();
}

void LCD_CMD(unsigned char a_CMD)
{
	unsigned char Low4, High4;
	Low4  = a_CMD & 0x0F;
	High4 = a_CMD & 0xF0;

	// Seleccionar Registro de Comando
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.RS_PIN, 0);

    // Enviar comando a LCD
	// Enviar flanco a la señal ENable. No olvidar delays.
    LCD_DATA((High4>>4));
    LCD_DATA(Low4);

}

void LCD_Clear()
{
    LCD_CMD(0x1);
	LCD_CMD(0x2); //Return home
	DWT_Delay_ms(2);
}

/* Setea posicion de cursor */
void LCD_SetCursor(unsigned char fila, unsigned char columna)
{
    if(fila == 1)
    {
        LCD_CMD(0x80 + (columna - 1));
        DWT_Delay_us(40);

    }

    if(fila == 2)
    {
    	LCD_CMD(0xC0 + (columna - 1));
    	DWT_Delay_us(40);
    }

    DWT_Delay_ms(2);
}

void LCD_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	// Inicializa la funcion de delay DWT 
	DWT_Delay_Init();
	
    // habilitacion de clock en GPIOs
	if(LCD16x2_CfgParam.LCD_GPIO == GPIOA)
	    __HAL_RCC_GPIOA_CLK_ENABLE();
	else if(LCD16x2_CfgParam.LCD_GPIO == GPIOB)
	    __HAL_RCC_GPIOB_CLK_ENABLE();
	else if(LCD16x2_CfgParam.LCD_GPIO == GPIOC)
		__HAL_RCC_GPIOC_CLK_ENABLE();
	else if(LCD16x2_CfgParam.LCD_GPIO == GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();
	
	
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D4_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D5_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D6_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D7_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.RS_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.EN_PIN, GPIO_PIN_RESET);
	
	GPIO_InitStruct.Pin = LCD16x2_CfgParam.D4_PIN | LCD16x2_CfgParam.D5_PIN |
			LCD16x2_CfgParam.D6_PIN |LCD16x2_CfgParam.D7_PIN | LCD16x2_CfgParam.RS_PIN |
			LCD16x2_CfgParam.EN_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD16x2_CfgParam.LCD_GPIO, &GPIO_InitStruct);

	// Inicializar display de acuerdo a la especificiación provista en DataSheet

	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.RS_PIN, 0);
	DWT_Delay_ms(16); //Delay +15 ms
	LCD_DATA(0x3); //Comando de config
	DWT_Delay_ms(5); //Delay +4.1ms
	LCD_DATA(0x3); //Comando de config
	DWT_Delay_us(110); //Delay +100ms
	LCD_DATA(0x3); //Comando de config
	LCD_DATA(0x2); //Comando de config

	LCD_CMD(0x2C);
	LCD_CMD(0x8);
	LCD_CMD(0x1);
	LCD_CMD(0xE);
	LCD_CMD(0x6); //Entry Mode Set
	LCD_CMD(0x2); //Return home
	DWT_Delay_ms(2);

	//Init Done
}
void LCD_WriteChar(char Data) //El LCD opera en ASCII (con algunos cambios, no requiere conversión)
{
   // Puede usar HAL_GPIO_WritePin()

	char Low4, High4;
    Low4  = Data & 0x0F;
    High4 = Data & 0xF0;

    HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.RS_PIN, 1);

   	// Enviar nibble alto
    LCD_DATA((High4>>4));

   	// Enviar nibble bajo
    LCD_DATA(Low4);
}

void LCD_WriteString(char *str)
{
	int i = 0;
    while(str[i] != '\0')
    {
 	   LCD_WriteChar(str[i]);
	   DWT_Delay_us(40);
	   i++;
    }
}
void LCD_ShiftLeft()
{
	LCD_CMD(0x7); //Entry Mode Set - S = 1
	DWT_Delay_us(40);
	LCD_CMD(0x18); //Display shift left
	DWT_Delay_us(40);
	LCD_CMD(0x6); //Entry Mode Set - S = 0
	DWT_Delay_us(40);
}
void LCD_ShiftRight()
{
	LCD_CMD(0x7); //Entry Mode Set - S = 1
	DWT_Delay_us(40);
	LCD_CMD(0x1C); //Display shift right
	DWT_Delay_us(40);
	LCD_CMD(0x6); //Entry Mode Set - S = 0
	DWT_Delay_us(40);
}

void LCD_EN_PULSE()
{
    HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.EN_PIN, 1);
	DWT_Delay_ms(5);
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.EN_PIN, 0);
}
