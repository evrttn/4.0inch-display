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
#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//使用SysTick的普通计数模式对延迟进行管理
//包括delay_us,delay_ms
//V1.2修改说明
//修正了中断中调用出现死循环的错误
//防止延时不准确,采用do while结构!
//////////////////////////////////////////////////////////////////////////////////	 
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























