/*
 * Operation.h
 *
 *  Created on: 7 ก.ค. 2564
 *      Author: NB-036
 */
#define  BUFFER_SIZE1 100
#define  BUFFER_SIZE2 250
#define  BUFFER_SIZE3 550

uint8_t AD101_Str[BUFFER_SIZE2];
uint8_t FMA_Str[BUFFER_SIZE2];

uint8_t AD101_Status[BUFFER_SIZE1];
uint8_t AD101_Test[BUFFER_SIZE1];
uint8_t FMA1[BUFFER_SIZE1];
uint8_t FMA2[BUFFER_SIZE1];

uint8_t Com_buf[BUFFER_SIZE3];
uint32_t tickSec;
uint32_t Sec;

/*//////////////////PWM S//////////////////////*/
#define  ADC_CONVERTED_DATA_BUFFER_SIZE 1
uint16_t   aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
uint16_t   Data;
uint16_t   vADC;
#define   vMin 0
#define    vMax  4095
#define    rMin  23000
#define    rMax  50000
uint8_t   Lock;
uint8_t   Ena;
uint8_t   Dir;
/*//////////////////PWM E//////////////////////*/

/*//////////////////AD101 S//////////////////////*/
uint8_t   AD101T;
uint8_t   AD101S;
/*//////////////////AD101 E//////////////////////*/

/*//////////////////FMA S//////////////////////*/
uint8_t FMA_buf1[4];
uint8_t FMA_buf2[4];
#define  ADDR  0x28<<1//Shift Read 1 Bit
uint8_t ST1 ;
uint8_t ST2 ;

#define  Force_Max  13106//80 % of 14 Bit
#define  Force_Min  3277//20 % of 14 Bit
#define  Force_N 5//Force Range


typedef union {
 uint16_t Word;
 struct {
  int8_t L;
  int8_t H;
 };

} ui16_Type;

uint8_t Status1;
ui16_Type ADC_Force1;
ui16_Type ADC_Temp1;

float ADC_Force_Bit1;
float ADC_Temp_Bit1;

float Temp1;
float Force1;

uint8_t Status2;
ui16_Type ADC_Force2;
ui16_Type ADC_Temp2;

float ADC_Force_Bit2;
float ADC_Temp_Bit2;

float Temp2;
float Force2;
/*//////////////////FMA E//////////////////////*/

extern void operation();
extern void AD101();
extern void FMA();
extern void PWM();

#ifndef INC_OPERATION_H_
#define INC_OPERATION_H_



#endif /* INC_OPERATION_H_ */
