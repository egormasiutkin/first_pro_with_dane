#include "main.h"
#include "sd.h"
#include "RA8876.h"
//Buffer
unsigned char DATA[512];

//===========================================================
//write one byte to SD
void SdWrite(unsigned char n)
{
	unsigned char i;

	for(i=8;i;i--)
	{
		HAL_GPIO_WritePin(SD_CLK_GPIO_Port, SD_CLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SD_DO_GPIO_Port, SD_DO_Pin, (n&0x80)?GPIO_PIN_SET:GPIO_PIN_RESET);
		n<<=1;
		HAL_GPIO_WritePin(SD_CLK_GPIO_Port, SD_CLK_Pin, GPIO_PIN_SET);
	}
	HAL_GPIO_WritePin(SD_CLK_GPIO_Port, SD_CLK_Pin, GPIO_PIN_SET);
}
//===========================================================
//read one byte
unsigned char SdRead()
{
	unsigned char n = 0,i = 0;
	for(i=8;i;i--)
	{
		HAL_GPIO_WritePin(SD_CLK_GPIO_Port, SD_CLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SD_CLK_GPIO_Port, SD_CLK_Pin, GPIO_PIN_SET);
		n<<=1;
		if(HAL_GPIO_ReadPin(SD_DI_GPIO_Port, SD_DI_Pin) == GPIO_PIN_SET) n|=1;
	}
	return n;
}
//============================================================
//Detecting the response of the SD card
unsigned char SdResponse()
{
	unsigned char i=0,response;

	while(i<=8)
	{
		response = SdRead();
		if(response==0x00)
			break;
		if(response==0x01)
			break;
		i++;
	}
	return response;
}

void SdCommand(unsigned char command, unsigned long argument, unsigned char crc)
{
	SdWrite(command|0x40);
	SdWrite(((unsigned char *)&argument)[0]);
	SdWrite(((unsigned char *)&argument)[1]);
	SdWrite(((unsigned char *)&argument)[2]);
	SdWrite(((unsigned char *)&argument)[3]);
	SdWrite(crc);
}

unsigned char SdInit(void)
{
	unsigned char i;
	unsigned char response=0x01;

	HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
	for(i=0;i<=9;i++)
	SdWrite(0xff);
	HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);
	//Send Command 0 to put MMC in SPI mode
	SdCommand(0x00,0,0x95);

	response=SdResponse();

	if(response!=0x01)
	{
		return 0;
	}

	while(response==0x01)
	{
		HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
		SdWrite(0xff);
		HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);
		SdCommand(0x01,0x00ffc000,0xff);
		response=SdResponse();
	}

	HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
	SdWrite(0xff);
	return 1;
}
//================================================================
//Specify the address to write  the SD card data, a maximum of 512 bytes
unsigned char SdWriteBlock(unsigned char *Block, unsigned long address,int len)
{
	unsigned int count;
	unsigned char dataResp;
	//Block size is 512 bytes exactly
	//First Lower SS

	HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);
	//Then send write command
	SdCommand(0x18,address,0xff);

	if(SdResponse()==00)
	{
		SdWrite(0xff);
		SdWrite(0xff);
		SdWrite(0xff);
		//command was a success - now send data
		//start with DATA TOKEN = 0xFE
		SdWrite(0xfe);
		//now send data
		for(count=0;count<len;count++) SdWrite(*Block++);

		for(;count<512;count++) SdWrite(0);
		//data block sent - now send checksum
		SdWrite(0xff);
		SdWrite(0xff);
		//Now read in the DATA RESPONSE token
		dataResp=SdRead();
		//Following the DATA RESPONSE token
		//are a number of BUSY bytes
		//a zero byte indicates the MMC is busy

		while(SdRead()==0);

		dataResp=dataResp&0x0f; //mask the high byte of the DATA RESPONSE token
		HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
		SdWrite(0xff);
		if(dataResp==0x0b)
		{
			//printf("DATA WAS NOT ACCEPTED BY CARD -- CRC ERROR\n");
			return 0;
		}
		if(dataResp==0x05)
			return 1;

		//printf("Invalid data Response token.\n");
		return 0;
	}
	//printf("Command 0x18 (Write) was not received by the MMC.\n");
	return 0;
}

//=======================================================================
//Specified address from the SD card to read data, a maximum of 512 bytes
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,int len)
{
	unsigned int count;
	//Block size is 512 bytes exactly
	//First Lower SS

	 //printf("MMC_read_block\n");

	HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);
	//Then send write command
	SdCommand(0x11,address,0xff);

	if(SdResponse()==00)
	{
		//command was a success - now send data
		//start with DATA TOKEN = 0xFE
		while(SdRead()!=0xfe);

		for(count=0;count<len;count++) *Block++=SdRead();

		for(;count<512;count++) SdRead();

		//data block sent - now send checksum
		SdRead();
		SdRead();
		//Now read in the DATA RESPONSE token
		HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
		SdRead();
		return 1;
	}
	 //printf("Command 0x11 (Read) was not received by the MMC.\n");
	return 0;
}


void Display_JPG_SDCARD()///SD card physical address of the first data address initial value
{ unsigned long jn;
  unsigned int in;
 // unsigned long AddTemp=314880;///SD card physical address of the first data address initial value  1G CARD
  unsigned long AddTemp=0;///SD card physical address of the first data address initial value    256M CARD
  //unsigned char mn=3;  //Total number of picture


    Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

	Canvas_Image_Start_address(0);//Layer 1
	Canvas_image_width(400);//
    Active_Window_XY(0,0);
	Active_Window_WH(400,1280);

	Foreground_color_256(Blue);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();


 		SdInit();
	//for(;mn!=0;mn--)
	 {
      	LCD_SetCursor(0,0);
     //	LCD_CmdWrite(0x04); //

	 	for(jn=0;jn<2000;jn++)      //A picture information  comprising 2400x512 bytes
		    {
		    SdReadBlock(DATA,AddTemp+(jn*512),512);
		   	LCD_WriteRAM_Prepare();
		    for(in=0;in<512;in+=2)
			   {
			    LCD_DataWrite(DATA[in+1]);
			    LCD_DataWrite(DATA[in]);
		       }

		     }
		     AddTemp = AddTemp+((jn+16)*512);
	}
}
