#include "battery.h"

extern ADC_HandleTypeDef hadc1;

float Battery_ADC_Measurement() // 阻塞式读取 谁没事一直看啊 除了ui界面
{
  // 1. 启动 ADC 转换
  HAL_ADC_Start(&hadc1);

  // 2. 等待转换完成（超时时间 100ms ）
  if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
  {
    // 3. 读取 ADC 原始值（12 位时范围 0~4095 ）
    int16_t adc_value = HAL_ADC_GetValue(&hadc1);

    // 4. 计算电压：原始值 → 实际电压 具体内容套公式 就不宏定义了 太少了（考虑分压则乘以系数 ）
    return (float)(adc_value * 3.3 / 4095.0f) * 5; 
  }
  else
  {
    return 0.0f;
  }
}