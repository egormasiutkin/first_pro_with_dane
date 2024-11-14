/*
 * display.c
 *
 *  Created on: Jan 25, 2024
 *      Author: DaneRuyle
 *
 *  Viewable area
 *  271 x 1125
 *  Line_Start_XY(46,90);
 *  Line_End_XY(316,1215);
 */

#include "main.h"
#include "display.h"
#include "pattern.h"
#include "RA8876.h"
#include "commands.h"
#include <stdlib.h>
#include <stdio.h>

uint16_t		  offer        = 0;
uint16_t		  offersLeft   = 0;
unsigned int      colors[6]    = {color256_red,	color256_green, color256_blue,
								  color256_yellow, color256_cyan, color256_purple};

// DANE TESTING
void DaneFontTest(void)
{

//	LCD_Clear(color256_black);  <-- THIS IS SLOW
	Font_0_degree(); // Normal
	//Font_90_degree(); // Normal
	//LCD_PutChar(uint x,uint y,uchar c,uint charColor,uint bkColor);
    //LCD_SetCursor(uint Xpos, uint Ypos);
	//Font_Width_X4();
//	LCD_SetCursor(10, 10);
//	  Background_color_256(color65k_white);

//	Foreground_color_256(color256_white);
//    LCD_DisplayString(10,10, "Hello!", Yellow, color65k_black);  // Slow

//	Active_Window_XY(0,0);
//    Active_Window_WH(400,1280);
//	Font_Background_select_Color();  //

//	Font_Line_Distance(2);      // <-- What does this do?
//	Set_Font_to_Font_Width(2);  // <-- What does this do?

	Foreground_color_256(color65k_black);
	Background_color_256(color256_red);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);  // 1280x400
	Start_Square_Fill();
	Font_Select_16x32_32x32();
	Font_Width_X1();
 	Goto_Text_XY(0,Line26);
	Show_String("Hello!");
	HAL_Delay(2000);

	Line_Start_XY(0,0);
	Line_End_XY(399,1279);  // 1280x400
	Start_Square_Fill();
 	Goto_Text_XY(0,Line26);
 	Font_Width_X2();
	Show_String("WORLD!");
	HAL_Delay(2000);
}


void debug(unsigned char mode, unsigned char index, unsigned char a,unsigned char b, unsigned int c, unsigned int d){
	char txt[20];
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);  //  Y,X
	Start_Square_Fill();
	Font_Width_X2();
	Font_Height_X2();
	Foreground_color_256(color256_white);
	Goto_Text_XY(33,20);       //  Y, X
	sprintf(txt,"%d\t\t%d",mode,index);
	Show_String((char *)txt);
	Goto_Text_XY(133,20);       //  Y, X
	sprintf(txt,"%d\t\t%d",a,b);
	Show_String((char *)txt);
	Goto_Text_XY(233,20);       //  Y, X
	sprintf(txt,"%d\t\t%d",c,d);
	Show_String((char *)txt);
}


void screen0x00(void)
{
	// Show text from the game
	//  Set display text properties for on screen using a big font
	// show packet = rxBuff[3] = 0x01, [4] = 0x00, [5] = 0x00

    Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);  //  Y,X
	Start_Square_Fill();

	if(fullcolor)
	Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	Font_Background_select_Color();
	//Font_Select_12x24_24x24();
	Disable_Font_Alignment();     // 76 Chars Wide
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);
    Font_Width_X3();
    Font_Height_X4();
	Goto_Text_XY(116,225);       //  Y, X -- 4-19-2024  It was 116,95.
	Show_String((char *)screen0x00Text);
	screen0x00Text[0] = '\0';
	currentTask = 0;
}


void screen0x01(void)
{
	unsigned char	   topText[80] = {0};
	unsigned char   bottomText[80] = {0};
	int                          c = 0;
	int							 d = 0;
	int 					  oldc = 0;
	int 				      oldd = 14;
	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);
    if(fullcolor) Foreground_color_256(color256_white);
    else Foreground_color_256(MONOCHROME_COLOR);

   	Background_color_256(color256_black);

    Disable_Font_Alignment();
	//Font_Background_select_Color();
	Font_Background_select_Transparency();
	Goto_Text_XY(116,225);       //  Y, X  4-19-2024  it was 116, 95
    Font_Width_X3();
    Font_Height_X4();
	Show_String((char *)screen0x01Text);
	screen0x01Text[0] = '\0';

    // Draw Top bar area
	if(fullcolor)
		Foreground_color_256(color256_green);
	else
		Foreground_color_256(MONOCHROME_COLOR);

	Background_color_256(color256_black);
	Line_Start_XY(58,90);  //       4-11-2024  it was 0, 0
    Line_End_XY(92,1240);  //  Y,X  4-11-2024  it was 35, 1279   1215
    Start_Square_Fill();

    // Draw Bottom bar area
	if(fullcolor) Foreground_color_256(color256_green);
	else Foreground_color_256(MONOCHROME_COLOR);

	Background_color_256(color256_black);
	Line_Start_XY(286,90);   //       4-11-2024 it was 365, 0
    Line_End_XY(318,1240);  //  Y,X  4-11-2024 it was 399, 1279
    Start_Square_Fill();

	Enable_Font_Alignment();      // 46 Chars Wide
	//Disable_Font_Alignment();     // 76 Chars Wide
	Font_Background_select_Color();
	//Font_Background_select_Transparency();
//    Foreground_color_256(color256_black);
//	Background_color_256(color256_black);
    Font_Width_X1();
    Font_Height_X1();
    // Length of displayText is 15
    // 75 chars total on one line
	displayText[0]  = 'T';  displayText[1]  = 'O';  displayText[2]  = 'P';  displayText[3]  = ' ';
	displayText[4]  = 'D';  displayText[5]  = 'O';  displayText[6]  = 'L';  displayText[7]  = 'L';
	displayText[8]  = 'A';  displayText[9]  = 'R';  displayText[10] = ' ';  displayText[11] = ' ';
	displayText[12] = ' ';  displayText[13] = ' ';  displayText[14] = ' ';  displayText[15] = '\0';

/*
	// Fill array
	// topText
	c = 0;
	for (int b = 0; b < 45; b++)       // 4-12-2024  it was b < 46
	{
		topText[b] = displayText[c];
		c++;
		if (c == 15) c = 0;
	}

	// bottomText
	d = 14;
//	for (int b = 75; b != -1; b--)
	for (int b = 45; b != -1; b--)      // 4-12-2024  it was b = 43
	{
		bottomText[b] = displayText[d];
		d--;
		if (d == -1) d = 14;
	}
*/
	c = 0;
	d = 0;
	currentTask = 0;
	while(currentTask == 0)
	{
		// Scroll Left
		oldc++;
		if(oldc == 15) oldc = 0;
		c = oldc;
		for (int b = 0; b < 38; b++)      // 4-12-2024 it was b < 41
		{
			topText[b] = displayText[c];
			c++;
			if (c == 15) c = 0;
		}

		// Scroll Right
		oldd--;
		if (oldd == -1) oldd = 14;
		d = oldd;
		//for (int b = 75; b != -1; b--)
		for (int b = 38; b != -1; b--)      // 4-12-2024  it was b=41
		{
			bottomText[b] = displayText[d];
			d--;
			if (d == -1) d = 14;   // 14
		}

		// TOP DOLLAR show top
	    // Draw Top bar area
		if(fullcolor) Foreground_color_256(color256_green);
		else Foreground_color_256(MONOCHROME_COLOR);

		Background_color_256(color256_black);
		Line_Start_XY(58,90);  //       4-11-2024  it was 0, 0
	    Line_End_XY(92,1240);  //  Y,X  4-11-2024  it was 35, 1279   1215
	    Start_Square_Fill();

	    Foreground_color_256(color256_black);   // Background

	    if(fullcolor) Background_color_256(color256_green);   // Text
	    else Background_color_256(MONOCHROME_COLOR);

	    Goto_Text_XY(58,90);  //  Y, X  4-11-2024  it was 0, 0   48
		Show_String((char *)topText);

	    // Draw Bottom bar area
		if(fullcolor) Foreground_color_256(color256_green);
		else Foreground_color_256(MONOCHROME_COLOR);

		Background_color_256(color256_black);
		Line_Start_XY(288,90);
	    Line_End_XY(318,1240);  //  318
	    Start_Square_Fill();

	    // TOP DOLLAR show bottom text
		Goto_Text_XY(286,90);       //  282,90
		Foreground_color_256(color256_black);

		if(fullcolor) Background_color_256(color256_green);   // Text
	    else Background_color_256(MONOCHROME_COLOR);

		Show_String((char *)bottomText);
	}
}


void screen0x02(void)
{
	// GOOD LUCK
	// Move it up 17 lines and over ?

	unsigned int     frameDelay		  = 70;
	unsigned int     colorChangeDelay = 500;
	unsigned int     colorChangeLoop  = 2;
	unsigned int     yOffset          = 17;

	displayText[0] = 'G';  displayText[1] = 'O';  displayText[2] = 'O';  displayText[3] = 'D';
	displayText[4] = ' ';  displayText[5] = 'L';  displayText[6] = 'U';  displayText[7] = 'C';
	displayText[8] = 'K';  displayText[9] = '\0';

    // Clear
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);  //  Y,X
	Start_Square_Fill();

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	Font_Select_16x32_32x32();
	Font_Background_select_Color();
	Disable_Font_Alignment();
	currentTask = 0;
	while(currentTask == 0)
	{
		if(fullcolor) Foreground_color_256(color256_white);
		else Foreground_color_256(MONOCHROME_COLOR);
		Goto_Text_XY(145-yOffset,600);       //  Y, X  4-11-2024  it was 145, 600
		Font_Width_X1();
		Font_Height_X2();
		Show_String(".... ....");
		if(currentTask) break;
		HAL_Delay(frameDelay);

//		Foreground_color_256(color256_white);
		Set_GTFont_Decoder(0x21);
		Goto_Text_XY(181-yOffset,555);       //  Y, X  4-11-2024  it was 181, 555
		Font_Width_X1();
		Font_Height_X1();
		Show_String((char *)displayText);
		if(currentTask) break;
		HAL_Delay(frameDelay);

//		//Foreground_color_256(color256_white);
//		Background_color_256(color256_black);
//		Goto_Text_XY(160-yOffset,555);       //  Y, X  4-11-2024  it was 160, 555
//		Font_Width_X1();
//		Font_Height_X2();
//		Show_String((char *)displayText);
//		if(currentTask) break;
//		HAL_Delay(frameDelay);

		//Foreground_color_256(color256_white);
		Background_color_256(color256_black);
		Goto_Text_XY(142-yOffset,456);       //  Y, X  4-11-2024  it was 142, 456
		Font_Width_X2();
		Font_Height_X3();
		Show_String((char *)displayText);
		if(currentTask) break;
		HAL_Delay(frameDelay);

		//Foreground_color_256(color256_white);
		Background_color_256(color256_black);
		Goto_Text_XY(142-yOffset,350);       //  Y, X  4-11-2024  it was 142, 350
		Font_Width_X3();
		Font_Height_X3();
		Show_String((char *)displayText);
		if(currentTask) break;
		HAL_Delay(frameDelay);


		// color256_white
		//Foreground_color_256(color256_white);
		Background_color_256(color256_black);
		Goto_Text_XY(126-yOffset,260);       //  Y, X  4-11-2024  it was 126, 260
		Font_Width_X4();
		Font_Height_X4();
		Show_String((char *)displayText);
		if(currentTask) break;
		HAL_Delay(colorChangeDelay);


		for (int l = 0; l < colorChangeLoop; l++)
		{
			// color256_red
			if(currentTask) break;
			Goto_Text_XY(126-yOffset,260);       //  Y, X  4-11-2026  it was 126, 260
			if(fullcolor) Foreground_color_256(color256_red);
			else Foreground_color_256(MONOCHROME_COLOR);

			Background_color_256(color256_black);
			Show_String((char *)displayText);
			HAL_Delay(colorChangeDelay);
			if(currentTask) break;
			// color256_white
			Goto_Text_XY(126-yOffset,260);       //  Y, X  4-11-2024  it was 126, 260
			if(fullcolor) Foreground_color_256(color256_white);
			else Foreground_color_256(MONOCHROME_COLOR);

			Background_color_256(color256_black);
			Show_String((char *)displayText);
			HAL_Delay(colorChangeDelay);
			if(currentTask) break;

			// color256_blue
			Goto_Text_XY(126-yOffset,260);       //  Y, X  4-11-2024  it was 126, 260
			if(fullcolor) Foreground_color_256(color256_blue);
			else Foreground_color_256(MONOCHROME_COLOR);

			Background_color_256(color256_black);
			Show_String((char *)displayText);
			HAL_Delay(colorChangeDelay);
			if(currentTask) break;

		}

		if(currentTask) break;
		// Clear the screen
		Goto_Text_XY(126-yOffset,260);       //  Y, X  4-11-2024  it was 126, 260
		Foreground_color_256(color256_black);
		Background_color_256(color256_black);
		Show_String((char *)displayText);

	}
}


// On payline with max bet
void screen0x03(void)
{
	unsigned int xOffset = 18;

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    BTE_logo_disp(55,100,2,480,264);  // NEW LOGO   4-11-2024  it was 50, 40, 3   4-12-2024 30,48,3

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	Font_Select_12x24_24x24();
	Set_GTFont_Decoder(0x21);
	Font_Width_X2();
	Font_Height_X3();

	Goto_Text_XY(44,624-xOffset);       //  Y, X  4-11-2024  it was 40, 624

	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

	Background_color_256(color256_black);
	Show_String("ON  PAYLINE  WITH  MAX");
	Goto_Text_XY(140,640-xOffset);      //        4-11-2024  it was 160, 640
	Show_String("CREDITS  BET  AWARDS");
	Goto_Text_XY(234,670-xOffset);      //        4-11-2024  it was 280, 670
	Show_String("TOP  DOLLAR  BONUS");
	currentTask = 0;
}


