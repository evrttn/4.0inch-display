// IMPORTANT: LCDWIKI_KBV LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.

//This program is a demo of clearing screen to display red,green,blue.
//This program don't need to rely on any libraries and Can run directly.

//Set the pins to the correct ones for your development shield or breakout board.
//the 16bit mode only use in Mega.you must modify the mode in the file of lcd_mode.h
//when using the BREAKOUT BOARD only and using these 16 data lines to the LCD,
//pin usage as follow:
//             CS  CD  WR  RD  RST  D0  D1  D2  D3  D4  D5  D6  D7  D8  D9  D10  D11  D12  D13  D14  D15 
//Arduino Mega 40  38  39  43  41   37  36  35  34  33  32  31  30  22  23  24   25   26   27   28   29
//             TP_IRQ  MOSI  MISO  TP_CS  EX_CLK
//Arduino Mega   44    51     50    53      52

//Remember to set the pins to suit your display module!
/**********************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE 
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**********************************************************************************/

#define LCD_RD   43
#define LCD_WR   39     
#define LCD_RS   38        
#define LCD_CS   40       
#define LCD_REST 41

void Lcd_Write_Bus(unsigned int d)
{
  PORTA = d >> 8; 
  PORTC = d;	
 *(portOutputRegister(digitalPinToPort(LCD_WR))) &=  ~digitalPinToBitMask(LCD_WR);
 *(portOutputRegister(digitalPinToPort(LCD_WR)))|=  digitalPinToBitMask(LCD_WR);
}


void Lcd_Write_Com(unsigned int VH)  
{   
  *(portOutputRegister(digitalPinToPort(LCD_RS))) &=  ~digitalPinToBitMask(LCD_RS);//LCD_RS=0;
  Lcd_Write_Bus(VH);
}

void Lcd_Write_Data(unsigned int VH)
{
  *(portOutputRegister(digitalPinToPort(LCD_RS)))|=  digitalPinToBitMask(LCD_RS);//LCD_RS=1;
  Lcd_Write_Bus(VH);
}

void Lcd_Write_Com_Data(unsigned int com,unsigned int dat)
{
  Lcd_Write_Com(com);
  Lcd_Write_Data(dat);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
  Lcd_Write_Com(0x2a00);
	Lcd_Write_Data(x1>>8);
  Lcd_Write_Com(0x2a01);
	Lcd_Write_Data(x1);
  Lcd_Write_Com(0x2a02);
	Lcd_Write_Data(x2>>8);
  Lcd_Write_Com(0x2a03);
	Lcd_Write_Data(x2);
  Lcd_Write_Com(0x2b00);
	Lcd_Write_Data(y1>>8);
  Lcd_Write_Com(0x2b01);
	Lcd_Write_Data(y1);
  Lcd_Write_Com(0x2b02);
	Lcd_Write_Data(y2>>8);
  Lcd_Write_Com(0x2b03);
	Lcd_Write_Data(y2);
	Lcd_Write_Com(0x2c00); 							 
}

