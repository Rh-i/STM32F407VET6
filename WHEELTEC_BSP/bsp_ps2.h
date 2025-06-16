#ifndef __BSP_PS2_H
#define __BSP_PS2_H

#include <stdint.h>
#include "usbh_hid.h"

// 有线ps2 V1 手柄设备标识符
#define Wired_PS2_V1_VID 0x0810
#define Wired_PS2_V1_PID 0x0001

// 有线ps2 V2 手柄设备标识符
#define Wired_PS2_V2_VID 0x0079
#define Wired_PS2_V2_PID 0x0006

// 无线ps2手柄安卓模式设备标识符
#define Wireless_Android_PS2_VID 0x045E
#define Wireless_Android_PS2_PID 0x028E

// 无线ps2手柄PC模式设备标识符
#define Wireless_PC_PS2_VID 0x0079
#define Wireless_PC_PS2_PID 0x0126

// PS2手柄类型定义
typedef enum PS2_TYPE_t
{
	UnKnown_Dev,					 // 未知设备
	Wired_V1_PS2,					 // 有线ps2手柄 V1
	Wired_V2_PS2,					 // 有线ps2手柄 V2
	Wiredless_PC_PS2,			 // 无线ps2手柄pc模式
	Wiredless_Android_PS2, // 无线ps2手柄安卓模式
} PS2_TYPE_t;

// 表示按键的状态值
#define PS2KEY_PressDOWN 1 // 按键松开
#define PS2KEY_PressUP 0	 // 按键按下

// PS2按键状态(无状态、单击、双击、长按)
typedef enum PS2KEY_State_t
{
	PS2KEYSTATE_NONE,
	PS2KEYSTATE_SINGLECLICK,
	PS2KEYSTATE_DOUBLECLICK,
	PS2KEYSTATE_LONGCLICK
} PS2KEY_State_t;

// PS2手柄对象
typedef struct PS2INFO_t
{
	uint8_t LX; // 4个方向摇杆值,取值0~255
	uint8_t LY;
	uint8_t RX;
	uint8_t RY;
	PS2KEY_State_t (*getKeyEvent)(uint8_t keybit); // 获取按键事件,有单击、双击、长按3种事件可以获取,入口参数为按键的枚举键值
	uint8_t (*getKeyState)(uint8_t keybit);				 // 获取按键的状态,0表示按下,1表示松开,入口参数为按键的枚举键值
} PS2INFO_t;

// PS2按键位置枚举(bit0~bit15分别为下面的0~15)
enum PS2KEY_NAME_t
{
	PS2KEY_SELECT = 0, // 选择按键
	PS2KEY_LROCKER,		 // 左右摇杆按下键值
	PS2KEY_RROCKER,
	PS2KEY_START, // 开始按键
	PS2KEY_UP,		// 左按键区域
	PS2KEY_RIGHT,
	PS2KEY_DOWN,
	PS2KEY_LEFT,
	PS2KEY_L2, // 左右扳机按键值
	PS2KEY_R2,
	PS2KEY_L1,
	PS2KEY_R1,
	PS2KEY_1GREEN, // 右按键区域
	PS2KEY_2RED,
	PS2KEY_3BLUE,
	PS2KEY_4PINK
};

extern PS2INFO_t ps2_info; // 存储对应回调得到的手柄对象的结构体 现在先在这extern 到时候到控制代码的时候 就可以把这行放到对应位置 避免接口暴露

#endif
