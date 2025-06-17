#ifndef __OTHER_H
#define __OTHER_H

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "HWT906.h"

// 定义指令码
typedef enum {
    CMD_LED_TOGGLE = 'L',     // 切换LED状态命令
    CMD_READ_VOLTAGE = 'V',   // 读取电压值命令
    CMD_START_LOGGING = 'S',  // 开始日志记录命令
    CMD_STOP_LOGGING = 'P'    // 停止日志记录命令
} CommandCode;

extern uint8_t usart1_rx_buffer;
extern CommandCode cmd;
extern int fputc(int ch, FILE *f);

void Process_Command(void); // 处理相应消息
void Menu_Init();

#endif // !__OTHER_H
