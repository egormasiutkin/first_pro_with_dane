#ifndef __8876DEMO_H__
#define __8876DEMO_H__

#define layer_start_addr(x) (512000*x)   //400*1280*2*5

void BTE_Compare(void);
void BTE_Color_Expansion(void);
void BTE_Pattern_Fill(void);
void PIP_Demo(void);
void App_Demo_Waveform(void);
void App_Demo_Scrolling_Text(void);
void App_Demo_multi_frame_buffer(void);
void App_Demo_slide_frame_buffer(void);
void App_Demo_Alpha_Blending(void);
void mono_Demo(void);
void Text_Demo(void);
void DMA_Demo(void);
void Geometric(void);
void gray(void);

#endif//__8876DEMO_H__
