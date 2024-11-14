#include "main.h"
#include "RA8876.h"
#include "8876demo.h"
#include "pattern.h"
#include "stdlib.h"

int           imgWidth			= 400;
int			  imgHeight			= 560;


void BTE_Compare(void)
{
   unsigned int i,temp;
	unsigned long im=1;


  	Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

	Canvas_Image_Start_address(0);//Layer 1
	Canvas_image_width(400);//
    Active_Window_XY(0,0);
	Active_Window_WH(400,1280);

 	Foreground_color_65k(White);
	Line_Start_XY(0,0);
	Line_End_XY(399,1247);
	Start_Square_Fill();

	Foreground_color_65k(Blue2);
	Line_Start_XY(0,1248);
	Line_End_XY(399,1279);
	Start_Square_Fill();

	//Foreground_color_65k(White);
	//Background_color_65k(Blue2);
	//Goto_Text_XY(300,576);
	//Show_String("Demo DMA");


	Foreground_color_65k(White);
	Background_color_65k(Blue2);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(0,Line52);
	Show_String(" Demo BTE Compare");
	Foreground_color_65k(Black);
	Background_color_65k(White);
	Font_Select_12x24_24x24();
	Goto_Text_XY(0,Line51);
	Show_String("Execute Logic 'OR' 0xf000");



    //DMA initial setting
	Enable_SFlash_SPI();
    Select_SFI_1();
    Select_SFI_DMA_Mode();
    Select_SFI_24bit_Address();

    //Select_SFI_Waveform_Mode_0();
    Select_SFI_Waveform_Mode_3();

    //Select_SFI_0_DummyRead();	//normal read mode
    Select_SFI_8_DummyRead(); //1byte dummy cycle
    //Select_SFI_16_DummyRead();
   //Select_SFI_24_DummyRead();

    Select_SFI_Single_Mode();
    //Select_SFI_Dual_Mode0();
   // Select_SFI_Dual_Mode1();

    SPI_Clock_Period(0);


	SFI_DMA_Destination_Upper_Left_Corner(50,40);
    SFI_DMA_Transfer_Width_Height(200,200);
    SFI_DMA_Source_Width(400);//

	SFI_DMA_Source_Start_Address(2457600);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();

	SFI_DMA_Destination_Upper_Left_Corner(50+200+50,40);
    SFI_DMA_Transfer_Width_Height(200,200);
    SFI_DMA_Source_Width(400);//

	SFI_DMA_Source_Start_Address(2048000);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();

	SFI_DMA_Destination_Upper_Left_Corner(50+200+50+200+50,40);
    SFI_DMA_Transfer_Width_Height(200,200);
    SFI_DMA_Source_Width(400);//

	SFI_DMA_Source_Start_Address(im*400*1280*2*2);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();

 	Foreground_color_65k(Black);
	Background_color_65k(White);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(50,40+200+40 );
	Show_String("Without BTE");
 	Goto_Text_XY(50+200+50,40+200+40 );
	Show_String("With BTE Write");
  	Goto_Text_XY(50+200+50,40+200+40+24 );
	Show_String("ROP");
  	Goto_Text_XY(50+200+50+200+50,40+200+40 );
	Show_String("With BTE Move");
  	Goto_Text_XY(50+200+50+200+50,40+200+40+24 );
	Show_String("ROP");

	HAL_Delay(1000);
	 //first block, MCU read and 'OR'0xff00 then write back
   //while(1)
   //{
	   //GPIO_SetBits(GPIOC, GPIO_Pin_1);		//test pin
	   Active_Window_XY(50,40);
	   Active_Window_WH(200,200);

	    Goto_Pixel_XY(50,40);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();  //dummy

	    for(i=0; i<200*200;i++)
		  {
		   temp = LCD_DataRead();
		   Check_Mem_RD_FIFO_not_Empty();
		   temp |= 0xf000;
		   LCD_DataWrite(temp);
		   Check_Mem_WR_FIFO_not_Full();
		   }

	  //GPIO_ResetBits(GPIOC, GPIO_Pin_1);		//test pin
	// }
		Active_Window_XY(0,0);
	    Active_Window_WH(400,1280);

	  //spent 51.12ms
	 	Foreground_color_65k(Black);
		Background_color_65k(White);
		CGROM_Select_Internal_CGROM();
		Font_Select_12x24_24x24();
		Goto_Text_XY(50,40+200+40+72 );
		Show_String("Spent 51.12ms");

	   HAL_Delay(1000);

	  //second block, MCU write with BTE ROP

	  //while(1)
      //{
	   //GPIO_SetBits(GPIOC, GPIO_Pin_1);

	   BTE_S0_Color_16bpp();

       BTE_S1_Color_16bpp();
       BTE_S1_Memory_Start_Address(0);
       BTE_S1_Image_Width(400);
       BTE_S1_Window_Start_XY(50+200+50,40);

       BTE_Destination_Color_16bpp();
       BTE_Destination_Memory_Start_Address(0);
       BTE_Destination_Image_Width(400);
       BTE_Destination_Window_Start_XY(50+200+50,40);
       BTE_Window_Size(200,200);

	   BTE_ROP_Code(14);
       BTE_Operation_Code(0); //BTE write
       BTE_Enable();

       LCD_CmdWrite(0x04);
       	for(i=0; i<200*200;i++)
		  {
		   LCD_DataWrite(0xf0);
		   Check_Mem_WR_FIFO_not_Full();
		   }
       Check_Mem_WR_FIFO_Empty();//
       Check_BTE_Busy();

	   //GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	   //}
	   //spent 25.56ms
 	 	Foreground_color_65k(Black);
		Background_color_65k(White);
		CGROM_Select_Internal_CGROM();
		Font_Select_12x24_24x24();
		Goto_Text_XY(50+200+50,40+200+40+72 );
		Show_String("Spent 25.56ms");
	   HAL_Delay(1000);




	  //third block, BTE MOVE with ROP
	  //while(1)
      //{
	  // GPIO_SetBits(GPIOC, GPIO_Pin_1);
	   	Canvas_Image_Start_address(layer_start_addr(1));//
	    Canvas_image_width(400);//
        Active_Window_XY(0,0);
	    Active_Window_WH(400,1280);

 	    Foreground_color_65k(0xf000);
	    Line_Start_XY(0,0);
	    Line_End_XY(200,200);
	    Start_Square_Fill();
		//Main_Image_Start_Address(layer_start_addr(1));

		BTE_S0_Color_16bpp();
		BTE_S0_Memory_Start_Address(layer_start_addr(1));
		BTE_S0_Image_Width(400);
	    BTE_S0_Window_Start_XY(0,0);

        BTE_S1_Color_16bpp();
        BTE_S1_Memory_Start_Address(layer_start_addr(0));
        BTE_S1_Image_Width(400);
        BTE_S1_Window_Start_XY(50+200+50+200+50,40);

        BTE_Destination_Color_16bpp();
        BTE_Destination_Memory_Start_Address(layer_start_addr(0));
        BTE_Destination_Image_Width(400);
        BTE_Destination_Window_Start_XY(50+200+50+200+50,40);
        BTE_Window_Size(200,200);

	    BTE_ROP_Code(14);
        BTE_Operation_Code(2); //BTE write
        BTE_Enable();
		Check_BTE_Busy();

		//GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	   //}
	    //spent 6.4ms
		Canvas_Image_Start_address(layer_start_addr(0));//
	 	Foreground_color_65k(Black);
		Background_color_65k(White);
		CGROM_Select_Internal_CGROM();
		Font_Select_12x24_24x24();
		Goto_Text_XY(50+200+50+200+50,40+200+40+72 );
		Show_String("SSpent 6.4ms");
			HAL_Delay(1000);

}


