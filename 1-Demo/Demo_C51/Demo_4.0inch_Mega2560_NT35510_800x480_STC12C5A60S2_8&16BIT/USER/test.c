//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STC12C5A60S2,晶振30M  单片机工作电压3.3V
//QDtech-LCD液晶驱动 for C51
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
//创建日期:2018/7/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
//********************************************************************************
//=========================================电源接线================================================//
//VDD接DC 5V
//GND接地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为16位并口
//液晶屏模块             C51单片机
// DB0~DB7       接       P00~P07        //数据低8位(8位模式接DB0~DB7)
// DB8~DB15      接       P20~P27        //数据高8位
//=======================================液晶屏控制线接线==========================================//
//液晶屏模块             C51单片机
//   CS          接        P13           //片选控制信号
//   RST         接        P33           //复位信号
//   RS          接        P12           //数据/命令选择控制信号
//   WR          接        P11           //写控制信号
//   RD          接        P10           //读控制信号

//=========================================触摸屏接线=========================================//
//不使用触摸或者模块本身不带触摸，则可不连接
//触摸屏使用的数据总线类型为SPI
//触摸屏模块             C51单片机
//  CLK          接        P36           //触摸屏SPI时钟信号
//  T_CS         接        P37           //触摸屏片选控制信号
//  MISO         接        P35           //触摸屏SPI写信号
//  MOSI         接        P34           //触摸屏SPI读信号
//  T_IRQ        接        P40           //触摸屏响应检查信号，如单片机无P4组，请自行更改其他可用IO并修改代码定义
//**************************************************************************************************/		
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include <string.h>
#include "sys.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include "pic.h"

//========================variable==========================//
u16 ColorTab[5]={RED,GREEN,BLUE,YELLOW,BRED};//定义颜色数组
u16 ColornTab[8]={RED,MAGENTA,GREEN,DARKBLUE,BLUE,BLACK,LIGHTGREEN};
//=====================end of variable======================//

/*****************************************************************************
 * @name       :void DrawTestPage(u8 *str)
 * @date       :2018-08-09 
 * @function   :Drawing test interface
 * @parameters :str:the start address of the Chinese and English strings
 * @retvalue   :None
******************************************************************************/ 
void DrawTestPage(u8 *str)
{
POINT_COLOR=WHITE;
//绘制固定栏up
LCD_Fill(0,0,lcddev.width,20,BLUE);
Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//居中显示
//绘制固定栏down
LCD_Fill(0,lcddev.height-20,lcddev.width-1,lcddev.height-1,BLUE);
Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"http://www.lcdwiki.com",16,1);//居中显示
//绘制测试区域
LCD_Fill(0,21,lcddev.width-1,lcddev.height-21,WHITE);
}