// 3 symbols on a played line...
void screen0x04(void)
{
	unsigned int xOffset = -65;
//	unsigned int yOffset = 26;

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    BTE_logo_disp(55,180,2,480,264);  // 4-11-2024  it was 40, 142, 3.. 4-12-2024 it was 30, 232, 3
	Select_Main_Window_8bpp();
	Main_Image_Start_Address(layer_start_addr(10));
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

	Canvas_Image_Start_address(layer_start_addr(10));
	Canvas_image_width(400);
	Active_Window_XY(0,0);
	Active_Window_WH(400,1280);
	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

	Background_color_256(color256_black);
	//Font_Select_16x32_32x32();
	Font_Select_12x24_24x24();
	Set_GTFont_Decoder(0x21);
	Font_Width_X2();
	Font_Height_X4();
	Goto_Text_XY(38,678);       //  Y, X  4-11-2024  it was 20, 750
	Show_String("ON  A  PLAYED  LINE");
	Goto_Text_XY(130,820);      //        4-11-2024  it was 142, 870
	Show_String("AWARDS");
	Goto_Text_XY(220,730+xOffset);      //        4-11-2024  it was 260, 730
	Show_String("TOP  DOLLAR  BONUS");
	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);
	Font_Width_X4();
	Font_Height_X4();
	Goto_Text_XY(113,108);       //        4-11-2024  it was 130, 60
	Show_String("3");
	currentTask = 0;
}


// TOP DOLLAR BONUS
void screen0x05(void)
{
	int		x           = 90;
	int 	y           = 0;
	int     minx        = 0;
	int     miny        = 0;
	int     maxx        = 0;
	int     maxy        = 0;
	int     scrollDelay = 2;
	uint8_t dx          = 0;
	uint8_t dy          = 0;

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

	currentTask = 0;

	Select_Main_Window_8bpp();
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);

	Main_Image_Start_Address(layer_start_addr(0));

	VSCAN_T_to_B();
	Select_PIP1_Parameter();
	Select_PIP1_Window_8bpp();//
	PIP_Display_Start_XY(0,0);
	PIP_Image_Start_Address(layer_start_addr(1));
	PIP_Image_Width(400);
	PIP_Window_Image_Start_XY(0,0);


	PIP_Window_Width_Height(198,360);
	minx = 30;
	maxx = 180;
	miny = 30;
	maxy = 890;

	if(currentTask == 0) Enable_PIP1();
	if(currentTask == 0) HAL_Delay(50);

	while(currentTask == 0)
	{
		if(currentTask) break;
		PIP_Display_Start_XY(x,y);
		if(currentTask) break;

		if(dx == 0) x++;
		else x--;

		if(currentTask) break;

		if(x > maxx) dx = 1;     // 4-16-2024  it was 170
		else if(x < minx) dx = 0; // 4-16-2024  it was 46

		if(currentTask) break;

		if(dy == 0) y++;
		else y--;

		if(currentTask) break;

		if(y > maxy) dy = 1;     // 4-16-2024  it was 600
		else if(y < miny) dy = 0; // 4-16-2025  it was 90

		if(currentTask) break;

		if(currentTask == 0) HAL_Delay(scrollDelay);

		if(currentTask) break;
	}

	VSCAN_B_to_T();
	Main_Image_Start_Address(layer_start_addr(10));
}


// TEST SCREEN
void screen0x06(void)
{
	// Just need to send this....  Clear screen is sent next.. and Cycles
	currentTask = 0;
	while(currentTask == 0)
		Color_Bar_ON();
}


// PLAY TOP DOLLAR
#define numDollarSigns        12
void screen0x07(void)
{
	int xOffset              = -20;
	int yOffset				 = -20;

	int  i                   = 0;
	int  j                   = 0;
	int  playFrameDelay      = 10;
	int  topdollarFrameDelay = 20;
	int  dx[numDollarSigns]  = {100,279,444,895,1040,1140,400,984,1090,150,1155,220};
	int  dy[numDollarSigns]  = {60,41,57,20,35,60,153,151,250,200,200,130};
	int  dc[numDollarSigns]  = {0};
	char ds[numDollarSigns]  = {0x33,0x33,0x22,0x33,0x33,0x22,0x00,0x00,0x00,0x22,0x22,0x00};
	// This is for font size.  I predict you suggest the font  size ahahahaha
	// 0xAB		A is width and B is Height. 0 is minimum and 3 is maximum.

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);
	//Font_Background_select_Transparency();
	currentTask = 0;
	while(currentTask == 0) {

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);  //  Y,X
	Start_Square_Fill();

	// Draw $'s
	for(i = 0;i<numDollarSigns;i++)
	{
		//if(currentTask) break;
//		dc[i] = rand()%0xff;
		dc[i] = RandomColor_256();
		Goto_Text_XY(dy[i],dx[i]);
		if(fullcolor) Foreground_color_256(dc[i]);
		else Foreground_color_256(dc[i] > 0x0f ? MONOCHROME_COLOR:color256_black);
		switch(ds[i]>>4)
		{
		case 0:		Font_Width_X1();			break;
		case 1:		Font_Width_X2();			break;
		case 2:		Font_Width_X3();			break;
		case 3:		Font_Width_X4();			break;
		}
		switch(ds[i]&0x0f)
		{
		case 0:		Font_Height_X1();			break;
		case 1:		Font_Height_X2();			break;
		case 2:		Font_Height_X3();			break;
		case 3:		Font_Height_X4();			break;
		}
		Show_String("$");
	}

		Font_Width_X4();
		Font_Height_X4();
		for(j = 0;j<60;j+=2)
		{
			// Clear Region PLAY
			//if(currentTask) break;
			Foreground_color_256(color256_black);
			Background_color_256(color256_black);
			Line_Start_XY(0,540+xOffset);
			Line_End_XY(180, 870+xOffset);  //  Y,X
			Start_Square_Fill();


			for(i = 0;i<numDollarSigns;i++)
			{
				//if(currentTask) break;
				//dc[i] = rand()%0xff;
				dc[i] = RandomColor_256();
				Goto_Text_XY(dy[i],dx[i]);
				if(fullcolor) Foreground_color_256(dc[i]);
				else Foreground_color_256(dc[i] > 0x0f ? MONOCHROME_COLOR:color256_black);
				switch(ds[i]>>4)
				{
				case 0:		Font_Width_X1();			break;
				case 1:		Font_Width_X2();			break;
				case 2:		Font_Width_X3();			break;
				case 3:		Font_Width_X4();			break;
				}
				switch(ds[i]&0x0f)
				{
				case 0:		Font_Height_X1();			break;
				case 1:		Font_Height_X2();			break;
				case 2:		Font_Height_X3();			break;
				case 3:		Font_Height_X4();			break;
				}
				Show_String("$");
			}

			Font_Width_X4();
			Font_Height_X4();
			if(fullcolor) Foreground_color_256(color256_white);
			else Foreground_color_256(MONOCHROME_COLOR);

			Goto_Text_XY(j+yOffset,535+xOffset);
			Show_String("PLAY");
			if(currentTask != 0) break;
			HAL_Delay(playFrameDelay);
		}
		if(currentTask != 0) break;
		//if(currentTask) break;
		for(j = 0; j < 200; j+=10)       //  Change J from 200 to 220??
		{
			//if(currentTask) break;
			// Clear Region TOP DOLLAR
			Foreground_color_256(color256_black);
			Background_color_256(color256_black);
			Line_Start_XY(200,240+xOffset);
			Line_End_XY(400, 1080+xOffset);  //  Y,X
			Start_Square_Fill();

			for(i = 0; i < numDollarSigns; i++)
			{
				//if(currentTask) break;
				//dc[i] = rand()%0xff;
				dc[i] = RandomColor_256();
				Goto_Text_XY(dy[i],dx[i]);
				if(fullcolor) Foreground_color_256(dc[i]);
				else Foreground_color_256(dc[i] > 0x0f ? MONOCHROME_COLOR:color256_black);

				switch(ds[i]>>4)
				{
				case 0:		Font_Width_X1();			break;
				case 1:		Font_Width_X2();			break;
				case 2:		Font_Width_X3();			break;
				case 3:		Font_Width_X4();			break;
				}
				switch(ds[i]&0x0f)
				{
				case 0:		Font_Height_X1();			break;
				case 1:		Font_Height_X2();			break;
				case 2:		Font_Height_X3();			break;
				case 3:		Font_Height_X4();			break;
				}
				Show_String("$");
			}

			if(fullcolor) Foreground_color_256(color256_white);
			else Foreground_color_256(MONOCHROME_COLOR);

			Font_Width_X4();
			Font_Height_X4();

			Goto_Text_XY(((390-j)+yOffset),250+xOffset);
			Show_String("TOP DOLLAR");
			if(currentTask != 0) break;
			HAL_Delay(topdollarFrameDelay);
		}
		if(currentTask != 0) break;
		for(j = 0;j<110;j++)
		{
			if(currentTask != 0) break;
			//if(currentTask) break;
			for(i = 0;i<numDollarSigns;i++)
			{
				//dc[i] = rand()%0xff;
				dc[i] = RandomColor_256();
				Goto_Text_XY(dy[i],dx[i]);
				if(fullcolor) Foreground_color_256(dc[i]);
				else Foreground_color_256(dc[i] > 0x0f ? MONOCHROME_COLOR:color256_black);

				switch(ds[i]>>4)
				{
				case 0:		Font_Width_X1();			break;
				case 1:		Font_Width_X2();			break;
				case 2:		Font_Width_X3();			break;
				case 3:		Font_Width_X4();			break;
				}
				switch(ds[i]&0x0f)
				{
				case 0:		Font_Height_X1();			break;
				case 1:		Font_Height_X2();			break;
				case 2:		Font_Height_X3();			break;
				case 3:		Font_Height_X4();			break;
				}
				Show_String("$");
			}
			HAL_Delay(20);
		}
	}
}


// YOU ARE AWARDED 4 OFFERS!
void screen0x08(void)
{
	int wordDelay   =  100;
//	int holdTime    =  250;
//	int numLoop     =  3;

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    if(fullcolor) Foreground_color_256(color256_white);
    else Foreground_color_256(MONOCHROME_COLOR);

    Background_color_256(color256_black);
	Goto_Text_XY(70,166);       //  Y, X  4-11-2024  it was 60, 150
	Font_Width_X3();
	Font_Height_X3();
	Show_String("YOU");

	Goto_Text_XY(70,406);       //  Y, X  4-11-2024  it was 60, 390
	Font_Width_X3();
	Font_Height_X3();
	Show_String("ARE");

	Goto_Text_XY(70,646);       //  Y, X  4-11-2024  it was 60, 630
	Font_Width_X3();
	Font_Height_X3();
	Show_String("AWARDED");

	Goto_Text_XY(170,400);       //  Y, X  4-11-2024  it was 200, 384
    if(fullcolor) Foreground_color_256(color256_green);
    else Foreground_color_256(MONOCHROME_COLOR);
	Font_Width_X4();
	Font_Height_X4();
	Show_String("4");

	Goto_Text_XY(200,524);       //  Y, X  4-11-2024  it was 230, 508
    if(fullcolor) Foreground_color_256(color256_white);
    else Foreground_color_256(MONOCHROME_COLOR);
	Font_Width_X3();
	Font_Height_X3();
	Show_String("OFFERS!");

	//for(int b = 0; b < numLoop; b++)
	currentTask = 0;

	while(currentTask == 0)
	{
		Foreground_color_256(color256_black);
		Background_color_256(color256_black);
		Goto_Text_XY(170,400);       //  Y, X  4-11-2024  it was 200, 384
		Font_Width_X4();
		Font_Height_X4();
		Show_String("4");
		if(currentTask) break;
		HAL_Delay(wordDelay);

	    if(fullcolor) Foreground_color_256(color256_green);
	    else Foreground_color_256(MONOCHROME_COLOR);
		Background_color_256(color256_black);
		Goto_Text_XY(170,400);       //  Y, X  4-11-2024  it was 200, 384
		Font_Width_X4();
		Font_Height_X4();
		Show_String("4");
		if(currentTask) break;
		HAL_Delay(wordDelay);
	}

/*
	if(currentTask == 0) 		HAL_Delay(holdTime);
	if(currentTask == 0) 		HAL_Delay(holdTime);
	if(currentTask == 0) 		HAL_Delay(holdTime);
	if(currentTask == 0) 		HAL_Delay(holdTime);
	if(currentTask == 0) 		HAL_Delay(holdTime);


	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);
    Start_Square_Fill();

	if(currentTask == 0)
	HAL_Delay(wordDelay);
	*/
}


