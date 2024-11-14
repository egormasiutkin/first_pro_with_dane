#ifndef __SD_H__
#define __SD_H__

void SdWrite(unsigned char n);
unsigned char SdRead();
unsigned char SdResponse();
void SdCommand(unsigned char command, unsigned long argument, unsigned char crc);
unsigned char SdInit(void);
unsigned char SdWriteBlock(unsigned char *Block, unsigned long address,int len);
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,int len);
void Display_JPG_SDCARD();

#endif//__SD_H__

