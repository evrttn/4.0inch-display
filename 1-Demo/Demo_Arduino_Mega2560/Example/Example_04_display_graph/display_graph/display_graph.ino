// IMPORTANT: LCDWIKI_KBV LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.

//This program is a demo of how to display picture and how to use rotate function to display string.

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

#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library
#include "font.h"

//the definiens of 8bit mode as follow:
//if the IC model is known or the modules is unreadable,you can use this constructed function
LCDWIKI_KBV mylcd(NT35510,40,38,39,43,41); //model,cs,cd,wr,rd,reset

//define some colour values
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void show_16font(uint16_t x, uint16_t y,uint16_t fc, uint16_t bc,uint8_t *str,uint8_t mode)
{
    uint16_t i,j,k,c_num,color=0;
    boolean first = true;
    c_num = sizeof(tfont16)/sizeof(typFNT_GB16);
    for(k=0;k<c_num;k++)
    {         //pgm_read_byte
         if((pgm_read_byte(&tfont16[k].Index[0])==*str) && (pgm_read_byte(&tfont16[k].Index[1])==*(str+1)))
         {
            mylcd.Set_Addr_Window(x, y, x+16-1, y+16-1); 
            for(j=0;j<32;j++)
            {
               for(i = 0;i<8;i++)  
               {
                  if(mode) //叠加模式
                  {
                    if(pgm_read_byte(&tfont16[k].Msk[j])&(0x80>>i))
                    {
                        mylcd.Set_Draw_color(fc);
                        mylcd.Draw_Pixel(x+((j*8+i)%16),y+((j*8+i)/16));
                     }
                    // x++;
                    // if((x-x0)==16)
                    // {
                   //     x = x0;
                   //     y++;   
                   //  }
     
                  }
                  else   //非叠加模式
                  {
                    if(pgm_read_byte(&tfont16[k].Msk[j])&(0x80>>i))
                    {
                         color = fc;
                     }
                     else
                     {
                        color = bc;
                     }
                     mylcd.Push_Any_Color(&color, 1, first, 0);
                    first = false;
                  }
               }
            } 
      }
    } 
          
}

void show_24font(uint16_t x, uint16_t y,uint16_t fc, uint16_t bc,uint8_t *str,uint8_t mode)
{
    uint16_t i,j,k,c_num,color;
    boolean first = true;
    c_num = sizeof(tfont24)/sizeof(typFNT_GB24);
    for(k=0;k<c_num;k++)
    {
         if((pgm_read_byte(&tfont24[k].Index[0])==*str) && (pgm_read_byte(&tfont24[k].Index[1])==*(str+1)))
         {
            mylcd.Set_Addr_Window(x, y, x+24-1, y+24-1); 
            for(j=0;j<72;j++)
            {
               for(i = 0;i<8;i++)  
               {
                  if(mode) //叠加模式
                  {
                    if(pgm_read_byte(&tfont24[k].Msk[j])&(0x80>>i))
                    {
                        mylcd.Set_Draw_color(fc);
                        mylcd.Draw_Pixel(x+((j*8+i)%24),y+((j*8+i)/24));
                     }
                  //   x++;
                  //   if((x-x0)==32)
                  //   {
                  //      x = x0;
                  //      y++;   
                  //   }
                  }
                  else   //非叠加模式
                  {
                    if(pgm_read_byte(&tfont24[k].Msk[j])&(0x80>>i))
                    {
                         color = fc;
                     }
                     else
                     {
                        color = bc;
                     }
                     mylcd.Push_Any_Color(&color, 1, first, 0);
                     first = false;
                  }
               }
            } 
      }
    }
}

void show_32font(uint16_t x, uint16_t y,uint16_t fc, uint16_t bc,uint8_t *str,uint8_t mode)
{
    uint16_t i,j,k,c_num,color;
    boolean first = true;
    c_num = sizeof(tfont32)/sizeof(typFNT_GB32);
    for(k=0;k<c_num;k++)
    {
         if((pgm_read_byte(&tfont32[k].Index[0])==*str) && (pgm_read_byte(&tfont32[k].Index[1])==*(str+1)))
         {
            mylcd.Set_Addr_Window(x, y, x+32-1, y+32-1); 
            for(j=0;j<128;j++)
            {
               for(i = 0;i<8;i++)  
               {
                  if(mode) //叠加模式
                  {
                    if(pgm_read_byte(&tfont32[k].Msk[j])&(0x80>>i))
                    {
                        mylcd.Set_Draw_color(fc);
                        mylcd.Draw_Pixel(x+((j*8+i)%32),y+((j*8+i)/32));
                     }
                   //  x++;
                  //   if((x-x0)==32)
                   //  {
                  //      x = x0;
                  //      y++;   
                   //  }
                  }
                  else   //非叠加模式
                  {
                    if(pgm_read_byte(&tfont32[k].Msk[j])&(0x80>>i))
                    {
                         color = fc;
                     }
                     else
                     {
                        color = bc;
                     }
                    mylcd.Push_Any_Color(&color, 1, first, 0);
                    first = false;
                  }
               }
            } 
      }
    }
}

