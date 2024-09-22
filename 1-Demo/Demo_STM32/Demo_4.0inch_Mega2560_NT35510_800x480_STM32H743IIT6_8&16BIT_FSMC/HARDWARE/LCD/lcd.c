//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32H743IIT6,����ԭ��Apollo STM32F4/F7������,��Ƶ400MHZ������12MHZ
//QDtech-TFTҺ������ for STM32 FSMC
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
//��������:2018/08/09
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
//     LCDģ��                STM32��Ƭ��
//      VDD          ��          DC5V           //��Դ
//      GND          ��          GND            //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ16λ��������
//     LCDģ��                STM32��Ƭ��      _
//			 DB0          ��          PD14           |
//			 DB1          ��          PD15           | 
//       DB2          ��          PD0            |
//       DB3          ��          PD1            |
//       DB4          ��          PE7            |
//       DB5          ��          PE8            |
//       DB6          ��          PE9            |
//       DB7          ��          PE10           |==>>Һ����16λ���������ź�(8Ϊģʽʹ��DB0~DB7)
//       DB8          ��          PE11           | 
//       DB9          ��          PE12           |
//       DB10         ��          PE13           |
//       DB11         ��          PE14           |
//       DB12         ��          PE15           |
//       DB13         ��          PD8            |
//       DB14         ��          PD9            |
//       DB15         ��          PD10          _|
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 				        STM32��Ƭ�� 
//       WR          ��          PD5           //Һ����д���ݿ����ź�
//       RD          ��          PD4           //Һ���������ݿ����ź�
//       RS          ��          PD13          //Һ��������/��������ź�
//       RST         ��        ��λ����        //Һ������λ�����ź�(Ĭ��ʹ��IC��λ���ţ�����ʹ��GPIO�����PB8)
//       CS          ��          PD7           //Һ����Ƭѡ�����ź�

//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��                STM32��Ƭ�� 
//       T_IRQ       ��          PH7           //�����������ж��ź�
//       MISO        ��          PG3           //������SPI���߶��ź�
//       MOSI        ��          PI3           //������SPI����д�ź�
//       T_CS        ��          PI8           //������Ƭѡ�����ź�
//       CLK         ��          PH6           //������SPI����ʱ���ź�
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

SRAM_HandleTypeDef SRAM_Handler;    //SRAM���(���ڿ���LCD)
	   
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//������ɫ,������ɫ
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 

