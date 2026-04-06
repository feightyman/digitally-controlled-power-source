#include "header.h"


extern UART_HandleTypeDef huart1;
uint8_t KeyNum;
char buf[20];
uint8_t rx_data;
bool rx_flag=false,notarize_flag=true,change_flag=false;
volatile uint16_t rx_index;
char rx_buf[1024];
extern ADC_HandleTypeDef hadc1;
double Vin,V_set=5,current_V,current_I;
volatile uint16_t data=0;
char tx_buf[20];
uint16_t oled_tick=500;
//extern uint32_t cnt;


void Process_Init(void)
{
	current_V=V_set;
	MCP4725_SetVoltage_EEPROM(current_V*4095/12.0);
}

void key_proc(void)
{
	KeyNum = Key_GetNum();
		
	if (KeyNum == 1)
	{
		change_flag=true;
		V_set+=0.1;
	}
	if (KeyNum == 2)
	{
		change_flag=true;
		V_set-=0.1;
	}
	if (KeyNum == 3)
	{
		V_set=current_V;
		change_flag=false;
	}
	if (KeyNum == 4)
	{
		notarize_flag=true;
		change_flag=false;
		current_V=V_set;
	}
	
}

void uart_proc(void)
{
	if(rx_flag)
	{
		if(strcmp((const char*)rx_buf,(const char*)"set")==0)
		{
			sprintf(tx_buf,"please set Voltage\r\n");
			change_flag=true;
			HAL_UART_Transmit(&huart1,(uint8_t*)tx_buf,strlen(tx_buf),100);
		}
		else if(change_flag)
		{
			if(rx_buf[1]=='.')
			{
				if((rx_buf[0]-'0')>=1&&(rx_buf[0]-'0'<=9)&&(rx_buf[2]-'0')>=0&&(rx_buf[2]-'0'<=9))
				{
					sprintf(tx_buf,"Successfully Set\r\n");
					V_set=(rx_buf[0]-'0')+0.1*(rx_buf[2]-'0');
					current_V=V_set;
					change_flag=false;
					notarize_flag=true;
					HAL_UART_Transmit(&huart1,(uint8_t*)tx_buf,strlen(tx_buf),100);
				}
				else
				{
					V_set=current_V;
					change_flag=false;
					sprintf(tx_buf,"ERROR\r\n");
					HAL_UART_Transmit(&huart1,(uint8_t*)tx_buf,strlen(tx_buf),100);
				}
			}
			else if(rx_buf[2]=='.')
			{
				if((rx_buf[0]-'0')==1&&(rx_buf[1]-'0')>=0&&(rx_buf[1]-'0')<=2&&(rx_buf[3]-'0')>=0&&(rx_buf[3]-'0'<=9))
				{
					sprintf(tx_buf,"Successfully Set\r\n");
					V_set=(rx_buf[0]-'0')*10+(rx_buf[1]-'0')*1+0.1*(rx_buf[3]-'0');
					current_V=V_set;
					change_flag=false;
					notarize_flag=true;
					HAL_UART_Transmit(&huart1,(uint8_t*)tx_buf,strlen(tx_buf),100);
				}
				else
				{
					V_set=current_V;
					change_flag=false;
					sprintf(tx_buf,"ERROR\r\n");
					HAL_UART_Transmit(&huart1,(uint8_t*)tx_buf,strlen(tx_buf),100);
				}
			}
			else
			{
				V_set=current_V;
				change_flag=false;
				sprintf(tx_buf,"ERROR\r\n");
				HAL_UART_Transmit(&huart1,(uint8_t*)tx_buf,strlen(tx_buf),100);
			}
		}
		else
		{
			sprintf(tx_buf,"ERROR\r\n");
			HAL_UART_Transmit(&huart1,(uint8_t*)tx_buf,strlen(tx_buf),100);
		}
		memset((void*)rx_buf,0,1024);
		rx_index=0;
		rx_flag=false;
	}
}

void sig_proc(void)
{
	HAL_ADC_Start(&hadc1);
	Vin=3.3*HAL_ADC_GetValue(&hadc1)/4095;
	current_I=(Vin-2.5)/0.185;
	if(current_I>=1.25)
	{
		V_set=0;
		current_V=V_set;
		MCP4725_SetVoltage_EEPROM(current_V*4095/12.0);
	}
}

void oled_proc(void)
{
	sprintf(buf,"   V:%.1fV  ",V_set);
	OLED_ShowString(0,0,(uint8_t*)buf,16);
	if(change_flag)
	{
		
		if(oled_tick==500)
		{
			oled_tick=0;
			sprintf(buf,"              ");
			OLED_ShowString(0,0,(uint8_t*)buf,16);
		}
	}
	current_I=current_I<0?0:current_I;
	sprintf(buf,"   I:%.2fA  ",current_I);
	OLED_ShowString(0,3,(uint8_t*)buf,16);
}

void i2c_proc()
{
	if(notarize_flag)
	{
		MCP4725_SetVoltage_EEPROM(current_V*4095/12.0);
		notarize_flag=false;
	}
}
