//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103ZET6,正点原子ELITE STM32开发板,主频72MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 FSMC
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/09
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VDD          接          DC5V           //电源
//      GND          接          GND            //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为16位并口总线
//     LCD模块                STM32单片机      _
//			 DB0          接          PD14           |
//			 DB1          接          PD15           | 
//       DB2          接          PD0            |
//       DB3          接          PD1            |
//       DB4          接          PE7            |
//       DB5          接          PE8            |
//       DB6          接          PE9            |
//       DB7          接          PE10           |==>>液晶屏16位并口数据信号(8位模式只使用DB0~DB7)
//       DB8          接          PE11           | 
//       DB9          接          PE12           |
//       DB10         接          PE13           |
//       DB11         接          PE14           |
//       DB12         接          PE15           |
//       DB13         接          PD8            |
//       DB14         接          PD9            |
//       DB15         接          PD10          _|
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 				        STM32单片机 
//       WR          接          PD5           //液晶屏写数据控制信号
//       RD          接          PD4           //液晶屏读数据控制信号
//       RS          接          PG0           //液晶屏数据/命令控制信号
//       RST         接        复位引脚        //液晶屏复位控制信号(默认使用IC复位引脚，如需GPIO控制，请接PD11)
//       CS          接          PG12          //液晶屏片选控制信号

//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//      T_IRQ        接          PF10          //触摸屏触摸中断信号
//       MISO        接          PB2           //触摸屏SPI总线读信号
//       MOSI        接          PF9           //触摸屏SPI总线写信号
//       T_CS        接          PF11          //触摸屏片选控制信号
//       CLK         接          PB1           //触摸屏SPI总线时钟信号
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/		
#include "lcd.h"
#include "stdlib.h"
#include "delay.h"	 

	   
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 

