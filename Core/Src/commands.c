/*
 * commands.c
 *
 *  Created on: Jan 25, 2024
 *      Author: DaneRuyle
 */
#include "main.h"
#include "display.h"
#include "commands.h"
#include "2828.h"
#include "RA8876.h"
#include "pattern.h"
#include "8876demo.h"
#include "string.h"

enum RX_STATE{_CMD = 0,_LENGTH,_DATA};

unsigned short aRxBuffer[BUFFER_LEN]   = {0};
unsigned short mainBuffer[BUFFER_LEN]  = {0};
unsigned short aTxBuffer[BUFFER_LEN]   = {0};
unsigned char  displayText[BUFFER_LEN] = {0};
unsigned char  screen0x00Text[40]      = {0};
unsigned char  screen0x01Text[40]      = {0};
unsigned char  offerText[6]            = {0};
unsigned char  offerMultiplied[6]      = {0};
unsigned char  offerLeftText[2]        = {0};
unsigned char  currentTask             = TASK_NO;
unsigned char  rxState                 = 0;
unsigned char  cmd                     = 0;
unsigned char  len                     = 0;
unsigned char  initping                = 0;
unsigned char  mtflag                  = 0;
unsigned char  currentIndex            = 0;
unsigned char  multiIndex[2]           = {0};
unsigned int   multiTime               = 0;
unsigned int   multiLength[2]          = {0};
unsigned int   idleTime                = 0;      // User to calculate the packet timeout
unsigned char  rxidlecnt               = 0;
unsigned char  fullcolor               = 1;         // 0 = Monochrome, 1 = Color, Set this to 1


void InitCommands(void)
{
	if(HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin) == GPIO_PIN_RESET)
		fullcolor = 0;  // Set this to 0

    //  Uncomment to force monochrome
	//	fullcolor = 0;


	// Turn on the LED... PA5
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);  // GPIO_PIN_SET

	// Please see the length = 2
	if(HAL_UART_Receive_IT(&huart3, (uint8_t *)aRxBuffer, 2) != HAL_OK)
		Error_Handler();
	// SSD2828 RAM Initial
	SSD2828_Reset();
	SSD2828_initial();
	//Initial_Display_test	and  set SW2 pin2 = 1
	RA8876_HW_Reset();
	while(LCD_StatusRead()&0x02);
	RA8876_initial();
	Display_ON();
	HAL_Delay(20);
	// Main buffer config in 1ayer10
	Main_Image_Start_Address(layer_start_addr(10));
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);
	Canvas_Image_Start_address(layer_start_addr(10));
	Canvas_image_width(400);
	Active_Window_XY(0,0);
	Active_Window_WH(400,1280);
	// Clear screen
	Foreground_color_256(Black);
	Background_color_256(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);  //  Y,X
	Start_Square_Fill();
	// Font Chip setting
	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	Font_Select_16x32_32x32();
	Select_SFI_0();
	Select_SFI_Font_Mode();
	Select_SFI_24bit_Address();
	Select_SFI_Waveform_Mode_0();
	Select_SFI_0_DummyRead();
	Select_SFI_Single_Mode();
	SPI_Clock_Period(4);	 // Freq must setting <=20MHZ
	Enable_SFlash_SPI();
	// Preloading
	WelcomeAnimation();
	preloadScreen();

	//screen0x12();
	//screen0x03();
	//while(1);
}

