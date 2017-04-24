
#include "SystemCore.h"

uint16_t MyMAFilter(MovingAverageFilter_t* This,uint16_t InputData)
{
		uint8_t Index_Temp = 0;
		This->FilterLevel = (unsigned char)RANGE(This->FilterLevel,1,DataNum);
		This->DataTotal = 0;
		This->Now = InputData;
		This->DataHistory[This->Rear] = This->Now;
		if(++This->Rear >= This->FilterLevel) This->Rear = 0;
		for(Index_Temp = 0;Index_Temp < This->FilterLevel ; ++Index_Temp)
		{
				This->DataTotal += This->DataHistory[Index_Temp];
		}
		return This->DataTotal / This->FilterLevel;

}

float MySISOKalmanFilter(SISOKalmanFilter_Type* This,float Measure)
{
    float Temp1,Temp2;   
    //arm_sqrt_f32(1.0/(This->EstimateCovariance*This->EstimateCovariance
    //                  +This->MeasureCovariance*This->MeasureCovariance),&Temp1);
	  Temp1 = sqrt(1.0/(This->EstimateCovariance*This->EstimateCovariance
                      +This->MeasureCovariance*This->MeasureCovariance));
    This->KalmanGain=Temp1*This->EstimateCovariance;
    if (This->KalmanGain <This->Kalman_MIN)
        This->KalmanGain =This->Kalman_MIN;
    This->EstimateValue=This->EstimateValue+
                            This->KalmanGain*(Measure-This->EstimateValue);
    //arm_sqrt_f32(1-This->KalmanGain,&Temp2);
		Temp2 = sqrt(1-This->KalmanGain);
    This->EstimateCovariance=Temp2*This->EstimateCovariance;
    if (This->EstimateCovariance<This->Kalman_MIN)
        This->EstimateCovariance =This->Kalman_MIN;
    This->MeasureCovariance=Temp2*This->MeasureCovariance;
    if(This->MeasureCovariance<This->Kalman_MIN)
       This->MeasureCovariance=This->Kalman_MIN;
    return This->EstimateValue; 
}

unsigned char ByteFindMax(unsigned char num1,unsigned char num2,unsigned char num3,unsigned char num4)
{
	unsigned char max;
	unsigned char temp;
	temp = num1>num2?num1:num2;
	temp = temp>num3?temp:num3;
	max  = temp>num4?temp:num4;
	return max;
}