void show_chinese(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint16_t csize,uint8_t mode)
{    
    int i = 0;
    if(x>(mylcd.Get_Display_Width()-csize)||y>(mylcd.Get_Display_Height()-csize))
      { 
        return;
      }              
    while(*str!='\0')
    { 
  //      i += 5;
  //      mylcd.Draw_Fast_VLine(i, 10, 100);
       if(csize==32)
       {
          show_32font(x, y,fc, bc,str,mode);
       }
      else if(csize==24)
      {
          show_24font(x, y,fc, bc,str,mode);
      }
      else
      {
          show_16font(x, y,fc, bc,str,mode);
      } 
      str+=3; 
      x+=csize;              
    }   
}

void show_chinese_test(void)
{
     show_chinese(0, 10,RED,BLACK,"欢迎您",16,1);
     show_chinese(0, 26,RED,BLACK,"欢迎您",24,1);
     show_chinese(0, 50,RED,BLACK,"欢迎您",32,1);
     show_chinese(0, 82,GREEN,BLACK,"欢迎您",16,1);
     show_chinese(0, 98,GREEN,BLACK,"欢迎您",24,1);
     show_chinese(0, 122,GREEN,BLACK,"欢迎您",32,1);
     show_chinese(0, 154,BLUE,BLACK,"欢迎您",16,1);
     show_chinese(0, 170,BLUE,BLACK,"欢迎您",24,1);
     show_chinese(0, 194,BLUE,BLACK,"欢迎您",32,1);
}


//clear screen
void fill_screen_test()
{
  mylcd.Fill_Screen(BLACK);  
  mylcd.Fill_Screen(RED);
  mylcd.Fill_Screen(GREEN);
  mylcd.Fill_Screen(BLUE);
  mylcd.Fill_Screen(BLACK);
}

//display some strings
void text_test()
{
  mylcd.Set_Text_Mode(0);
  
  mylcd.Fill_Screen(BLACK);  
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(1);
  mylcd.Print_String("Hello World!", 0, 0);
  
  mylcd.Set_Text_colour(YELLOW);
  mylcd.Set_Text_Size(2);
  mylcd.Print_Number_Float(1234.56,2,0, 8, '.', 0, ' ');
  
  mylcd.Set_Text_colour(RED);
  mylcd.Set_Text_Size(3);
  //mylcd.Print_String("DEADBEEF", 0, 24); 
  mylcd.Print_Number_Int(0xDEADBEF, 0, 24, 0, ' ', 16); 

  mylcd.Set_Text_colour(GREEN);
  mylcd.Set_Text_Size(5);  
  mylcd.Print_String("Groop", 0, 56); 

  mylcd.Set_Text_Size(2);
  mylcd.Print_String("I implore thee,", 0, 96);

  mylcd.Set_Text_Size(1);
  mylcd.Print_String("my foonting turlingdromes.", 0, 112);
  mylcd.Print_String("And hooptiously drangle me", 0, 120);
  mylcd.Print_String("with crinkly bindlewurdles,", 0, 128);
  mylcd.Print_String("Or I will rend thee", 0, 136);
  mylcd.Print_String("in the gobberwarts", 0, 144);
  mylcd.Print_String("with my blurglecruncheon,", 0, 152);
  mylcd.Print_String("see if I don't!", 0, 160);
}