void BTE_Color_Expansion(void)
{
	// Line 337, set height and width  <-- tweaking this line
	// Line 368, set first parameter as array count
	// Line 411, must be the same as line 368

	unsigned int i,j;
    //unsigned long im=1;

 	Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
    Main_Image_Width(400);  // worked in portrait mode
	Main_Window_Start_XY(0,0);

	Canvas_Image_Start_address(layer_start_addr(0));//Layer 1
    Canvas_image_width(400); // worked in portrait mode
	Active_Window_XY(0,0);
    Active_Window_WH(400,1280);  // worked in portrait mode

 	Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1247);
	Start_Square_Fill();

	// This is vertical blue bar.. Do not need this
	//Foreground_color_65k(Blue2);
	//Line_Start_XY(0,1248);
	//Line_End_XY(399,1279);
	//Start_Square_Fill();


	// This is light blue text showing up in wrong place
	//Foreground_color_65k(White);
	//Background_color_65k(Blue2);
	//CGROM_Select_Internal_CGROM();
	//Font_Select_12x24_24x24();
	//Goto_Text_XY(0,Line52);
	//Show_String(" Demo BTE Color Expansion");


    //DMA initial setting
//	Enable_SFlash_SPI();
//    Select_SFI_1();
//    Select_SFI_DMA_Mode();
//    Select_SFI_24bit_Address();

    //Select_SFI_Waveform_Mode_0();
//    Select_SFI_Waveform_Mode_3();

    //Select_SFI_0_DummyRead();	//normal read mode
//    Select_SFI_8_DummyRead(); //1byte dummy cycle
    //Select_SFI_16_DummyRead();
    //Select_SFI_24_DummyRead();

//    Select_SFI_Single_Mode();
    //Select_SFI_Dual_Mode0();
    //Select_SFI_Dual_Mode1();

//    SPI_Clock_Period(0);


//	SFI_DMA_Destination_Upper_Left_Corner(0,0);
//    SFI_DMA_Transfer_Width_Height(400,400);
//	SFI_DMA_Source_Width(400);//

//	SFI_DMA_Source_Start_Address(im*400*1280*3);//
//	Start_SFI_DMA();
//    Check_Busy_SFI_DMA();



	//color expansion and bte memory copy(move)

  j=0;
 do
 {
   for(i=0;i<3;i++)
   {
	 //switch to layer2 update screen, and execute color expansion and copy to layer1
	 Canvas_Image_Start_address(layer_start_addr(1));//
 	 Foreground_color_65k(Black);
	 Line_Start_XY(0,0);
	 Line_End_XY(399,1247);  //  Our screen is 400x1280..  should 1247 be 1279?
	 Start_Square_Fill();
	 //SFI_DMA_Source_Start_Address(640*480*2*i);//
//	 Start_SFI_DMA();
//     Check_Busy_SFI_DMA();

	 BTE_S0_Color_16bpp();

     BTE_S1_Color_16bpp();

     BTE_Destination_Color_16bpp();
     BTE_Destination_Memory_Start_Address(layer_start_addr(1));
     BTE_Destination_Image_Width(400);   // Worked in portrait
     BTE_Destination_Window_Start_XY(40,80);
     //BTE_Window_Size(560,304);           //560x304  // Worked in portait
     //BTE_Window_Size(304,560);  // This makes the White area wider
     BTE_Window_Size(320,560);
	 Foreground_color_65k(color65k_green);
     Background_color_65k(color65k_black);
	 BTE_ROP_Code(15);
     BTE_Operation_Code(8);              //BTE color expansion

	 BTE_Enable();
	 LCD_CmdWrite(0x04);
/*
	 switch(i)
	 {
	  case 0 :

			  Show_picture1(10*160,f1);
			  Check_Mem_WR_FIFO_Empty();
	          Check_BTE_Busy();
			  break;
	  case 1 :
			  Show_picture1(10*160,f2);
			  Check_Mem_WR_FIFO_Empty();
	          Check_BTE_Busy();
			  break;
	  case 2 :
			  Show_picture1(10*160,f3);
			  Check_Mem_WR_FIFO_Empty();
	          Check_BTE_Busy();
			  break;
	   default:
			  break;
	 }
	 */

/*
	Show_picture1(10640,TopDollar1_560x304);	//set first parameter as array count
	Check_Mem_WR_FIFO_Empty();
	Check_BTE_Busy();

	Foreground_color_65k(Black);
	Background_color_65k(White);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(0+70,0+70+160+20);
	Show_String("Color Expansion");



	 Foreground_color_65k(color65k_black);
	 Background_color_65k(color65k_white);

	 BTE_Operation_Code(9); //BTE color expansion with chroma key
	 BTE_Destination_Window_Start_XY(0,0);
	 BTE_Enable();
	 LCD_CmdWrite(0x04);
*/
/*
	 switch(i)
	 {
		  case 0 :
				  Show_picture1(10*160,f1);
				  Check_Mem_WR_FIFO_Empty();
				  Check_BTE_Busy();
				  break;
		  case 1 :
				  Show_picture1(10*160,f2);
				  Check_Mem_WR_FIFO_Empty();
				  Check_BTE_Busy();
				  break;
		  case 2 :
				  Show_picture1(10*160,f3);
				  Check_Mem_WR_FIFO_Empty();
				  Check_BTE_Busy();
				  break;
		   default:
				  break;
	 }
*/


		//Show_picture1(11200,TopDollarLogo);	// here must be same with line 368
		//Show_picture1(21280,Egor_TopDollar_Reverse_304x560);
		Check_Mem_WR_FIFO_Empty();
		Check_BTE_Busy();
		Foreground_color_65k(Black);
		Background_color_65k(White);
	//	CGROM_Select_Internal_CGROM();
	//	Font_Select_12x24_24x24();
	//	Goto_Text_XY(70,650);
	//	Show_String("Color Expansion");
	//	Goto_Text_XY(70,674);
	//	Show_String("Color With chroma key");

		 Foreground_color_65k(color65k_blue);
		 Background_color_65k(color65k_red);


		//BTE memory(move) layer2 to layer1
		//BTE_S0_Color_16bpp();
		BTE_S0_Memory_Start_Address(layer_start_addr(1));
		BTE_S0_Image_Width(400);  // worked in portrait mode
		BTE_S0_Window_Start_XY(0,0);

		//BTE_Destination_Color_16bpp();
		BTE_Destination_Memory_Start_Address(layer_start_addr(0));
		//BTE_Destination_Image_Width(304);  // worked in portrait mode
		BTE_Destination_Image_Width(400);   // This makes the image big
		BTE_Destination_Window_Start_XY(0,0);
		BTE_Window_Size(400,1247);  // worked in portrait mode

		BTE_ROP_Code(12);
		BTE_Operation_Code(2); //BTE move
		BTE_Enable();		  //memory copy s0(layer2) to layer1
		Check_BTE_Busy();


		j++;
	}

   }while(j<5);

 			

}



