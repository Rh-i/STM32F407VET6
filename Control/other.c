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

// 实现简易串口调试的菜单功能 以及对应的#if

CommandCode cmd ;
uint8_t usart1_rx_buffer = 0; // 单字节接收缓冲区

// 调用usart1 进行读取串口发送来的数据
void Menu_Init()
{
  HAL_UART_Receive_IT(&huart1, &usart1_rx_buffer, 1);
}

// 指令缓冲区和状态标志
void Process_Command(void) // 处理相应消息
{
  switch (usart1_rx_buffer)
  {
  case CMD_LED_TOGGLE:
    printf("LED toggle command received\n");
    usart1_rx_buffer = 0; // 清除接收缓冲区
    break;

  case CMD_READ_VOLTAGE:
    printf("Voltage read command received\n");
    usart1_rx_buffer = 0; // 清除接收缓冲区
    break;

  case CMD_START_LOGGING:
    printf("Start logging command received\n");
    usart1_rx_buffer = 0; // 清除接收缓冲区
    break;

  case CMD_STOP_LOGGING:
    printf("Stop logging command received\n");
    usart1_rx_buffer = 0; // 清除接收缓冲区
    break;

  default:
    if(usart1_rx_buffer != 0)
    {
      printf("%c\n",usart1_rx_buffer);
      usart1_rx_buffer = 0;
    }
    HAL_UART_Receive_IT(&huart1, &usart1_rx_buffer, 1);
    break;
  }
}

////// 实现重载弱定义回调函数部分
// UART DMA接收完成回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == UART5) // 陀螺仪数据读取
  {
    Process_DMA_Buffer();
  }
}
// UART DMA半传输完成回调函数
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == UART5) // 陀螺仪数据读取
  {
    Process_DMA_Buffer();
  }
}
