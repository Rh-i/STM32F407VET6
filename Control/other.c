#include "other.h"
#include "usart.h"

// 外部变量声明
extern DMA_HandleTypeDef hdma_uart5_rx;

// 实现printf功能 没加错误处理 这玩意能出错也是这辈子有了
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100); // 发送单字节数据
  return (ch);
}

// 实现重载回调函数部分

// UART DMA接收完成回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == UART5) //陀螺仪数据读取
  {
    Process_DMA_Buffer();
  }
}