void BTE_Pattern_Fill(void)
{	    unsigned long im=1;

 	Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

	Canvas_Image_Start_address(0);//Layer 1
	Canvas_image_width(400);//
    Active_Window_XY(0,0);
	Active_Window_WH(400,1280);

 	Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1247);
	Start_Square_Fill();

	Foreground_color_65k(Blue2);
	Line_Start_XY(0,1248);
	Line_End_XY(399,1279);
	Start_Square_Fill();

	Foreground_color_65k(White);
	Background_color_65k(Blue2);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(0,Line52);
	Show_String("Demo BTE Pattern Fill");


    //DMA initial setting
	Enable_SFlash_SPI();
    Select_SFI_1();
    Select_SFI_DMA_Mode();
    Select_SFI_24bit_Address();

    //Select_SFI_Waveform_Mode_0();
    Select_SFI_Waveform_Mode_3();

    //Select_SFI_0_DummyRead();	//normal read mode
    Select_SFI_8_DummyRead(); //1byte dummy cycle
    //Select_SFI_16_DummyRead();
    //Select_SFI_24_DummyRead();

    Select_SFI_Single_Mode();
    //Select_SFI_Dual_Mode0();
    //Select_SFI_Dual_Mode1();

    SPI_Clock_Period(0);


	SFI_DMA_Destination_Upper_Left_Corner(0,40);
    SFI_DMA_Transfer_Width_Height(400,400);
    SFI_DMA_Source_Width(400);//

	SFI_DMA_Source_Start_Address(im*400*1280*2*1);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();


	//write 16x16 pattern to sdram
	Pattern_Format_16X16();
	Canvas_Image_Start_address(layer_start_addr(5));//any layer
    Canvas_image_width(16);
    Active_Window_XY(0,0);
    Active_Window_WH(16,16);
    Goto_Pixel_XY(0,0);
    //Show_picture(16*16,pattern16x16_16bpp);

	Canvas_Image_Start_address(layer_start_addr(0));//
    Canvas_image_width(400);
    Active_Window_XY(0,0);
    Active_Window_WH(400,1280);


	BTE_S0_Color_16bpp();
    BTE_S0_Memory_Start_Address(layer_start_addr(5));
    BTE_S0_Image_Width(16);

    BTE_S1_Color_16bpp();
    BTE_S1_Memory_Start_Address(0);
    BTE_S1_Image_Width(400);

    BTE_Destination_Color_16bpp();
    BTE_Destination_Memory_Start_Address(layer_start_addr(0));
    BTE_Destination_Image_Width(400);

    BTE_ROP_Code(0xc);
    BTE_Operation_Code(0x06);//pattern fill

	BTE_S1_Window_Start_XY(0,0);
    BTE_Destination_Window_Start_XY(0,40);
    BTE_Window_Size(300,300);

    BTE_Enable();
    Check_BTE_Busy();

	Foreground_color_65k(Black);
	Background_color_65k(White);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(0,350);
	Show_String("Pattern Fill");

	Background_color_65k(color65k_red);
	BTE_S1_Window_Start_XY(0,0);
    BTE_Destination_Window_Start_XY(0,440);
    BTE_Window_Size(300,300);
	BTE_Operation_Code(0x07);//pattern fill with chroma key

	BTE_Enable();
    Check_BTE_Busy();

	Foreground_color_65k(Black);
	Background_color_65k(White);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(40,750);
	Show_String("Pattern Fill With");

	Foreground_color_65k(Black);
	Background_color_65k(White);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(40,774);
	Show_String("Chroma Key");
    HAL_Delay(1000);
  	
}


void PIP_Demo(void)
{
	unsigned long i;
 	//unsigned long im=1;

	Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

	Canvas_Image_Start_address(0);//Layer 1
	Canvas_image_width(400);//
    Active_Window_XY(0,0);
    Active_Window_WH(400,1280);  // Y, X

 	Foreground_color_65k(White);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	//Line_End_XY(1247, 399);
	Start_Square_Fill();

	Foreground_color_65k(Blue2);
	//Line_Start_XY(0,1248);
	//Line_End_XY(399,1279);
	Line_Start_XY(370, 0);
	Line_End_XY(399, 1279);
	Start_Square_Fill();

	Foreground_color_65k(White);
	Background_color_65k(Blue2);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(372,610);
	Show_String(" Demo PIP");



	 Memory_16bpp_Mode();
	 HAL_Delay(20);
	 Canvas_Image_Start_address(layer_start_addr(1));//Layer 2
	 Canvas_image_width(400);//  Y.....
     Active_Window_XY(0,0);
	 Active_Window_WH(400,1280);
	 Foreground_color_65k(Red);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	//Line_End_XY(1247, 399);
	Start_Square_Fill();

	Foreground_color_65k(White);
	Background_color_65k(color65k_blue);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(120,120);
	Show_String("PIP1");

	Canvas_Image_Start_address(layer_start_addr(2));//Layer 3
	Canvas_image_width(400);//
    Active_Window_XY(0,0);
	Active_Window_WH(400,1280);
	Foreground_color_65k(Green);
		Line_Start_XY(0,0);
		Line_End_XY(399,1279);
		//Line_End_XY(1247, 399);
		Start_Square_Fill();

	Foreground_color_65k(White);
	Background_color_65k(color65k_red);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(120,120);
	Show_String("PIP2");
	VSCAN_T_to_B();
 	//PIP1
	Select_PIP1_Parameter();  // check1
	HAL_Delay(50);
 	Select_PIP1_Window_16bpp();// check2
 	HAL_Delay(50);
 	PIP_Display_Start_XY(100,100); // check3
 	HAL_Delay(50);
 	PIP_Image_Start_Address(layer_start_addr(1)); // check4
 	HAL_Delay(50);
 	PIP_Image_Width(100); // check5
 	HAL_Delay(50);
 	PIP_Window_Image_Start_XY(100,100); // check6
 	HAL_Delay(50);
 	PIP_Window_Width_Height(100,100); // check7
 	HAL_Delay(50);
	Enable_PIP1();
	HAL_Delay(50);

	//PIP2
	Select_PIP2_Parameter();
    Select_PIP2_Window_16bpp();
	HAL_Delay(50);

	PIP_Image_Start_Address(layer_start_addr(2));
	PIP_Image_Width(400);
	PIP_Window_Width_Height(200,200);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(0,0);
	Enable_PIP2();
	HAL_Delay(50);

     for(i=0;i<160;i++)
	 {
	  Select_PIP1_Parameter();
	  //PIP_Display_Start_XY(0+i,40+i);
	   PIP_Display_Start_XY(80,40+i);
	  HAL_Delay(1);
	  Select_PIP2_Parameter();
	  PIP_Display_Start_XY(0+200-i,40+200-i);
	  //PIP_Display_Start_XY(80+320,40+240-i);
	  HAL_Delay(10);
	 }

	 for(i=0;i<160;i++)
	 {
	  Select_PIP1_Parameter();
	  PIP_Display_Start_XY(0+159-i,40+159-i);
	  //PIP_Display_Start_XY(80+159,40+159-i);
	  HAL_Delay(1);
	  Select_PIP2_Parameter();
	  PIP_Display_Start_XY(0+200-159+i,40+200-159+i);
	   //PIP_Display_Start_XY(80+320-159,40+240-159+i);
	  HAL_Delay(10);
	  }

	  HAL_Delay(2000);

	  /*
	//DMA initial setting
	Enable_SFlash_SPI();
    Select_SFI_1();
    Select_SFI_DMA_Mode();
    Select_SFI_24bit_Address();

    //Select_SFI_Waveform_Mode_0();
    Select_SFI_Waveform_Mode_3();

    //Select_SFI_0_DummyRead();	//normal read mode
    Select_SFI_8_DummyRead(); //1byte dummy cycle
    //Select_SFI_16_DummyRead();
    //Select_SFI_24_DummyRead();

    Select_SFI_Single_Mode();
    //Select_SFI_Dual_Mode0();
    //Select_SFI_Dual_Mode1();

    SPI_Clock_Period(0);

	Canvas_Image_Start_address(layer_start_addr(1));//Layer 2
	Canvas_image_width(400);//

	SFI_DMA_Destination_Upper_Left_Corner(0,0);
    SFI_DMA_Transfer_Width_Height(200,200);
    SFI_DMA_Source_Width(400);//
	SFI_DMA_Source_Start_Address(0);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();

    Canvas_Image_Start_address(layer_start_addr(2));//Layer 3
	Canvas_image_width(400);//

	SFI_DMA_Destination_Upper_Left_Corner(0,0);
    SFI_DMA_Transfer_Width_Height(200,200);
    SFI_DMA_Source_Width(400);//
	SFI_DMA_Source_Start_Address(im*400*1280*2*2);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();


	 for(i=0;i<160;i++)
	 {
	  Select_PIP1_Parameter();
	  PIP_Display_Start_XY(0+i,40+i);
	  //PIP_Display_Start_XY(80+i,40);
	  HAL_Delay(1);
	  Select_PIP2_Parameter();
	  PIP_Display_Start_XY(0+200-i,40+200-i);
	  //PIP_Display_Start_XY(80+320-i,40+240);
	  HAL_Delay(10);
	 }

	 for(i=0;i<160;i++)
	 {
	  Select_PIP1_Parameter();
	  PIP_Display_Start_XY(0+159-i,40+159-i);
	  //PIP_Display_Start_XY(80+159-i,40+159);
	  HAL_Delay(1);
	  Select_PIP2_Parameter();
	  PIP_Display_Start_XY(0+200-159+i,40+200-159+i);
	  //PIP_Display_Start_XY(80+320-159+i,40+240-159);
	  HAL_Delay(10);
	  }

	  HAL_Delay(1000);
		

	  Disable_PIP1();
	  Disable_PIP2();
	  */
	  VSCAN_B_to_T();
}