void Lcd_Init(void)
{
  digitalWrite(LCD_REST,HIGH);
  delay(10); 
  digitalWrite(LCD_REST,LOW);
  delay(100);
  digitalWrite(LCD_REST,HIGH);
  delay(50);

  digitalWrite(LCD_CS,HIGH);
  digitalWrite(LCD_WR,HIGH);
  digitalWrite(LCD_CS,LOW);  //CS
  delay(50);
/*  
  Lcd_Write_Com(0xf000);
  Lcd_Write_Data(0x55);
  Lcd_Write_Com(0xf001);
  Lcd_Write_Data(0x09);
  Lcd_Write_Com(0xff02);
  Lcd_Write_Data(0x01);
*/
  Lcd_Write_Com(0xF000); Lcd_Write_Data(0x55);
    Lcd_Write_Com(0xF001); Lcd_Write_Data(0xAA);
    Lcd_Write_Com(0xF002); Lcd_Write_Data(0x52);
    Lcd_Write_Com(0xF003); Lcd_Write_Data(0x08);
    Lcd_Write_Com(0xF004); Lcd_Write_Data(0x01);
    //# AVDD: manual); Lcd_Write_Data(
    Lcd_Write_Com(0xB600); Lcd_Write_Data(0x34);
    Lcd_Write_Com(0xB601); Lcd_Write_Data(0x34);
    Lcd_Write_Com(0xB602); Lcd_Write_Data(0x34);

    Lcd_Write_Com(0xB000); Lcd_Write_Data(0x0D);//09
    Lcd_Write_Com(0xB001); Lcd_Write_Data(0x0D);
    Lcd_Write_Com(0xB002); Lcd_Write_Data(0x0D);
    //# AVEE: manual); Lcd_Write_Data( -6V
    Lcd_Write_Com(0xB700); Lcd_Write_Data(0x24);
    Lcd_Write_Com(0xB701); Lcd_Write_Data(0x24);
    Lcd_Write_Com(0xB702); Lcd_Write_Data(0x24);

    Lcd_Write_Com(0xB100); Lcd_Write_Data(0x0D);
    Lcd_Write_Com(0xB101); Lcd_Write_Data(0x0D);
    Lcd_Write_Com(0xB102); Lcd_Write_Data(0x0D);
    //#Power Control for
    //VCL
    Lcd_Write_Com(0xB800); Lcd_Write_Data(0x24);
    Lcd_Write_Com(0xB801); Lcd_Write_Data(0x24);
    Lcd_Write_Com(0xB802); Lcd_Write_Data(0x24);

    Lcd_Write_Com(0xB200); Lcd_Write_Data(0x00);

    //# VGH: Clamp Enable); Lcd_Write_Data(
    Lcd_Write_Com(0xB900); Lcd_Write_Data(0x24);
    Lcd_Write_Com(0xB901); Lcd_Write_Data(0x24);
    Lcd_Write_Com(0xB902); Lcd_Write_Data(0x24);

    Lcd_Write_Com(0xB300); Lcd_Write_Data(0x05);
    Lcd_Write_Com(0xB301); Lcd_Write_Data(0x05);
    Lcd_Write_Com(0xB302); Lcd_Write_Data(0x05);

    ///Lcd_Write_Com(0xBF00); Lcd_Write_Data(0x01);

    //# VGL(LVGL):
    Lcd_Write_Com(0xBA00); Lcd_Write_Data(0x34);
    Lcd_Write_Com(0xBA01); Lcd_Write_Data(0x34);
    Lcd_Write_Com(0xBA02); Lcd_Write_Data(0x34);
    //# VGL_REG(VGLO)
    Lcd_Write_Com(0xB500); Lcd_Write_Data(0x0B);
    Lcd_Write_Com(0xB501); Lcd_Write_Data(0x0B);
    Lcd_Write_Com(0xB502); Lcd_Write_Data(0x0B);
    //# VGMP/VGSP:
    Lcd_Write_Com(0xBC00); Lcd_Write_Data(0X00);
    Lcd_Write_Com(0xBC01); Lcd_Write_Data(0xA3);
    Lcd_Write_Com(0xBC02); Lcd_Write_Data(0X00);
    //# VGMN/VGSN
    Lcd_Write_Com(0xBD00); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xBD01); Lcd_Write_Data(0xA3);
    Lcd_Write_Com(0xBD02); Lcd_Write_Data(0x00);
    //# VCOM=-0.1
    Lcd_Write_Com(0xBE00); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xBE01); Lcd_Write_Data(0x63);//4f
      //  VCOMH+0x01;
    //#R+
    Lcd_Write_Com(0xD100); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD101); Lcd_Write_Data(0x37);
    Lcd_Write_Com(0xD102); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD103); Lcd_Write_Data(0x52);
    Lcd_Write_Com(0xD104); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD105); Lcd_Write_Data(0x7B);
    Lcd_Write_Com(0xD106); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD107); Lcd_Write_Data(0x99);
    Lcd_Write_Com(0xD108); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD109); Lcd_Write_Data(0xB1);
    Lcd_Write_Com(0xD10A); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD10B); Lcd_Write_Data(0xD2);
    Lcd_Write_Com(0xD10C); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD10D); Lcd_Write_Data(0xF6);
    Lcd_Write_Com(0xD10E); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD10F); Lcd_Write_Data(0x27);
    Lcd_Write_Com(0xD110); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD111); Lcd_Write_Data(0x4E);
    Lcd_Write_Com(0xD112); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD113); Lcd_Write_Data(0x8C);
    Lcd_Write_Com(0xD114); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD115); Lcd_Write_Data(0xBE);
    Lcd_Write_Com(0xD116); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD117); Lcd_Write_Data(0x0B);
    Lcd_Write_Com(0xD118); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD119); Lcd_Write_Data(0x48);
    Lcd_Write_Com(0xD11A); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD11B); Lcd_Write_Data(0x4A);
    Lcd_Write_Com(0xD11C); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD11D); Lcd_Write_Data(0x7E);
    Lcd_Write_Com(0xD11E); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD11F); Lcd_Write_Data(0xBC);
    Lcd_Write_Com(0xD120); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD121); Lcd_Write_Data(0xE1);
    Lcd_Write_Com(0xD122); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD123); Lcd_Write_Data(0x10);
    Lcd_Write_Com(0xD124); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD125); Lcd_Write_Data(0x31);
    Lcd_Write_Com(0xD126); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD127); Lcd_Write_Data(0x5A);
    Lcd_Write_Com(0xD128); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD129); Lcd_Write_Data(0x73);
    Lcd_Write_Com(0xD12A); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD12B); Lcd_Write_Data(0x94);
    Lcd_Write_Com(0xD12C); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD12D); Lcd_Write_Data(0x9F);
    Lcd_Write_Com(0xD12E); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD12F); Lcd_Write_Data(0xB3);
    Lcd_Write_Com(0xD130); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD131); Lcd_Write_Data(0xB9);
    Lcd_Write_Com(0xD132); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD133); Lcd_Write_Data(0xC1);
    //#G+
    Lcd_Write_Com(0xD200); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD201); Lcd_Write_Data(0x37);
    Lcd_Write_Com(0xD202); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD203); Lcd_Write_Data(0x52);
    Lcd_Write_Com(0xD204); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD205); Lcd_Write_Data(0x7B);
    Lcd_Write_Com(0xD206); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD207); Lcd_Write_Data(0x99);
    Lcd_Write_Com(0xD208); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD209); Lcd_Write_Data(0xB1);
    Lcd_Write_Com(0xD20A); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD20B); Lcd_Write_Data(0xD2);
    Lcd_Write_Com(0xD20C); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD20D); Lcd_Write_Data(0xF6);
    Lcd_Write_Com(0xD20E); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD20F); Lcd_Write_Data(0x27);
    Lcd_Write_Com(0xD210); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD211); Lcd_Write_Data(0x4E);
    Lcd_Write_Com(0xD212); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD213); Lcd_Write_Data(0x8C);
    Lcd_Write_Com(0xD214); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD215); Lcd_Write_Data(0xBE);
    Lcd_Write_Com(0xD216); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD217); Lcd_Write_Data(0x0B);
    Lcd_Write_Com(0xD218); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD219); Lcd_Write_Data(0x48);
    Lcd_Write_Com(0xD21A); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD21B); Lcd_Write_Data(0x4A);
    Lcd_Write_Com(0xD21C); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD21D); Lcd_Write_Data(0x7E);
    Lcd_Write_Com(0xD21E); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD21F); Lcd_Write_Data(0xBC);
    Lcd_Write_Com(0xD220); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD221); Lcd_Write_Data(0xE1);
    Lcd_Write_Com(0xD222); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD223); Lcd_Write_Data(0x10);
    Lcd_Write_Com(0xD224); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD225); Lcd_Write_Data(0x31);
    Lcd_Write_Com(0xD226); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD227); Lcd_Write_Data(0x5A);
    Lcd_Write_Com(0xD228); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD229); Lcd_Write_Data(0x73);
    Lcd_Write_Com(0xD22A); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD22B); Lcd_Write_Data(0x94);
    Lcd_Write_Com(0xD22C); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD22D); Lcd_Write_Data(0x9F);
    Lcd_Write_Com(0xD22E); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD22F); Lcd_Write_Data(0xB3);
    Lcd_Write_Com(0xD230); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD231); Lcd_Write_Data(0xB9);
    Lcd_Write_Com(0xD232); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD233); Lcd_Write_Data(0xC1);
    //#B+
    Lcd_Write_Com(0xD300); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD301); Lcd_Write_Data(0x37);
    Lcd_Write_Com(0xD302); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD303); Lcd_Write_Data(0x52);
    Lcd_Write_Com(0xD304); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD305); Lcd_Write_Data(0x7B);
    Lcd_Write_Com(0xD306); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD307); Lcd_Write_Data(0x99);
    Lcd_Write_Com(0xD308); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD309); Lcd_Write_Data(0xB1);
    Lcd_Write_Com(0xD30A); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD30B); Lcd_Write_Data(0xD2);
    Lcd_Write_Com(0xD30C); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD30D); Lcd_Write_Data(0xF6);
    Lcd_Write_Com(0xD30E); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD30F); Lcd_Write_Data(0x27);
    Lcd_Write_Com(0xD310); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD311); Lcd_Write_Data(0x4E);
    Lcd_Write_Com(0xD312); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD313); Lcd_Write_Data(0x8C);
    Lcd_Write_Com(0xD314); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD315); Lcd_Write_Data(0xBE);
    Lcd_Write_Com(0xD316); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD317); Lcd_Write_Data(0x0B);
    Lcd_Write_Com(0xD318); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD319); Lcd_Write_Data(0x48);
    Lcd_Write_Com(0xD31A); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD31B); Lcd_Write_Data(0x4A);
    Lcd_Write_Com(0xD31C); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD31D); Lcd_Write_Data(0x7E);
    Lcd_Write_Com(0xD31E); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD31F); Lcd_Write_Data(0xBC);
    Lcd_Write_Com(0xD320); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD321); Lcd_Write_Data(0xE1);
    Lcd_Write_Com(0xD322); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD323); Lcd_Write_Data(0x10);
    Lcd_Write_Com(0xD324); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD325); Lcd_Write_Data(0x31);
    Lcd_Write_Com(0xD326); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD327); Lcd_Write_Data(0x5A);
    Lcd_Write_Com(0xD328); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD329); Lcd_Write_Data(0x73);
    Lcd_Write_Com(0xD32A); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD32B); Lcd_Write_Data(0x94);
    Lcd_Write_Com(0xD32C); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD32D); Lcd_Write_Data(0x9F);
    Lcd_Write_Com(0xD32E); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD32F); Lcd_Write_Data(0xB3);
    Lcd_Write_Com(0xD330); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD331); Lcd_Write_Data(0xB9);
    Lcd_Write_Com(0xD332); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD333); Lcd_Write_Data(0xC1);

    //#R-///////////////////////////////////////////
    Lcd_Write_Com(0xD400); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD401); Lcd_Write_Data(0x37);
    Lcd_Write_Com(0xD402); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD403); Lcd_Write_Data(0x52);
    Lcd_Write_Com(0xD404); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD405); Lcd_Write_Data(0x7B);
    Lcd_Write_Com(0xD406); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD407); Lcd_Write_Data(0x99);
    Lcd_Write_Com(0xD408); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD409); Lcd_Write_Data(0xB1);
    Lcd_Write_Com(0xD40A); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD40B); Lcd_Write_Data(0xD2);
    Lcd_Write_Com(0xD40C); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD40D); Lcd_Write_Data(0xF6);
    Lcd_Write_Com(0xD40E); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD40F); Lcd_Write_Data(0x27);
    Lcd_Write_Com(0xD410); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD411); Lcd_Write_Data(0x4E);
    Lcd_Write_Com(0xD412); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD413); Lcd_Write_Data(0x8C);
    Lcd_Write_Com(0xD414); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD415); Lcd_Write_Data(0xBE);
    Lcd_Write_Com(0xD416); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD417); Lcd_Write_Data(0x0B);
    Lcd_Write_Com(0xD418); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD419); Lcd_Write_Data(0x48);
    Lcd_Write_Com(0xD41A); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD41B); Lcd_Write_Data(0x4A);
    Lcd_Write_Com(0xD41C); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD41D); Lcd_Write_Data(0x7E);
    Lcd_Write_Com(0xD41E); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD41F); Lcd_Write_Data(0xBC);
    Lcd_Write_Com(0xD420); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD421); Lcd_Write_Data(0xE1);
    Lcd_Write_Com(0xD422); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD423); Lcd_Write_Data(0x10);
    Lcd_Write_Com(0xD424); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD425); Lcd_Write_Data(0x31);
    Lcd_Write_Com(0xD426); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD427); Lcd_Write_Data(0x5A);
    Lcd_Write_Com(0xD428); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD429); Lcd_Write_Data(0x73);
    Lcd_Write_Com(0xD42A); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD42B); Lcd_Write_Data(0x94);
    Lcd_Write_Com(0xD42C); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD42D); Lcd_Write_Data(0x9F);
    Lcd_Write_Com(0xD42E); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD42F); Lcd_Write_Data(0xB3);
    Lcd_Write_Com(0xD430); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD431); Lcd_Write_Data(0xB9);
    Lcd_Write_Com(0xD432); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD433); Lcd_Write_Data(0xC1);

    //#G-//////////////////////////////////////////////
    Lcd_Write_Com(0xD500); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD501); Lcd_Write_Data(0x37);
    Lcd_Write_Com(0xD502); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD503); Lcd_Write_Data(0x52);
    Lcd_Write_Com(0xD504); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD505); Lcd_Write_Data(0x7B);
    Lcd_Write_Com(0xD506); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD507); Lcd_Write_Data(0x99);
    Lcd_Write_Com(0xD508); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD509); Lcd_Write_Data(0xB1);
    Lcd_Write_Com(0xD50A); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD50B); Lcd_Write_Data(0xD2);
    Lcd_Write_Com(0xD50C); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD50D); Lcd_Write_Data(0xF6);
    Lcd_Write_Com(0xD50E); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD50F); Lcd_Write_Data(0x27);
    Lcd_Write_Com(0xD510); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD511); Lcd_Write_Data(0x4E);
    Lcd_Write_Com(0xD512); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD513); Lcd_Write_Data(0x8C);
    Lcd_Write_Com(0xD514); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD515); Lcd_Write_Data(0xBE);
    Lcd_Write_Com(0xD516); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD517); Lcd_Write_Data(0x0B);
    Lcd_Write_Com(0xD518); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD519); Lcd_Write_Data(0x48);
    Lcd_Write_Com(0xD51A); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD51B); Lcd_Write_Data(0x4A);
    Lcd_Write_Com(0xD51C); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD51D); Lcd_Write_Data(0x7E);
    Lcd_Write_Com(0xD51E); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD51F); Lcd_Write_Data(0xBC);
    Lcd_Write_Com(0xD520); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD521); Lcd_Write_Data(0xE1);
    Lcd_Write_Com(0xD522); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD523); Lcd_Write_Data(0x10);
    Lcd_Write_Com(0xD524); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD525); Lcd_Write_Data(0x31);
    Lcd_Write_Com(0xD526); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD527); Lcd_Write_Data(0x5A);
    Lcd_Write_Com(0xD528); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD529); Lcd_Write_Data(0x73);
    Lcd_Write_Com(0xD52A); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD52B); Lcd_Write_Data(0x94);
    Lcd_Write_Com(0xD52C); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD52D); Lcd_Write_Data(0x9F);
    Lcd_Write_Com(0xD52E); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD52F); Lcd_Write_Data(0xB3);
    Lcd_Write_Com(0xD530); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD531); Lcd_Write_Data(0xB9);
    Lcd_Write_Com(0xD532); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD533); Lcd_Write_Data(0xC1);
    //#B-///////////////////////////////
    Lcd_Write_Com(0xD600); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD601); Lcd_Write_Data(0x37);
    Lcd_Write_Com(0xD602); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD603); Lcd_Write_Data(0x52);
    Lcd_Write_Com(0xD604); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD605); Lcd_Write_Data(0x7B);
    Lcd_Write_Com(0xD606); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD607); Lcd_Write_Data(0x99);
    Lcd_Write_Com(0xD608); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD609); Lcd_Write_Data(0xB1);
    Lcd_Write_Com(0xD60A); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD60B); Lcd_Write_Data(0xD2);
    Lcd_Write_Com(0xD60C); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xD60D); Lcd_Write_Data(0xF6);
    Lcd_Write_Com(0xD60E); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD60F); Lcd_Write_Data(0x27);
    Lcd_Write_Com(0xD610); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD611); Lcd_Write_Data(0x4E);
    Lcd_Write_Com(0xD612); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD613); Lcd_Write_Data(0x8C);
    Lcd_Write_Com(0xD614); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xD615); Lcd_Write_Data(0xBE);
    Lcd_Write_Com(0xD616); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD617); Lcd_Write_Data(0x0B);
    Lcd_Write_Com(0xD618); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD619); Lcd_Write_Data(0x48);
    Lcd_Write_Com(0xD61A); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD61B); Lcd_Write_Data(0x4A);
    Lcd_Write_Com(0xD61C); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD61D); Lcd_Write_Data(0x7E);
    Lcd_Write_Com(0xD61E); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD61F); Lcd_Write_Data(0xBC);
    Lcd_Write_Com(0xD620); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xD621); Lcd_Write_Data(0xE1);
    Lcd_Write_Com(0xD622); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD623); Lcd_Write_Data(0x10);
    Lcd_Write_Com(0xD624); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD625); Lcd_Write_Data(0x31);
    Lcd_Write_Com(0xD626); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD627); Lcd_Write_Data(0x5A);
    Lcd_Write_Com(0xD628); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD629); Lcd_Write_Data(0x73);
    Lcd_Write_Com(0xD62A); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD62B); Lcd_Write_Data(0x94);
    Lcd_Write_Com(0xD62C); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD62D); Lcd_Write_Data(0x9F);
    Lcd_Write_Com(0xD62E); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD62F); Lcd_Write_Data(0xB3);
    Lcd_Write_Com(0xD630); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD631); Lcd_Write_Data(0xB9);
    Lcd_Write_Com(0xD632); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xD633); Lcd_Write_Data(0xC1);



    //#Enable Page0
    Lcd_Write_Com(0xF000); Lcd_Write_Data(0x55);
    Lcd_Write_Com(0xF001); Lcd_Write_Data(0xAA);
    Lcd_Write_Com(0xF002); Lcd_Write_Data(0x52);
    Lcd_Write_Com(0xF003); Lcd_Write_Data(0x08);
    Lcd_Write_Com(0xF004); Lcd_Write_Data(0x00);
    //# RGB I/F Setting
    Lcd_Write_Com(0xB000); Lcd_Write_Data(0x08);
    Lcd_Write_Com(0xB001); Lcd_Write_Data(0x05);
    Lcd_Write_Com(0xB002); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xB003); Lcd_Write_Data(0x05);
    Lcd_Write_Com(0xB004); Lcd_Write_Data(0x02);
    //## SDT:
    Lcd_Write_Com(0xB600); Lcd_Write_Data(0x08);
    Lcd_Write_Com(0xB500); Lcd_Write_Data(0x50);//0x6b ???? 480x854       0x50 ???? 480x800

    //## Gate EQ:
    Lcd_Write_Com(0xB700); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xB701); Lcd_Write_Data(0x00);

    //## Source EQ:
    Lcd_Write_Com(0xB800); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xB801); Lcd_Write_Data(0x05);
    Lcd_Write_Com(0xB802); Lcd_Write_Data(0x05);
    Lcd_Write_Com(0xB803); Lcd_Write_Data(0x05);

    //# Inversion: Column inversion (NVT)
    Lcd_Write_Com(0xBC00); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xBC01); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xBC02); Lcd_Write_Data(0x00);

    //# BOE's Setting(default)
    Lcd_Write_Com(0xCC00); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xCC01); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xCC02); Lcd_Write_Data(0x00);

    //# Display Timing:
    Lcd_Write_Com(0xBD00); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xBD01); Lcd_Write_Data(0x84);
    Lcd_Write_Com(0xBD02); Lcd_Write_Data(0x07);
    Lcd_Write_Com(0xBD03); Lcd_Write_Data(0x31);
    Lcd_Write_Com(0xBD04); Lcd_Write_Data(0x00);

    Lcd_Write_Com(0xBA00); Lcd_Write_Data(0x01);

    Lcd_Write_Com(0xFF00); Lcd_Write_Data(0xAA);
    Lcd_Write_Com(0xFF01); Lcd_Write_Data(0x55);
    Lcd_Write_Com(0xFF02); Lcd_Write_Data(0x25);
    Lcd_Write_Com(0xFF03); Lcd_Write_Data(0x01);

    Lcd_Write_Com(0x3500); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0x3600); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0x3a00); Lcd_Write_Data(0x55);  ////55=16?/////66=18?
    Lcd_Write_Com(0x1100);
    delay(120); 
    Lcd_Write_Com(0x2900 ); 
    Lcd_Write_Com(0x2c00);
}

