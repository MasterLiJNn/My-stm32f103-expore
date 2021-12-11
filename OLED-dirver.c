#include "stm32f1xx.h"
#include "common.h"
#include "oled.h"
#include "oledfont.h" 	
#include "main.h"
extern int _time ;



/*	IO口初始化
 OLED_DC_Pin GPIO_PIN_13， GPIOC
 OLED_RST_Pin GPIO_PIN_13， GPIOB
 OLED_SDA_Pin GPIO_PIN_14， GPIOB
 OLED_SCL_Pin GPIO_PIN_15， GPIOB
*/
void OLED_GPIO_Init(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB,OLED_RST_Pin|OLED_SDA_Pin|OLED_SCL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = OLED_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(OLED_DC_GPIO_Port, &GPIO_InitStruct);


  /*Configure GPIO pins : PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = OLED_RST_Pin|OLED_SDA_Pin|OLED_SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/*			向SSD1306写入一个字节
	dat:要写入的数据/命令
	cmd:数据/命令标志 0,表示命令;1,表示数据;
*/
void OLED_Write_Byte(unsigned char dat,unsigned char cmd){	
	unsigned char i;			  
	
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	
	for(i=0;i<8;i++){			  
		OLED_SCLK_Clr();
		
		if(dat&0x80){
		   OLED_SDIN_Set();
		}
		else
			OLED_SDIN_Clr();
	
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	
	OLED_DC_Set();   	  
} 

/*
	    
*/

void OLED_Set_Pos(unsigned char x, unsigned char y) { 
	OLED_Write_Byte(0xb0+y,OLED_CMD);
	OLED_Write_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_Write_Byte((x&0x0f)|0x01,OLED_CMD); 
}

/*
	开启OLED显示      
*/
void OLED_Display_On(void){
	OLED_Write_Byte(0X8D,OLED_CMD);  //SET DCDC?üá?
	OLED_Write_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_Write_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/*
	关闭OLED显示      
*/
void OLED_Display_Off(void){
	OLED_Write_Byte(0X8D,OLED_CMD);  //SET DCDC?üá?
	OLED_Write_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_Write_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}


/*
		清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!! 
*/
void OLED_Clear(void)  {  
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Byte (0xb0+i,OLED_CMD);    //éè??ò3μ??·￡¨0~7￡?
		OLED_Write_Byte (0x00,OLED_CMD);      //éè????ê??????aáDμíμ??·
		OLED_Write_Byte (0x10,OLED_CMD);      //éè????ê??????aáD??μ??·   
		for(n=0;n<128;n++)OLED_Write_Byte(0,OLED_DATA); 
	} //?üD???ê?
}
/*
		亮屏函数,点亮完屏,整个屏幕是点亮的 
*/
void OLED_Retain(void)  {  
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Byte (0xb0+i,OLED_CMD);    
		OLED_Write_Byte (0x00,OLED_CMD);      
		OLED_Write_Byte (0x10,OLED_CMD);       
		for(n=0;n<128;n++)OLED_Write_Byte(0xff,OLED_DATA); 
	} 
}


/*
	在指定位置显示一个字符,包括部分字符
	x:0~127
	y:0~63
	mode:0,反白显示;1,正常显示				 
	size:选择字体 16/12 
*/
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr){      	
	unsigned char c=0,i=0;	
	c=chr-' ';//μ?μ???ò?oóμ??μ			
	
	if(x>Max_Column-1){x=0;y=y+2;}
	if(SIZE ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
		OLED_Write_Byte(F8X16[c*16+i],OLED_DATA);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_Write_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else
	{	
		OLED_Set_Pos(x,y+1);
		for(i=0;i<6;i++)
			OLED_Write_Byte(F6x8[c][i],OLED_DATA);
	}
}


/*
	显示一个字符号串
*/
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr){
	unsigned char j=0;
	while (chr[j]!='\0')
	{
		OLED_ShowChar(x,y,chr[j]);
		if(SIZE==16)x+=8;
		else x+=6;
		
		if(x>120){x=0;y+=2;}
			j++;
	}
}


/*
	显示汉字
*/
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no){      			    
	unsigned char t,adder=0;
	OLED_Set_Pos(x,y);	
  for(t=0;t<16;t++)
	{
		OLED_Write_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
  }	
	OLED_Set_Pos(x,y+1);	
  for(t=0;t<16;t++)
	{	
		OLED_Write_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
  }					
}

/*
显示显示BMP图片128×64起始点坐标(x,y),
	x的范围0～127，y为页的范围0～7
*/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]){ 	
	unsigned int j=0;
 	unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
   	for(x=x0;x<x1;x++)
	  {      
	   	OLED_Write_Byte(BMP[j++],OLED_DATA);	    	
	  }
	}
} 

/*
	BMP 图反显
*/
void OLED_DrawConvertBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP [ ]){
	unsigned int j=0;
 	unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
   	for(x=x0;x<x1;x++)
	  {      
			OLED_Write_Byte(~BMP[j++],OLED_DATA);	    	
	  }
	}
}


/*
		初始化SSD1306
*/				    
void OLED_Init(void){
	OLED_GPIO_Init();
	
	OLED_RST_Clr();
	HAL_Delay(100);
	OLED_RST_Set(); 

	OLED_Write_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_Write_Byte(0x00,OLED_CMD);//---set low column address
	OLED_Write_Byte(0x10,OLED_CMD);//---set high column address
	OLED_Write_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_Write_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_Write_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_Write_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0×óóò·′?? 0xa1?y3￡
	OLED_Write_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0é???·′?? 0xc8?y3￡
	OLED_Write_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_Write_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_Write_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_Write_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_Write_Byte(0x00,OLED_CMD);//-not offset
	OLED_Write_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_Write_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_Write_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_Write_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_Write_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_Write_Byte(0x12,OLED_CMD);
	OLED_Write_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_Write_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_Write_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_Write_Byte(0x02,OLED_CMD);//
	OLED_Write_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_Write_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_Write_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_Write_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_Write_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_Write_Byte(0xAF,OLED_CMD); /*display ON*/ 

	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  