void App_Demo_Waveform(void)
{
    unsigned int i,h;

	unsigned int point1y,point2y;
	unsigned int point21y,point22y;
	unsigned int point31y,point32y;
	point2y = 0; //initial value
	point22y = 0; //initial value
	point32y = 0; //initial value

  #define grid_width 601
  #define grid_high  401
  #define grid_gap 50

	Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

 	Canvas_Image_Start_address(0);//Layer 1
	Canvas_image_width(400);//
    Active_Window_XY(0,0);
	Active_Window_WH(400,1280);

 	Foreground_color_65k(Blue);
	Line_Start_XY(0,0);
	Line_End_XY(399,1247);
	Start_Square_Fill();

	Foreground_color_65k(Blue2);
	Line_Start_XY(0,1248);
	Line_End_XY(399,1279);
	Start_Square_Fill();

 	Foreground_color_65k(White);
	Background_color_65k(Blue2);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(0,Line52);
	Show_String("Application Demo Waveform");


 	Foreground_color_65k(White);
	Background_color_65k(Blue);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(0,Line50);
	Show_String("BTE memory copy + Geometric draw demo waveform");

	//creat grid 500*400 to layer2 used geometric draw

	Canvas_Image_Start_address(layer_start_addr(1));//Layer 2

;

    Foreground_color_65k(Black);  //clear layer2 to color black
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();

    for(i=0;i<=grid_width;i+=grid_gap)
	{
	 Foreground_color_65k(color65k_grayscale12);
	 Line_Start_XY(i,0);
     Line_End_XY(i,grid_high-1);
	 Start_Line();
	}

	 for(i=0;i<=grid_high;i+=grid_gap)
	{
	 Foreground_color_65k(color65k_grayscale12);
	 Line_Start_XY(0,i);
     Line_End_XY(grid_width-1,i);
	 Start_Line();
	}

	//BTE memory(move) grid to layer1
	   BTE_S0_Color_16bpp();
       BTE_S0_Memory_Start_Address(layer_start_addr(1));
       BTE_S0_Image_Width(400);
       BTE_S0_Window_Start_XY(0,0);

//       BTE_S1_Color_16bpp();
//       BTE_S1_Memory_Start_Address(800*600*2*2);
//       BTE_S1_Image_Width(800);
//       BTE_S1_Window_Start_XY(0,0);

       BTE_Destination_Color_16bpp();
       BTE_Destination_Memory_Start_Address(0);
       BTE_Destination_Image_Width(400);
       BTE_Destination_Window_Start_XY(10,40);
       BTE_Window_Size(601,401);

	   //move with ROP 0
       BTE_ROP_Code(12); //memory copy s0(layer2)grid to layer1
       BTE_Operation_Code(2); //BTE move
       BTE_Enable();
       Check_BTE_Busy();


	   Canvas_Image_Start_address(0);//Layer 1

	 h=0;
	do{
	    for(i=0;i<1280;i+=2)
	   {
		// copy layer2 grid column to layer1
		 BTE_S0_Window_Start_XY(i,0);

		 BTE_Destination_Window_Start_XY(10+i,40);
         BTE_Window_Size(2,401);
		 BTE_Enable();
         Check_BTE_Busy();


		 point1y = point2y;
         point2y = rand()%90;//

		 point21y = point22y;
         point22y = rand()%99;//
//
		 point31y = point32y;
         point32y = rand()%67;//

		 Foreground_color_65k(color65k_yellow);//
		 Line_Start_XY(i+10,point1y+80);
		 Line_End_XY(i+1+10,point2y+80);
	     Start_Line();

		 Foreground_color_65k(color65k_purple);//
		 Line_Start_XY(i+10,point21y+200);
		 Line_End_XY(i+1+10,point22y+200);
	     Start_Line();
//
		 Foreground_color_65k(color65k_green);//
		 Line_Start_XY(i+10,point31y+300);
		 Line_End_XY(i+1+10,point32y+300);
	     Start_Line();

		 //HAL_Delay(1000);
	    }

	   h++;
	  }
	 while(h<4);

 		

}


void App_Demo_Scrolling_Text(void)
{
    unsigned int i;



 	Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

	Canvas_Image_Start_address(layer_start_addr(1));
	Canvas_image_width(400);//
    Active_Window_XY(0,0);
	Active_Window_WH(400,1280);

 	Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1247);
	Start_Square_Fill();

	Foreground_color_65k(Blue2);
	Line_Start_XY(0,1248);
	Line_End_XY(399,1279);
	Start_Square_Fill();



	Foreground_color_65k(White);
	Background_color_65k(Blue2);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(0,Line52);
	Show_String("Demo Scrolling Text");
	Foreground_color_65k(White);
	Background_color_65k(Black);