/*****************************************************************************
 * @name       :void LCD_WR_REG(u16 data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(vu16 data)
{ 
	data=data;  //ʹ��-O2�Ż���ʱ��,����������ʱ
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
void LCD_WR_DATA(vu16 data)
{
	data=data;			//ʹ��-O2�Ż���ʱ��,����������ʱ
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
	LCD->LCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������
	LCD->LCD_RAM = LCD_RegValue;//д������	     		
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
	 LCD->LCD_RAM = Data;    //дʮ��λ��ɫֵ
#else
	 LCD->LCD_RAM = Data;  //дʮ��λ��ɫֵ
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
	LCD_SetCursor(x,y);//���ù��λ�� 
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
	 LCD->LCD_RAM = Color;    //дʮ��λ��ɫֵ
#else
		LCD->LCD_RAM = Color;
#endif
	}
}

/*****************************************************************************
 * @name       :void LCD_MPU_Config(void)
 * @date       :2018-12-18 
 * @function   :Configure the region of the MPU, and configure 
								the external SRAM area to be in write-through mode.
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_MPU_Config(void)
{	
	MPU_Region_InitTypeDef MPU_Initure;

	HAL_MPU_Disable();							//����MPU֮ǰ�ȹر�MPU,��������Ժ���ʹ��MPU	
	//�ⲿSRAMΪregion0����СΪ2MB��������ɶ�д
	MPU_Initure.Enable=MPU_REGION_ENABLE;	    //ʹ��region
	MPU_Initure.Number=LCD_REGION_NUMBER;		//����region���ⲿSRAMʹ�õ�region0
	MPU_Initure.BaseAddress=LCD_ADDRESS_START;	//region����ַ
	MPU_Initure.Size=LCD_REGION_SIZE;			//region��С
	MPU_Initure.SubRegionDisable=0X00;
	MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;
	MPU_Initure.AccessPermission=MPU_REGION_FULL_ACCESS;	//��region�ɶ�д
	MPU_Initure.DisableExec=MPU_INSTRUCTION_ACCESS_ENABLE;	//�����ȡ�������е�ָ��
	MPU_Initure.IsShareable=MPU_ACCESS_NOT_SHAREABLE;
	MPU_Initure.IsCacheable=MPU_ACCESS_NOT_CACHEABLE;
	MPU_Initure.IsBufferable=MPU_ACCESS_BUFFERABLE;
	HAL_MPU_ConfigRegion(&MPU_Initure);
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);     //����MPU
}

/*****************************************************************************
 * @name       :void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
 * @date       :2018-12-18 
 * @function   :SRAM underlying driver, clock enable, pin assignment
								This function will be called by HAL_SRAM_Init ()
 * @parameters :hsram:SRAM handle
 * @retvalue   :None
******************************************************************************/
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_FMC_CLK_ENABLE();				//ʹ��FMCʱ��
	__HAL_RCC_GPIOD_CLK_ENABLE();			//ʹ��GPIODʱ��
	__HAL_RCC_GPIOE_CLK_ENABLE();			//ʹ��GPIOEʱ��
	
	//��ʼ��PD0,1,4,5,7,8,9,10,13,14,15
	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|\
                     GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_Initure.Mode=GPIO_MODE_AF_PP; 		//����
	GPIO_Initure.Pull=GPIO_PULLUP;			//����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;	//����
	GPIO_Initure.Alternate=GPIO_AF12_FMC;	//����ΪFMC
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
	//��ʼ��PE7,8,9,10,11,12,13,14,15
	GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE,&GPIO_Initure);
}