/*****************************************************************************
 * @name       :void LCD_WR_REG(u16 data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u16 data)
{ 
#if LCD_USE8BIT_MODEL	
	 LCD->LCD_REG = data>>8;
	 LCD->LCD_REG = data;   
#else
	 LCD->LCD_REG = data;  
#endif
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u16 data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u16 data)
{
	data=data;			//使用-O2优化的时候,必须插入的延时
#if LCD_USE8BIT_MODEL
	LCD->LCD_RAM=0x00;
	LCD->LCD_RAM=data;
#else
	LCD->LCD_RAM=data;
#endif
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
#if LCD_USE8BIT_MODEL
	LCD->LCD_REG = LCD_Reg>>8;
	LCD->LCD_REG = LCD_Reg;
	LCD->LCD_RAM = 0x00;
	LCD->LCD_RAM = LCD_RegValue;
#else
	LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号
	LCD->LCD_RAM = LCD_RegValue;//写入数据	     		
#endif	
}	   

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{	
#if LCD_USE8BIT_MODEL	
	 LCD->LCD_RAM = Data>>8;
	 LCD->LCD_RAM = Data;    //写十六位颜色值
#else
	 LCD->LCD_RAM = Data;  //写十六位颜色值
#endif
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
  unsigned int i; 
	u32 total_point=lcddev.width*lcddev.height;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	for(i=0;i<total_point;i++)
	{ 
#if LCD_USE8BIT_MODEL	
	 LCD->LCD_RAM = Color>>8;
	 LCD->LCD_RAM = Color;    //写十六位颜色值
#else
		LCD->LCD_RAM = Color;
#endif
	}
} 

/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;
	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//使能FSMC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG,ENABLE);//使能PORTB,D,E,G以及AFIO复用功能时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PD11 推挽输出 复位
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_11);
	
 	//PORTD复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
  	 
	//PORTE复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);    	    	 											 

   	//	//PORTG12复用推挽输出 A0	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;	 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure); 

	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
  readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
  readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
  readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
  readWriteTiming.FSMC_CLKDivision = 0x00;
  readWriteTiming.FSMC_DataLatency = 0x00;
  readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
    

	writeTiming.FSMC_AddressSetupTime = 0x00;	 //地址建立时间（ADDSET）为1个HCLK  
  writeTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A		
  writeTiming.FSMC_DataSetupTime = 0x03;		 ////数据保存时间为4个HCLK	
  writeTiming.FSMC_BusTurnAroundDuration = 0x00;
  writeTiming.FSMC_CLKDivision = 0x00;
  writeTiming.FSMC_DataLatency = 0x00;
  writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 

 
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
  FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM  
#if LCD_USE8BIT_MODEL 	
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;//存储器数据宽度为8bit 
#else
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit 
#endif  
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

 	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // 使能BANK1 	
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST=0;
	delay_ms(100);	
	LCD_RST=1;
	delay_ms(50);
}

/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{  
	#if LCD_USE8BIT_MODEL	
	u8 init_flag=0;
	#endif
	LCD_GPIOInit();//LCD GPIO初始化	
	#if LCD_USE8BIT_MODEL		
Init:
	#endif
	delay_ms(50); //必须加延时，不然reset后屏底部会出现横条纹
	//LCD_RESET();    //如果不使用开发板复位引脚，则调用此复位函数
//************* NT35510初始化**********//	
	LCD_WR_REG(0xF000); LCD_WR_DATA(0x55);
			LCD_WR_REG(0xF001); LCD_WR_DATA(0xAA);
			LCD_WR_REG(0xF002); LCD_WR_DATA(0x52);
			LCD_WR_REG(0xF003); LCD_WR_DATA(0x08);
			LCD_WR_REG(0xF004); LCD_WR_DATA(0x01);
			//# AVDD: manual); LCD_WR_DATA(
			LCD_WR_REG(0xB600); LCD_WR_DATA(0x34);
			LCD_WR_REG(0xB601); LCD_WR_DATA(0x34);
			LCD_WR_REG(0xB602); LCD_WR_DATA(0x34);

			LCD_WR_REG(0xB000); LCD_WR_DATA(0x0D);//09
			LCD_WR_REG(0xB001); LCD_WR_DATA(0x0D);
			LCD_WR_REG(0xB002); LCD_WR_DATA(0x0D);
			//# AVEE: manual); LCD_WR_DATA( -6V
			LCD_WR_REG(0xB700); LCD_WR_DATA(0x24);
			LCD_WR_REG(0xB701); LCD_WR_DATA(0x24);
			LCD_WR_REG(0xB702); LCD_WR_DATA(0x24);

			LCD_WR_REG(0xB100); LCD_WR_DATA(0x0D);
			LCD_WR_REG(0xB101); LCD_WR_DATA(0x0D);
			LCD_WR_REG(0xB102); LCD_WR_DATA(0x0D);
			//#Power Control for
			//VCL
			LCD_WR_REG(0xB800); LCD_WR_DATA(0x24);
			LCD_WR_REG(0xB801); LCD_WR_DATA(0x24);
			LCD_WR_REG(0xB802); LCD_WR_DATA(0x24);

			LCD_WR_REG(0xB200); LCD_WR_DATA(0x00);

			//# VGH: Clamp Enable); LCD_WR_DATA(
			LCD_WR_REG(0xB900); LCD_WR_DATA(0x24);
			LCD_WR_REG(0xB901); LCD_WR_DATA(0x24);
			LCD_WR_REG(0xB902); LCD_WR_DATA(0x24);

			LCD_WR_REG(0xB300); LCD_WR_DATA(0x05);
			LCD_WR_REG(0xB301); LCD_WR_DATA(0x05);
			LCD_WR_REG(0xB302); LCD_WR_DATA(0x05);

			///LCD_WR_REG(0xBF00); LCD_WR_DATA(0x01);

			//# VGL(LVGL):
			LCD_WR_REG(0xBA00); LCD_WR_DATA(0x34);
			LCD_WR_REG(0xBA01); LCD_WR_DATA(0x34);
			LCD_WR_REG(0xBA02); LCD_WR_DATA(0x34);
			//# VGL_REG(VGLO)
			LCD_WR_REG(0xB500); LCD_WR_DATA(0x0B);
			LCD_WR_REG(0xB501); LCD_WR_DATA(0x0B);
			LCD_WR_REG(0xB502); LCD_WR_DATA(0x0B);
			//# VGMP/VGSP:
			LCD_WR_REG(0xBC00); LCD_WR_DATA(0X00);
			LCD_WR_REG(0xBC01); LCD_WR_DATA(0xA3);
			LCD_WR_REG(0xBC02); LCD_WR_DATA(0X00);
			//# VGMN/VGSN
			LCD_WR_REG(0xBD00); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xBD01); LCD_WR_DATA(0xA3);
			LCD_WR_REG(0xBD02); LCD_WR_DATA(0x00);
			//# VCOM=-0.1
			LCD_WR_REG(0xBE00); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xBE01); LCD_WR_DATA(0x63);//4f
				//  VCOMH+0x01;
			//#R+
			LCD_WR_REG(0xD100); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD101); LCD_WR_DATA(0x37);
			LCD_WR_REG(0xD102); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD103); LCD_WR_DATA(0x52);
			LCD_WR_REG(0xD104); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD105); LCD_WR_DATA(0x7B);
			LCD_WR_REG(0xD106); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD107); LCD_WR_DATA(0x99);
			LCD_WR_REG(0xD108); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD109); LCD_WR_DATA(0xB1);
			LCD_WR_REG(0xD10A); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD10B); LCD_WR_DATA(0xD2);
			LCD_WR_REG(0xD10C); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD10D); LCD_WR_DATA(0xF6);
			LCD_WR_REG(0xD10E); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD10F); LCD_WR_DATA(0x27);
			LCD_WR_REG(0xD110); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD111); LCD_WR_DATA(0x4E);
			LCD_WR_REG(0xD112); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD113); LCD_WR_DATA(0x8C);
			LCD_WR_REG(0xD114); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD115); LCD_WR_DATA(0xBE);
			LCD_WR_REG(0xD116); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD117); LCD_WR_DATA(0x0B);
			LCD_WR_REG(0xD118); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD119); LCD_WR_DATA(0x48);
			LCD_WR_REG(0xD11A); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD11B); LCD_WR_DATA(0x4A);
			LCD_WR_REG(0xD11C); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD11D); LCD_WR_DATA(0x7E);
			LCD_WR_REG(0xD11E); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD11F); LCD_WR_DATA(0xBC);
			LCD_WR_REG(0xD120); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD121); LCD_WR_DATA(0xE1);
			LCD_WR_REG(0xD122); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD123); LCD_WR_DATA(0x10);
			LCD_WR_REG(0xD124); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD125); LCD_WR_DATA(0x31);
			LCD_WR_REG(0xD126); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD127); LCD_WR_DATA(0x5A);
			LCD_WR_REG(0xD128); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD129); LCD_WR_DATA(0x73);
			LCD_WR_REG(0xD12A); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD12B); LCD_WR_DATA(0x94);
			LCD_WR_REG(0xD12C); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD12D); LCD_WR_DATA(0x9F);
			LCD_WR_REG(0xD12E); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD12F); LCD_WR_DATA(0xB3);
			LCD_WR_REG(0xD130); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD131); LCD_WR_DATA(0xB9);
			LCD_WR_REG(0xD132); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD133); LCD_WR_DATA(0xC1);
			//#G+
			LCD_WR_REG(0xD200); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD201); LCD_WR_DATA(0x37);
			LCD_WR_REG(0xD202); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD203); LCD_WR_DATA(0x52);
			LCD_WR_REG(0xD204); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD205); LCD_WR_DATA(0x7B);
			LCD_WR_REG(0xD206); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD207); LCD_WR_DATA(0x99);
			LCD_WR_REG(0xD208); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD209); LCD_WR_DATA(0xB1);
			LCD_WR_REG(0xD20A); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD20B); LCD_WR_DATA(0xD2);
			LCD_WR_REG(0xD20C); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD20D); LCD_WR_DATA(0xF6);
			LCD_WR_REG(0xD20E); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD20F); LCD_WR_DATA(0x27);
			LCD_WR_REG(0xD210); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD211); LCD_WR_DATA(0x4E);
			LCD_WR_REG(0xD212); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD213); LCD_WR_DATA(0x8C);
			LCD_WR_REG(0xD214); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD215); LCD_WR_DATA(0xBE);
			LCD_WR_REG(0xD216); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD217); LCD_WR_DATA(0x0B);
			LCD_WR_REG(0xD218); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD219); LCD_WR_DATA(0x48);
			LCD_WR_REG(0xD21A); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD21B); LCD_WR_DATA(0x4A);
			LCD_WR_REG(0xD21C); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD21D); LCD_WR_DATA(0x7E);
			LCD_WR_REG(0xD21E); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD21F); LCD_WR_DATA(0xBC);
			LCD_WR_REG(0xD220); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD221); LCD_WR_DATA(0xE1);
			LCD_WR_REG(0xD222); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD223); LCD_WR_DATA(0x10);
			LCD_WR_REG(0xD224); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD225); LCD_WR_DATA(0x31);
			LCD_WR_REG(0xD226); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD227); LCD_WR_DATA(0x5A);
			LCD_WR_REG(0xD228); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD229); LCD_WR_DATA(0x73);
			LCD_WR_REG(0xD22A); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD22B); LCD_WR_DATA(0x94);
			LCD_WR_REG(0xD22C); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD22D); LCD_WR_DATA(0x9F);
			LCD_WR_REG(0xD22E); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD22F); LCD_WR_DATA(0xB3);
			LCD_WR_REG(0xD230); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD231); LCD_WR_DATA(0xB9);
			LCD_WR_REG(0xD232); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD233); LCD_WR_DATA(0xC1);
			//#B+
			LCD_WR_REG(0xD300); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD301); LCD_WR_DATA(0x37);
			LCD_WR_REG(0xD302); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD303); LCD_WR_DATA(0x52);
			LCD_WR_REG(0xD304); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD305); LCD_WR_DATA(0x7B);
			LCD_WR_REG(0xD306); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD307); LCD_WR_DATA(0x99);
			LCD_WR_REG(0xD308); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD309); LCD_WR_DATA(0xB1);
			LCD_WR_REG(0xD30A); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD30B); LCD_WR_DATA(0xD2);
			LCD_WR_REG(0xD30C); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD30D); LCD_WR_DATA(0xF6);
			LCD_WR_REG(0xD30E); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD30F); LCD_WR_DATA(0x27);
			LCD_WR_REG(0xD310); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD311); LCD_WR_DATA(0x4E);
			LCD_WR_REG(0xD312); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD313); LCD_WR_DATA(0x8C);
			LCD_WR_REG(0xD314); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD315); LCD_WR_DATA(0xBE);
			LCD_WR_REG(0xD316); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD317); LCD_WR_DATA(0x0B);
			LCD_WR_REG(0xD318); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD319); LCD_WR_DATA(0x48);
			LCD_WR_REG(0xD31A); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD31B); LCD_WR_DATA(0x4A);
			LCD_WR_REG(0xD31C); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD31D); LCD_WR_DATA(0x7E);
			LCD_WR_REG(0xD31E); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD31F); LCD_WR_DATA(0xBC);
			LCD_WR_REG(0xD320); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD321); LCD_WR_DATA(0xE1);
			LCD_WR_REG(0xD322); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD323); LCD_WR_DATA(0x10);
			LCD_WR_REG(0xD324); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD325); LCD_WR_DATA(0x31);
			LCD_WR_REG(0xD326); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD327); LCD_WR_DATA(0x5A);
			LCD_WR_REG(0xD328); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD329); LCD_WR_DATA(0x73);
			LCD_WR_REG(0xD32A); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD32B); LCD_WR_DATA(0x94);
			LCD_WR_REG(0xD32C); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD32D); LCD_WR_DATA(0x9F);
			LCD_WR_REG(0xD32E); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD32F); LCD_WR_DATA(0xB3);
			LCD_WR_REG(0xD330); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD331); LCD_WR_DATA(0xB9);
			LCD_WR_REG(0xD332); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD333); LCD_WR_DATA(0xC1);

			//#R-///////////////////////////////////////////
			LCD_WR_REG(0xD400); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD401); LCD_WR_DATA(0x37);
			LCD_WR_REG(0xD402); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD403); LCD_WR_DATA(0x52);
			LCD_WR_REG(0xD404); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD405); LCD_WR_DATA(0x7B);
			LCD_WR_REG(0xD406); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD407); LCD_WR_DATA(0x99);
			LCD_WR_REG(0xD408); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD409); LCD_WR_DATA(0xB1);
			LCD_WR_REG(0xD40A); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD40B); LCD_WR_DATA(0xD2);
			LCD_WR_REG(0xD40C); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD40D); LCD_WR_DATA(0xF6);
			LCD_WR_REG(0xD40E); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD40F); LCD_WR_DATA(0x27);
			LCD_WR_REG(0xD410); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD411); LCD_WR_DATA(0x4E);
			LCD_WR_REG(0xD412); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD413); LCD_WR_DATA(0x8C);
			LCD_WR_REG(0xD414); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD415); LCD_WR_DATA(0xBE);
			LCD_WR_REG(0xD416); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD417); LCD_WR_DATA(0x0B);
			LCD_WR_REG(0xD418); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD419); LCD_WR_DATA(0x48);
			LCD_WR_REG(0xD41A); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD41B); LCD_WR_DATA(0x4A);
			LCD_WR_REG(0xD41C); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD41D); LCD_WR_DATA(0x7E);
			LCD_WR_REG(0xD41E); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD41F); LCD_WR_DATA(0xBC);
			LCD_WR_REG(0xD420); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD421); LCD_WR_DATA(0xE1);
			LCD_WR_REG(0xD422); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD423); LCD_WR_DATA(0x10);
			LCD_WR_REG(0xD424); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD425); LCD_WR_DATA(0x31);
			LCD_WR_REG(0xD426); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD427); LCD_WR_DATA(0x5A);
			LCD_WR_REG(0xD428); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD429); LCD_WR_DATA(0x73);
			LCD_WR_REG(0xD42A); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD42B); LCD_WR_DATA(0x94);
			LCD_WR_REG(0xD42C); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD42D); LCD_WR_DATA(0x9F);
			LCD_WR_REG(0xD42E); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD42F); LCD_WR_DATA(0xB3);
			LCD_WR_REG(0xD430); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD431); LCD_WR_DATA(0xB9);
			LCD_WR_REG(0xD432); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD433); LCD_WR_DATA(0xC1);

			//#G-//////////////////////////////////////////////
			LCD_WR_REG(0xD500); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD501); LCD_WR_DATA(0x37);
			LCD_WR_REG(0xD502); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD503); LCD_WR_DATA(0x52);
			LCD_WR_REG(0xD504); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD505); LCD_WR_DATA(0x7B);
			LCD_WR_REG(0xD506); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD507); LCD_WR_DATA(0x99);
			LCD_WR_REG(0xD508); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD509); LCD_WR_DATA(0xB1);
			LCD_WR_REG(0xD50A); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD50B); LCD_WR_DATA(0xD2);
			LCD_WR_REG(0xD50C); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD50D); LCD_WR_DATA(0xF6);
			LCD_WR_REG(0xD50E); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD50F); LCD_WR_DATA(0x27);
			LCD_WR_REG(0xD510); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD511); LCD_WR_DATA(0x4E);
			LCD_WR_REG(0xD512); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD513); LCD_WR_DATA(0x8C);
			LCD_WR_REG(0xD514); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD515); LCD_WR_DATA(0xBE);
			LCD_WR_REG(0xD516); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD517); LCD_WR_DATA(0x0B);
			LCD_WR_REG(0xD518); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD519); LCD_WR_DATA(0x48);
			LCD_WR_REG(0xD51A); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD51B); LCD_WR_DATA(0x4A);
			LCD_WR_REG(0xD51C); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD51D); LCD_WR_DATA(0x7E);
			LCD_WR_REG(0xD51E); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD51F); LCD_WR_DATA(0xBC);
			LCD_WR_REG(0xD520); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD521); LCD_WR_DATA(0xE1);
			LCD_WR_REG(0xD522); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD523); LCD_WR_DATA(0x10);
			LCD_WR_REG(0xD524); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD525); LCD_WR_DATA(0x31);
			LCD_WR_REG(0xD526); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD527); LCD_WR_DATA(0x5A);
			LCD_WR_REG(0xD528); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD529); LCD_WR_DATA(0x73);
			LCD_WR_REG(0xD52A); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD52B); LCD_WR_DATA(0x94);
			LCD_WR_REG(0xD52C); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD52D); LCD_WR_DATA(0x9F);
			LCD_WR_REG(0xD52E); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD52F); LCD_WR_DATA(0xB3);
			LCD_WR_REG(0xD530); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD531); LCD_WR_DATA(0xB9);
			LCD_WR_REG(0xD532); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD533); LCD_WR_DATA(0xC1);
			//#B-///////////////////////////////
			LCD_WR_REG(0xD600); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD601); LCD_WR_DATA(0x37);
			LCD_WR_REG(0xD602); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD603); LCD_WR_DATA(0x52);
			LCD_WR_REG(0xD604); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD605); LCD_WR_DATA(0x7B);
			LCD_WR_REG(0xD606); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD607); LCD_WR_DATA(0x99);
			LCD_WR_REG(0xD608); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD609); LCD_WR_DATA(0xB1);
			LCD_WR_REG(0xD60A); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD60B); LCD_WR_DATA(0xD2);
			LCD_WR_REG(0xD60C); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xD60D); LCD_WR_DATA(0xF6);
			LCD_WR_REG(0xD60E); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD60F); LCD_WR_DATA(0x27);
			LCD_WR_REG(0xD610); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD611); LCD_WR_DATA(0x4E);
			LCD_WR_REG(0xD612); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD613); LCD_WR_DATA(0x8C);
			LCD_WR_REG(0xD614); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xD615); LCD_WR_DATA(0xBE);
			LCD_WR_REG(0xD616); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD617); LCD_WR_DATA(0x0B);
			LCD_WR_REG(0xD618); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD619); LCD_WR_DATA(0x48);
			LCD_WR_REG(0xD61A); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD61B); LCD_WR_DATA(0x4A);
			LCD_WR_REG(0xD61C); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD61D); LCD_WR_DATA(0x7E);
			LCD_WR_REG(0xD61E); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD61F); LCD_WR_DATA(0xBC);
			LCD_WR_REG(0xD620); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xD621); LCD_WR_DATA(0xE1);
			LCD_WR_REG(0xD622); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD623); LCD_WR_DATA(0x10);
			LCD_WR_REG(0xD624); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD625); LCD_WR_DATA(0x31);
			LCD_WR_REG(0xD626); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD627); LCD_WR_DATA(0x5A);
			LCD_WR_REG(0xD628); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD629); LCD_WR_DATA(0x73);
			LCD_WR_REG(0xD62A); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD62B); LCD_WR_DATA(0x94);
			LCD_WR_REG(0xD62C); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD62D); LCD_WR_DATA(0x9F);
			LCD_WR_REG(0xD62E); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD62F); LCD_WR_DATA(0xB3);
			LCD_WR_REG(0xD630); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD631); LCD_WR_DATA(0xB9);
			LCD_WR_REG(0xD632); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xD633); LCD_WR_DATA(0xC1);



			//#Enable Page0
			LCD_WR_REG(0xF000); LCD_WR_DATA(0x55);
			LCD_WR_REG(0xF001); LCD_WR_DATA(0xAA);
			LCD_WR_REG(0xF002); LCD_WR_DATA(0x52);
			LCD_WR_REG(0xF003); LCD_WR_DATA(0x08);
			LCD_WR_REG(0xF004); LCD_WR_DATA(0x00);
			//# RGB I/F Setting
			LCD_WR_REG(0xB000); LCD_WR_DATA(0x08);
			LCD_WR_REG(0xB001); LCD_WR_DATA(0x05);
			LCD_WR_REG(0xB002); LCD_WR_DATA(0x02);
			LCD_WR_REG(0xB003); LCD_WR_DATA(0x05);
			LCD_WR_REG(0xB004); LCD_WR_DATA(0x02);
			//## SDT:
			LCD_WR_REG(0xB600); LCD_WR_DATA(0x08);
			LCD_WR_REG(0xB500); LCD_WR_DATA(0x50);//0x6b ???? 480x854       0x50 ???? 480x800

			//## Gate EQ:
			LCD_WR_REG(0xB700); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xB701); LCD_WR_DATA(0x00);

			//## Source EQ:
			LCD_WR_REG(0xB800); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xB801); LCD_WR_DATA(0x05);
			LCD_WR_REG(0xB802); LCD_WR_DATA(0x05);
			LCD_WR_REG(0xB803); LCD_WR_DATA(0x05);

			//# Inversion: Column inversion (NVT)
			LCD_WR_REG(0xBC00); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xBC01); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xBC02); LCD_WR_DATA(0x00);

			//# BOE's Setting(default)
			LCD_WR_REG(0xCC00); LCD_WR_DATA(0x03);
			LCD_WR_REG(0xCC01); LCD_WR_DATA(0x00);
			LCD_WR_REG(0xCC02); LCD_WR_DATA(0x00);

			//# Display Timing:
			LCD_WR_REG(0xBD00); LCD_WR_DATA(0x01);
			LCD_WR_REG(0xBD01); LCD_WR_DATA(0x84);
			LCD_WR_REG(0xBD02); LCD_WR_DATA(0x07);
			LCD_WR_REG(0xBD03); LCD_WR_DATA(0x31);
			LCD_WR_REG(0xBD04); LCD_WR_DATA(0x00);

			LCD_WR_REG(0xBA00); LCD_WR_DATA(0x01);

			LCD_WR_REG(0xFF00); LCD_WR_DATA(0xAA);
			LCD_WR_REG(0xFF01); LCD_WR_DATA(0x55);
			LCD_WR_REG(0xFF02); LCD_WR_DATA(0x25);
			LCD_WR_REG(0xFF03); LCD_WR_DATA(0x01);

			LCD_WR_REG(0x3500); LCD_WR_DATA(0x00);
			LCD_WR_REG(0x3600); LCD_WR_DATA(0x00);
			LCD_WR_REG(0x3a00); LCD_WR_DATA(0x55);  ////55=16?/////66=18?
			LCD_WR_REG(0x1100);
			delay_ms(120); 
			LCD_WR_REG(0x2900 ); 
			LCD_WR_REG(0x2c00);
	#if LCD_USE8BIT_MODEL	 //8位模式下，LCD初始化两遍，防止reset或者断电重启后，显示异常
	if(init_flag==0)
	{
		init_flag=1;
		goto Init;
	}
	#endif
  LCD_direction(USE_HORIZONTAL);//设置LCD显示方向 
	LCD_Clear(WHITE);//清全屏白色
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(xStar>>8);  
	LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(xStar&0XFF);	  
	LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA(xEnd>>8);   
	LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA(xEnd&0XFF);   
	LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(yStar>>8);   
	LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(yStar&0XFF);  
	LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA(yEnd>>8);   
	LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA(yEnd&0XFF);

	LCD_WriteRAM_Prepare();	//开始写入GRAM			
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
	lcddev.setxcmd=0x2A00;
	lcddev.setycmd=0x2B00;
	lcddev.wramcmd=0x2C00;
	lcddev.rramcmd=0x2E00;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;		
			LCD_WriteReg(0x3600,0x00);
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x3600,(1<<5)|(1<<6));
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x3600,(1<<7)|(1<<6));
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x3600,(1<<7)|(1<<5));
		break;	
		default:break;
	}	
}	 