/*	Font_Select_12x24_24x24();
	Goto_Text_XY(0,Line51);
	Show_String("Used Move BTE with Chroma Key ");
   */

    //DMA initial setting
	Enable_SFlash_SPI();
    Select_SFI_1();
    Select_SFI_DMA_Mode();
    Select_SFI_24bit_Address();

    //Select_SFI_Waveform_Mode_0();
    Select_SFI_Waveform_Mode_3();

    //Select_SFI_0_DummyRead();	//normal read mode
    Select_SFI_8_DummyRead(); //1byte dummy cycle
    //Select_SFI_16_DummyRead();
    //Select_SFI_24_DummyRead();

    Select_SFI_Single_Mode();
    //Select_SFI_Dual_Mode0();
    //Select_SFI_Dual_Mode1();

    SPI_Clock_Period(0);


	SFI_DMA_Destination_Upper_Left_Corner(0,0);
    SFI_DMA_Transfer_Width_Height(400,1280);
    SFI_DMA_Source_Width(400);//
	SFI_DMA_Source_Start_Address(0);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();


	//BTE memory(move) layer2 to layer1
	   BTE_S0_Color_16bpp();
       BTE_S0_Memory_Start_Address(layer_start_addr(1));
       BTE_S0_Image_Width(400);
       BTE_S0_Window_Start_XY(0,0);



       BTE_Destination_Color_16bpp();
       BTE_Destination_Memory_Start_Address(layer_start_addr(0));
       BTE_Destination_Image_Width(400);
       BTE_Destination_Window_Start_XY(0,0);
       BTE_Window_Size(400,1280);

       BTE_ROP_Code(12);
       BTE_Operation_Code(2); //BTE move
       BTE_Enable();		  //memory copy s0(layer3) to layer1
       Check_BTE_Busy();


	  //write text to layer3
	  Canvas_Image_Start_address(layer_start_addr(2));
	  Canvas_image_width(400);//
      Active_Window_XY(0,0);
	  Active_Window_WH(400,1280);

	  Foreground_color_65k(Red);
	  Line_Start_XY(0,0);
	  Line_End_XY(399,1279);
	  Start_Square_Fill();

	Foreground_color_65k(Green);
	Background_color_65k(Red);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(0,Line0);
	Show_String("Demo Scrolling Text");

	Foreground_color_65k(Yellow);
	Background_color_65k(Red);
	Goto_Text_XY(0,Line1);
	Show_String("Demo Scrolling Text");
	Foreground_color_65k(Magenta);
	Background_color_65k(Red);
	Goto_Text_XY(0,Line2);
	Show_String("Demo Scrolling Text");
	Foreground_color_65k(Grey);
	Background_color_65k(Red);
	Goto_Text_XY(0,Line3);
	Show_String("Demo Scrolling Text");


	  //Move BTE with chroma key layer3 to layer2 then move layer2 to layer1 to display

	  for(i=0;i<200;i+=8)
	  {
	   	Canvas_Image_Start_address(layer_start_addr(1));
	    Canvas_image_width(400);//
        Active_Window_XY(0,0);
	    Active_Window_WH(400,1280);

 	     Foreground_color_65k(Black);
	     Line_Start_XY(0,0);
	     Line_End_XY(399,1279);
	     Start_Square_Fill();

		 SFI_DMA_Destination_Upper_Left_Corner(0,0);
         SFI_DMA_Transfer_Width_Height(400,1280);
         SFI_DMA_Source_Width(400);//
	     SFI_DMA_Source_Start_Address(0);//
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA();

	  //BTE memory(move) layer3 to layer2
	   //BTE_S0_Color_16bpp();
       BTE_S0_Memory_Start_Address(layer_start_addr(2));
       BTE_S0_Image_Width(400);
       BTE_S0_Window_Start_XY(0,0);



       //BTE_Destination_Color_16bpp();
       BTE_Destination_Memory_Start_Address(layer_start_addr(1));
       BTE_Destination_Image_Width(400);
       BTE_Destination_Window_Start_XY(391-i,72);
       BTE_Window_Size(0+i,24*4);

	   Background_color_65k(Red);//

       BTE_ROP_Code(12);
       BTE_Operation_Code(5); //BTE move with chroma key
       BTE_Enable();		  //memory copy s0(layer3) to layer1
       Check_BTE_Busy();




   	  //BTE memory(move) layer2 to layer1
	   //BTE_S0_Color_16bpp();
       BTE_S0_Memory_Start_Address(layer_start_addr(1));
       BTE_S0_Image_Width(400);
       BTE_S0_Window_Start_XY(0,0);

       //BTE_Destination_Color_16bpp();
       BTE_Destination_Memory_Start_Address(layer_start_addr(0));
       BTE_Destination_Image_Width(400);
       BTE_Destination_Window_Start_XY(0,0);
       BTE_Window_Size(400,1280);

       BTE_ROP_Code(12);
       BTE_Operation_Code(2); //BTE move
       BTE_Enable();		  //memory copy s0(layer2) to layer1
       Check_BTE_Busy();
	  }

	  for(i=0;i<100;i+=8)
	  {
	   	Canvas_Image_Start_address(layer_start_addr(1));
	    Canvas_image_width(400);//
        Active_Window_XY(0,0);
	    Active_Window_WH(400,1280);

 	     Foreground_color_65k(Black);
	     Line_Start_XY(0,0);
	     Line_End_XY(399,1200);
	     Start_Square_Fill();

		 SFI_DMA_Destination_Upper_Left_Corner(0,0);
         SFI_DMA_Transfer_Width_Height(400,1280);
         SFI_DMA_Source_Width(400);//
	     SFI_DMA_Source_Start_Address(640000);//	??
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA();

	  //BTE memory(move) layer3 to layer2
	   //BTE_S0_Color_16bpp();
       BTE_S0_Memory_Start_Address(layer_start_addr(2));
       BTE_S0_Image_Width(400);
       BTE_S0_Window_Start_XY(i,0);



       //BTE_Destination_Color_16bpp();
       BTE_Destination_Memory_Start_Address(layer_start_addr(1));
       BTE_Destination_Image_Width(400);
       BTE_Destination_Window_Start_XY(0,72);
       BTE_Window_Size(399-i,24*4);

	   Background_color_65k(Red);//

       BTE_ROP_Code(12);
       BTE_Operation_Code(5); //BTE move with chroma key
       BTE_Enable();		  //memory copy s0(layer3) to layer1
       Check_BTE_Busy();

//	   Main_Image_Width(800);
//       Main_Window_Start_XY(0,0);
//	   Main_Image_Start_Address(layer_start_addr(1));//switch display windows to
//	   HAL_Delay(20);


   	  //BTE memory(move) layer2 to layer1
	   //BTE_S0_Color_16bpp();
       BTE_S0_Memory_Start_Address(layer_start_addr(1));
       BTE_S0_Image_Width(400);
       BTE_S0_Window_Start_XY(0,0);

       //BTE_Destination_Color_16bpp();
       BTE_Destination_Memory_Start_Address(layer_start_addr(0));
       BTE_Destination_Image_Width(400);
       BTE_Destination_Window_Start_XY(0,0);
       BTE_Window_Size(400,1280);

       BTE_ROP_Code(12);
       BTE_Operation_Code(2); //BTE move
       BTE_Enable();		  //memory copy s0(layer2) to layer1
       Check_BTE_Busy();
	  }

	 		

}



void App_Demo_multi_frame_buffer(void)
{    unsigned int i,j;
      unsigned long im=1;
    Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

      //DMA initial setting
	Enable_SFlash_SPI();
    Select_SFI_1();
    Select_SFI_DMA_Mode();
    Select_SFI_24bit_Address();

    //Select_SFI_Waveform_Mode_0();
    Select_SFI_Waveform_Mode_3();

    //Select_SFI_0_DummyRead();	//normal read mode
    Select_SFI_8_DummyRead(); //1byte dummy cycle
    //Select_SFI_16_DummyRead();
    //Select_SFI_24_DummyRead();

    Select_SFI_Single_Mode();
    //Select_SFI_Dual_Mode0();
    //Select_SFI_Dual_Mode1();

    SPI_Clock_Period(0);

	SFI_DMA_Destination_Upper_Left_Corner(0,0);
    SFI_DMA_Transfer_Width_Height(400,1280);
    SFI_DMA_Source_Width(400);//


 for(i=0;i<6;i++)
  {

	Canvas_Image_Start_address(im*400*1280*2*i);//Layer1~6
	Canvas_image_width(400);//
    Active_Window_XY(0,0);
	Active_Window_WH(400,1280);

 /*	Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1247);
	Start_Square_Fill();

	Foreground_color_65k(Blue2);
	Line_Start_XY(0,1248);
	Line_End_XY(399,1279);
	Start_Square_Fill();


	Foreground_color_65k(White);
	Background_color_65k(Blue2);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(0,Line52);
	Show_String("Demo Multi Frame Buffer");

	//LCD_DisplayString(0,Line23 ,"  Demo Mulit Frame Buffer",Black,White);

   */


	  SFI_DMA_Source_Start_Address(im*400*1280*2*i);//
	  Start_SFI_DMA();
      Check_Busy_SFI_DMA();
	  //HAL_Delay(500);
	  HAL_Delay(100);
	  Main_Image_Width(400);
      Main_Window_Start_XY(0,0);
	  Main_Image_Start_Address(im*400*1280*2*i);//switch display windows to
	 }
	//HAL_Delay(2000);
//	Color_Bar_ON();

	//set canvas to 8188
    Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(3200);
	Main_Window_Start_XY(0,0);
	Canvas_Image_Start_address(0);
	Canvas_image_width(3200);//
	Active_Window_XY(0,0);
	Active_Window_WH(3200,1280);

	for(im=0;im<6;im++)
    {
	Canvas_Image_Start_address(0);//Layer1~6

 	Foreground_color_65k(Black);
	Line_Start_XY(0+im*400,0);
	Line_End_XY(399+im*400,1255);
	Start_Square_Fill();

	Foreground_color_65k(Blue2);
	Line_Start_XY(0+im*400,1256);
	Line_End_XY(399+im*400,1279);
	Start_Square_Fill();


    SFI_DMA_Destination_Upper_Left_Corner(0+im*400,0);
	SFI_DMA_Source_Start_Address(im*1280*400*2);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();
	 	 HAL_Delay(5);

/*	Foreground_color_65k(White);
	Background_color_65k(Blue2);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(i*1024,Line52);
	Show_String("Demo Mulit Frame Buffer");
	 */
	}
//	Color_Bar_OFF();

   for(j=0;j<6;j++)
   {
	for(i=0;i<400;i++)
	{
	 Main_Window_Start_XY(i+j*400,0);
	 HAL_Delay(5);
	}
    HAL_Delay(30);
   }

   for(j=5;j>0;j--)
   {
	for(i=0;i<400;i++)
	{
	 Main_Window_Start_XY(j*400-i,0);
	 HAL_Delay(5);
	}

   }

   HAL_Delay(500);

    		
}



