/*
 * commands.h
 *
 *  Created on: Jan 25, 2024
 *      Author: DaneRuyle
 */

#ifndef INC_COMMANDS_H_
#define INC_COMMANDS_H_

#define BUFFER_LEN 256
#define TIME_LIMIT 10000

enum TASK{TASK_NO = 0,TASK_RESET, TASK_CLEAR, TASK_PAUSE,TASK_SINGLE,TASK_MULTI};

extern unsigned int  idleTime;
extern unsigned char rxidlecnt;
extern unsigned char currentTask;
extern unsigned short mainBuffer[BUFFER_LEN];
extern unsigned short aRxBuffer[BUFFER_LEN];
extern unsigned char displayText[BUFFER_LEN];

extern unsigned char screen0x00Text[40];
extern unsigned char screen0x01Text[40];
extern unsigned char offerText[6];
extern unsigned char offerMultiplied[6];
extern unsigned char offerLeftText[2];

extern unsigned char mtflag;
extern unsigned char multiIndex[2];
extern unsigned char currentIndex;
extern unsigned int  multiTime;
extern unsigned int  multiLength[2];
extern unsigned char fullcolor;

void InitCommands(void);
void PacketProc(void);
void HandlePacket(void);

uint16_t crc16_ccitt_reversed_w16(const uint16_t* data, size_t length);
#endif /* INC_COMMANDS_H_ */
