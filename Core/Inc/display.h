#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define layer_start_addr(x) (512000*x)
void debug(unsigned char mode, unsigned char index, unsigned char a,unsigned char b, unsigned int c, unsigned int d);
void DaneFontTest(void);
void preloadScreen(void);
void screen0x00(void);  // CUSTOM TEXT
void screen0x01(void);  // CUSTOM TEXT with TOP DOLLAR scrolling on top and bottom
void screen0x02(void);  // GOOD LUCK
void screen0x03(void);  // <top dollar> ON PAY LINE WITH MAX BET
void screen0x04(void);  // 3 <top dollar graphic> ON PLAYED LINE...
void screen0x05(void);  // TOP DOLLAR BONUS
void screen0x06(void);  // SCREEN TEST
void screen0x07(void);  // $$$ PLAY TOP DOLLAR $$$
void screen0x08(void);  // YOU ARE AWARDED 4 OFFERS!
void screen0x09(void);  // FIRST OFFER
void screen0x0a(void);  // SECOND OFFER
void screen0x0b(void);  // THIRD OFFER
void screen0x0c(void);  // FINAL OFFER
void screen0x0d(void);  // BEST PLAY SAYS  |  "TAKE OFFER"
void screen0x0e(void);  // BEST PLAY SAYS  |  "TRY AGAIN"
void screen0x0f(void);  // TAKE OFFER or TRY AGAIN
void screen0x10(void);  // CHOOSE A BUTTON |  ARROWS
void screen0x11(void);  // CONGRATULATIONS YOU WIN 1,245 CREDITS
void screen0x12(void);  // TOTAL BONUS WIN X,XXX CREDITS
void screen0x13(void);  // CURRENT OFFER, OFFERS LEFT
void screen0x17(void);  // TOTAL BONUS WIN X2 XXXX,XXXX CREDITS
void screen0x18(void);  // TOTAL BONUS WIN X2 X2, XXXX,XXXX CREDITS
void screen0x1B(void);  // <double top dollar> ON PAY LINE WITH MAX BET
void screen0x1C(void);  // 3 <double top dollar graphic> ON PLAYED LINE...
void screen0x1D(void);  // $$$ PLAY <DOUBLE> TOP DOLLAR $$$
void screen0x1E(void);  // CUSTOM TEXT with DOUBLE TOP DOLLAR scrolling on top and bottom
void screen0x1F(void);  // 3 <double top dollar graphic> ON PLAYED LINE...
void screen0x99(void);  // TRIAL TIMEOUT SCREEN
void WelcomeAnimation(void);
uint8_t RandomColor_256(void);
uint16_t RandomColor_65k(void);
uint32_t RandomColor_16M(void);
void BTE_logo_disp(int x, int y, int logo_index, int w, int h);
#endif//_DISPLAY_H_