void App_Demo_slide_frame_buffer(void)
{		unsigned long im=1;
     unsigned int i,j;

    Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

      //DMA initial setting
	Enable_SFlash_SPI();
    Select_SFI_1();
    Select_SFI_DMA_Mode();
    Select_SFI_24bit_Address();

    //Select_SFI_Waveform_Mode_0();
    Select_SFI_Waveform_Mode_3();

    //Select_SFI_0_DummyRead();	//normal read mode
    Select_SFI_8_DummyRead(); //1byte dummy cycle
    //Select_SFI_16_DummyRead();
    //Select_SFI_24_DummyRead();

    Select_SFI_Single_Mode();
    //Select_SFI_Dual_Mode0();
    //Select_SFI_Dual_Mode1();

    SPI_Clock_Period(0);

	SFI_DMA_Destination_Upper_Left_Corner(0,0);
    SFI_DMA_Transfer_Width_Height(400,1280);
    SFI_DMA_Source_Width(400);//


	//Color_Bar_ON();

	//set canvas to 1600
    Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(800);
	Main_Window_Start_XY(0,0);
	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,2560);

	Canvas_Image_Start_address(0);//

 	Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(799,2559);
	Start_Square_Fill();

    SFI_DMA_Destination_Upper_Left_Corner(0,0);
	SFI_DMA_Source_Start_Address(0);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();

    SFI_DMA_Destination_Upper_Left_Corner(400,0);
	SFI_DMA_Source_Start_Address(im*400*1280*2);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();

	SFI_DMA_Destination_Upper_Left_Corner(0,1280);
	SFI_DMA_Source_Start_Address(im*400*1280*4);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();

	SFI_DMA_Destination_Upper_Left_Corner(400,1280);
	SFI_DMA_Source_Start_Address(im*400*1280*6);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();



	//Color_Bar_OFF();


	for(i=0;i<800-400+1;i++)
	{
	 Main_Window_Start_XY(i,0);
	 HAL_Delay(5);
	}
    HAL_Delay(1);

	for(j=0;j<2560-1280+1;j++)
	{
	 Main_Window_Start_XY(400,j);
	 HAL_Delay(5);
	}
	HAL_Delay(1);


	for(i=800-400;i>0;i--)
	{
	 Main_Window_Start_XY(i,1280);
	 HAL_Delay(5);
	}
	Main_Window_Start_XY(0,1280);
	HAL_Delay(1);

	for(j=2560-1280;j>0;j--)
	{
	 Main_Window_Start_XY(0,j);
	 HAL_Delay(5);
	}
	Main_Window_Start_XY(0,0);
	HAL_Delay(5);


 		
}


void App_Demo_Alpha_Blending(void)
{		 unsigned long im=1;
	    unsigned int i,j;

    Select_Main_Window_16bpp();
	Main_Image_Start_Address(layer_start_addr(0));
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

	Canvas_Image_Start_address(layer_start_addr(0));//
	Main_Image_Width(400);
    Active_Window_XY(0,0);
	Active_Window_WH(400,1280);


 	Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1247);
	Start_Square_Fill();

	Foreground_color_65k(Blue2);
	Line_Start_XY(0,1248);
	Line_End_XY(399,1279);
	Start_Square_Fill();



	Foreground_color_65k(White);
	Background_color_65k(Blue2);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(0,Line52);
	Show_String(" Demo Alpha Blending");
	Foreground_color_65k(White);
	Background_color_65k(Black);
	Font_Select_12x24_24x24();
	Goto_Text_XY(0,Line51);
	Show_String("Fade in and fade out");

	//DMA initial setting
	Enable_SFlash_SPI();
    Select_SFI_1();
    Select_SFI_DMA_Mode();
    Select_SFI_24bit_Address();

    //Select_SFI_Waveform_Mode_0();
    Select_SFI_Waveform_Mode_3();

    //Select_SFI_0_DummyRead();	//normal read mode
    Select_SFI_8_DummyRead(); //1byte dummy cycle
    //Select_SFI_16_DummyRead();
    //Select_SFI_24_DummyRead();

    Select_SFI_Single_Mode();
    //Select_SFI_Dual_Mode0();
    //Select_SFI_Dual_Mode1();

	SPI_Clock_Period(0);


	//Clear layer2 to color black
	Canvas_Image_Start_address(layer_start_addr(1));//
    Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();

    //DMA picture to layer2
	SFI_DMA_Destination_Upper_Left_Corner(0,0);
    SFI_DMA_Transfer_Width_Height(400,1280);
    SFI_DMA_Source_Width(400);//
	SFI_DMA_Source_Start_Address(0);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();


	//Clear layer3 to color black
	Canvas_Image_Start_address(layer_start_addr(2));//
    Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();

    //DMA picture to layer3
	SFI_DMA_Destination_Upper_Left_Corner(0,0);
    SFI_DMA_Transfer_Width_Height(400,1280);
    SFI_DMA_Source_Width(400);//
	SFI_DMA_Source_Start_Address(im*400*1280*2);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();

	//BTE move alpha blending by picture
	 BTE_Destination_Color_16bpp();
     BTE_Destination_Memory_Start_Address(layer_start_addr(0));
     BTE_Destination_Image_Width(400);
     BTE_Destination_Window_Start_XY(0,0);
     BTE_Window_Size(400,1280);

     BTE_S0_Color_16bpp();
     BTE_S0_Memory_Start_Address(layer_start_addr(1));
     BTE_S0_Image_Width(400);
     BTE_S0_Window_Start_XY(0,0);

     BTE_S1_Color_16bpp();
     BTE_S1_Memory_Start_Address(layer_start_addr(2));
     BTE_S1_Image_Width(400);
     BTE_S1_Window_Start_XY(0,0);


	BTE_ROP_Code(15);
    BTE_Operation_Code(10); //BTE move

  for(j=0;j<4;j+=2)
  {
	//DMA picture to layer2
	Canvas_Image_Start_address(layer_start_addr(1));//
    SFI_DMA_Destination_Upper_Left_Corner(0,0);
    SFI_DMA_Transfer_Width_Height(400,1280);
    SFI_DMA_Source_Width(400);//
	SFI_DMA_Source_Start_Address(im*j*400*1280*2);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();
	//DMA picture to layer3
	Canvas_Image_Start_address(layer_start_addr(2));//
	SFI_DMA_Source_Start_Address(im*(j+1)*400*1280*2);//
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();

    for(i=0;i<32;i++)
    {
	 BTE_Alpha_Blending_Effect(i);
	 BTE_Enable();
	 HAL_Delay(200);
    }

    for(i=32;i>0;i--)
    {
	 BTE_Alpha_Blending_Effect(i);
	 BTE_Enable();
	 HAL_Delay(200);
    }
   }

 	

}

