/*
 * Operation.c
 *
 *  Created on: 7 ก.ค. 2564
 *      Author: NB-036
 */
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "Operation.h"
#include "stdio.h"
#include "string.h"


void operation()
{
	if(HAL_GetTick()>tickSec)
	{
		tickSec = HAL_GetTick()+990;
		Sec++;
		HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	}

	for(int i = 0;i<sizeof(BUFFER_SIZE3);i++)
	{
		Com_buf[i] = " ";
		if(i<sizeof(BUFFER_SIZE1))
		{
			AD101_Status[i] = " ";
			AD101_Test[i] = " ";
			FMA1[i] = " ";
			FMA2[i] = " ";
		}
		if(i<sizeof(BUFFER_SIZE2))
		{
			AD101_Str[i] = " ";
			FMA_Str[i] = " ";
		}
	}


	AD101();
	FMA();
	PWM();


	//sprintf(Com_buf,"\r\n%d %s | %s",Sec,AD101_Str,FMA_Str);
	sprintf(Com_buf,"\r\n%d\t%d\t%d\t%.3f\t%.3f\t%.3f\t%.3f",Sec,AD101T,AD101S,Force1,Temp1,Force2,Temp2);
	HAL_UART_Transmit(&huart2, Com_buf, sizeof(Com_buf), HAL_MAX_DELAY);

}

void AD101()
{

	AD101S = HAL_GPIO_ReadPin(AD101_O_GPIO_Port, AD101_O_Pin);
	AD101T = HAL_GPIO_ReadPin(AD101_S_GPIO_Port, AD101_S_Pin);

	if(!AD101S) sprintf(AD101_Status,"Liquid"); else sprintf(AD101_Status,"Bubble");
	if(!AD101T) sprintf(AD101_Test,"Fault"); else sprintf(AD101_Test,"Pass");

	sprintf(AD101_Str,"AD101 Self : %s Status : %s",AD101_Test,AD101_Status);
}

void FMA()
{
	  ST1 = HAL_I2C_Master_Receive(&hi2c3, ADDR, FMA_buf1, sizeof(FMA_buf1), 5000);
	  HAL_Delay(5);
	  ST2 = HAL_I2C_Master_Receive(&hi2c1, ADDR, FMA_buf2, sizeof(FMA_buf2), 5000);

	  ADC_Force1.H = FMA_buf1[0];//Set ADC_Force Bit High
	  ADC_Force1.L = FMA_buf1[1];//Set ADC_Force Bit Low
	  ADC_Temp1.H = FMA_buf1[2];//Set ADC_Temp Bit High
	  ADC_Temp1.L = FMA_buf1[3];//Set ADC_Temp Bit Low

	  ADC_Force_Bit1 = ADC_Force1.Word & 0x3FFF ;// AND Bit Status
	  Force1 = (((ADC_Force_Bit1-Force_Min)*Force_N)/(Force_Max-Force_Min));//Calculate Force

	  ADC_Temp_Bit1 = ADC_Temp1.Word>>5;//Shift Bit ignored 5 Bit
	  Temp1 = ((ADC_Temp_Bit1/2047)*200)-50;//Calculate Temp

	  Status1 = ADC_Force1.Word>>14;//Shift Bit Data Force 14 Bit

	  ADC_Force2.H = FMA_buf2[0];//Set ADC_Force Bit High
	  ADC_Force2.L = FMA_buf2[1];//Set ADC_Force Bit Low
	  ADC_Temp2.H = FMA_buf2[2];//Set ADC_Temp Bit High
	  ADC_Temp2.L = FMA_buf2[3];//Set ADC_Temp Bit Low

	  ADC_Force_Bit2 = ADC_Force2.Word & 0x3FFF ;// AND Bit Status
	  Force2 = (((ADC_Force_Bit2-Force_Min)*Force_N)/(Force_Max-Force_Min));//Calculate Force

	  ADC_Temp_Bit2 = ADC_Temp2.Word>>5;//Shift Bit ignored 5 Bit
	  Temp2 = ((ADC_Temp_Bit2/2047)*200)-50;//Calculate Temp

	  Status2 = ADC_Force2.Word>>14;//Shift Bit Data Force 14 Bit


	  if(ST1 != HAL_OK )sprintf(FMA1,"FMA1 : Fault");
	  else sprintf(FMA1,"FMA1 F.ADC : %.0f F. : %.3f T.ADC : %.0f T. : %.3f",ADC_Force_Bit1,Force1,ADC_Temp_Bit1,Temp1);

	  if(ST2 != HAL_OK )sprintf(FMA2,"FMA2 : Fault");
	  else sprintf(FMA2,"FMA2 F.ADC : %.0f F. : %.3f T.ADC : %.0f T. : %.3f",ADC_Force_Bit2,Force2,ADC_Temp_Bit2,Temp2);

	  sprintf(FMA_Str,"%s | %s",FMA1,FMA2);
}

uint16_t Map(uint16_t val,uint16_t Minval,uint16_t Maxval,uint16_t Minrange,uint16_t Maxrange)
{
	float Value;
	Value = ((val-Minval)*100.0)/(Maxval-Minval);
	Value = Minrange+((Value*(Maxrange-Minrange))/100.0);

	if(Value < Minrange)Value = Minrange;
	if(Value > Maxrange)Value = Maxrange;

	return(Value);
}

void PWM()
{

	Ena = HAL_GPIO_ReadPin(S_ENA_GPIO_Port, S_ENA_Pin);
	Dir = HAL_GPIO_ReadPin(S_DIR_GPIO_Port, S_DIR_Pin);

	if(!Ena && !Lock){ Lock = 1;HAL_GPIO_TogglePin(ENA_GPIO_Port, ENA_Pin); }
	if(!Dir && !Lock){ Lock = 1;HAL_GPIO_TogglePin(DIR_GPIO_Port, DIR_Pin); }

	if(Ena && Dir) Lock = 0;

	vADC =  aADCxConvertedData[0];
	Data = Map(vADC,vMin,vMax,rMin,rMax);
	htim2.Instance->ARR = Data;
	htim2.Instance->CCR1 = Data/2;
}
