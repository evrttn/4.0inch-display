//////////////////////////////////////////////////////////////////////////////////	 	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STC89C52RC,����30M  ��Ƭ��������ѹ3.3V
//QDtech-LCDҺ������ for C51
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/7/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
//********************************************************************************
//=========================================��Դ����================================================//
//VDD��DC 5V��3.3V��Դ
//GND�ӵ�
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ16λ����
//Һ����ģ��             C51��Ƭ��
// DB0~DB7         ��       P30~P37        //���ݵ�8λ(8λģʽ��DB0~DB7)
// DB8~DB15        ��       P20~P27        //���ݸ�8λ
//=======================================Һ���������߽���==========================================//
//Һ����ģ��             C51��Ƭ��
//   CS          ��        P13           //Ƭѡ�����ź�
//   RST         ��        P14           //��λ�ź�
//   RS          ��        P12           //����/����ѡ������ź�
//   WR          ��        P11           //д�����ź�
//   RD          ��        P10           //�������ź�

//=========================================����������=========================================//
//STC89C52RC��Ƭ��ROM̫С���޷����д��������ܵĳ������Դ���������Ҫ����
//**************************************************************************************************/				
#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"

/////////////////////////////////////�û�������///////////////////////////////////	 
//֧�ֺ��������ٶ����л�
#define USE_HORIZONTAL  	  0   //����Һ����˳ʱ����ת���� 	0-0����ת��1-90����ת��2-180����ת��3-270����ת
#define LCD_USE8BIT_MODEL   0 	//�������������Ƿ�ʹ��8λģʽ 0,ʹ��16λģʽ.1,ʹ��8λģʽ
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////	  
//����LCD�ĳߴ�
#define LCD_W 480
#define LCD_H 800

//IO����
#define  LCD_DataPortH P2     //��8λ���ݿ�,8λģʽ��ֻʹ�ø�8λ 
#define  LCD_DataPortL P3     //��8λ���ݿ�	,8λģʽ�µ�8λ���Բ�����,��ȷ��P0���Ѿ�����10K����,����̫С����С4.7K,�Ƽ�10K.
sbit LCD_RS = P1^2;  		 //����/�����л�
sbit LCD_WR = P1^1;		  //д����
sbit LCD_RD = P1^0;		     //������
sbit LCD_CS = P1^3;		//Ƭѡ	
sbit LCD_RESET = P1^4;	      //��λ 
//sbit LCD_BL=P1^5;		//������ƣ��������Ҫ���ƣ���3.3V

//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	 wramcmd;		//��ʼдgramָ��
	u16  setxcmd;		//����x����ָ��
	u16  setycmd;		//����y����ָ��	 
}_lcd_dev; 	

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
void LCD_Init(void); 
void LCD_Clear(u16 Color);
void spi_write_byte(u8 d);  //ͨ��SPIд��һ���ֽڵ�����
void LCD_WR_DATA(u16 Data);
void LCD_WR_REG(u16 Reg);
void LCD_SetCursor(u16 Xpos, u16 Ypos);//���ù��λ��
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);//������ʾ����
void LCD_DrawPoint(u16 x,u16 y);//����
void LCD_WriteRAM_Prepare(void);
void LCD_direction(u8 direction );
void LCD_WR_DATA_16Bit(u16 Data);


//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


					  		 
#endif  
	 
	 



