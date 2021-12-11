#ifndef __OLED_H
#define __OLED_H			  	 


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_MODE 0


//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define SIZE 8
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	   

//引脚定义
#define OLED_CLK_PIN 		GPIO_PIN_15
#define OLED_CLK_Port 	GPIOB
#define OLED_MOSI_PIN 	GPIO_PIN_14
#define OLED_MOSI_Port 	GPIOB
#define OLED_RST_PIN 		GPIO_PIN_13
#define OLED_RST_Port 	GPIOB
#define OLED_DC_PIN 		GPIO_PIN_13
#define OLED_DC_Port 		GPIOC

#define OLED_DC_Pin GPIO_PIN_13
#define OLED_DC_GPIO_Port GPIOC
#define OLED_RST_Pin GPIO_PIN_13
#define OLED_RST_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_14
#define OLED_SDA_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_15
#define OLED_SCL_GPIO_Port GPIOB



#define OLED_RST_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_RESET)
#define OLED_RST_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,GPIO_PIN_RESET)
#define OLED_DC_Set() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,GPIO_PIN_SET)

#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,GPIO_PIN_RESET)
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_RESET)
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_SET)
		     

//OLED控制用函数

void OLED_Write_Byte(unsigned char dat,unsigned char cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Retain(void);  
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t);
void OLED_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char dot);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr);
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size2);
void OLED_ShowString(unsigned char x,unsigned char y, unsigned char *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_DrawConvertBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP [ ]);

#endif  
	 