void mono_Demo(void)
{


	Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);
	Canvas_Image_Start_address(0);
	Canvas_image_width(400);//
	Active_Window_XY(0,0);
	Active_Window_WH(400,1280);


	Foreground_color_65k(Red);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();
	
	Foreground_color_65k(Green);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();
	
 	Foreground_color_65k(Blue);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();
	
	Foreground_color_65k(Cyan);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();
	
	Foreground_color_65k(Yellow);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();
	
  	Foreground_color_65k(Magenta);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();
	
 	Foreground_color_65k(White);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();
	
  	Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();
	
}


void Text_Demo(void)
{
//    unsigned int i,j;
 	Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

	Canvas_Image_Start_address(0);//Layer 1
	Canvas_image_width(400);//
    Active_Window_XY(0,0);
	Active_Window_WH(400,1280);

 	Foreground_color_65k(White);
	Line_Start_XY(0,0);
	Line_End_XY(399,1247);
	Start_Square_Fill();

	  //Foreground_color_65k(color65k_yellow);
      //Background_color_65k(color65k_purple);

	   //Font_Select_UserDefine_Mode();
       //CGROM_Select_Internal_CGROM();
        CGROM_Select_Genitop_FontROM();

	 //GTFont_Select_GT21L16TW_GT21H16T1W();
     //GTFont_Select_GT23L16U2W(); //  <-- We do NOT have this chip!!  Show poop.
     //GTFont_Select_GT23L24T3Y_GT23H24T3Y();
        //GTFont_Select_GT23L24M1Z();
        //GTFont_Select_GT23L32S4W_GT23H32S4W();
        //GTFont_Select_GT20L24F6Y();
        //GTFont_Select_GT21L24S1W();
        //GTFont_Select_GT22L16A1Y();


     GTFont_Select_GT23L32S4W_GT23H32S4W();  //  <--- We have this chip!!

	  Font_Width_X4();
      Font_Height_X4();
	  CGROM_Select_Genitop_FontROM();
      Font_Select_16x32_32x32();
	  Select_SFI_0();
      Select_SFI_Font_Mode();
      Select_SFI_24bit_Address();
      Select_SFI_Waveform_Mode_0();
      Select_SFI_0_DummyRead();
      Select_SFI_Single_Mode();
      SPI_Clock_Period(4);	 // Freq must setting <=20MHZ

	  Enable_SFlash_SPI();

	  Foreground_color_65k(color65k_black);
	  Background_color_65k(color65k_white);
	  Font_Background_select_Transparency();
	  //Set_GTFont_Decoder(0x11);  //BIG5
	  // 0b00100011		0x23	ASCI BOLD
	  // 0b00100011		0x22	ASCI Roman
	  // 0b00100011		0x21	ASCI Arial
	  //Set_GTFont_Decoder(0x20);  //BIG5
   	  Set_GTFont_Decoder(0x21);
	  Goto_Text_XY(0,0);
   	  Show_String("$1,234.56");
   	  Set_GTFont_Decoder(0x21);
	  Goto_Text_XY(133,0);
	  Show_String("ABCDEFGH");
	  //Set_GTFont_Decoder(0x22);
	  Goto_Text_XY(266,0);
	  Show_String("IJKLMNOP");
	  //Set_GTFont_Decoder(0x23);
	  Goto_Text_XY(0,640);
	  Show_String("QRSTUVWX");
	  //Set_GTFont_Decoder(0x00);
	  Goto_Text_XY(133,640);
	  Show_String("YZ....");
//   	  Foreground_color_65k(color65k_blue);
//		Goto_Text_XY(190,190);
//		Show_String("Dane is excellent");// let's go
//	  Set_GTFont_Decoder(0x11);  //BIG5
	  while(1)
	  {


	  }
   /*
	  for(i=0;i<14;i++)
	  {
	   HAL_Delay(100);
	   Text_Cursor_H_V(1+i,15-i);
	  }
	  	 HAL_Delay(2000);

	  Disable_Text_Cursor();

	  CGROM_Select_Internal_CGROM();
	  Font_Select_8x16_16x16();

	  Foreground_color_65k(color65k_blue);
	  Goto_Text_XY(0,484);
   	  Show_String("Demo graphic cursor:");

	  Set_Graphic_Cursor_Color_1(0xff);
      Set_Graphic_Cursor_Color_2(0x00);

	  Graphic_cursor_initial();
	  Graphic_Cursor_XY(0,508);
	  Select_Graphic_Cursor_1();
	  	 HAL_Delay(2000);
	  Select_Graphic_Cursor_2();
	  HAL_Delay(2000);
	  Select_Graphic_Cursor_3();
	  HAL_Delay(2000);
	  Select_Graphic_Cursor_4();
	  HAL_Delay(2000);
	  Select_Graphic_Cursor_2();

	  for(j=0;j<2;j++)
	  {
	   for(i=0;i<400;i++)
	   {
	    Graphic_Cursor_XY(i,508+j*20);
		HAL_Delay(2);
	   }
	  }
	   Graphic_Cursor_XY(0,508);

	 HAL_Delay(200);
	 Disable_Graphic_Cursor();
 */



 }


 void DMA_Demo(void)
{
		unsigned long i;

 ///////////////////////////////////////////////////////////////

 	Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

	Canvas_Image_Start_address(0);//Layer 1
	Canvas_image_width(400);//
    Active_Window_XY(0,0);
	Active_Window_WH(400,1280);



 	Foreground_color_65k(White);
	Line_Start_XY(0,0);
	Line_End_XY(399,1247);
	Start_Square_Fill();

	Foreground_color_65k(Blue2);
	Line_Start_XY(0,1248);
	Line_End_XY(399,1279);
	Start_Square_Fill();

	//Foreground_color_65k(White);
	//Background_color_65k(Blue2);
	//Goto_Text_XY(300,576);
	//Show_String("Demo DMA");

 /*	Foreground_color_65k(White);
	Background_color_65k(Blue2);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
	Goto_Text_XY(0,Line52);
	Show_String(" Demo DMA");

	LCD_DisplayString(0,Line51 ,"Demo direct access serial FLASH to show picture",Black,White);

   */

    //DMA initial setting
	Enable_SFlash_SPI();
    Select_SFI_1();
    Select_SFI_DMA_Mode();
    Select_SFI_24bit_Address();

    //Select_SFI_Waveform_Mode_0();
    Select_SFI_Waveform_Mode_3();

    //Select_SFI_0_DummyRead();	//normal read mode
    Select_SFI_8_DummyRead(); //1byte dummy cycle
    //Select_SFI_16_DummyRead();
   //Select_SFI_24_DummyRead();

   // Select_SFI_Single_Mode();
    Select_SFI_Dual_Mode0();
   // Select_SFI_Dual_Mode1();

    SPI_Clock_Period(0);



//DMA initail normally command setting
//	LCD_CmdWrite(0x01);
//	LCD_DataWrite(0x83);
//	LCD_CmdWrite(0xB7);
//	LCD_DataWrite(0xd4);
//	LCD_CmdWrite(0xBB);
//  LCD_DataWrite(0x00);

   /*
   //if used 32bit address Flash ex.256Mbit,512Mbit
   //must be executed following in 24bit address mode to switch to 32bit address mode

   //Select_nSS_drive_on_xnsfcs0();
   Select_nSS_drive_on_xnsfcs1();

   Reset_CPOL();
   //Set_CPOL();
   Reset_CPHA();
   //Set_CPHA();

  //Enter 4-byte mode
   nSS_Active();
   SPI_Master_FIFO_Data_Put(0xB7);  //switch to 32bit address mode
   delay_us(10);
   nSS_Inactive();

   Select_SFI_32bit_Address();
   */


	SFI_DMA_Destination_Upper_Left_Corner(0,0);
    SFI_DMA_Transfer_Width_Height(400,1280);
    SFI_DMA_Source_Width(400);//

	//execute DMA to show 400x1280 picture
	 for(i=0;i<7;i++)
	 {
	  SFI_DMA_Source_Start_Address(i*400*1280*2);//
	  Start_SFI_DMA();
      Check_Busy_SFI_DMA();
	  HAL_Delay(30);
 	
	 }
}

 void Geometric(void)
{unsigned int i;
 	Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);

	Canvas_Image_Start_address(0);//Layer 1
	Canvas_image_width(400);//
    Active_Window_XY(0,0);
	Active_Window_WH(400,1280);