void PacketProc(void)
{
	/* Set transmission flag: transfer complete */
	uint8_t                            a, y = 0;
	idleTime = 0;
	switch(rxState)
	{
	case _CMD:
		cmd = aRxBuffer[1];
		//  poll packet check
		if(cmd == 0){
			// poll packet response
			aTxBuffer[0] = 0x0E;
			if(initping){
				aTxBuffer[1] = 0x00;
				if(HAL_UART_Transmit_IT(&huart3, (uint8_t *)aTxBuffer, 2) != HAL_OK)
					Error_Handler();
			}
			else{
				initping = 1;
				aTxBuffer[1] = 0x25;
				aTxBuffer[2] = 0x06;
				aTxBuffer[3] = 0x00;
				aTxBuffer[4] = 0x14;
				aTxBuffer[5] = 0xC0;
				if(HAL_UART_Transmit_IT(&huart3, (uint8_t *)aTxBuffer, 6) != HAL_OK)
					Error_Handler();
			}
			if(HAL_UART_Receive_IT(&huart3, (uint8_t *)aRxBuffer, 2) != HAL_OK)
				Error_Handler();
		}
		else
		{
			// Here must be cmd check
			rxState = _LENGTH;
			memcpy(mainBuffer,aRxBuffer,4);
			// please see the length = 1
			if(HAL_UART_Receive_IT(&huart3, (uint8_t *)aRxBuffer, 1) != HAL_OK)
				Error_Handler();
		}
		break;
	case _LENGTH:
		rxState = _DATA;
		len = aRxBuffer[0] - 3;
		mainBuffer[2] = aRxBuffer[0];
		// please see the length = len
		if(HAL_UART_Receive_IT(&huart3, (uint8_t *)aRxBuffer, len) != HAL_OK)
			Error_Handler();
		break;
	case _DATA:
		rxState = _CMD;
		memcpy(mainBuffer+3,aRxBuffer,2*len);
		if(crc16_ccitt_reversed_w16(mainBuffer,len+1) == mainBuffer[len+2] *256 + mainBuffer[len+1])
		{
			//rxState = _CMD;
			if(!(mainBuffer[1] == 1 && mainBuffer[2] == 5)){
				aTxBuffer[0] = 0x0E;
				aTxBuffer[1] = 0x01;
				aTxBuffer[2] = 0x05;
				aTxBuffer[3] = 0x6E;
				aTxBuffer[4] = 0x5E;
				if(HAL_UART_Transmit_IT(&huart3, (uint8_t *)aTxBuffer, 5))
					Error_Handler();
			}
			// please see the length = 2
			if(HAL_UART_Receive_IT(&huart3, (uint8_t *)aRxBuffer, 2) != HAL_OK)
				Error_Handler();

			// Packet categorize
			if(mainBuffer[1] == 0x01) currentTask = TASK_RESET;
			else if(mainBuffer[1] == 0x31){
				currentTask = TASK_CLEAR;

			}
			else if((mainBuffer[1]&0x3F) == 0x30){// load data
				multiIndex[mainBuffer[3]] = mainBuffer[4];	// this line is error?
				if(mainBuffer[4] == 0x06 || mainBuffer[4] == 0x12)
					currentTask = TASK_SINGLE;	// for exception 0x06 AND 0x12
				if(mainBuffer[2] == 0x08 && mainBuffer[3] == 0x00 && mainBuffer[4] == 0x00 && mainBuffer[5] == 0x00)
				{
					// pause
					// Clear Screen
					//Color_Bar_OFF();
				    currentTask = TASK_PAUSE;
					break;
				}
				//multiLength[mainBuffer[3]] = (uint16_t)mainBuffer[5]*200;
				//multiLength[0] = multiLength[1] = 5000;
				//data fetch or pre process
				if(mainBuffer[4] == 0x00){
					for (a = 6; y < (mainBuffer[2] - 8); a++)
					{
					  screen0x00Text[y] = mainBuffer[a];
					  y++;
					}
					screen0x00Text[y] = '\0';  // 4-18-2024 it was 0
				}
				else if(mainBuffer[4] == 0x01){

					for (a = 6; y < (mainBuffer[2] - 8); a++)
					{
					  screen0x01Text[y] = mainBuffer[a];
					  y++;
					}
					screen0x01Text[y] = '\0';   // 4-18-2024 it was 0
				}

				else if(mainBuffer[4] == 0x11 || mainBuffer[4] == 0x12){  // Screen0x11
					// Emtpy offerText buffer
					offerText[0] = '\0';
					offerText[1] = '\0';
					offerText[2] = '\0';
					offerText[3] = '\0';
					offerText[4] = '\0';
					offerText[5] = '\0';

					if(mainBuffer[6] == '0' && mainBuffer[7] == '0' && mainBuffer[8] == '0'){
						offerText[0] = mainBuffer[9];
					}else if(mainBuffer[6] == '0' && mainBuffer[7] == '0'){
						offerText[0] = mainBuffer[8];
						offerText[1] = mainBuffer[9];
					}else if(mainBuffer[6] == '0'){
						offerText[0] = mainBuffer[7];
						offerText[1] = mainBuffer[8];
						offerText[2] = mainBuffer[9];
					}else{
						offerText[0] = mainBuffer[6];
						offerText[1] = ',';
						offerText[2] = mainBuffer[7];
						offerText[3] = mainBuffer[8];
						offerText[4] = mainBuffer[9];
					}
				}
				else if(mainBuffer[4] == 0x13){
					// Emtpy offerText buffer
					offerText[0] = '\0';
					offerText[1] = '\0';
					offerText[2] = '\0';
					offerText[3] = '\0';
					offerText[4] = '\0';
					offerText[5] = '\0';

					if(mainBuffer[6] == '0' && mainBuffer[7] == '0' && mainBuffer[8] == '0'){
						offerText[0] = mainBuffer[9];
						offerText[1] = '\0';
					}else if(mainBuffer[6] == '0' && mainBuffer[7] == '0'){
						offerText[0] = mainBuffer[8];
						offerText[1] = mainBuffer[9];
						offerText[2] = '\0';
					}else if(mainBuffer[6] == '0'){
						offerText[0] = mainBuffer[7];
						offerText[1] = mainBuffer[8];
						offerText[2] = mainBuffer[9];
						offerText[3] = '\0';
					}else{
						offerText[0] = mainBuffer[6];
						offerText[1] = ',';
						offerText[2] = mainBuffer[7];
						offerText[3] = mainBuffer[8];
						offerText[4] = mainBuffer[9];
						offerText[5] = '\0';
					}
					offerLeftText[0] = mainBuffer[10];
					offerLeftText[1] = '\0';
				}
				else if(mainBuffer[4] == 0x17 || mainBuffer[4] == 0x18){  // Screen0x17  Double
					// Emtpy offerText buffer
					      offerText[0] = '\0';
					      offerText[1] = '\0';
					      offerText[2] = '\0';
					      offerText[3] = '\0';
					      offerText[4] = '\0';
					      offerText[5] = '\0';
					offerMultiplied[0] = '\0';
					offerMultiplied[1] = '\0';
					offerMultiplied[2] = '\0';
					offerMultiplied[3] = '\0';
					offerMultiplied[4] = '\0';
					offerMultiplied[5] = '\0';

					if(mainBuffer[6] == '0' && mainBuffer[7] == '0' && mainBuffer[8] == '0'){
						offerText[0] = mainBuffer[9];
					}else if(mainBuffer[6] == '0' && mainBuffer[7] == '0'){
						offerText[0] = mainBuffer[8];
						offerText[1] = mainBuffer[9];
					}else if(mainBuffer[6] == '0'){
						offerText[0] = mainBuffer[7];
						offerText[1] = mainBuffer[8];
						offerText[2] = mainBuffer[9];
					}else{
						offerText[0] = mainBuffer[6];
						offerText[1] = ',';
						offerText[2] = mainBuffer[7];
						offerText[3] = mainBuffer[8];
						offerText[4] = mainBuffer[9];
					}
					if(mainBuffer[10] == '0' && mainBuffer[11] == '0' && mainBuffer[12] == '0') {
						offerMultiplied[0] = mainBuffer[13];
					}else if (mainBuffer[10] == '0' && mainBuffer[11] == '0') {
						offerMultiplied[0] = mainBuffer[12];
						offerMultiplied[1] = mainBuffer[13];
					}else if (mainBuffer[10] == '0') {
						offerMultiplied[0] = mainBuffer[11];
						offerMultiplied[1] = mainBuffer[12];
						offerMultiplied[2] = mainBuffer[13];
					}else {
						offerMultiplied[0] = mainBuffer[10];
						offerMultiplied[1] = ',';
						offerMultiplied[2] = mainBuffer[11];
						offerMultiplied[3] = mainBuffer[12];
						offerMultiplied[4] = mainBuffer[13];
					}
				}
			}else if((mainBuffer[1]&0x3F) == 0x31){// show data
				if(mainBuffer[2] == 0x06 && mainBuffer[3] == 1){
					currentTask = TASK_CLEAR;
					mtflag = 0;
				}
				else if(mainBuffer[2] == 0x08){
					//single
					currentIndex = mainBuffer[4];
					currentTask = TASK_SINGLE;
					mtflag = 0;
					//if(mainBuffer[5] == 0) 	// let's take it later.
				}else if(mainBuffer[2] == 0x0A){
					//multiple
					multiLength[mainBuffer[4]] = (uint16_t)mainBuffer[5]*200;
					multiLength[mainBuffer[6]] = (uint16_t)mainBuffer[7]*200;
					currentIndex = 0;
					currentTask = TASK_MULTI;
					mtflag = 1;
				}
			}
		}
		//  ALL DONE DANE CODE
		else
		{
			//crc failure
			//rxState = _CMD;
			// please see the length = 2
			if(HAL_UART_Receive_IT(&huart3, (uint8_t *)aRxBuffer, 2) != HAL_OK)
				Error_Handler();
		}
	    break;
	default:
		// rxState value unexpected
		rxState = _CMD;
		// please see the length = 2
		if(HAL_UART_Receive_IT(&huart3, (uint8_t *)aRxBuffer, 2) != HAL_OK)
			Error_Handler();
		break;
	}// switch rxState

}