//  FIRST OFFER
void screen0x09(void)
{
	int  holdTime   		  = 125;
	int  wordFlap   		  = 25;
	int  littley			  = 140;   // 4-11-2024  it was 168
	int  littlex			  = 235;   // 4-11-2024  it was 265
	int  bigy                 = 112;   // 4-11-2024  it was 140
	int  bigx1                = 237;   // 4-11-2024  it was 265
	int  bigx2                = 648;   // 4-11-2024  it was 676
	unsigned int randColor    = 0;
	srand(HAL_GetTick());


	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    if(fullcolor) Foreground_color_256(color256_red);
    else Foreground_color_256(MONOCHROME_COLOR);

    Background_color_256(color256_black);
	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);

	Font_Background_select_Color();
	//Font_Background_select_Transparency();

	currentTask = 0;
	while(currentTask == 0)
	{
	    randColor = RandomColor_256();
		if(fullcolor) Foreground_color_256(randColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

	    Goto_Text_XY(bigy,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("FIRST");

		Goto_Text_XY(bigy,bigx2);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("OFFER");

		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;

		Foreground_color_256(color256_black);
		Goto_Text_XY(bigy,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("FIRST");


		if(fullcolor) Foreground_color_256(randColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

		Goto_Text_XY(littley,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X3();
		Show_String("FIRST");

		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;

		Foreground_color_256(color256_black);
		Goto_Text_XY(littley,littlex);       //  Y, X
		Font_Width_X4();
		Font_Height_X3();
		Show_String("FIRST");

		if(fullcolor) Foreground_color_256(randColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

		Goto_Text_XY(bigy,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("FIRST");

		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;

		Goto_Text_XY(bigy,bigx2);       //  Y, X
		Foreground_color_256(color256_black);
		Font_Width_X4();
		Font_Height_X4();
		Show_String("OFFER");

		if(fullcolor) Foreground_color_256(randColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

		Goto_Text_XY(littley,bigx2);       //  Y, X
		Font_Width_X4();
		Font_Height_X3();
		Show_String("OFFER");

		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;

		Goto_Text_XY(bigy,bigx2);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("OFFER");

		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
	}
}



// SECOND OFFER
void screen0x0a(void)
{
	int  holdTime   		  = 125;
	int  wordFlap   		  = 25;
	int  littlex			  = 172;    // 4-11-2024  it was 154
	int  littley			  = 140;    // 4-11-2024  it was 168
	int  bigx1                = 148;    // 4-11-2024  it was 130
	int  bigx2                = 728;    // 4-11-2024  it was 710
	int  bigy                 = 112;    // 4-11-2024  it was 140
	unsigned short  TextColor = 0;

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    if(fullcolor) Foreground_color_256(color256_white);
    else Foreground_color_256(MONOCHROME_COLOR);
	Background_color_256(color256_black);
	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);

	Font_Background_select_Color();
	//Font_Background_select_Transparency();

	currentTask = 0;
	while(currentTask == 0)
	{
		TextColor = RandomColor_256();

		if(fullcolor) Foreground_color_256(TextColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

		Goto_Text_XY(bigy,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("SECOND");

		Goto_Text_XY(bigy,bigx2);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("OFFER");

		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;

		Foreground_color_256(color256_black);
		Goto_Text_XY(bigy,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("SECOND");

	    if(fullcolor) Foreground_color_256(TextColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

	    Goto_Text_XY(littley,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X3();
		Show_String("SECOND");

		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;

		Foreground_color_256(color256_black);
		Goto_Text_XY(littley,littlex);       //  Y, X
		Font_Width_X4();
		Font_Height_X3();
		Show_String("SECOND");

	    if(fullcolor) Foreground_color_256(TextColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

	    Goto_Text_XY(bigy,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("SECOND");

		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;

		Goto_Text_XY(bigy,bigx2);       //  Y, X
		Foreground_color_256(color256_black);
		Font_Width_X4();
		Font_Height_X4();
		Show_String("OFFER");

	    if(fullcolor) Foreground_color_256(TextColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

	    Goto_Text_XY(littley,bigx2);       //  Y, X
		Font_Width_X4();
		Font_Height_X3();
		Show_String("OFFER");

		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;

		Goto_Text_XY(bigy,bigx2);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("OFFER");

		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
	}
}


// THIRD OFFER
void screen0x0b(void)
{
	int  holdTime   		  = 125;
	int  wordFlap   		  = 25;
	int  littley			  = 140;
	int  littlex			  = 225;
	int  bigy                 = 112;
	int  bigx1                = 225;
	int  bigx2                = 656;
	unsigned short  TextColor = 0;

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    if(fullcolor) Foreground_color_256(color256_white);
    else Foreground_color_256(MONOCHROME_COLOR);

	Background_color_256(color256_black);
	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);

	Font_Background_select_Color();
	//Font_Background_select_Transparency();

	currentTask = 0;
	while(currentTask == 0)
	{
		TextColor = RandomColor_256();

	    if(fullcolor) Foreground_color_256(TextColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

		Goto_Text_XY(bigy,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("THIRD");

		Goto_Text_XY(bigy,bigx2);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("OFFER");

		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;

		Foreground_color_256(color256_black);
		Goto_Text_XY(bigy,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("THIRD");


	    if(fullcolor) Foreground_color_256(TextColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

		Goto_Text_XY(littley,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X3();
		Show_String("THIRD");

		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;

		Foreground_color_256(color256_black);
		Goto_Text_XY(littley,littlex);       //  Y, X
		Font_Width_X4();
		Font_Height_X3();
		Show_String("THIRD");

	    if(fullcolor) Foreground_color_256(TextColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

		Goto_Text_XY(bigy,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("THIRD");

		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;

		Goto_Text_XY(bigy,bigx2);       //  Y, X
		Foreground_color_256(color256_black);
		Font_Width_X4();
		Font_Height_X4();
		Show_String("OFFER");

	    if(fullcolor) Foreground_color_256(TextColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

		Goto_Text_XY(littley,bigx2);       //  Y, X
		Font_Width_X4();
		Font_Height_X3();
		Show_String("OFFER");

		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;

//		Foreground_color_256(color256_black);
//		Goto_Text_XY(164,640);       //  Y, X
//		Font_Width_X4();
//		Font_Height_X3();
//		Show_String("OFFER");

		Goto_Text_XY(bigy,bigx2);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("OFFER");

		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
	}
}


// FINAL OFFER
void screen0x0c(void)
{
	int  holdTime   		  = 125;
	int  wordFlap   		  = 25;
	int  littley			  = 140;
	int  littlex			  = 225;
	int  bigy                 = 112;
	int  bigx1                = 225;
	int  bigx2                = 656;
	unsigned short  TextColor = 0;

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    if(fullcolor) Foreground_color_256(color256_white);
    else Foreground_color_256(MONOCHROME_COLOR);
    Background_color_256(color256_black);
	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);
	Font_Background_select_Color();
	//Font_Background_select_Transparency();

	currentTask = 0;
	while(currentTask == 0)
	{
		TextColor = RandomColor_256();

	    if(fullcolor) Foreground_color_256(TextColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

	    Goto_Text_XY(bigy,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("FINAL");

		Goto_Text_XY(bigy,bigx2);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("OFFER");

		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;

		Foreground_color_256(color256_black);
		Goto_Text_XY(bigy,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("FINAL");


	    if(fullcolor) Foreground_color_256(TextColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

	    Goto_Text_XY(littley,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X3();
		Show_String("FINAL");

		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;

		Foreground_color_256(color256_black);
		Goto_Text_XY(littley,littlex);       //  Y, X
		Font_Width_X4();
		Font_Height_X3();
		Show_String("FINAL");

	    if(fullcolor) Foreground_color_256(TextColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

	    Goto_Text_XY(bigy,bigx1);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("FINAL");

		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;

		Goto_Text_XY(bigy,bigx2);       //  Y, X
		Foreground_color_256(color256_black);
		Font_Width_X4();
		Font_Height_X4();
		Show_String("OFFER");

	    if(fullcolor) Foreground_color_256(TextColor);
	    else Foreground_color_256(MONOCHROME_COLOR);

	    Goto_Text_XY(littley,bigx2);       //  Y, X
		Font_Width_X4();
		Font_Height_X3();
		Show_String("OFFER");

		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;
		if(currentTask == 0) HAL_Delay(wordFlap);
		else break;

		Goto_Text_XY(bigy,bigx2);       //  Y, X
		Font_Width_X4();
		Font_Height_X4();
		Show_String("OFFER");

		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
		if(currentTask == 0) HAL_Delay(holdTime);
		else break;
	}
}


// BEST PLAY SAYS  |  "TAKE OFFER"
void screen0x0d(void)
{
	int          holdDelay =  125;
	unsigned int randColor = 0;
	srand(HAL_GetTick());

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);
	Foreground_color_256(color256_white);
	Background_color_256(color256_black);
	//Disable_Font_Alignment();
	//Font_Background_select_Color();
	Font_Background_select_Transparency();
	Font_Width_X3();
	Font_Height_X4();

	currentTask = 0;

	while(currentTask == 0)
    {
	    if(fullcolor) randColor = RandomColor_256();
	    else randColor = MONOCHROME_COLOR;

    	// Left Box Highlighted - random color
    	Background_color_256(randColor);
		Foreground_color_256(randColor);
		Line_Start_XY(0,0);
		Line_End_XY(400,640);  //  Y,X
		Start_Square_Fill();

		//right box - black
		Background_color_256(color256_black);
		Foreground_color_256(color256_black);
		Line_Start_XY(0,641);
		Line_End_XY(399,1279);  //  Y,X
		Start_Square_Fill();

		// Left Text - Black
		Foreground_color_256(color256_black);
		Goto_Text_XY(50,96);       //  Y, X    4-13-2024  it was 50,56
		Show_String("BEST PLAY");
		Goto_Text_XY(180,236);       //  Y, X  4-13-2024  it was 210,188
		Show_String("SAYS");

		// Right Text - random color
		if(fullcolor) Foreground_color_256(randColor);
		else Foreground_color_256(MONOCHROME_COLOR);

		//Background_color_256(color256_black);
		Goto_Text_XY(50,728);       //  Y, X
		Show_String("\"TAKE");
		Goto_Text_XY(180,773);       //  Y, X  4-13-2024 it was 210,801
		Show_String("OFFER\"");

		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;

		// Clear Left Box
		Foreground_color_256(color256_black);
		Background_color_256(color256_black);
		Line_Start_XY(0,0);
		Line_End_XY(400,640);  //  Y,X
		Start_Square_Fill();

		// Right Box - random color
		//randColor = RandomColor_256();
		if(fullcolor) Foreground_color_256(randColor);
		else Foreground_color_256(MONOCHROME_COLOR);
		if(fullcolor) Background_color_256(randColor);
		else Background_color_256(MONOCHROME_COLOR);

		Line_Start_XY(0,641);
		Line_End_XY(399,1279);  //  Y,X
		Start_Square_Fill();

		// Left Box Text
		if(fullcolor) Foreground_color_256(randColor);
		else Foreground_color_256(MONOCHROME_COLOR);
		Goto_Text_XY(50,96);       //  Y, X
		Show_String("BEST PLAY");
		Goto_Text_XY(180,236);       //  Y, X  228
		Show_String("SAYS");

		// Right Box Text
		Foreground_color_256(color256_black);
		if(fullcolor) Background_color_256(color256_white);
		else Background_color_256(MONOCHROME_COLOR);

		Goto_Text_XY(50,728);       //  Y, X
		Show_String("\"TAKE");
		Goto_Text_XY(180,773);       //  Y, X  801
		Show_String("OFFER\"");

		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
    }
}



// BEST PLAY SAYS  |  "TRY AGAIN"
void screen0x0e(void)
{
	int holdDelay    =  125;
	unsigned int randColor;
	srand(HAL_GetTick());

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();


	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);
	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);
	Background_color_256(color256_black);
	//Disable_Font_Alignment();
	//Font_Background_select_Color();
	Font_Background_select_Transparency();
	Font_Width_X3();
	Font_Height_X4();

	currentTask = 0;
    while(currentTask == 0)
    {
    	randColor = RandomColor_256();
		// Left Box Highlighted
		if(fullcolor) Foreground_color_256(randColor);
		else Foreground_color_256(MONOCHROME_COLOR);
		if(fullcolor) Background_color_256(randColor);
		else Background_color_256(MONOCHROME_COLOR);
		Line_Start_XY(0,0);
		Line_End_XY(400,640);  //  Y,X
		Start_Square_Fill();
		Foreground_color_256(color256_black);
		Background_color_256(color256_black);
		Line_Start_XY(0,641);
		Line_End_XY(399,1279);  //  Y,X
		Start_Square_Fill();

		Foreground_color_256(color256_black);
		Goto_Text_XY(50,96);       //  Y, X
		Show_String("BEST PLAY");
		Goto_Text_XY(180,236);       //  Y, X
		Show_String("SAYS");

		if(fullcolor) Foreground_color_256(randColor);
		else Foreground_color_256(MONOCHROME_COLOR);
		//Background_color_256(color256_black);
		Goto_Text_XY(50,742);       //  Y, X
		Show_String("\"TRY");
		Goto_Text_XY(180,793);       //  Y, X
		Show_String("AGAIN\"");



		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;

		// Right Box Highlighted
		Foreground_color_256(color256_black);
		Background_color_256(color256_black);
		Line_Start_XY(0,0);
		Line_End_XY(400,640);  //  Y,X
		Start_Square_Fill();
		if(fullcolor) Foreground_color_256(randColor);
		else Foreground_color_256(MONOCHROME_COLOR);
		if(fullcolor) Background_color_256(randColor);
		else Background_color_256(MONOCHROME_COLOR);
		Line_Start_XY(0,641);
		Line_End_XY(399,1279);  //  Y,X
		Start_Square_Fill();

		if(fullcolor) Foreground_color_256(randColor);
		else Foreground_color_256(MONOCHROME_COLOR);

		Goto_Text_XY(50,96);       //  Y, X
		Show_String("BEST PLAY");
		Goto_Text_XY(180,236);       //  Y, X
		Show_String("SAYS");

		Foreground_color_256(color256_black);
		//Background_color_256(color256_white);
		Goto_Text_XY(50,742);       //  Y, X
		Show_String("\"TRY");
		Goto_Text_XY(180,793);       //  Y, X
		Show_String("AGAIN\"");


		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
		if(currentTask == 0) 		HAL_Delay(holdDelay);
		else break;
    }

}

// TAKE OFFER or TRY AGAIN
void screen0x0f(void)
{
	int holdDelay    =  62;
	int screenHold   =  125;
	int flopLoop     =  2;
	unsigned int randColor;

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);
	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);
	Background_color_256(color256_black);
	//Disable_Font_Alignment();
	Font_Background_select_Color();
	//Font_Background_select_Transparency();
	Font_Width_X3();
	Font_Height_X3();
	if(fullcolor) Foreground_color_256(color256_blue);
	else Foreground_color_256(MONOCHROME_COLOR);
	Goto_Text_XY(130,588);       //  Y, X
	Show_String("OR");
	Font_Width_X4();
	Font_Height_X4();
	if(fullcolor) Foreground_color_256(color256_cyan);
	else Foreground_color_256(MONOCHROME_COLOR);
	Goto_Text_XY(40,852);       //  Y, X   40,852
	Show_String("TRY");
	Goto_Text_XY(174,774);       //  Y, X  4-13-2024 it was 170, 774
	Show_String("AGAIN");

	currentTask = 0;

	while(currentTask == 0)
    {
    	//  Loop TAKE OFFER text
    	for(int l = 0; l < flopLoop; l++)
    	{
    		// Little
    		Foreground_color_256(color256_black);
    		Goto_Text_XY(40,156);       //  Y, X
			Show_String("TAKE");
			Goto_Text_XY(174,116);       //  Y, X
			Show_String("OFFER");
    		randColor = RandomColor_256();
    		if(fullcolor) Foreground_color_256(randColor);
    		else Foreground_color_256(MONOCHROME_COLOR);
    		Font_Height_X3();
    		Goto_Text_XY(71,156);       //  Y, X  71,96
			Show_String("TAKE");
			Goto_Text_XY(182,116);       //  Y, X  190,56
			Show_String("OFFER");
			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;

    		// Big
			//HAL_Delay(1000);
			if(fullcolor) randColor = RandomColor_256();
			else randColor = MONOCHROME_COLOR;
    		Foreground_color_256(randColor);
    		Font_Height_X4();
    		Goto_Text_XY(40,156);       //  Y, X
			Show_String("TAKE");
			Goto_Text_XY(174,116);       //  Y, X
			Show_String("OFFER");

			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
    	}
		// Big TRY AGAIN
		if(fullcolor) Foreground_color_256(color256_white);
		else Foreground_color_256(MONOCHROME_COLOR);
		Background_color_256(color256_black);
		Font_Height_X4();
		Goto_Text_XY(40,156);       //  Y, X
		Show_String("TAKE");
		Goto_Text_XY(174,116);       //  Y, X
		Show_String("OFFER");

		if(currentTask == 0) 		HAL_Delay(screenHold);
		else break;
		if(currentTask == 0) 		HAL_Delay(screenHold);
		else break;
		if(currentTask == 0) 		HAL_Delay(screenHold);
		else break;
		if(currentTask == 0) 		HAL_Delay(screenHold);
		else break;

    	// Loop TRY AGAIN text
		for(int l = 0; l < flopLoop; l++)
    	{
			// Little
			Foreground_color_256(color256_black);
			Goto_Text_XY(40,852);       //  Y, X
			Show_String("TRY");
			Goto_Text_XY(174,774);       //  Y, X  4-13-2024 it was 210,801
			Show_String("AGAIN");
			randColor = RandomColor_256();
    		if(fullcolor) Foreground_color_256(randColor);
    		else Foreground_color_256(MONOCHROME_COLOR);
    		Font_Height_X3();
    		Goto_Text_XY(71,852);       //  Y, X   4-13-2024 it was 71, 888
			Show_String("TRY");
			Goto_Text_XY(182,774);       //  Y, X  4-13-2024 it was 182,801
			Show_String("AGAIN");

			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;

			// Big
			randColor = RandomColor_256();
    		if(fullcolor) Foreground_color_256(randColor);
    		else Foreground_color_256(MONOCHROME_COLOR);
    		Font_Height_X4();
    		Goto_Text_XY(40,852);       //  Y, X
			Show_String("TRY");
			Goto_Text_XY(174,774);       //  Y, X  4-13-2024 it was 210,801
			Show_String("AGAIN");

			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
			if(currentTask == 0) 		HAL_Delay(holdDelay);
			else break;
    	}

		// Big TRY AGAIN
		if(fullcolor) Foreground_color_256(color256_white);
		else Foreground_color_256(MONOCHROME_COLOR);
		Background_color_256(color256_black);
		//Disable_Font_Alignment();
		//Font_Background_select_Color();
		//Font_Background_select_Transparency();
		Font_Width_X4();
		Font_Height_X4();
		Goto_Text_XY(40,852);       //  Y, X
		Show_String("TRY");
		Goto_Text_XY(174,774);       //  Y, X
		Show_String("AGAIN");

		if(currentTask == 0) HAL_Delay(screenHold);
		else break;
		if(currentTask == 0) HAL_Delay(screenHold);
		else break;
		if(currentTask == 0) HAL_Delay(screenHold);
		else break;
		if(currentTask == 0) HAL_Delay(screenHold);
		else break;
    }
}


//  CHOOSE A BUTTON
void screen0x10(void)
{
	int xOffset = 20;
	int yOffset = 16;
	int i = 0;
	char dir = 1;
	char cnt = 0;

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	Font_Select_12x24_24x24();
	//Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);               //  <--- Activate the chip...
	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);
	Background_color_256(color256_black);
	//Disable_Font_Alignment();
	Font_Background_select_Color();
	//Font_Background_select_Transparency();
	//Font_Width_X4();
	//Font_Height_X4();

	// Clear Screen
    Foreground_color_256(color256_black);
    Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    if(fullcolor) Foreground_color_256(color256_white);
    else Foreground_color_256(MONOCHROME_COLOR);

	Font_Width_X4();
	Font_Height_X3();
	Goto_Text_XY(56,440);     // 4-13-2024 it was 40,372
	Show_String("CHOOSE");

	Goto_Text_XY(146,616);
    Show_String("A");

	Goto_Text_XY(228,456);
	Show_String("BUTTON");

	currentTask = 0;

	while(currentTask == 0)
	{
		// Left Arrow
		if(fullcolor) Foreground_color_256(color256_blue);
		else Foreground_color_256(MONOCHROME_COLOR);
		Triangle_Point1_XY(200-yOffset,200-i);
		Triangle_Point2_XY(100-yOffset,300-i);
		Triangle_Point3_XY(300-yOffset,300-i);
		Start_Triangle_Fill();
		Line_Start_XY(150-yOffset,250-i);
		Line_End_XY(250-yOffset,450-i);
		Start_Square_Fill();

		// Right Arrow
		Triangle_Point1_XY(100-yOffset,(980+xOffset)+i);
		Triangle_Point2_XY(300-yOffset,(980+xOffset)+i);
		Triangle_Point3_XY(200-yOffset,(1080+xOffset)+i);
		Start_Triangle_Fill();
		Line_Start_XY(150-yOffset,(830+xOffset)+i);
		Line_End_XY(250-yOffset,(1030+xOffset)+i);
		Start_Square_Fill();
		HAL_Delay(1);

		// Left Arrow
		Foreground_color_256(color256_black);
		Triangle_Point1_XY(200-yOffset,200-i);
		Triangle_Point2_XY(100-yOffset,300-i);
		Triangle_Point3_XY(300-yOffset,300-i);
		Start_Triangle_Fill();
		Line_Start_XY(150-yOffset,250-i);
		Line_End_XY(250-yOffset,450-i);
		Start_Square_Fill();

		// Right Arrow
		Triangle_Point1_XY(100-yOffset,(980+xOffset)+i);
		Triangle_Point2_XY(300-yOffset,(980+xOffset)+i);
		Triangle_Point3_XY(200-yOffset,(1080+xOffset)+i);
		Start_Triangle_Fill();
		Line_Start_XY(150-yOffset,830+i);
		Line_End_XY(250-yOffset,1030+i);
		Start_Square_Fill();

		if(dir > 0)
		{
			i+=2;
			if(i>100) dir = 0,cnt++;
		}
		else
		{
			i-=2;
			if(i<0) dir = 1;
		}
		if(cnt > 5) break;
	}

	if(fullcolor) Foreground_color_256(color256_blue);
	else Foreground_color_256(MONOCHROME_COLOR);
	Triangle_Point1_XY(200-yOffset,150);
	Triangle_Point2_XY(100-yOffset,250);
	Triangle_Point3_XY(300-yOffset,250);
	Start_Triangle_Fill();
	Line_Start_XY(150-yOffset,200);
	Line_End_XY(250-yOffset,400);
	Start_Square_Fill();

	Triangle_Point1_XY(100-yOffset,1030);
	Triangle_Point2_XY(300-yOffset,1030);
	Triangle_Point3_XY(200-yOffset,1130);
	Start_Triangle_Fill();
	Line_Start_XY(150-yOffset,880);
	Line_End_XY(250-yOffset,1080);
	Start_Square_Fill();
}


// CONGRATULATIONS YOU WIN XXXX CREDITS
void screen0x11(void)
{
//	char temp[5]         = {0};
//	int  x               = 0;
	int  screenHold      = 125;
	int  backgroundDelay = 100;
	int  colorLoop       = 3;
	int  line1x          = 298;    // was 140
	int  line1y          = 45;     // was 65
	int  youwinx         = 154;     // was 60
	int  youwiny         = 210;     // was 230
	int  totalx          = 538;    // was 485
	int  totaly          = 190;    // was 200
	int  creditsx        = 805;    // was 771
	int  creditsy        = 210;    // was 230
	unsigned int rndclr  = 0;

	currentTask = 0;

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	Font_Select_12x24_24x24();
	//Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);               //  <--- Activate the chip...
	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);
	Background_color_256(color256_black);
	//Disable_Font_Alignment();
	//Font_Background_select_Color();
	Font_Background_select_Transparency();

	Font_Width_X3();
	Font_Height_X4();
	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);
	Background_color_256(color256_black);
	Goto_Text_XY(line1y,line1x);
    Show_String("CONGRATULATIONS!");

	Goto_Text_XY(youwiny,youwinx);
	Font_Width_X3();
	Font_Height_X3();
    Show_String("YOU WIN ");

	if(offerText[1] == '\0' && offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(totaly,totalx+90);  // 1 digit
	else if(offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(totaly,totalx+60);                     // 2 digits
	else if(offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(totaly,totalx+38);                                             // 3 digits
	if(offerText[1] == ',') Goto_Text_XY(totaly,totalx-8);                                                                            // 5 digits

    if(fullcolor) Foreground_color_256(color256_green);
	else Foreground_color_256(MONOCHROME_COLOR);
	Font_Width_X4();
	Font_Height_X4();
    Show_String((char *)offerText);

	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);
	Goto_Text_XY(creditsy,creditsx);
	Font_Width_X3();
	Font_Height_X3();
    Show_String(" CREDITS");

    if(currentTask == 0)  HAL_Delay(screenHold);
    else return;
    if(currentTask == 0)  HAL_Delay(screenHold);
    else return;
    if(currentTask == 0)  HAL_Delay(screenHold);
    else return;
    if(currentTask == 0)  HAL_Delay(screenHold);
    else return;

    while(currentTask == 0)
    {
		for(int t = 0; t < colorLoop; t++)
		{
			if(currentTask) break;
			if(fullcolor) rndclr = RandomColor_256();
			else rndclr = MONOCHROME_COLOR;
			Foreground_color_256(rndclr);
			Line_Start_XY(0,0);
			Line_End_XY(399,1279);  //  Y,X
			Start_Square_Fill();

			Foreground_color_256(color256_black);

			Font_Width_X3();
			Font_Height_X4();
			Goto_Text_XY(line1y,line1x);
			Show_String("CONGRATULATIONS!");

			Goto_Text_XY(youwiny,youwinx);
			Font_Width_X3();
			Font_Height_X3();
			Show_String("YOU WIN ");

			if(offerText[1] == '\0' && offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(totaly,totalx+90);  // 1 digit
			else if(offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(totaly,totalx+60);                     // 2 digits
			else if(offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(totaly,totalx+38);                                             // 3 digits
			if(offerText[1] == ',') Goto_Text_XY(totaly,totalx-8);                                                                            // 5 digits

//			Goto_Text_XY(totaly,totalx);
			Font_Width_X4();
			Font_Height_X4();
		    Show_String((char *)offerText);

			if(currentTask) break;

			Goto_Text_XY(creditsy,creditsx);
			Font_Width_X3();
			Font_Height_X3();
			Show_String(" CREDITS");

			if(currentTask == 0) HAL_Delay(backgroundDelay);
			else break;

			Background_color_256(color256_black);
			if(fullcolor) Foreground_color_256(rndclr);
			else Foreground_color_256(MONOCHROME_COLOR);
			Line_Start_XY(0,0);
			Line_End_XY(399,1279);  //  Y,X
			Start_Square_Fill();

			//Foreground_color_256(color256_white);
			Font_Width_X3();
			Font_Height_X4();
			//Goto_Text_XY(65,140);
			Goto_Text_XY(line1y,line1x);
			Show_String("CONGRATULATIONS!");
			Font_Width_X3();
			Font_Height_X3();
	//			Goto_Text_XY(230,90);
			Goto_Text_XY(youwiny,youwinx);
			Show_String("YOU WIN ");

			//Goto_Text_XY(200,525);
//			Goto_Text_XY(totaly,totalx);

			if(offerText[1] == '\0' && offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(totaly,totalx+90);  // 1 digit
			else if(offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(totaly,totalx+60);                     // 2 digits
			else if(offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(totaly,totalx+38);                                             // 3 digits
			if(offerText[1] == ',') Goto_Text_XY(totaly,totalx-8);                                                                            // 5 digits

			Font_Width_X4();
			Font_Height_X4();
		    Show_String((char *)offerText);

			//Goto_Text_XY(230,741);
			Goto_Text_XY(creditsy,creditsx);
			Font_Width_X3();
			Font_Height_X3();
			Show_String(" CREDITS");

			if(currentTask) break;
		}

		// Clear Screen
		Foreground_color_256(color256_black);
		Background_color_256(color256_black);
		Line_Start_XY(0,0);
		Line_End_XY(399,1279);  //  Y,X
		Start_Square_Fill();

		Font_Width_X3();
		Font_Height_X4();
		if(fullcolor) Foreground_color_256(color256_white);
		else Foreground_color_256(MONOCHROME_COLOR);
		Background_color_256(color256_black);
		Goto_Text_XY(line1y,line1x);
		Show_String("CONGRATULATIONS!");

		Goto_Text_XY(youwiny,youwinx);
		Font_Width_X3();
		Font_Height_X3();
		Show_String("YOU WIN ");

		if(offerText[1] == '\0' && offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(totaly,totalx+90);  // 1 digit
		else if(offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(totaly,totalx+60);                     // 2 digits
		else if(offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(totaly,totalx+38);                                             // 3 digits
		if(offerText[1] == ',') Goto_Text_XY(totaly,totalx-8);                                                                            // 5 digits

		if(fullcolor) Foreground_color_256(color256_green);
		else Foreground_color_256(MONOCHROME_COLOR);
		Font_Width_X4();
		Font_Height_X4();
	    Show_String((char *)offerText);

		if(fullcolor) Foreground_color_256(color256_white);
		else Foreground_color_256(MONOCHROME_COLOR);
		Goto_Text_XY(creditsy,creditsx);
		Font_Width_X3();
		Font_Height_X3();
		Show_String(" CREDITS");

		if(currentTask == 0)  HAL_Delay(screenHold);
		else break;
		if(currentTask == 0)  HAL_Delay(screenHold);
		else break;
		if(currentTask == 0)  HAL_Delay(screenHold);
		else break;
		if(currentTask == 0)  HAL_Delay(screenHold);
		else break;
    }
}



// TOTAL BONUS WIN X,YYY  CREDITS
void screen0x12(void)
{
	unsigned int   totalx   = 518;
	//unsigned int   totaly   = 200;
	unsigned int   creditsx = 620;
	unsigned int   creditsy = 192;
	unsigned int   holdTime = 25;
	unsigned short rndColor = 0;

	// testing
	// 5 Digits   8,8888
	//offerText[0] = '8';        offerText[1] = ',';        offerText[2] = '8';        offerText[3] = '8';        offerText[4] = '8';        offerText[5] = '\0';

	// 3 Digits   500
	//offerText[0] = '5';        offerText[1] = '0';        offerText[2] = '0';        offerText[3] = '\0';        offerText[4] = '\0';        offerText[5] = '\0';

	// 2 Digits   50
	//offerText[0] = '5';        offerText[1] = '0';        offerText[2] = '\0';        offerText[3] = '\0';       offerText[4] = '\0';        offerText[5] = '\0';

	// 1 Digit    5
    //offerText[0] = '5';        offerText[1] = '\0';        offerText[2] = '\0';        offerText[3] = '\0';       offerText[4] = '\0';        offerText[5] = '\0';

	currentTask = 0;

	if(offerText[1] == '\0' && offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0')
		{
		  totalx = 390;   // 1 digit
		creditsx = 490;
		}
	else if(offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0')
		{
		  totalx = 370;   // 2 digits
		creditsx = 510;
		}
	else if(offerText[3] == '\0' && offerText[4] == '\0')
		{
		  totalx = 340;   // 3 digits
		creditsx = 530;
		}
	if(offerText[1] == ',')
		{
		  totalx = 310;   // 5 digits
		creditsx = 580;
		}

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	Font_Select_12x24_24x24();
	//Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);               //  <--- Activate the chip...
	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);
	Background_color_256(color256_black);
	//Disable_Font_Alignment();
	//Font_Background_select_Color();
	//Font_Background_select_Transparency();
	//Font_Width_X4();
	//Font_Height_X4();

    if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

    Font_Width_X4();
	Font_Height_X4();
	Goto_Text_XY(65,190);            // it was 65, 30
	Show_String("TOTAL BONUS WIN");

	Font_Width_X4();
	Font_Height_X4();
	//Foreground_color_256(color256_green);
	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

	Goto_Text_XY(190,totalx);
	Show_String((char *)offerText);

	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

	Font_Width_X4();
	Font_Height_X4();
	Goto_Text_XY(creditsy,creditsx);           // was 245, 806
	Show_String("CREDITS");
	Font_Width_X4();
	Font_Height_X4();

    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);


	while(currentTask == 0)
	{
		if(fullcolor) rndColor = RandomColor_256();
		else rndColor = MONOCHROME_COLOR;
		Font_Width_X4();
		Font_Height_X4();
		Foreground_color_256(rndColor);
		Goto_Text_XY(65,190);
		Show_String("TOTAL BONUS WIN");
		Goto_Text_XY(creditsy,creditsx);          // was 245,806
		Font_Width_X4();
		Font_Height_X4();
		Show_String("CREDITS");
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	};
}


// CURRENT OFFER, OFFERS LEFT
void screen0x13(void)
{
    unsigned int framePause        = 50;
    unsigned int frameReplay       = 2;
    unsigned int frameReplayLoop   = 2;
    int          bigcurrentx_pos   = 124;    // was 50
    int          smallcurrentx_pos = 224;   // was 180
    int          bigofferx_pos     = 744;   // was 746
    int          smallofferx_pos   = 864;   // was 866

    // Set CURRENT OFFER, OFFERS LEFT screen also get the credits from mainBuffer[6], - mainBuffer[10]
    // mainBuffer[2] - 2 should == (Y) Offers Left
    // mainBuffer[2] - 6, 5, 4, 3 should == (XXXX) Current offer amount
//    mainBuffer[6] =  '8';
//    mainBuffer[7] =  '8';
//    mainBuffer[8] =  '8';
//    mainBuffer[9] =  '8';
//    mainBuffer[10] = '3';
    //offersLeft      = tempBuff[0] * 1000 + tempBuff[1] * 100 + tempBuff[2] * 10 + tempBuff[3];

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);               //  <--- Activate the chip...
	Font_Background_select_Color();
	//Font_Background_select_Transparency();

	Background_color_256(color256_black);
	Foreground_color_256(color256_black);


    currentTask = 0;

    // Clear Screen
    Foreground_color_256(color256_black);
    Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    // Draw divider line
    if(fullcolor) Foreground_color_256(color256_blue);
	else Foreground_color_256(MONOCHROME_COLOR);

    Line_Start_XY(30,695);  // Y,X     30,635
    Line_End_XY(370,705);  // Y,X      370,645
    Start_Square_Fill();

    // FAT FONT
    // Draw offer amount
    // Note:  No 4 digits because of the comma added as a separator
	if(offerText[1] == '\0' && offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(146,405);  // 1 digit
	else if(offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(146,332);                     // 2 digits
	else if(offerText[3] == '\0' && offerText[4] == '\0') Goto_Text_XY(146,260);                                             // 3 digits
	if(offerText[1] == ',') Goto_Text_XY(146,152);                           // 5 digits   332

	if(fullcolor) Foreground_color_256(color256_green);
	else Foreground_color_256(MONOCHROME_COLOR);

	Font_Width_X4();
	Font_Height_X4();
	Show_String((char *)offerText);

    // Draw CREDITS
    if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

    Goto_Text_XY(174,490);   // Y,X
    Font_Width_X1();
    Font_Height_X3();
    Show_String("CREDITS");

    // Draw the # of offers left
    if(fullcolor) Foreground_color_256(color256_yellow);
	else Foreground_color_256(MONOCHROME_COLOR);

    Goto_Text_XY(146,920);   // Y,X
    Font_Width_X4();
    Font_Height_X4();
    Show_String((char *)offerLeftText);

    // Draw CURRENT OFFER in BIG
    if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

    Goto_Text_XY(44,bigcurrentx_pos);   // Y,X
    Font_Width_X2();
    Font_Height_X3();
    Show_String("CURRENT OFFER");
    //HAL_Delay(framePause);

    if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

    Goto_Text_XY(44,bigofferx_pos);   // Y,X
    Font_Width_X2();
    Font_Height_X3();
    Show_String("OFFERS LEFT");

    //HAL_Delay(framePause);
    if(currentTask == 0)  HAL_Delay(framePause);
    if(currentTask == 0)  HAL_Delay(framePause);
    if(currentTask == 0)  HAL_Delay(framePause);
    if(currentTask == 0)  HAL_Delay(framePause);
    if(currentTask == 0)  HAL_Delay(framePause);

    /*
    // Draw CURRENT OFFER small
    Foreground_color_256(color256_white);
    Goto_Text_XY(10,100);   // Y,X
    Font_Width_X1();
    Font_Height_X3();
    Show_String("CURRENT OFFER");

    // Draw OFFERS LEFT small
    Goto_Text_XY(10,800);   // Y,X
    Font_Width_X1();
    Font_Height_X3();
    Show_String("OFFERS LEFT");
    HAL_Delay(framePause);
*/
    currentTask = 0;
    while(currentTask == 0)
    {
	    frameReplay = 0;
	    while (frameReplay < frameReplayLoop)
	    {
		    // Frame 1
		    // Draw CURRENT OFFER in SMALL
		    // Clear small area where big font was
		    Foreground_color_256(color256_black);

		    Line_Start_XY(46,90);
		    Line_End_XY(146,660);  //  Y,X   140,633
		    Start_Square_Fill();
		    if(fullcolor) Foreground_color_256(color256_white);
			else Foreground_color_256(MONOCHROME_COLOR);

		    Goto_Text_XY(44,smallcurrentx_pos);   // Y,X
		    Font_Width_X1();
		    Font_Height_X3();
		    Show_String("CURRENT OFFER");
		    if(currentTask) break;
//		    HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);

		    // Frame 2
		    // Draw CURRENT OFFER in BIG
		    if(fullcolor) Foreground_color_256(color256_white);
			else Foreground_color_256(MONOCHROME_COLOR);

		    Goto_Text_XY(44,bigcurrentx_pos);   // Y,X
		    Font_Width_X2();
		    Font_Height_X3();
		    Show_String("CURRENT OFFER");
		    if(currentTask) break;
//		    HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    frameReplay++;
	    }
	    if(currentTask) break;
	    frameReplay = 0;
	    while (frameReplay < frameReplayLoop)
	    {
		    // OFFERS LEFT
		    // Frame 3
		    // Clear area where bigger font was
		    Foreground_color_256(color256_black);

		    Line_Start_XY(46,720);   //  0,646
		    Line_End_XY(146,1279);  //  Y,X  100,1279
		    Start_Square_Fill();
		    // Draw OFFERS LEFT SMALL
		    if(fullcolor) Foreground_color_256(color256_white);
			else Foreground_color_256(MONOCHROME_COLOR);

		    Goto_Text_XY(44,smallofferx_pos);   // Y,X
		    Font_Width_X1();
		    Font_Height_X3();
		    Show_String("OFFERS LEFT");
		    if(currentTask) break;
//		    HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);


		    // Draw OFFERS LEFT big
//		    // Clear area where smaller font was
//		    Foreground_color_256(color256_black);
//		    Line_Start_XY(0,646);
//		    Line_End_XY(100,1279);  //  Y,X
//		    Start_Square_Fill();

		    if(fullcolor) Foreground_color_256(color256_white);
			else Foreground_color_256(MONOCHROME_COLOR);

		    Goto_Text_XY(44,bigofferx_pos);   // Y,X
		    Font_Width_X2();
		    Font_Height_X3();
		    Show_String("OFFERS LEFT");
		    if(currentTask) break;
//		    HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);
		    if(currentTask == 0)  HAL_Delay(framePause);

		    frameReplay++;
	    }
	    if(currentTask) break;
	    if(currentTask == 0) HAL_Delay(200);  // delay between replaying frames
	    if(currentTask == 0) HAL_Delay(200);
	    if(currentTask == 0) HAL_Delay(200);
	    if(currentTask == 0) HAL_Delay(200);
	    if(currentTask == 0) HAL_Delay(200);
    }

}


// TOTAL BONUS WIN XXXX x2 = YYYY  CREDITS
void screen0x17(void)
{
	unsigned int   totalx   = 170;  // 518
	unsigned int   totaly   = 190;
	unsigned int   creditsx = 940;
	unsigned int   creditsy = 214;
	unsigned int   holdTime = 25;
	unsigned short rndColor = 0;

	currentTask = 0;

	// testing
	// 5 Digits and 5 Digits   8,888  and 8,888
	//      offerText[0] = '8';        offerText[1] = ',';        offerText[2] = '8';        offerText[3] = '8';        offerText[4] = '8';        offerText[5] = '\0';
	//offerMultiplied[0] = '8';  offerMultiplied[1] = ',';  offerMultiplied[2] = '8';  offerMultiplied[3] = '8';  offerMultiplied[4] = '8';  offerMultiplied[5] = '\0';

	// 3 Digits and 5 Digits   500 and 1000
	//      offerText[0] = '5';        offerText[1] = '0';        offerText[2] = '0';        offerText[3] = '\0';        offerText[4] = '\0';        offerText[5] = '\0';
	//offerMultiplied[0] = '1';  offerMultiplied[1] = ',';  offerMultiplied[2] = '0';  offerMultiplied[3] = '0';   offerMultiplied[4] = '0';   offerMultiplied[5] = '\0';

	// 3 Digits and 3 Digits   200 and 400
    //      offerText[0] = '2';        offerText[1] = '0';        offerText[2] = '0';        offerText[3] = '\0';       offerText[4] = '\0';         offerText[5] = '\0';
	//offerMultiplied[0] = '4';  offerMultiplied[1] = '0';  offerMultiplied[2] = '0';  offerMultiplied[3] = '\0';  offerMultiplied[4] = '\0';  offerMultiplied[5] = '\0';

	// 2 Digits and 3 Digits   50 and 100
	//      offerText[0] = '5';        offerText[1] = '0';        offerText[2] = '\0';        offerText[3] = '\0';       offerText[4] = '\0';        offerText[5] = '\0';
	//offerMultiplied[0] = '1';  offerMultiplied[1] = '0';  offerMultiplied[2] = '0';  offerMultiplied[3] = '\0';  offerMultiplied[4] = '\0';  offerMultiplied[5] = '\0';

	// 2 Digits and 2 Digits   20 and 40
    //      offerText[0] = '2';        offerText[1] = '0';        offerText[2] = '\0';        offerText[3] = '\0';       offerText[4] = '\0';        offerText[5] = '\0';
    //offerMultiplied[0] = '4';  offerMultiplied[1] = '0';  offerMultiplied[2] = '\0';  offerMultiplied[3] = '\0'; offerMultiplied[4] = '\0';  offerMultiplied[5] = '\0';

	// 1 Digit and 2 Digits     5 and 10
    //      offerText[0] = '5';        offerText[1] = '\0';        offerText[2] = '\0';        offerText[3] = '\0';       offerText[4] = '\0';        offerText[5] = '\0';
    //offerMultiplied[0] = '1';  offerMultiplied[1] = '0';   offerMultiplied[2] = '\0';  offerMultiplied[3] = '\0'; offerMultiplied[4] = '\0';  offerMultiplied[5] = '\0';


	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	Font_Select_12x24_24x24();
	//Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);               //  <--- Activate the chip...
	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);
	Background_color_256(color256_black);
	//Disable_Font_Alignment();
	//Font_Background_select_Color();
	//Font_Background_select_Transparency();
	//Font_Width_X4();
	//Font_Height_X4();

    if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

    Font_Width_X4();
	Font_Height_X4();
	Goto_Text_XY(65,190);            // it was 65, 30
	Show_String("TOTAL BONUS WIN");

	Font_Width_X4();
	Font_Height_X4();
	//Foreground_color_256(color256_green);

	// 1 digit and 1 digit
	if( offerText[1] == '\0' && offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0' && offerText[5] == '\0' \
		&& offerMultiplied[1] == '\0' && offerMultiplied[2] == '\0' && offerMultiplied[3] == '\0' && offerMultiplied[4] == '\0' \
		&& offerMultiplied[5] == '\0' )
		{
		  totalx = 260;  // 170
		creditsx = 940;
		}
	// 1 digit and 2 digits
	if(offerText[1] == '\0' && offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0' && offerText[5] == '\0' \
		&& offerMultiplied[2] == '\0' && offerMultiplied[3] == '\0' && offerMultiplied[4] == '\0' && offerMultiplied[5] == '\0' )
		{
		  totalx = 307;
		creditsx = 785;
		}
	// 2 digits and 2 digits
	else if(offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0' && offerText[5] == '\0' \
			&& offerMultiplied[2] == '\0' && offerMultiplied[3] == '\0' && offerMultiplied[4] == '\0' \
			&& offerMultiplied[5] == '\0' )
		{
		  totalx = 282;
		creditsx = 810;
		}
	// 2 digits and 3 digits
	else if(offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0' && offerText[5] == '\0' \
			&& offerMultiplied[3] == '\0' && offerMultiplied[4] == '\0' && offerMultiplied[5] == '\0' )
		{
		  totalx = 242;
		creditsx = 820;
		}

	// 3 digits and 3 digits
	else if(offerText[3] == '\0' && offerText[4] == '\0' && offerText[5] == '\0' \
			&& offerMultiplied[3] == '\0' && offerMultiplied[4] == '\0'  && offerMultiplied[5] == '\0' )
		{
		  totalx = 232;  // 170 - 16
		creditsx = 862;
		}

	// 3 digits and 5 digits
	else if(offerText[3] == '\0' && offerText[4] == '\0' && offerText[5] == '\0' \
			&& offerMultiplied[1] == ',')
		{
		  totalx = 194;
		creditsx = 894;
		}

	// 5 digits and 5 digits
	//if(offerText[1] == ',')
	else
		{
		  totalx = 162;  // 170 - 8
		creditsx = 940;
		}

	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);
	Goto_Text_XY(totaly,totalx);
	Show_String((char *)offerText);
	if(fullcolor) Foreground_color_256(color256_green);
	else Foreground_color_256(MONOCHROME_COLOR);
	Show_String(" X 2");

	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);
	Show_String(" = ");
	Show_String((char *)offerMultiplied);

	Font_Width_X2();
	Font_Height_X3();
	Goto_Text_XY(creditsy,creditsx);           // was 245, 806
	Show_String("CREDITS");

	if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);


	while(currentTask == 0)
	{
		if(fullcolor) rndColor = RandomColor_256();
		else rndColor = MONOCHROME_COLOR;
		Font_Width_X4();
		Font_Height_X4();
		Foreground_color_256(rndColor);
		Goto_Text_XY(65,190);
		Show_String("TOTAL BONUS WIN");
		Goto_Text_XY(creditsy,creditsx);          // was 245,806
		Font_Width_X2();
		Font_Height_X3();
		Show_String("CREDITS");
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	};
}


// TOTAL BONUS WIN XXXX x2 x2 = YYYY  CREDITS
void screen0x18(void)
{

	unsigned int   totalx   = 192;
	unsigned int   totaly   = 158;
	unsigned int   creditsx = 960;
	unsigned int   creditsy = 260;
	unsigned int   holdTime = 25;
	unsigned short rndColor = 0;

	// testing
	// 5 Digits and 5 Digits   8,888  and 8,888
	//      offerText[0] = '8';        offerText[1] = ',';        offerText[2] = '8';        offerText[3] = '8';        offerText[4] = '8';        offerText[5] = '\0';
	//offerMultiplied[0] = '8';  offerMultiplied[1] = ',';  offerMultiplied[2] = '8';  offerMultiplied[3] = '8';  offerMultiplied[4] = '8';  offerMultiplied[5] = '\0';

	// 3 Digits and 5 Digits   500 and 1000
	//      offerText[0] = '5';        offerText[1] = '0';        offerText[2] = '0';        offerText[3] = '\0';        offerText[4] = '\0';        offerText[5] = '\0';
	//offerMultiplied[0] = '1';  offerMultiplied[1] = ',';  offerMultiplied[2] = '0';  offerMultiplied[3] = '0';   offerMultiplied[4] = '0';   offerMultiplied[5] = '\0';

	// 3 Digits and 3 Digits   200 and 400
    //      offerText[0] = '2';        offerText[1] = '0';        offerText[2] = '0';        offerText[3] = '\0';       offerText[4] = '\0';         offerText[5] = '\0';
	//offerMultiplied[0] = '4';  offerMultiplied[1] = '0';  offerMultiplied[2] = '0';  offerMultiplied[3] = '\0';  offerMultiplied[4] = '\0';  offerMultiplied[5] = '\0';

	// 2 Digits and 3 Digits   50 and 100
	//      offerText[0] = '5';        offerText[1] = '0';        offerText[2] = '\0';        offerText[3] = '\0';       offerText[4] = '\0';        offerText[5] = '\0';
	//offerMultiplied[0] = '1';  offerMultiplied[1] = '0';  offerMultiplied[2] = '0';  offerMultiplied[3] = '\0';  offerMultiplied[4] = '\0';  offerMultiplied[5] = '\0';

	// 2 Digits and 2 Digits   20 and 40
    //      offerText[0] = '2';        offerText[1] = '0';        offerText[2] = '\0';        offerText[3] = '\0';       offerText[4] = '\0';        offerText[5] = '\0';
    //offerMultiplied[0] = '4';  offerMultiplied[1] = '0';  offerMultiplied[2] = '\0';  offerMultiplied[3] = '\0'; offerMultiplied[4] = '\0';  offerMultiplied[5] = '\0';

	// 1 Digit and 2 Digits     5 and 10
    //      offerText[0] = '5';        offerText[1] = '\0';        offerText[2] = '\0';        offerText[3] = '\0';       offerText[4] = '\0';        offerText[5] = '\0';
    //offerMultiplied[0] = '2';  offerMultiplied[1] = '0';   offerMultiplied[2] = '\0';  offerMultiplied[3] = '\0'; offerMultiplied[4] = '\0';  offerMultiplied[5] = '\0';

	currentTask = 0;

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	Font_Select_12x24_24x24();
	//Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);               //  <--- Activate the chip...
	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);
	Background_color_256(color256_black);
	//Disable_Font_Alignment();
	//Font_Background_select_Color();
	//Font_Background_select_Transparency();
	//Font_Width_X4();
	//Font_Height_X4();

    if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

    Font_Width_X4();
	Font_Height_X4();
	Goto_Text_XY(65,190);            // it was 65, 30
	Show_String("TOTAL BONUS WIN");

	Font_Width_X4();
	Font_Height_X4();
	//Foreground_color_256(color256_green);
	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

	// 1 digit and 1 digit
	if( offerText[1] == '\0' && offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0' && offerText[5] == '\0' \
		&& offerMultiplied[1] == '\0' && offerMultiplied[2] == '\0' && offerMultiplied[3] == '\0' && offerMultiplied[4] == '\0' \
		&& offerMultiplied[5] == '\0' )	  totalx = 260;  // 170
	// 1 digit and 2 digits
	if(offerText[1] == '\0' && offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0' && offerText[5] == '\0' \
		&& offerMultiplied[2] == '\0' && offerMultiplied[3] == '\0' && offerMultiplied[4] == '\0' && offerMultiplied[5] == '\0' )
		  totalx = 370;
	// 2 digits and 2 digits
	else if(offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0' && offerText[5] == '\0' \
			&& offerMultiplied[2] == '\0' && offerMultiplied[3] == '\0' && offerMultiplied[4] == '\0' \
			&& offerMultiplied[5] == '\0' )	  totalx = 340;
	// 2 digits and 3 digits
	else if(offerText[2] == '\0' && offerText[3] == '\0' && offerText[4] == '\0' && offerText[5] == '\0' \
			&& offerMultiplied[3] == '\0' && offerMultiplied[4] == '\0' && offerMultiplied[5] == '\0' )  totalx = 310;

	// 3 digits and 3 digits
	else if(offerText[3] == '\0' && offerText[4] == '\0' && offerText[5] == '\0' \
			&& offerMultiplied[3] == '\0' && offerMultiplied[4] == '\0'  && offerMultiplied[5] == '\0' )  totalx = 274;

	// 3 digits and 5 digits
	else if(offerText[3] == '\0' && offerText[4] == '\0' && offerText[5] == '\0' && offerMultiplied[1] == ',')  totalx = 230;

	// 5 digits and 5 digits
	else totalx = 192;


	Goto_Text_XY(totaly,totalx);
	Show_String((char *)offerText);
	if(fullcolor) Foreground_color_256(color256_green);
	else Foreground_color_256(MONOCHROME_COLOR);
	Show_String(" x 2 x 2");
	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);
	Show_String(" = ");
	Show_String((char *)offerMultiplied);

	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

	Font_Width_X2();
	Font_Height_X2();
	Goto_Text_XY(creditsy,creditsx);           // was 245, 806
	Show_String("CREDITS");
//	Font_Width_X4();
//	Font_Height_X4();

    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);
    if(currentTask == 0) HAL_Delay(holdTime);


	while(currentTask == 0)
	{
		if(fullcolor) rndColor = RandomColor_256();
		else rndColor = MONOCHROME_COLOR;
		Font_Width_X4();
		Font_Height_X4();
		Foreground_color_256(rndColor);
		Goto_Text_XY(65,190);
		Show_String("TOTAL BONUS WIN");
		Goto_Text_XY(creditsy,creditsx);          // was 245,806
		Font_Width_X2();
		Font_Height_X2();
		Show_String("CREDITS");
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	    if(currentTask == 0) HAL_Delay(holdTime);
	    else break;
	};
}



// DOUBLE Logo On payline with max bet
void screen0x1B(void)
{
	unsigned int xOffset = 68;  //was 18

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

	//BTE_logo_disp(62,140,3,373,247);
	if(fullcolor) BTE_logo_disp(62,140,3,373,247);
	else BTE_logo_disp(62,140,3,480,320);
    //BTE_logo_disp(55,100,2,480,264);  // NEW LOGO   4-11-2024  it was 50, 40, 3   4-12-2024 30,48,3

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	Font_Select_12x24_24x24();
	Set_GTFont_Decoder(0x21);
	Font_Width_X2();
	Font_Height_X3();

	Goto_Text_XY(44,624-xOffset);       //  Y, X  4-11-2024  it was 40, 624

	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

	Background_color_256(color256_black);
	Show_String("ON  PAYLINE  WITH  MAX");
	Goto_Text_XY(140,640-xOffset);      //        4-11-2024  it was 160, 640
	Show_String("CREDITS  BET  AWARDS");
	Goto_Text_XY(234,670-xOffset);      //        4-11-2024  it was 280, 670
	Show_String("TOP  DOLLAR  BONUS");
	currentTask = 0;
}


// DOUBLE TOP DOLLAR BONUS
void screen0x1C(void)
{
	int		x           = 90;
	int 	y           = 0;
	int     minx        = 0;
	int     miny        = 0;
	int     maxx        = 0;
	int     maxy        = 0;
	int     scrollDelay = 2;
	uint8_t dx          = 0;
	uint8_t dy          = 0;

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

	currentTask = 0;

	Select_Main_Window_8bpp();
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);

	Main_Image_Start_Address(layer_start_addr(0));

	VSCAN_T_to_B();
	Select_PIP1_Parameter();
	Select_PIP1_Window_8bpp();//
	PIP_Display_Start_XY(0,0);
	PIP_Image_Start_Address(layer_start_addr(4));
	PIP_Image_Width(400);
	PIP_Window_Image_Start_XY(0,0);


	if(fullcolor)	PIP_Window_Width_Height(247,373);
	else PIP_Window_Width_Height(160,240);
	minx = 10;
	maxx = 160;
	miny = 30;
	maxy = 870;

	if(currentTask == 0) Enable_PIP1();
	if(currentTask == 0) HAL_Delay(50);

	while(currentTask == 0)
	{
		if(currentTask) break;
		PIP_Display_Start_XY(x,y);
		if(currentTask) break;

		if(dx == 0) x++;
		else x--;

		if(currentTask) break;

		if(x > maxx) dx = 1;     // 4-16-2024  it was 170
		else if(x < minx) dx = 0; // 4-16-2024  it was 46

		if(currentTask) break;

		if(dy == 0) y++;
		else y--;

		if(currentTask) break;

		if(y > maxy) dy = 1;     // 4-16-2024  it was 600
		else if(y < miny) dy = 0; // 4-16-2025  it was 90

		if(currentTask) break;

		if(currentTask == 0) HAL_Delay(scrollDelay);

		if(currentTask) break;
	}

	VSCAN_B_to_T();
	Main_Image_Start_Address(layer_start_addr(10));
}


// PLAY DOUBLE TOP DOLLAR
#define num_of_x2 10
void screen0x1D(void)
{
	int xOffset              = -20;
	int yOffset				 = -20;

	int  i                   = 0;
	int  j                   = 0;
	int  playFrameDelay      = 10;
	int  topdollarFrameDelay = 20;
	int  dx[num_of_x2]  = { 100, 130, 265, 380, 400, 860, 990,1080,1080,1120};
	int  dy[num_of_x2]  = { 112, 210, 140, 110,  57, 118, 130, 112, 196, 210};
	int  dc[num_of_x2]  = {0,};
	char ds[num_of_x2]  = {0x22,0x11,0x00,0x11,0x00,0x11,0x00,0x11,0x00,0x11};
	// This is for font size.  I predict you suggest the font  size ahahahaha
	// 0xAB		A is width and B is Height. 0 is minimum and 3 is maximum.

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);
	//Font_Background_select_Transparency();
	currentTask = 0;
	while(currentTask == 0)
	{
		// Clear Screen
		Foreground_color_256(color256_black);
		Background_color_256(color256_black);
		Line_Start_XY(0,0);
		Line_End_XY(399,1279);  //  Y,X
		Start_Square_Fill();

		// Draw "DOUBLE"
		//Foreground_color_256(color256_white);
		// Top Left
		dc[i] = RandomColor_256();
		if(fullcolor) Foreground_color_256(dc[i]);
		else Foreground_color_256(dc[i] > 0x0f ? MONOCHROME_COLOR:color256_black);
		Goto_Text_XY(52,100);  // y, x
		Font_Width_X2();
		Font_Height_X2();
		Show_String("DOUBLE");

		// Top Right
		dc[i] = RandomColor_256();
		if(fullcolor) Foreground_color_256(dc[i]);
		else Foreground_color_256(dc[i] > 0x0f ? MONOCHROME_COLOR:color256_black);
		Goto_Text_XY(52,900);  // y, x
		Font_Width_X2();
		Font_Height_X2();
		Show_String("DOUBLE");

		// Bottom Left
		dc[i] = RandomColor_256();
		if(fullcolor) Foreground_color_256(dc[i]);
		else Foreground_color_256(dc[i] > 0x0f ? MONOCHROME_COLOR:color256_black);
		Goto_Text_XY(280,90);  // y, x
		Font_Width_X1();
		Font_Height_X1();
		Show_String("DOUBLE");

		// Bottom Right
//	    dy[0] = 112;        // dy[0] will be something else (233?) after the next line executes
//		dc[i] = rand()%0xff;   // Why does this change dy[0] ?
		dc[i] = RandomColor_256();
		dy[0] = 112;           // Resetting it here

		if(fullcolor) Foreground_color_256(dc[i]);
		else Foreground_color_256(dc[i] > 0x0f ? MONOCHROME_COLOR:color256_black);
		Goto_Text_XY(280,1060);  // y, x
		Font_Width_X1();
		Font_Height_X1();
		Show_String("DOUBLE");


		// Draw x2's
		for(i = 0;i<num_of_x2;i++)
		{
			//if(currentTask) break;
			//dc[i] = rand()%0xff;
			dc[i] = RandomColor_256();
			Goto_Text_XY(dy[i],dx[i]);
			if(fullcolor) Foreground_color_256(dc[i]);
			else Foreground_color_256(dc[i] > 0x0f ? MONOCHROME_COLOR:color256_black);
			switch(ds[i]>>4)
			{
			case 0:		Font_Width_X1();			break;
			case 1:		Font_Width_X2();			break;
			case 2:		Font_Width_X3();			break;
			case 3:		Font_Width_X4();			break;
			}
			switch(ds[i]&0x0f)
			{
			case 0:		Font_Height_X1();			break;
			case 1:		Font_Height_X2();			break;
			case 2:		Font_Height_X3();			break;
			case 3:		Font_Height_X4();			break;
			}
			Show_String("x2");
		}

//		Font_Width_X4();
//		Font_Height_X4();
		for(j = 0;j<60;j+=2)
		{
			// Clear Region PLAY
			//if(currentTask) break;
			Foreground_color_256(color256_black);
			Background_color_256(color256_black);
			Line_Start_XY(0,540+xOffset);
			Line_End_XY(180, 870+xOffset);  //  Y,X
			Start_Square_Fill();


			for(i = 0;i<num_of_x2;i++)
			{
				//if(currentTask) break;
				//dc[i] = rand()%0xff;
				dc[i] = RandomColor_256();
				Goto_Text_XY(dy[i],dx[i]);
				if(fullcolor) Foreground_color_256(dc[i]);
				else Foreground_color_256(dc[i] > 0x0f ? MONOCHROME_COLOR:color256_black);
				switch(ds[i]>>4)
				{
					case 0:		Font_Width_X1();			break;
					case 1:		Font_Width_X2();			break;
					case 2:		Font_Width_X3();			break;
					case 3:		Font_Width_X4();			break;
				}

				switch(ds[i]&0x0f)
				{
					case 0:		Font_Height_X1();			break;
					case 1:		Font_Height_X2();			break;
					case 2:		Font_Height_X3();			break;
					case 3:		Font_Height_X4();			break;
				}
				Show_String("x2");
			}

			Font_Width_X4();
			Font_Height_X4();
			if(fullcolor) Foreground_color_256(color256_white);
			else Foreground_color_256(MONOCHROME_COLOR);

			Goto_Text_XY(j+yOffset,535+xOffset);
			Show_String("PLAY");
			if(currentTask != 0) break;
			HAL_Delay(playFrameDelay);
		}
		if(currentTask != 0) break;
		//if(currentTask) break;
		for(j = 0; j < 200; j+=10)       //  Change J from 200 to 220??
		{
			//if(currentTask) break;
			// Clear Region TOP DOLLAR
			Foreground_color_256(color256_black);
			Background_color_256(color256_black);
			Line_Start_XY(200,240+xOffset);
			Line_End_XY(400, 1080+xOffset);  //  Y,X
			Start_Square_Fill();

			for(i = 0; i < num_of_x2; i++)
			{
				//if(currentTask) break;
				//dc[i] = rand()%0xff;
				dc[i] = RandomColor_256();
				Goto_Text_XY(dy[i],dx[i]);
				if(fullcolor) Foreground_color_256(dc[i]);
				else Foreground_color_256(dc[i] > 0x0f ? MONOCHROME_COLOR:color256_black);

				switch(ds[i]>>4)
				{
					case 0:		Font_Width_X1();			break;
					case 1:		Font_Width_X2();			break;
					case 2:		Font_Width_X3();			break;
					case 3:		Font_Width_X4();			break;
				}
				switch(ds[i]&0x0f)
				{
					case 0:		Font_Height_X1();			break;
					case 1:		Font_Height_X2();			break;
					case 2:		Font_Height_X3();			break;
					case 3:		Font_Height_X4();			break;
				}
				Show_String("x2");
			}

			if(fullcolor) Foreground_color_256(color256_white);
			else Foreground_color_256(MONOCHROME_COLOR);

			Font_Width_X4();
			Font_Height_X4();

			Goto_Text_XY(((390-j)+yOffset),250+xOffset);
			Show_String("TOP DOLLAR");
			if(currentTask != 0) break;
			HAL_Delay(topdollarFrameDelay);
		}
		if(currentTask != 0) break;
		for(j = 0;j<110;j++)
		{
			if(currentTask != 0) break;
			//if(currentTask) break;
			for(i = 0;i<num_of_x2;i++)
			{
				//dc[i] = rand()%0xff;
				dc[i] = RandomColor_256();
				Goto_Text_XY(dy[i],dx[i]);
				if(fullcolor) Foreground_color_256(dc[i]);
				else Foreground_color_256(dc[i] > 0x0f ? MONOCHROME_COLOR:color256_black);

				switch(ds[i]>>4)
				{
					case 0:		Font_Width_X1();			break;
					case 1:		Font_Width_X2();			break;
					case 2:		Font_Width_X3();			break;
					case 3:		Font_Width_X4();			break;
				}
				switch(ds[i]&0x0f)
				{
					case 0:		Font_Height_X1();			break;
					case 1:		Font_Height_X2();			break;
					case 2:		Font_Height_X3();			break;
					case 3:		Font_Height_X4();			break;
				}
				Show_String("x2");
			}
			HAL_Delay(20);
		}
	}
}


// Double Top Dollar scrolling top and bottom and text in the middle
void screen0x1E(void)
{
	unsigned char	   topText[80] = {0};
	unsigned char   bottomText[80] = {0};
	int                          c = 0;
	int							 d = 0;
	int 					  oldc = 0;
	int 				      oldd = 14;
	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);
    if(fullcolor) Foreground_color_256(color256_white);
    else Foreground_color_256(MONOCHROME_COLOR);

   	Background_color_256(color256_black);

    Disable_Font_Alignment();
	//Font_Background_select_Color();
	Font_Background_select_Transparency();
	Goto_Text_XY(116,225);       //  Y, X  4-19-2024  it was 116, 95
    Font_Width_X3();
    Font_Height_X4();
	Show_String((char *)screen0x01Text);
	screen0x01Text[0] = '\0';

    // Draw Top bar area
	if(fullcolor)
		Foreground_color_256(color256_green);
	else
		Foreground_color_256(MONOCHROME_COLOR);

	Background_color_256(color256_black);
	Line_Start_XY(58,90);  //       4-11-2024  it was 0, 0
    Line_End_XY(92,1240);  //  Y,X  4-11-2024  it was 35, 1279   1215
    Start_Square_Fill();

    // Draw Bottom bar area
	if(fullcolor) Foreground_color_256(color256_green);
	else Foreground_color_256(MONOCHROME_COLOR);

	Background_color_256(color256_black);
	Line_Start_XY(286,90);   //       4-11-2024 it was 365, 0
    Line_End_XY(318,1240);  //  Y,X  4-11-2024 it was 399, 1279
    Start_Square_Fill();

	Enable_Font_Alignment();      // 46 Chars Wide
	//Disable_Font_Alignment();     // 76 Chars Wide
	Font_Background_select_Color();
	//Font_Background_select_Transparency();
//    Foreground_color_256(color256_black);
//	Background_color_256(color256_black);
    Font_Width_X1();
    Font_Height_X1();
    // Length of displayText is 15
    // 75 chars total on one line
    // Make this DOUBLE TOP DOLLAR LATER
    displayText[0]  = 'D';  displayText[1]  = 'O';  displayText[2]  = 'U';  displayText[3]  = 'B';
    displayText[4]  = 'L';  displayText[5]  = 'E';  displayText[6]  = ' ';  displayText[6]  = 'T';
    displayText[7]  = 'O';  displayText[8]  = 'P';  displayText[9]  = ' ';	displayText[10] = 'D';
    displayText[11] = 'O';  displayText[12] = 'L';  displayText[13] = 'L';	displayText[14] = 'A';
    displayText[15] = 'R';  displayText[16] = ' ';  displayText[17] = ' ';	displayText[18] = ' ';
    displayText[19] = ' ';  displayText[20] = ' ';  displayText[21] = '\0';

/*
	// Fill array
	// topText
	c = 0;
	for (int b = 0; b < 45; b++)       // 4-12-2024  it was b < 46
	{
		topText[b] = displayText[c];
		c++;
		if (c == 15) c = 0;
	}

	// bottomText
	d = 14;
//	for (int b = 75; b != -1; b--)
	for (int b = 45; b != -1; b--)      // 4-12-2024  it was b = 43
	{
		bottomText[b] = displayText[d];
		d--;
		if (d == -1) d = 14;
	}
*/
	c = 0;
	d = 0;
	currentTask = 0;
	while(currentTask == 0)
	{
		// Scroll Left
		oldc++;
		if(oldc == 21) oldc = 0;
		c = oldc;
		for (int b = 0; b < 38; b++)      // 4-12-2024 it was b < 41
		{
			topText[b] = displayText[c];
			c++;
			if (c == 21) c = 0;
		}

		// Scroll Right
		oldd--;
		if (oldd == -1) oldd = 20;
		d = oldd;
		//for (int b = 75; b != -1; b--)
		for (int b = 38; b != -1; b--)      // 4-12-2024  it was b=41
		{
			bottomText[b] = displayText[d];
			d--;
			if (d == -1) d = 20;   // 14
		}

		// TOP DOLLAR show top
	    // Draw Top bar area
		if(fullcolor) Foreground_color_256(color256_green);
		else Foreground_color_256(MONOCHROME_COLOR);

		Background_color_256(color256_black);
		Line_Start_XY(58,90);  //       4-11-2024  it was 0, 0
	    Line_End_XY(92,1240);  //  Y,X  4-11-2024  it was 35, 1279   1215
	    Start_Square_Fill();

	    Foreground_color_256(color256_black);   // Background

	    if(fullcolor) Background_color_256(color256_green);   // Text
	    else Background_color_256(MONOCHROME_COLOR);

	    Goto_Text_XY(58,90);  //  Y, X  4-11-2024  it was 0, 0   48
		Show_String((char *)topText);

	    // Draw Bottom bar area
		if(fullcolor) Foreground_color_256(color256_green);
		else Foreground_color_256(MONOCHROME_COLOR);

		Background_color_256(color256_black);
		Line_Start_XY(288,90);
	    Line_End_XY(318,1240);  //  318
	    Start_Square_Fill();

	    // TOP DOLLAR show bottom text
		Goto_Text_XY(286,90);       //  282,90
		Foreground_color_256(color256_black);

		if(fullcolor) Background_color_256(color256_green);   // Text
	    else Background_color_256(MONOCHROME_COLOR);

		Show_String((char *)bottomText);
	}
}



// 3 DOUBLE symbols on a played line...
void screen0x1F(void)
{
	unsigned int xOffset = 75;
//	unsigned int yOffset = 26;

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();

    if(fullcolor) BTE_logo_disp(62,240,3,373,247);
    else BTE_logo_disp(62,240,3,480,320);
    //BTE_logo_disp(55,180,2,480,264);  // 4-11-2024  it was 40, 142, 3.. 4-12-2024 it was 30, 232, 3
	Select_Main_Window_8bpp();
	Main_Image_Start_Address(layer_start_addr(10));
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

	Canvas_Image_Start_address(layer_start_addr(10));
	Canvas_image_width(400);
	Active_Window_XY(0,0);
	Active_Window_WH(400,1280);
	if(fullcolor) Foreground_color_256(color256_white);
	else Foreground_color_256(MONOCHROME_COLOR);

	Background_color_256(color256_black);
	//Font_Select_16x32_32x32();
	Font_Select_12x24_24x24();
	Set_GTFont_Decoder(0x21);
	Font_Width_X2();
	Font_Height_X4();
	Goto_Text_XY(38,678);       //  Y, X  4-11-2024  it was 20, 750
	Show_String("ON  A  PLAYED  LINE");
	Goto_Text_XY(130,820);      //        4-11-2024  it was 142, 870
	Show_String("AWARDS");
	Goto_Text_XY(220,730-xOffset);      //        4-11-2024  it was 260, 730
	Show_String("TOP  DOLLAR  BONUS");
	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);
	Font_Width_X4();
	Font_Height_X4();
	Goto_Text_XY(113,128);       //   108
	Show_String("3");
	currentTask = 0;
}


#ifdef TIMEBOMB
void screen0x99(void)
{
    // Clear Screen
    Foreground_color_256(color256_black);
    Line_Start_XY(0,0);
    Line_End_XY(402,1279);  //  Y,X
    Start_Square_Fill();

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	//Font_Select_12x24_24x24();
	Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);               //  <--- Activate the chip...
	Font_Background_select_Color();
	//Font_Background_select_Transparency();

	Foreground_color_256(color256_black);
	Background_color_256(color256_yellow);

    Goto_Text_XY(130,140);   // Y,X
    Font_Width_X3();
    Font_Height_X3();
    Show_String("BETA RUNTIME OVER.");

}
#endif


void WelcomeAnimation(void)
{
	uint16_t it;

	CGROM_Select_Genitop_FontROM();
	GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!
	Font_Select_12x24_24x24();
	//Font_Select_16x32_32x32();
	Set_GTFont_Decoder(0x21);
    Font_Width_X1();
    Font_Height_X3();

	for(it = 0;it<640;it++)
	{
		Foreground_color_256(color256_blue);
		Triangle_Point1_XY(0,it);
		Triangle_Point2_XY(399,it);
		Triangle_Point3_XY(300,it+200);
		Start_Triangle_Fill();
		HAL_Delay(1);
	}

	for(it = 1;it<1000;it++)
	{
		Foreground_color_256(color256_green);
		Line_Start_XY(100,1279-it);
		Line_End_XY(200,1279 - it + 1);
		Start_Square_Fill();
		HAL_Delay(1);
	}

	Font_Background_select_Transparency();

	Foreground_color_256(color256_white);
	Font_Width_X1();
	Font_Height_X1();
	Goto_Text_XY(290,104);
	Show_String("LCD10014 - 3 REEL TOP DOLLAR");

#ifdef TIMEBOMB
	// Show TIMEBOMB Enabled in lower-right
	Foreground_color_256(color256_yellow);
	Font_Width_X1();
	Font_Height_X1();
	Goto_Text_XY(70,950);
	Show_String("TIMEBOMB ENABLED");
#endif

	Font_Width_X1();
	Font_Height_X1();
	Goto_Text_XY(290,904);  // 76
	if(fullcolor)
	{
		Foreground_color_256(RandomColor_256());
		Show_String("C");
		Foreground_color_256(RandomColor_256());
		Show_String("O");
		Foreground_color_256(RandomColor_256());
		Show_String("L");
		Foreground_color_256(RandomColor_256());
		Show_String("O");
		Foreground_color_256(RandomColor_256());
		Show_String("R");
	}
	else
	{
	Foreground_color_256(color256_cyan);
	Show_String("MONOCHROME");
	}
	Foreground_color_256(color256_white);
	Show_String(" mode enabled.");


	Foreground_color_256(color256_red);
	Goto_Text_XY(115,310);
	Font_Width_X3();
	Font_Height_X3();
	Show_String("Caching 0 of 4...");
}

void BTE_logo_disp(int x, int y, int logo_index, int w, int h)
{
	BTE_S0_Memory_Start_Address(layer_start_addr(logo_index));
	BTE_S0_Image_Width(400);
	BTE_S0_Window_Start_XY(0,0);
	BTE_S1_Memory_Start_Address(layer_start_addr(logo_index));
	BTE_S1_Image_Width(400);
	BTE_S1_Window_Start_XY(0,0);
	BTE_Destination_Memory_Start_Address(layer_start_addr(10));
	BTE_Destination_Image_Width(400);
	BTE_Destination_Window_Start_XY(x,y);
	BTE_S0_Color_8bpp();
	BTE_S1_Color_8bpp();
	BTE_Destination_Color_8bpp();
	BTE_Window_Size(h,w);		// original 304,560..  New 248,448
	BTE_ROP_Code(12);
	//BTE_ROP_Code(3);
	BTE_Operation_Code(2); //BTE move
	BTE_Enable();		  //memory copy s0(layer2) to layer1
	Check_BTE_Busy();
}

uint8_t RandomColor_256(void)
{
	uint8_t ret = rand()%6;
	return colors[ret];
//	uint8_t ret;
//	ret = rand();
//	return ret;
}

uint16_t RandomColor_65k(void)
{
	uint8_t ret;
	ret = rand();
	return ret;
//	uint16_t ret = rand()%6;
//	return colors[ret];
}

uint32_t RandomColor_16M(void)
{
	uint32_t ret = rand();
	return ret;
}

void preloadScreen(void)
{

	int x_text_offset     = 616;
	int y_text_offset     = 115;
	int x_green_box_start = 608;
	int y_green_box_start = 122;

	// Update "Caching screen 1 of x text on screen...
	Foreground_color_256(color256_green);
	Line_Start_XY(y_green_box_start,x_green_box_start);
	Line_End_XY(190,656);
	Start_Square_Fill();

	Foreground_color_256(color256_red);
	Background_color_256(color256_green);
	Font_Width_X3();
	Font_Height_X3();
	Font_Background_select_Transparency();
	Goto_Text_XY(y_text_offset,x_text_offset);
	Show_String("1");

	// Cache graphic 1
	BTE_Destination_Image_Width(400);
	BTE_Destination_Window_Start_XY(0,0);
	BTE_S0_Color_8bpp();
	BTE_S1_Color_8bpp();
	BTE_Destination_Color_8bpp();
	BTE_ROP_Code(12);

	BTE_Destination_Memory_Start_Address(layer_start_addr(0));
	BTE_Window_Size(400,1280);
	BTE_Operation_Code(0);
	BTE_Enable();
	if(fullcolor == 1)
		Show_pictureX2(400,1280,NewBonusBackground_Sized);
	else
		Show_pictureX2_mono(400,1280,NewBonusBackground_Sized);
		//Show_picture2(64000,screen05final_400x1280_NORMAL);

	Check_BTE_Busy();  // <-- Not needed, Show_Picture runs until complete..


	// Update "Caching screen x of 3 text on screen...
	Foreground_color_256(color256_green);
	Line_Start_XY(y_green_box_start,x_green_box_start);
	Line_End_XY(190,656);
	Start_Square_Fill();

	Foreground_color_256(color256_red);
	Goto_Text_XY(y_text_offset,x_text_offset);
	Show_String("2");


	// Cache graphic 2

	//Foreground_color_256(color256_green);
	Foreground_color_256(MONOCHROME_COLOR);
	Background_color_256(color256_black);
	BTE_Destination_Memory_Start_Address(layer_start_addr(1));
	BTE_Operation_Code(0);
	BTE_Window_Size(198,360);
	BTE_Enable();
	if(fullcolor) Show_picture3(198,360,Small_Top_Black_198x360);
	else Show_greenPicture(198,360,Small_Top_Black_198x360);
	//Show_picture2(11200,TopDollar2_CooperBlack_Font_FRAMED_NORMAL_160x560);
	Check_BTE_Busy();  // <-- Not needed, Show_Picture runs until complete..

	// Update "Caching screen x of 3 text on screen...
	Foreground_color_256(color256_green);
	Line_Start_XY(y_green_box_start,x_green_box_start);
	Line_End_XY(190,656);
	Start_Square_Fill();

	Foreground_color_256(color256_red);
	Background_color_256(color256_green);
	Goto_Text_XY(y_text_offset,x_text_offset);
	Show_String("3");


	// Cache graphic 3
	BTE_Destination_Memory_Start_Address(layer_start_addr(2));
	BTE_Window_Size(264,480);
	BTE_Operation_Code(0);
	BTE_Enable();
	if(fullcolor)
		Show_picture3(264,480,NewTOP);
	else
		Show_greenPicture(264,480,NewTOP);//

	Check_BTE_Busy();  // <-- Not needed, Show_Picture runs until complete..

	// Update "Caching screen 3 of 4 text on screen...
	Foreground_color_256(color256_green);
	Line_Start_XY(y_green_box_start,x_green_box_start);
	Line_End_XY(190,656);
	Start_Square_Fill();

	Foreground_color_256(color256_red);
	Background_color_256(color256_green);
	Font_Width_X3();
	Font_Height_X3();
	Font_Background_select_Transparency();
	Goto_Text_XY(y_text_offset,x_text_offset);
	Show_String("4");

	BTE_Destination_Memory_Start_Address(layer_start_addr(3));
	if(fullcolor) 	BTE_Window_Size(247,373);
	else 	BTE_Window_Size(160,240);
	BTE_Operation_Code(0);
	BTE_Enable();
	if(fullcolor)
		Show_picture3(247,373,DoubleTopLogo_Gold);
	else
		Show_greenPicture(160,240,DoubleTop_Mono);

	BTE_Destination_Memory_Start_Address(layer_start_addr(4));
	if(fullcolor) BTE_Window_Size(247,373);
	else BTE_Window_Size(160,240);
	BTE_Operation_Code(0);
	BTE_Enable();
	if(fullcolor)
		Show_picture2(92131,DoubleTopLogo_Gold);
	else
		Show_picture2_mono(38400,DoubleTop_Mono);

	Check_BTE_Busy();  // <-- Not needed, Show_Picture runs until complete..

	// Done loading graphics...
	Foreground_color_256(color256_green);
	Line_Start_XY(100,280);
	Line_End_XY(200,1279);
	Start_Square_Fill();

	Goto_Text_XY(115,310);
	Foreground_color_256(color256_red);
	Background_color_256(color256_green);
	Font_Width_X3();
	Font_Height_X3();
	Show_String("COMPLETE");
	HAL_Delay(500);

	// Reset screen settings
	Select_Main_Window_8bpp();
	Main_Image_Start_Address(layer_start_addr(10));
	Main_Window_Start_XY(0,0);
	Canvas_Image_Start_address(layer_start_addr(10));
	Active_Window_XY(0,0);
	Font_Background_select_Color();
	Main_Image_Width(400);
	Canvas_image_width(400);
	Active_Window_WH(400,1280);

	// Clear Screen
	Foreground_color_256(color256_black);
	Background_color_256(color256_black);
	Line_Start_XY(0,0);
    Line_End_XY(399,1279);  //  Y,X
    Start_Square_Fill();
}
