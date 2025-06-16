#ifndef __HWT906_H
#define __HWT906_H

#include "main.h"

typedef struct HWT906_DATA
{
  volatile float roll;
  volatile float pitch;
  volatile float yaw;
} HWT906_DATA;

void HWT906_Init();

// 陀螺仪数据处理相关函数声明 自动在回调函数里面调用 不需去看 只有增加数据的接收时需要看
// 回调函数为 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 中的uart5
void Parse_Gyro_Data(uint8_t *buffer, uint16_t start_pos);
void Process_DMA_Buffer(void);

extern volatile HWT906_DATA hwt906_info; // 封装接口 到时候对应的位置进行extern 而不是这里（暂时放着）

#endif // !__HWT906_H