/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(FMC_NORSRAM_TimingTypeDef FWT)
{
		GPIO_InitTypeDef GPIO_Initure;
		FMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim;
//    FMC_NORSRAM_TimingTypeDef FSMC_WriteTim;
		__HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
		GPIO_Initure.Pin=GPIO_PIN_8;            //PB5,������� PB8,��λ
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;    //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
    LCD_MPU_Config();                       //ʹ��MPU����LCD����
    SRAM_Handler.Instance=FMC_NORSRAM_DEVICE; //BANK1    
    SRAM_Handler.Extended=FMC_NORSRAM_EXTENDED_DEVICE;         
              
        
    SRAM_Handler.Init.NSBank=FMC_NORSRAM_BANK1;     //ʹ��NE1
    SRAM_Handler.Init.DataAddressMux=FMC_DATA_ADDRESS_MUX_DISABLE;  //����     ��������
    SRAM_Handler.Init.MemoryType=FMC_MEMORY_TYPE_SRAM;              //SRAM
#if LCD_USE8BIT_MODEL  
  SRAM_Handler.Init.MemoryDataWidth=FMC_NORSRAM_MEM_BUS_WIDTH_8; //8λ���ݿ��
#else
    SRAM_Handler.Init.MemoryDataWidth=FMC_NORSRAM_MEM_BUS_WIDTH_16; //16λ���ݿ��
#endif
    SRAM_Handler.Init.BurstAccessMode=FMC_BURST_ACCESS_MODE_DISABLE; //�Ƿ�ʹ��ͻ������,����ͬ��ͻ���洢����Ч,�˴�δ�õ�
    SRAM_Handler.Init.WaitSignalPolarity=FMC_WAIT_SIGNAL_POLARITY_LOW;//�ȴ��źŵļ���,����ͻ��ģʽ����������
    SRAM_Handler.Init.WaitSignalActive=FMC_WAIT_TIMING_BEFORE_WS;   //�洢�����ڵȴ�����֮ǰ��һ��ʱ�����ڻ��ǵȴ������ڼ�ʹ��NWAIT
    SRAM_Handler.Init.WriteOperation=FMC_WRITE_OPERATION_ENABLE;    //�洢��дʹ��
		SRAM_Handler.Init.WaitSignal=FMC_WAIT_SIGNAL_DISABLE;           //�ȴ�ʹ��λ,�˴�δ�õ�
    SRAM_Handler.Init.ExtendedMode=FMC_EXTENDED_MODE_ENABLE;        //��дʹ�ò�ͬ��ʱ��
    SRAM_Handler.Init.AsynchronousWait=FMC_ASYNCHRONOUS_WAIT_DISABLE;//�Ƿ�ʹ��ͬ������ģʽ�µĵȴ��ź�,�˴�δ�õ�
    SRAM_Handler.Init.WriteBurst=FMC_WRITE_BURST_DISABLE;           //��ֹͻ��д
    SRAM_Handler.Init.ContinuousClock=FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
        
    //FSMC��ʱ����ƼĴ���
    FSMC_ReadWriteTim.AddressSetupTime=0x11;    //��ַ����ʱ��(ADDSET)Ϊ17��HCLK 1/216M=4.6ns*17=78ns
    FSMC_ReadWriteTim.AddressHoldTime=0x00;
    FSMC_ReadWriteTim.DataSetupTime=0x55;       //���ݱ���ʱ��(DATAST)Ϊ85��HCLK	=4.6*85=391ns
    FSMC_ReadWriteTim.AccessMode=FMC_ACCESS_MODE_A; //ģʽA
    //FSMCдʱ����ƼĴ���
    FWT.AddressSetupTime=15;        //��ַ����ʱ��(ADDSET)Ϊ21��HCLK=96ns
    FWT.AddressHoldTime=0x0;
    FWT.DataSetupTime=0x25;           //���ݱ���ʱ��(DATAST)Ϊ4.6ns*21��HCLK=96ns
    FWT.AccessMode=FMC_ACCESS_MODE_A;     //ģʽA
		
    HAL_SRAM_Init(&SRAM_Handler,&FSMC_ReadWriteTim,&FWT);	
  	 							    
}

void LCD_FSMC_Write_Time_Set(FMC_NORSRAM_TimingTypeDef FWT,uint32_t AST, uint32_t DST)
{
//	FMC_NORSRAM_TimingTypeDef FSMC_WriteTim;
	FWT.AddressSetupTime=AST;      
  FWT.DataSetupTime=DST;         
  FMC_NORSRAM_Extended_Timing_Init(SRAM_Handler.Extended,&FWT,SRAM_Handler.Init.NSBank,SRAM_Handler.Init.ExtendedMode);
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
	LCD_RST_CLR();
	delay_ms(100);	
	LCD_RST_SET();
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
	FMC_NORSRAM_TimingTypeDef FSMC_WriteTim; 
	#if LCD_USE8BIT_MODEL	
	u8 init_flag=0;
	#endif
	LCD_GPIOInit(FSMC_WriteTim);//LCD GPIO��ʼ��	
	#if LCD_USE8BIT_MODEL		
Init:
	#endif
	delay_ms(50); //�������ʱ����Ȼreset�����ײ�����ֺ�����
	//LCD_RESET();    //�����ʹ�ÿ����帴λ���ţ�����ô˸�λ����
//************* NT35510��ʼ��**********//	
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
	#if LCD_USE8BIT_MODEL	 //8λģʽ�£�LCD��ʼ�����飬��ֹreset���߶ϵ���������ʾ�쳣
	if(init_flag==0)
	{
		init_flag=1;
		goto Init;
	}
	#endif
	//LCD_FSMC_Write_Time_Set(FSMC_WriteTim,5, 5);
  LCD_direction(USE_HORIZONTAL);//����LCD��ʾ���� 
	LCD_Clear(WHITE);//��ȫ����ɫ
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

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM			
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