///////////////////////////Square

   	Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();

   for(i=0;i<=200;i+=8)
	{Foreground_color_65k(Red);
	Line_Start_XY(0+i,0+i);
	Line_End_XY(399-i,1279-i);
	Start_Square();
	HAL_Delay(50);
	}

    for(i=0;i<=200;i+=8)
	{Foreground_color_65k(Black);
	Line_Start_XY(0+i,0+i);
	Line_End_XY(399-i,1279-i);
	Start_Square();
	HAL_Delay(50);
	}
 	HAL_Delay(2000);
///////////////////////////Square Of Circle
   	Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();

   for(i=0;i<=100;i+=8)
	{Foreground_color_65k(Green);
	Line_Start_XY(0+i,0+i);
	Line_End_XY(399-i,1279-i);
	Circle_Square_Radius_RxRy(10,10);
	Start_Circle_Square();
	HAL_Delay(50);
	}

    for(i=0;i<=100;i+=8)
	{Foreground_color_65k(Black);
	Line_Start_XY(0+i,0+i);
	Line_End_XY(399-i,1279-i);
	Circle_Square_Radius_RxRy(10,10);
	Start_Circle_Square();
	HAL_Delay(50);
	}
   	HAL_Delay(2000);

///////////////////////////Circle
  	Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();

   for(i=0;i<=200;i+=8)
	{Foreground_color_65k(Blue);
	Circle_Center_XY(400/2,1280/2);
	Circle_Radius_R(i);
	Start_Circle_or_Ellipse();
	HAL_Delay(50);
	}

    for(i=0;i<=200;i+=8)
	{Foreground_color_65k(Black);
	Circle_Center_XY(400/2,1280/2);
	Circle_Radius_R(i);
	Start_Circle_or_Ellipse();
	HAL_Delay(50);
	}
   	HAL_Delay(2000);

///////////////////////////Ellipse
  	Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();

   for(i=0;i<=200;i+=8)
	{Foreground_color_65k(White);
	Circle_Center_XY(400/2,1280/2);
	Ellipse_Radius_RxRy(i,i+100);
	Start_Circle_or_Ellipse();
	HAL_Delay(50);
	}

    for(i=0;i<=200;i+=8)
	{Foreground_color_65k(Black);
	Circle_Center_XY(400/2,1280/2);
	Ellipse_Radius_RxRy(i,i+100);
	Start_Circle_or_Ellipse();
	HAL_Delay(50);
	}
   	HAL_Delay(2000);

 ////////////////////////////Triangle
   	Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();

    for(i=0;i<=180;i+=8)
	{Foreground_color_65k(Yellow);
	Triangle_Point1_XY(400/2,i);
	Triangle_Point2_XY(i,1279-i);
	Triangle_Point3_XY(399-i,1279-i);
	Start_Triangle();
	HAL_Delay(50);
	}

    for(i=0;i<=180;i+=8)
	{Foreground_color_65k(Black);
	Triangle_Point1_XY(400/2,i);
	Triangle_Point2_XY(i,1279-i);
	Triangle_Point3_XY(399-i,1279-i);
	Start_Triangle();
	HAL_Delay(50);
	}
   	HAL_Delay(2000);


 ////////////////////////////line
   	Foreground_color_65k(Black);
	Line_Start_XY(0,0);
	Line_End_XY(399,1279);
	Start_Square_Fill();

    for(i=0;i<=400;i+=8)
	{Foreground_color_65k(Cyan);
	Line_Start_XY(i,0);
	Line_End_XY(399-i,1279);
	Start_Line();
	HAL_Delay(50);
	}
     for(i=0;i<=1280;i+=8)
	{Foreground_color_65k(Cyan);
	Line_Start_XY(0,1279-i);
	Line_End_XY(399,i);
	Start_Line();
	HAL_Delay(50);
	}

	/*
    for(i=0;i<=400;i+=8)
	{Foreground_color_65k(Black);
	Line_Start_XY(i,0);
	Line_End_XY(399-i,1279);
	Start_Line();
	HAL_Delay(50);
	}
	for(i=0;i<=1280;i+=8)
	{Foreground_color_65k(Black);
	Line_Start_XY(0,1279-i);
	Line_End_XY(399,i);
	Start_Line();
	HAL_Delay(50);
	}
		  */
   	HAL_Delay(2000);


}

void gray(void)
{ int i,col,line;

 	Select_Main_Window_16bpp();
	Main_Image_Start_Address(0);
	Main_Image_Width(400);
	Main_Window_Start_XY(0,0);
	Canvas_Image_Start_address(0);
	Canvas_image_width(400);//
	Active_Window_XY(0,0);
	Active_Window_WH(400,1280);



	 col=0;line=0;
	for(i=0;i<32;i++)
   	{	Foreground_color_65k(i<<11);
		Line_Start_XY(col,line);
		Line_End_XY(col+12,line+213);
		Start_Square_Fill();
		col+=12;
	}
	   col=0;line=213;
 	for(i=31;i>=0;i--)
   	{	Foreground_color_65k(i<<11);
		Line_Start_XY(col,line);
		Line_End_XY(col+12,line+213);
		Start_Square_Fill();
		col+=12;
	}

	 col=0;line=426;
	for(i=0;i<64;i++)
   	{	Foreground_color_65k(i<<5);
		Line_Start_XY(col,line);
		Line_End_XY(col+6,line+213);
		Start_Square_Fill();
		col+=6;
	}
	   col=0;line=639;
 	for(i=63;i>=0;i--)
   	{	Foreground_color_65k(i<<5);
		Line_Start_XY(col,line);
		Line_End_XY(col+6,line+213);
		Start_Square_Fill();
		col+=6;
	}


	 col=0;line=852;
	for(i=0;i<32;i++)
   	{	Foreground_color_65k(i);
		Line_Start_XY(col,line);
		Line_End_XY(col+12,line+213);
		Start_Square_Fill();
		col+=12;
	}
	   col=0;line=1065;
 	for(i=31;i>=0;i--)
   	{	Foreground_color_65k(i);
		Line_Start_XY(col,line);
		Line_End_XY(col+12,line+213);
		Start_Square_Fill();
		col+=12;
	}


	  HAL_Delay(1000);
 	
}
