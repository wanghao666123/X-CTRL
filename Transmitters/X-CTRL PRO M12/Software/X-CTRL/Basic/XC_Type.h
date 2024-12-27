#ifndef __XC_TYPE_H
#define __XC_TYPE_H

#include "stdint.h"
#include "Communication/RCX/RCX.h"

#pragma pack(1)

/*校准*/
typedef struct
{
    int16_t Val;    //!通道的当前值

    int16_t AdcVal; //!通道对应的 ADC 值（模拟到数字转换的值）
    int16_t Min;    //!通道的最小值
    int16_t Mid;    //!通道的中间值
    int16_t Max;    //!通道的最大值
    bool Reverse;   //!是否反转标志，表示通道的反向控制（例如某些设备的输入可能需要反转）
    struct
    {
        double Start;   //!曲线的起始值
        double End;     //!曲线的结束值
    } Curve;            //!曲线映射参数，包含起始和结束值
} XC_Channel_TypeDef;

/*摇杆*/
typedef struct
{
    XC_Channel_TypeDef X;
    XC_Channel_TypeDef Y;
} XC_Joystick_TypeDef;

/*功能使能*/
typedef struct
{
    bool Passback;      //!是否启用回传功能
    bool Handshake;     //!是否完成握手
    bool FHSS;          //!是否启用跳频扩频（FHSS）
    bool IdleWarn;      //!是否启用空闲警告
    bool SignWarn;      //!是否启用信号警告
    bool JostickFilter; //!是否启用摇杆滤波
    bool Sound;         //!是否启用声音
    bool LRA_Vibrate;   //!是否启用 LRA 振动器
    bool ERM_Vibrate;   //!是否启用 ERM 振动器
    bool IMU;           //!是否启用惯性测量单元 (IMU)
    bool Bluetooth;     //!是否启用蓝牙
    bool CurrentDisp;   //!是否显示当前状态
}XC_State_TypeDef;

/*射频配置*/
typedef struct
{
    uint8_t Speed;
    uint8_t Freq;
    uint8_t Addr[5];
}XC_RF_Config_TypeDef;

/*通道配置*/
typedef struct
{
    bool Reverse[RCX_CHANNEL_NUM];//!表示每个通道是否反向
    uint8_t AttachMap[RCX_CHANNEL_NUM];//!定义通道映射关系
}XC_CH_Config_TypeDef;

/*遥控器状态*/
typedef struct
{
    /*摇杆*/
    XC_Joystick_TypeDef JS_L;
    XC_Joystick_TypeDef JS_R;

    /*按键*/
    union
    {
        struct
        {
            uint8_t BT_UP : 1;
            uint8_t BT_DOWN : 1;
            uint8_t BT_BACK : 1;
            uint8_t BT_OK : 1;
            uint8_t BT_L1 : 1;
            uint8_t BT_L2 : 1;
            uint8_t BT_R1 : 1;
            uint8_t BT_R2 : 1;
        } Bit;
        uint8_t Value;
    } Key;

    /*三段式开关*/
    int16_t Switch[4];

    /*通道配置信息*/
    XC_CH_Config_TypeDef* CH_Config;

    /*功能开关*/
    XC_State_TypeDef* State;
    
    /*射频配置*/
    XC_RF_Config_TypeDef* RF_Config;
    
    uint8_t ModelIndex;

    /*电池信息*/
    struct
    {
        float Usage;
        float Voltage;
    } Battery;
} X_CTRL_TypeDef;

#pragma pack()

#endif