//draw some oblique lines
void lines_test(void)
{
    mylcd.Fill_Screen(BLACK);
      mylcd.Set_Draw_color(GREEN);
    int i = 0;   
    for(i = 0; i< mylcd.Get_Display_Width();i+=5)
    {
       mylcd.Draw_Line(0, 0, i, mylcd.Get_Display_Height()-1);
     }
     for(i = mylcd.Get_Display_Height()-1; i>= 0;i-=5)
     {
       mylcd.Draw_Line(0, 0, mylcd.Get_Display_Width()-1, i);
     }
     
     mylcd.Fill_Screen(BLACK); 
       mylcd.Set_Draw_color(RED);
    for(i = mylcd.Get_Display_Width() -1; i>=0;i-=5)
    {
      mylcd.Draw_Line(mylcd.Get_Display_Width()-1, 0, i, mylcd.Get_Display_Height()-1);
     }
    for(i = mylcd.Get_Display_Height()-1; i>=0;i-=5)
    {
      mylcd.Draw_Line(mylcd.Get_Display_Width()-1, 0, 0, i);
     }
     
     mylcd.Fill_Screen(BLACK); 
      mylcd.Set_Draw_color(BLUE);
     for(i = 0; i < mylcd.Get_Display_Width();i+=5)
    {
      mylcd.Draw_Line(0, mylcd.Get_Display_Height()-1, i, 0);
     }
     for(i = 0; i < mylcd.Get_Display_Height();i+=5)
    {
      mylcd.Draw_Line(0, mylcd.Get_Display_Height()-1, mylcd.Get_Display_Width()-1, i);
     }

     mylcd.Fill_Screen(BLACK);
      mylcd.Set_Draw_color(YELLOW);
     for(i = mylcd.Get_Display_Width()-1; i >=0;i-=5)
    {
      mylcd.Draw_Line(mylcd.Get_Display_Width()-1, mylcd.Get_Display_Height()-1, i, 0);
     }
     for(i = 0; i<mylcd.Get_Display_Height();i+=5)
    {
      mylcd.Draw_Line(mylcd.Get_Display_Width()-1, mylcd.Get_Display_Height()-1, 0, i);
     }
}

//draw some vertical lines and horizontal lines
void h_l_lines_test(void)
{
    int i=0;
   mylcd.Fill_Screen(BLACK);
     mylcd.Set_Draw_color(GREEN);
    for(i =0;i<mylcd.Get_Display_Height();i+=5)
    {
      mylcd.Draw_Fast_HLine(0,i,mylcd.Get_Display_Width()); 
      delay(5);
    }
     mylcd.Set_Draw_color(BLUE);
     for(i =0;i<mylcd.Get_Display_Width();i+=5)
    {
      mylcd.Draw_Fast_VLine(i,0,mylcd.Get_Display_Height()); 
           delay(5);
    }
}

//draw some rectangles
void rectangle_test(void)
{
  int i = 0;
   mylcd.Fill_Screen(BLACK);
     mylcd.Set_Draw_color(GREEN);
   for(i = 0;i<mylcd.Get_Display_Width()/2;i+=4)
   {
      mylcd.Draw_Rectangle(i,(mylcd.Get_Display_Height()-mylcd.Get_Display_Width())/2+i,mylcd.Get_Display_Width()-1-i,mylcd.Get_Display_Height()-(mylcd.Get_Display_Height()-mylcd.Get_Display_Width())/2-i);  
        delay(5);
   }
}

//draw some filled rectangles
void fill_rectangle_test(void)
{
  int i = 0;
   mylcd.Fill_Screen(BLACK);
     mylcd.Set_Draw_color(YELLOW);
   mylcd.Fill_Rectangle(0,(mylcd.Get_Display_Height()-mylcd.Get_Display_Width())/2,mylcd.Get_Display_Width()-1,mylcd.Get_Display_Height()-(mylcd.Get_Display_Height()-mylcd.Get_Display_Width())/2);
    mylcd.Set_Draw_color(MAGENTA);
   for(i = 0;i<mylcd.Get_Display_Width()/2;i+=4)
   {
      mylcd.Draw_Rectangle(i,(mylcd.Get_Display_Height()-mylcd.Get_Display_Width())/2+i,mylcd.Get_Display_Width()-1-i,mylcd.Get_Display_Height()-(mylcd.Get_Display_Height()-mylcd.Get_Display_Width())/2-i);  
        delay(5);
   }
   for(i = 0;i<mylcd.Get_Display_Width()/2;i+=4)
   {
       mylcd.Set_Draw_color(random(255), random(255), random(255));
      mylcd.Fill_Rectangle(i,(mylcd.Get_Display_Height()-mylcd.Get_Display_Width())/2+i,mylcd.Get_Display_Width()-1-i,mylcd.Get_Display_Height()-(mylcd.Get_Display_Height()-mylcd.Get_Display_Width())/2-i);  
        delay(5);
   }
}

//draw some filled circles
void fill_circles_test(void)
{
  int r=10,i=0,j=0;
  mylcd.Fill_Screen(BLACK);
   mylcd.Set_Draw_color(MAGENTA);
  for(i=r;i<mylcd.Get_Display_Width();i+=2*r)
  {
    for(j=r;j<mylcd.Get_Display_Height();j+=2*r)
    {
      mylcd.Fill_Circle(i, j, r);
    }
  }
}

