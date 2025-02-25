#include "JoystickMap.h"
#include <math.h>

JoystickMap::JoystickMap(int16_t* table, uint16_t size)
{
    InputMin = 0;
    InputMid = 4095 / 2;
    InputMax = 4095;
    InputDeadZone = 50;//!摇杆死区
    OutputMax = 1000;//!输出的最大值

    CurveTable = table;
    CurveTableSize = size;
    //!设置曲线 由于是两个5，所以这里的曲线是线性的
    SetCurve(5, 5);
}

JoystickMap::~JoystickMap()
{
}

void JoystickMap::SetInputReference(int16_t min, int16_t mid, int16_t max)
{
    InputMin = min;
    InputMid = mid;
    InputMax = max;
}

void JoystickMap::SetCurve(float startK, float endK)
{
    //!101
    for(int i = 0; i < CurveTableSize; i++)
    {
        CurveTable[i] = NonlinearMap(i, 0, CurveTableSize - 1, 0, OutputMax, startK, endK);
    }
}

float JoystickMap::GetOutputCurve(int16_t in_val, int16_t max)
{
    float retval;

    if(in_val < 0)
    {
        return 0.0f;
    }
    if(in_val >= max)
    {
        return OutputMax;
    }

    int index = in_val * (CurveTableSize - 1) / max;

    if(max % in_val == 0)
    {
        retval = CurveTable[index];
    }
    else
    {
        int16_t out_min = CurveTable[index];
        int16_t out_max = CurveTable[index + 1];
        float pos = LinearMap(in_val, 0, max, 0, CurveTableSize - 1);
        retval = LinearMap(pos, index, index + 1, out_min, out_max);
    }
    return retval;
}

int16_t JoystickMap::GetOutput()
{
    return Output;
}

int16_t JoystickMap::GetNext(int16_t in_val)
{
    SetInput(in_val);
    return Output;
}

void JoystickMap::SetOutputMax(int16_t max)
{
    OutputMax = max;
}

void JoystickMap::SetInputDeadZone(uint16_t in_dz)
{
    InputDeadZone = in_dz;
}

void JoystickMap::SetInput(int16_t in_val)
{
    if(in_val > InputMax)in_val = InputMax;
    else if(in_val < InputMin)in_val = InputMin;
    
    if(in_val - InputMid > InputDeadZone)
    {
        int16_t in = in_val - (InputMid + InputDeadZone);
        int16_t max = InputMax - (InputMid + InputDeadZone);
        Output = GetOutputCurve(in, max);
    }
    else if(InputMid - in_val > InputDeadZone)
    {
        int16_t in = (InputMid - InputDeadZone) - in_val;
        int16_t max = (InputMid - InputDeadZone) - InputMin;
        Output = -GetOutputCurve(in, max);
    }
    else
    {
        Output = 0;
    }
}
//!该函数将输入值从输入范围 [in_min, in_max] 映射到输出范围 [out_min, out_max]
//!线性映射
float JoystickMap::LinearMap(float value, float in_min, float in_max, float out_min, float out_max)
{
    return (((value) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)) + (out_min));
}
//!i ==> (0~100)
//!i,0,100,0,1000,5,5
float JoystickMap::NonlinearMap(float value, float in_min, float in_max, float out_min, float out_max, float startK, float endK)
{
    if(abs(startK - endK) < 0.001f)
        return LinearMap(value, in_min, in_max, out_min, out_max);
    //!如果 startK 和 endK 比较小（接近零），则 exp(startK) 和 exp(endK) 的值会接近 1，这样输出值变化会比较平缓；而如果 startK 和 endK 比较大，exp(startK) 和 exp(endK) 的值会显著增大，导致更快的输出变化
    float stY = exp(startK);
    float edY = exp(endK);
    //!使用线性映射将输入值 value 映射到 [startK, endK] 范围内
    float x = LinearMap(value, in_min, in_max, startK, endK);
    //!exp(x) 的值在 [exp(startK), exp(endK)] 范围内变化
    //!速度小的时候，exp(x) 的值接近 exp(startK)，输出值变化平缓；速度大的时候，exp(x) 的值接近 exp(endK)，输出值变化快
    return LinearMap(exp(x), stY, edY, out_min, out_max);
}