/*****************************************************************************
 * @name       :void main_test(void)
 * @date       :2018-08-09 
 * @function   :Drawing the main Interface of the Comprehensive Test Program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void main_test(void)
{
	DrawTestPage("全动电子综合测试程序");
	
	Gui_StrCenter(0,30,RED,BLUE,"全动电子",16,1);//居中显示
	Gui_StrCenter(0,60,RED,BLUE,"综合测试程序",16,1);//居中显示	
	Gui_StrCenter(0,90,MAGENTA,BLUE,"4.0\" Mega2560 NT35510 480X800",16,1);//居中显示
	Gui_StrCenter(0,120,BLUE,BLUE,"xiaoFeng@QDtech 2022-03-02",16,1);//居中显示
	delay_ms(1500);		
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Color(void)
 * @date       :2018-08-09 
 * @function   :Color fill test(white,black,red,green,blue)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Color(void)
{
	//DrawTestPage("测试1:纯色填充测试");
	LCD_Fill(0,0,lcddev.width-1,lcddev.height-1,WHITE);
	Show_Str(20,30,BLUE,YELLOW,"BL Test",16,1);delay_ms(800);
	LCD_Fill(0,0,lcddev.width-1,lcddev.height-1,RED);
	Show_Str(20,30,BLUE,YELLOW,"RED ",16,1);delay_ms(800);
	LCD_Fill(0,0,lcddev.width-1,lcddev.height-1,GREEN);
	Show_Str(20,30,BLUE,YELLOW,"GREEN ",16,1);delay_ms(800);
	LCD_Fill(0,0,lcddev.width-1,lcddev.height-1,BLUE);
	Show_Str(20,30,RED,YELLOW,"BLUE ",16,1);delay_ms(800);
}

/*****************************************************************************
 * @name       :void Test_FillRec(void)
 * @date       :2018-08-09 
 * @function   :Rectangular display and fill test
								Display red,green,blue,yellow,pink rectangular boxes in turn,
								1500 milliseconds later,
								Fill the rectangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_FillRec(void)
{
	u8 i=0;
	DrawTestPage("测试2:GUI矩形填充测试");
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		LCD_DrawRectangle(lcddev.width/2-110+(i*30),lcddev.height/2-110+(i*30),lcddev.width/2-110+(i*30)+100,lcddev.height/2-110+(i*30)+100); 
	}
	delay_ms(1500);	
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		LCD_DrawFillRectangle(lcddev.width/2-110+(i*30),lcddev.height/2-110+(i*30),lcddev.width/2-110+(i*30)+100,lcddev.height/2-110+(i*30)+100); 
	}
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Circle(void)
 * @date       :2018-08-09 
 * @function   :circular display and fill test
								Display red,green,blue,yellow,pink circular boxes in turn,
								1500 milliseconds later,
								Fill the circular in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Circle(void)
{
	u8 i=0;
	DrawTestPage("测试3:GUI画圆填充测试");
	for (i=0; i<5; i++)  
		gui_circle(lcddev.width/2-70+(i*35),lcddev.height/2-70+(i*35),ColorTab[i],50,0);
	delay_ms(1500);	
	for (i=0; i<5; i++) 
	  	gui_circle(lcddev.width/2-70+(i*35),lcddev.height/2-70+(i*35),ColorTab[i],50,1);
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Triangle(void)
 * @date       :2018-08-09 
 * @function   :triangle display and fill test
								Display red,green,blue,yellow,pink triangle boxes in turn,
								1500 milliseconds later,
								Fill the triangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Triangle(void)
{
	u8 i=0;
	DrawTestPage("测试4:GUI Triangle填充测试");
	for(i=0;i<5;i++)
	{
		POINT_COLOR=ColorTab[i];
		Draw_Triangel(lcddev.width/2-110+(i*30),lcddev.height/2-17+(i*30),lcddev.width/2-60-1+(i*30),lcddev.height/2-103+(i*30),lcddev.width/2-10-1+(i*30),lcddev.height/2-17+(i*30));
	}
	delay_ms(1500);	
	for(i=0;i<5;i++)
	{
		POINT_COLOR=ColorTab[i];
		Fill_Triangel(lcddev.width/2-110+(i*30),lcddev.height/2-17+(i*30),lcddev.width/2-60-1+(i*30),lcddev.height/2-103+(i*30),lcddev.width/2-10-1+(i*30),lcddev.height/2-17+(i*30));
	}
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void English_Font_test(void)
 * @date       :2018-08-09 
 * @function   :English display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void English_Font_test(void)
{
	DrawTestPage("测试5:英文显示测试");
	POINT_COLOR=RED;
	BACK_COLOR=BLUE;
	LCD_ShowString(10,30,12,"6X12:abcdefghijklmnopqrstuvwxyz",0);
	LCD_ShowString(10,45,12,"6X12:ABCDEFGHIJKLMNOP0123456789",1);
	LCD_ShowString(10,60,12,"6X12:~!@#$%^&*()_+{}:<>?/|-+.",0);
	LCD_ShowString(10,80,16,"8X16:abcdefghijklmnopqrstuvw",0);
	LCD_ShowString(10,100,16,"8X16:ABCDEFGHIJKLMN01234567",1);
	LCD_ShowString(10,120,16,"8X16:~!@#$%^&*()_+{}:<>?/|",0); 
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Chinese_Font_test(void)
 * @date       :2018-08-09 
 * @function   :chinese display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Chinese_Font_test(void)
{	
	DrawTestPage("测试6:中文显示测试");
	Show_Str(10,30,BLUE,YELLOW,"16X16:全动电子技术有限公司欢迎您",16,0);
	Show_Str(10,50,BLUE,YELLOW,"16X16:Welcome全动电子",16,0);
	Show_Str(10,70,BLUE,YELLOW,"24X24:深圳市中文测试",24,1);
	Show_Str(10,100,BLUE,YELLOW,"32X32:字体测试",32,1);
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Pic_test(void)
 * @date       :2018-08-09 
 * @function   :picture display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Pic_test(void)
{
	DrawTestPage("测试7:图片显示测试");
	Gui_Drawbmp16(30,30,40,40,gImage_qq);
	Show_Str(30+12,75,BLUE,YELLOW,"QQ",16,1);
	Gui_Drawbmp16(90,30,40,40,gImage_qq);
	Show_Str(90+12,75,BLUE,YELLOW,"QQ",16,1);
	Gui_Drawbmp16(150,30,40,40,gImage_qq);
	Show_Str(150+12,75,BLUE,YELLOW,"QQ",16,1);
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Rotate_Test(void)
 * @date       :2018-08-09 
 * @function   :rotate test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Rotate_Test(void)
{
	u8 i=0;
	u8 *Direction[4]={"Rotation:0","Rotation:90","Rotation:180","Rotation:270"};
	
	for(i=0;i<4;i++)
	{
	 LCD_Clear(WHITE);
	LCD_direction(i);
	DrawTestPage("测试9:屏幕角度测试");
	Show_Str(20,30,BLUE,YELLOW,Direction[i],16,1);
	Gui_Drawbmp16(30,50,40,40,gImage_qq);
	delay_ms(1000);delay_ms(1000);
	}
	LCD_direction(USE_HORIZONTAL);
}

/*****************************************************************************
 * @name       :void Touch_Test(void)
 * @date       :2018-08-09 
 * @function   :touch test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Touch_Test(void)
{ 
	u8 i=0;
	u16 j=0;
	u16 colorTemp=RED;
	DrawTestPage("测试10:Touch测试  ");
	LCD_ShowString(lcddev.width-24,0,16,"RST",1);//显示清屏区域
	LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,RED); 
	POINT_COLOR=RED;
	while(1)
	{	 	 
		if(Convert_Pos())			//触摸屏被按下
		{	
		 	if(tp_pixlcd.x<lcddev.width&&tp_pixlcd.y<lcddev.height)
			{	
				if(tp_pixlcd.x>(lcddev.width-24)&&tp_pixlcd.y<16)
				{
					DrawTestPage("测试10:Touch测试  ");//清除
					LCD_ShowString(lcddev.width-24,0,16,"RST",1);//显示清屏区域
					POINT_COLOR=colorTemp;
					LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,POINT_COLOR); 
				}
				else if((tp_pixlcd.x>(lcddev.width-60)&&tp_pixlcd.x<(lcddev.width-50+20))&&tp_pixlcd.y<20)
				{
				LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,ColorTab[j%5]); 
				POINT_COLOR=ColorTab[(j++)%5];
				colorTemp=POINT_COLOR;
				delay_ms(10);
				}

				else 
				TP_Draw_Big_Point(tp_pixlcd.x,tp_pixlcd.y,POINT_COLOR);    			   
			}
		}else delay_ms(10);	//没有按键按下的时候 	    				
	}   
}

/*****************************************************************************
 * @name       :void Test_Dynamic_Num(void)
 * @date       :2018-11-13 
 * @function   :Dynamic number test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Dynamic_Num(void)
{
		u8 i;
	srand(123456);
	DrawTestPage("测试8:动态显示");
	POINT_COLOR=BLUE;
	LCD_ShowString(15,50,16, " HCHO:           ug/m3",1);
	LCD_ShowString(15,70,16, "  CO2:           ppm",1);
	LCD_ShowString(15,90,16, " TVOC:           ug/m3",1);
	LCD_ShowString(15,110,16,"PM2.5:           ug/m3",1);
	LCD_ShowString(15,130,16," PM10:           ug/m3",1);
	LCD_ShowString(15,150,16,"  TEP:           C",1);
	LCD_ShowString(15,170,16,"  HUM:           %",1);
	POINT_COLOR=RED;
	BACK_COLOR=WHITE;
	for(i=0;i<15;i++)
	{
		LCD_ShowNum(100,50,rand()%10000,5,16);
		LCD_ShowNum(100,70,rand()%10000,5,16);
		LCD_ShowNum(100,90,rand()%10000,5,16);
		LCD_ShowNum(100,110,rand()%10000,5,16);
		LCD_ShowNum(100,130,rand()%10000,5,16);
		LCD_ShowNum(100,150,rand()%50,5,16);
		LCD_ShowNum(100,170,rand()%100,5,16);
		delay_ms(500);
	}
}

