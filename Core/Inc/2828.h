#ifndef __2828_H__
#define __2828_H__

#define LINE 4
#define LCD_VBPD	10	//	12   // 14		    
#define LCD_VFPD	12	//	6	// 16          
#define LCD_VSPW	2	//	2	//  2   	  
#define LCD_HBPD	160	//	98	// 130   
#define LCD_HFPD	160	//	20	// 130	
#define LCD_HSPW	24	//	24  // 10
#define hsize 400
#define vsize 1280

void SSD2828_Reset(void);
void SPI_3W_SET_Cmd(unsigned int Sdata);
void SPI_3W_SET_PAs(unsigned int Sdata);
unsigned char SPI_ReadData(void);
void SPI_WriteData(unsigned char value);
void SPI_WriteCmd(unsigned char value);
void GP_COMMAD_PA(unsigned int num);
void  LCD_initial();

void  SSD2828_initial();
#endif//__2828_H__
