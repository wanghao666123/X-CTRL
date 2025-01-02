#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Communication/ComPrivate.h"
#include "BSP/IMU_Private.h"
#include "Model/XC_Model.h"

/*ң�����ṹ��*/
X_CTRL_TypeDef CTRL;

/**
  * @brief  ������ʼ��
  * @param  ��
  * @retval true�ɹ� falseʧ��
  */
//!eeprom�д�ŵ�����
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
    
    //! ע����Ҫ���索����ı���,��ʱд����DataReg_List����
    EEPROM_REG_VALUE(CTRL.JS_L);//! ��ҡ����Ϣ
    EEPROM_REG_VALUE(CTRL.JS_R);//! ��ҡ����Ϣ
    EEPROM_REG_VALUE(CTRL.ModelIndex);//! ��ǰģ������
    EEPROM_REG_VALUE(XC_Model);//!ģ�͹�������
    
    EEPROM_REG_VALUE(IMU_Axis.Pitch.Limit);//! �����޷�
    EEPROM_REG_VALUE(IMU_Axis.Roll.Limit);//! ����޷�
    EEPROM_REG_VALUE(IMU_Axis.Yaw.Limit);//! �����޷�

    //!eeprom��ʼ��ʧ��
    if(eep_ret != 0)
    {
        Serial.printf("EEPROM error! (0x%x)\r\n", eep_ret);
        return false;
    }
    //!��ȡeeprom��Ϣ�����ʧ�ܣ����֮ǰeeprom��û�д�����ݣ���ʹ��Ĭ�ϲ�����ʼ��
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
    //!�������������������֣��ش�����
    CTRL.State->IMU = false;
    CTRL.State->Sound = true;
    CTRL.State->LRA_Vibrate = true;
    CTRL.State->ERM_Vibrate = true;
    CTRL.State->Handshake = true;
    CTRL.State->Passback = true;
    CTRL.State->FHSS = false;
    //!����ҡ��Ĭ��ֵ
    Joystick_SetDefault();
    //!����ͨ��Ĭ��ֵ
    Com_ChannelSetDefault();
    //!����IMUĬ��ֵ
    IMU_LimitSetDefault();
}

/**
  * @brief  ң������ʼ��
  * @param  ��
  * @retval ��
  */
void X_CTRL_Init()
{
    Serial.begin(115200);
    DEBUG_FUNC_LOG();
    
    I2C_Scan(false);
    
    Power_Init();
    //!��ȡeeprom��Ϣ�����ʧ�ܣ����֮ǰeeprom��û�д�����ݣ���ʹ��Ĭ�ϲ�����ʼ��
    Value_Init();
    //!��CTRLָ��XC_Model
    Model_Init();
    //!��ʾ����ʼ��
    Display_Init();
    //!������ʼ��
    Button_Init();
    //!��������ʼ��
    Audio_Init();
    //!IMU��ʼ��
    IMU_Init();
    //!ҡ�˳�ʼ�� ����adc dma
    Joystick_Init();
    //!������ʼ��
    Bluetooth_Init();
    //!������ʼ��
    MotorLRA_Init();
    MotorERM_Init();
    //!ͨ����ʼ��
    Com_ChannelInit();
    //!nrf24l01��ʼ��
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
