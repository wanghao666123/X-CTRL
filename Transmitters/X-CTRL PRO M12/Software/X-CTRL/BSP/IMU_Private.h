#ifndef __IMU_PRIVATE_H
#define __IMU_PRIVATE_H

#include <stdint.h>

typedef struct{
    float AngleReal;    //!实际测量的角度值
    float Angle;        //!处理后的角度值（如滤波或修正后的值）
    int16_t Limit;      //!角度限制值（可能是上限或阈值）
}IMU_Angle_TypeDef;

typedef struct{
    IMU_Angle_TypeDef Pitch;
    IMU_Angle_TypeDef Roll;
    IMU_Angle_TypeDef Yaw;
}IMU_Axis_TypeDef;

typedef struct{
    int16_t Data;
    int16_t Last;
    int16_t delta;
    bool Reverse;
}IMU_ChBasic_TypeDef;

typedef struct{
    IMU_ChBasic_TypeDef Pitch;
    IMU_ChBasic_TypeDef Roll;
    IMU_ChBasic_TypeDef Yaw;
}IMU_Channel_TypeDef;

extern IMU_Axis_TypeDef     IMU_Axis;
extern IMU_Channel_TypeDef  IMU_Channel;
extern bool IsCalibrateStart;

void IMU_NormReset();
void IMU_CalibrateStart();
void IMU_LimitSetDefault();

#endif
