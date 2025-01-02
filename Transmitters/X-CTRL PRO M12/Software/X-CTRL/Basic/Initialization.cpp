#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Communication/ComPrivate.h"
#include "BSP/IMU_Private.h"
#include "Model/XC_Model.h"

/*遥控器结构体*/
X_CTRL_TypeDef CTRL;

/**
  * @brief  变量初始化
  * @param  无
  * @retval true成功 false失败
  */
//!eeprom中存放的数据
//!CTRL.JS_L,
//!CTRL.JS_R,
//!CTRL.ModelIndex,
//!XC_Model,
//!IMU_Axis.Pitch.Limit,
//!IMU_Axis.Roll.Limit,
//!IMU_Axis.Yaw.Limit,
bool Value_Init()
{
    DEBUG_FUNC_LOG();
    uint8_t eep_ret = EEPROM_Init();
    
    //! 注册需要掉电储保存的变量,暂时写到了DataReg_List这里
    EEPROM_REG_VALUE(CTRL.JS_L);//! 左摇杆信息
    EEPROM_REG_VALUE(CTRL.JS_R);//! 右摇杆信息
    EEPROM_REG_VALUE(CTRL.ModelIndex);//! 当前模型索引
    EEPROM_REG_VALUE(XC_Model);//!模型功能配置
    
    EEPROM_REG_VALUE(IMU_Axis.Pitch.Limit);//! 俯仰限幅
    EEPROM_REG_VALUE(IMU_Axis.Roll.Limit);//! 横滚限幅
    EEPROM_REG_VALUE(IMU_Axis.Yaw.Limit);//! 航向限幅

    //!eeprom初始化失败
    if(eep_ret != 0)
    {
        Serial.printf("EEPROM error! (0x%x)\r\n", eep_ret);
        return false;
    }
    //!读取eeprom信息，如果失败（如果之前eeprom中没有存放数据），使用默认参数初始化
    if(!EEPROM_ReadAll())
    {
        X_CTRL_SetDefault();
        Serial.println("EEPROM data error! set data to default value");
        return false;
    }
    
    Serial.println("EEPROM init success");
    
    return true;
}

void X_CTRL_SetDefault()
{
    //!CTRL.ModelIndex = 0;
    //!XC_Model = 0
    //!CTRL.CH_Config = 0
    //!CTRL.RF_Config = 0
    //!CTRL.State = 0
    Model_SetDefault();
    //!开启蜂鸣器，震动马达，握手，回传功能
    CTRL.State->IMU = false;
    CTRL.State->Sound = true;
    CTRL.State->LRA_Vibrate = true;
    CTRL.State->ERM_Vibrate = true;
    CTRL.State->Handshake = true;
    CTRL.State->Passback = true;
    CTRL.State->FHSS = false;
    //!设置摇杆默认值
    Joystick_SetDefault();
    //!设置通道默认值
    Com_ChannelSetDefault();
    //!设置IMU默认值
    IMU_LimitSetDefault();
}

/**
  * @brief  遥控器初始化
  * @param  无
  * @retval 无
  */
void X_CTRL_Init()
{
    Serial.begin(115200);
    DEBUG_FUNC_LOG();
    
    I2C_Scan(false);
    
    Power_Init();
    //!读取eeprom信息，如果失败（如果之前eeprom中没有存放数据），使用默认参数初始化
    Value_Init();
    //!将CTRL指向XC_Model
    Model_Init();
    //!显示屏初始化
    Display_Init();
    //!按键初始化
    Button_Init();
    //!蜂鸣器初始化
    Audio_Init();
    //!IMU初始化
    IMU_Init();
    //!摇杆初始化 开启adc dma
    Joystick_Init();
    //!蓝牙初始化
    Bluetooth_Init();
    //!震动马达初始化
    MotorLRA_Init();
    MotorERM_Init();
    //!通道初始化
    Com_ChannelInit();
    //!nrf24l01初始化
    if(Com_Init())
    {
        Serial.println("Communication init success");
        Audio_PlayMusic(MC_Type::MC_StartUp);
    }
    else
    {
        Serial.println("Communication error!");
        Audio_PlayMusic(MC_Type::MC_Error);
    }
}
