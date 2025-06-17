#include "HWT906.h"
#include "usart.h"
#include "stdio.h"

extern DMA_HandleTypeDef hdma_uart5_rx; // DMA句柄
// 定义接收缓冲区大小
#define RX_BUFFER_SIZE 128
uint8_t rx_buffer[RX_BUFFER_SIZE]; // 接收缓冲区

volatile HWT906_DATA hwt906_info;

// 陀螺仪数据处理相关变量
uint16_t rx_write_pos = 0;   // DMA写入位置
uint16_t rx_read_pos = 0;    // 数据读取位置
void HWT906_Init()
{
  // 启动UART5的DMA接收
  HAL_StatusTypeDef dma_status = HAL_UART_Receive_DMA(&huart5, rx_buffer, RX_BUFFER_SIZE);
  __HAL_DMA_ENABLE_IT(&hdma_uart5_rx, DMA_IT_HT | DMA_IT_TC);
}

// 陀螺仪数据解析函数
void Parse_Gyro_Data(uint8_t *buffer, uint16_t start_pos)
{

  uint8_t checksum;
  int16_t data[3]; // 用于存储解析后的16位数据

  // 校验和验证
  checksum = 0x55 + buffer[(start_pos + 1) % RX_BUFFER_SIZE]; // 包头+类型
  for (int i = 0; i < 8; i++)
  {
    checksum += buffer[(start_pos + 2 + i) % RX_BUFFER_SIZE]; // 累加数据部分
  }

  if (checksum != buffer[(start_pos + 10) % RX_BUFFER_SIZE])
  {
    //! printf("Checksum failed\r\n");
    return; // 校验失败
  }

  // 检查是否为角度数据 同理 通过不同的包头可以确定是不同的数据 然后同样的逻辑进行拆包
  if (buffer[(start_pos + 1) % RX_BUFFER_SIZE] == 0x53)
  {
    // 数据解析
    for (int i = 0; i < 3; i++)
    {
      uint8_t low = buffer[(start_pos + 2 + 2 * i) % RX_BUFFER_SIZE];
      uint8_t high = buffer[(start_pos + 2 + 2 * i + 1) % RX_BUFFER_SIZE];
      data[i] = (int16_t)((int8_t)high << 8 | low);
    }

    // 转换为浮点角度并打印
    hwt906_info.roll = (float)data[0] / 32768.0f * 180;  // 横滚
    hwt906_info.pitch = (float)data[1] / 32768.0f * 180; // 俯仰
    hwt906_info.yaw = (float)data[2] / 32768.0f * 180;   // 航向

    // printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f\r\n", hwt906_info.roll, hwt906_info.pitch, hwt906_info.yaw);

  }
}

// 在DMA缓冲区中查找陀螺仪数据包
void Process_DMA_Buffer(void)
{
  uint16_t current_pos = RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart5_rx);

  // 检查是否有新数据
  while (rx_read_pos != current_pos)
  {
    // 查找包头0x55
    if (rx_buffer[rx_read_pos] == 0x55)
    {
      // 检查是否有完整的11字节数据包
      uint16_t remaining = (current_pos >= rx_read_pos) ? (current_pos - rx_read_pos) : (RX_BUFFER_SIZE - rx_read_pos + current_pos);

      if (remaining >= 11)
      {
        Parse_Gyro_Data(rx_buffer, rx_read_pos);
        rx_read_pos = (rx_read_pos + 11) % RX_BUFFER_SIZE;
      }
      else
      {
        break; // 数据不完整，等待更多数据
      }
    }
    else
    {
      rx_read_pos = (rx_read_pos + 1) % RX_BUFFER_SIZE;
    }
  }
}