//draw some circles
void circles_test(void)
{
  int r=10,i=0,j=0;
   mylcd.Set_Draw_color(GREEN);
  for(i=0;i<mylcd.Get_Display_Width()+r;i+=2*r)
  {
    for(j=0;j<mylcd.Get_Display_Height()+r;j+=2*r)
    {
      mylcd.Draw_Circle(i, j, r);
    }
  }  
}

//draw some triangles
void triangles_test(void)
{
   int i = 0;
   mylcd.Fill_Screen(BLACK);
   for(i=0;i<mylcd.Get_Display_Width()/2;i+=5)
   {
      mylcd.Set_Draw_color(0,i/2+50,i/2+50);
      mylcd.Draw_Triangle(mylcd.Get_Display_Width()/2-1,mylcd.Get_Display_Height()/2-1-i,
                    mylcd.Get_Display_Width()/2-1-i,mylcd.Get_Display_Height()/2-1+i,
                    mylcd.Get_Display_Width()/2-1+i,mylcd.Get_Display_Height()/2-1+i);                   
   }
}

//draw some filled triangles
void fill_triangles_test(void)
{
   int i = 0;
   mylcd.Fill_Screen(BLACK);
    for(i=mylcd.Get_Display_Width()/2-1;i>0;i-=5)
   {
    
      mylcd.Set_Draw_color(0,i/2+50,i/2+50);
      mylcd.Fill_Triangle(mylcd.Get_Display_Width()/2-1,mylcd.Get_Display_Height()/2-1-i,
                    mylcd.Get_Display_Width()/2-1-i,mylcd.Get_Display_Height()/2-1+i,
                    mylcd.Get_Display_Width()/2-1+i,mylcd.Get_Display_Height()/2-1+i);                   
      mylcd.Set_Draw_color(i,0,i);
      mylcd.Draw_Triangle(mylcd.Get_Display_Width()/2-1,mylcd.Get_Display_Height()/2-1-i,
                    mylcd.Get_Display_Width()/2-1-i,mylcd.Get_Display_Height()/2-1+i,
                    mylcd.Get_Display_Width()/2-1+i,mylcd.Get_Display_Height()/2-1+i);                   
   }
}

//draw some round rectangles
void round_rectangle(void)
{
   int i = 0;
   mylcd.Fill_Screen(BLACK);
     for(i = 0;i<mylcd.Get_Display_Width()/2;i+=4)
   {
       mylcd.Set_Draw_color(255-i/2,0,210-i/2);
      mylcd.Draw_Round_Rectangle(i,(mylcd.Get_Display_Height()-mylcd.Get_Display_Width())/2+i,mylcd.Get_Display_Width()-1-i,mylcd.Get_Display_Height()-(mylcd.Get_Display_Height()-mylcd.Get_Display_Width())/2-i,8);
        delay(5);
   } 
}

//draw some filled round rectangles
void fill_round_rectangle(void)
{
     int i = 0;
   mylcd.Fill_Screen(BLACK);
     for(i = 0;i<mylcd.Get_Display_Width()/2;i+=4)
   {
       mylcd.Set_Draw_color(255-i/2,210-i/2,0);
      mylcd.Fill_Round_Rectangle(i,(mylcd.Get_Display_Height()-mylcd.Get_Display_Width())/2+i,mylcd.Get_Display_Width()-1-i,mylcd.Get_Display_Height()-(mylcd.Get_Display_Height()-mylcd.Get_Display_Width())/2-i,8);
        delay(5);
   } 
}

void setup() 
{
  mylcd.Init_LCD();

  fill_screen_test();
  delay(500);
   show_chinese_test();
  delay(500);
  text_test();
  delay(500);
  lines_test();
  delay(500);
  h_l_lines_test();
  delay(500);
  rectangle_test();
  delay(500);
  fill_rectangle_test();
  delay(500);
  fill_circles_test();
  delay(500);
  circles_test();
  delay(500);
  triangles_test();
  delay(500);
  fill_triangles_test();
  delay(500);
   round_rectangle();
   delay(500);
   fill_round_rectangle();
   delay(3000);
}

void loop() 
{
  //rotate for four directions
 for(uint8_t rotation=0; rotation<4; rotation++) 
 {
    mylcd.Set_Rotation(rotation);
    text_test();
    delay(2000);
 }
}