void HandlePacket(void)
{
	if(currentTask == TASK_RESET)
	{
		Color_Bar_OFF();
		Foreground_color_256(color256_white);
		Background_color_256(color256_black);
		Goto_Text_XY(80,0);
		Font_Width_X1();
		Font_Height_X1();
//		Show_String("RESET response");
		currentTask = TASK_NO;
	}
	else if(currentTask == TASK_CLEAR)
	{
		Color_Bar_OFF();
		Foreground_color_256(color256_black);
		Line_Start_XY(0,0);
		Line_End_XY(399,1279);  // 1280x400
		Start_Square_Fill();
		currentTask = TASK_NO;
	}
	else if(currentTask == TASK_PAUSE){
		Color_Bar_OFF();
	}
	else if(currentTask == TASK_SINGLE || currentTask == TASK_MULTI){
		//debug(currentTask,currentIndex,multiIndex[0],multiIndex[1],multiLength[0],multiLength[1]);
		//currentTask = 0;

		switch(multiIndex[currentIndex]){
		case 0x00:	screen0x00();	break;
		case 0x01:	screen0x01();	break;
		case 0x02:	screen0x02();	break;
		case 0x03:	screen0x03();	break;
		case 0x04:	screen0x04();	break;
		case 0x05:	screen0x05();	break;
		case 0x06:	screen0x06();	break;
		case 0x07:	screen0x07();	break;
		case 0x08:	screen0x08();	break;
		case 0x09:	screen0x09();	break;
		case 0x0a:	screen0x0a();	break;
		case 0x0b:	screen0x0b();	break;
		case 0x0c:	screen0x0c();	break;
		case 0x0d:	screen0x0d();	break;
		case 0x0e:	screen0x0e();	break;
		case 0x0f:	screen0x0f();	break;
		case 0x10:	screen0x10();	break;
		case 0x11:	screen0x11();	break;
		case 0x12:	screen0x12();	break;
		case 0x13:	screen0x13();	break;
//		case 0x14:	screen0x14();	break;
//		case 0x15:	screen0x15();	break;
//		case 0x16:	screen0x16();	break;
		case 0x17:	screen0x17();	break;
		case 0x18:	screen0x18();	break;
//		case 0x19:	screen0x19();	break;
//		case 0x1A:	screen0x1A();	break;
		case 0x1B:	screen0x1B();	break;
		case 0x1C:	screen0x1C();	break;
		case 0x1D:	screen0x1D();	break;
		case 0x1E:	screen0x1E();	break;
		case 0x1F:	screen0x1F();	break;
		}
	}

	//if(currentTask != TASK_NO && currentTask != TASK_RESET && currentTask != TASK_CLEAR)

//	currentTask = TASK_NO;
	// Reset timeStart after every packet received like this - timeStart = HAL_GetTick();
	// This means there is no data coming in on the serial line.
	// Play "NO DATA" on the screen.. The original display would slide "NO DATA"
	// off the screen to the left, or drop it in from the top.  Maybe after 10 minutes
	// we can turn into a digital photo frame and show pictures on the SD card?
	if (idleTime == TIME_LIMIT) {
		idleTime = TIME_LIMIT+1;
		//HAL_UART_AbortReceive_IT(&huart3);
		//if(HAL_UART_Receive_IT(&huart3, (uint8_t *)aRxBuffer, 2) != HAL_OK)     // please see the length = 2
		//	Error_Handler();
		Foreground_color_256(color256_red);
		Line_Start_XY(0,0);
		Line_End_XY(399,1279);  // 1280x400
		Start_Square_Fill();
		// Show NO DATA
		displayText[0] = 'N';
		displayText[1] = 'O';
		displayText[2] = ' ';
		displayText[3] = 'D';
		displayText[4] = 'A';
		displayText[5] = 'T';
		displayText[6] = 'A';
		displayText[7] = '\0';
		Font_Width_X4();
		Font_Height_X4();
		Font_Select_16x32_32x32();
		Foreground_color_256(color256_black);
		Background_color_256(color256_red);
		Font_Background_select_Transparency();
		Set_GTFont_Decoder(0x21);
		Goto_Text_XY(100,360);
		Show_String((char *)displayText);
	}
}

/**
  * @brief  Calculate CRC-16-CCITT reversed.
	* @param  data : pointer of data to caculate.
  * @param  length : data's length
  * @retval the CRC checksum value
  */
uint16_t crc16_ccitt_reversed_w16(const uint16_t* data, size_t length)
{
	uint16_t crc = 0x0000; // Common initial value
	uint16_t poly = 0x8408; // Reversed polynomial for CRC-16-CCITT

	for (size_t i = 0; i < length; i++) {
		crc ^= (data[i]&0x00FF);
		for (uint8_t j = 0; j < 8; j++) {
			if (crc & 0x0001) {
				crc = (crc >> 1) ^ poly;
			}
			else {
				crc >>= 1;
			}
		}
	}
	return crc;
}