void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
{	
  unsigned int i,j;
  Lcd_Write_Com(0x2c00); //write_memory_start
  digitalWrite(LCD_RS,HIGH);
  digitalWrite(LCD_CS,LOW);
  l=l+x;
  Address_set(x,y,l,y);
  j=l*2;
  for(i=1;i<=j;i++)
  {
    Lcd_Write_Data(c);
  }
  digitalWrite(LCD_CS,HIGH);   
}

void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
{	
  unsigned int i,j;
  Lcd_Write_Com(0x2c00); //write_memory_start
  digitalWrite(LCD_RS,HIGH);
  digitalWrite(LCD_CS,LOW);
  l=l+y;
  Address_set(x,y,x,l);
  j=l*2;
  for(i=1;i<=j;i++)
  { 
    Lcd_Write_Data(c);
  }
  digitalWrite(LCD_CS,HIGH);   
}

void Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}

void Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
  unsigned int i;
  for(i=0;i<h;i++)
  {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
int RGB(int r,int g,int b)
{return r << 16 | g << 8 | b;
}
void LCD_Clear(unsigned int j)                   
{	
  unsigned int i,m;
  digitalWrite(LCD_CS,LOW);
 Address_set(0,0,479,799);
  //Lcd_Write_Com(0x02c); //write_memory_start
  //digitalWrite(LCD_RS,HIGH);
  for(i=0;i<480;i++)
  {
    for(m=0;m<800;m++)
   {
  //    Lcd_Write_Data(j>>8);
      Lcd_Write_Data(j);

    }
  }
  digitalWrite(LCD_CS,HIGH);   
}

void setup()
{/*
  for(int p=0;p<10;p++)
  {
    pinMode(p,OUTPUT);
  }*/
//  DDRH |= 0x78;
//  DDRE |= 0x38;
//  DDRG |= 0x20;
  DDRC = 0xFF;
  DDRA = 0xFF;
  pinMode(38,OUTPUT);
  pinMode(39,OUTPUT);
  pinMode(40,OUTPUT);
  pinMode(41,OUTPUT);
  pinMode(43,OUTPUT);
  digitalWrite(38, HIGH);
  digitalWrite(39, HIGH);
  digitalWrite(40, HIGH);
  digitalWrite(41, HIGH);
  digitalWrite(43, HIGH);
  Lcd_Init();
 //LCD_Clear(0xf800);
}

void loop()
{  
   LCD_Clear(0xFFFF); 
   delay(6000);
   LCD_Clear(0x0000); 
   LCD_Clear(0xf800);
   LCD_Clear(0x07E0);
   LCD_Clear(0x001F);
   delay(1000);
   LCD_Clear(0x0000);  
  for(int i=0;i<300;i++)
  {
    Rect(random(400),random(700),random(400),random(700),random(65535)); // rectangle at x, y, with, hight, color
  }
  delay(2000);
//  LCD_Clear(0xf800);
}
