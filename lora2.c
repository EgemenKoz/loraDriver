/*
 * lora2.cpp
 *
 *  Created on: Feb 12, 2024
 *      Author: egeme
 */


#include "lora2.h"

LoraState2 LoraCheckReady2()
{
	if(HAL_GPIO_ReadPin(LORA2_AUX_PORT, LORA2_AUX_PIN) != GPIO_PIN_SET)
		return 0;
	else
		return 1;
}

void LoraWrite2(loraHandler2 *hlora,uint8_t *buf,uint32_t len)
{
	

	HAL_UART_Transmit(hlora->huart, buf, len, LORA2_UART_TX_TIMEOUT);
}

void LoraRead2(loraHandler2 *hlora,uint8_t *buf,uint32_t len)
{


//	if(LORA2_RSSI_ENABLED)
//	{
//		len+=1;
//	}
	//HAL_UART_Receive_IT (&huart6, buf, len);
	HAL_UART_Receive(hlora->huart, buf, len, LORA2_UART_RX_TIMEOUT);
	//if rssi is enabled, the last byte will be rssi

}

void LoraReadCallback2(loraHandler2 *hlora,uint8_t *buf)
{

}

void LoraChangeMode2(loraHandler2 *hlora, LoraModes2 mode)
{
	while(!LoraCheckReady2())
	{
		
	}

	HAL_Delay(2);

	switch(mode)
	{
	case LORA_CONFIG2:
		//set m1 and m0 1
		HAL_GPIO_WritePin(LORA2_M0_PORT, LORA2_M0_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LORA2_M1_PORT, LORA2_M1_PIN, GPIO_PIN_SET);
		break;
	case LORA_NORMAL2:
		//set m1 and m0 0
		HAL_GPIO_WritePin(LORA2_M0_PORT, LORA2_M0_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LORA2_M1_PORT, LORA2_M1_PIN, GPIO_PIN_RESET);
		break;
	}
	while(!LoraCheckReady2())
		{

		}
	HAL_Delay(1);
}

void LoraConfig2(loraHandler2 *hlora)
{
	uint32_t rxBuf[4];
		uint32_t txBuf[6];
		//first change the mode to configure
		LoraChangeMode2(hlora, LORA_CONFIG2);
		//set the adress and the uart specs
		txBuf[0] = 	LORA_WRITE_REG_CMD;
		txBuf[1] = 	LORA_REG_ADDH;
		txBuf[2] =	0x03;
		txBuf[3] =	hlora->adressHigh;
		txBuf[4] =	hlora->adressLow;
		txBuf[5] =	hlora->reg0;
		LoraWrite2(hlora, (uint8_t*)txBuf, 24); //send write register command
		//set reg1
		txBuf[0] = 	LORA_READ_REG_CMD;
		txBuf[1] = 	LORA_REG_REG1;
		txBuf[2] = 	0x01;
		LoraWrite2(hlora, (uint8_t*)txBuf, 12);
		LoraRead2(hlora, (uint8_t*)rxBuf,16);
		txBuf[0] = 	LORA_WRITE_REG_CMD;
		txBuf[1] = 	LORA_REG_REG1;
		txBuf[2] = 	0x01;
		txBuf[3] = (hlora->reg1 | rxBuf[3]);
		LoraWrite2(hlora, (uint8_t*)txBuf, 16); //send write register command
		//set reg2
		txBuf[0] = 	LORA_WRITE_REG_CMD;
		txBuf[1] = 	LORA_REG_REG2;
		txBuf[2] = 	0x01;
		txBuf[3] = hlora->reg2;
		LoraWrite2(hlora, (uint8_t*)txBuf, 16);
		//set reg3
		txBuf[0] = 	LORA_READ_REG_CMD;
		txBuf[1] = 	LORA_REG_REG3;
		txBuf[2] = 	0x01;
		LoraWrite2(hlora, (uint8_t*)txBuf, 12);
		LoraRead2(hlora, (uint8_t*)rxBuf,16);

		txBuf[0] = 	LORA_WRITE_REG_CMD;
		txBuf[1] = 	LORA_REG_REG3;
		txBuf[2] = 	0x01;
		txBuf[3] = 	(hlora->reg3 | rxBuf[3]);
		LoraWrite2(hlora, (uint8_t*)txBuf, 16);
		//set crypt registers
		txBuf[0] = 	LORA_WRITE_REG_CMD;
		txBuf[1] = 	LORA_REG_CRYPT_H;
		txBuf[2] = 	0x02;
		txBuf[3] = 	0x00;
		txBuf[4] =	0x00;
		LoraWrite2(hlora, (uint8_t*)txBuf, 20); //send write register command

		LoraChangeMode2(hlora, LORA_NORMAL2);

		txBuf[0] = 	LORA_WRITE_REG_CMD;
}

void LoraSetFactoryDefault2(loraHandler2 *hlora)
{

	LoraChangeMode2(hlora, LORA_CONFIG2);

	uint32_t txBuf[11];

		txBuf[0] = 	LORA_WRITE_REG_CMD;
		txBuf[1] = 	LORA_REG_ADDH;
		txBuf[2] = 	0x08;
		txBuf[3] = 	0xAA;
		txBuf[4] =	0xAA;
		txBuf[5] =	0x62;
		txBuf[6] =	0x00;
		txBuf[7] = 	0x17;
		txBuf[8] =	0x00;
		txBuf[9] =	0x00;
		txBuf[10] =	0x00;

		LoraWrite2(hlora, (uint8_t*)txBuf, 44); //send write register command

		LoraChangeMode2(hlora, LORA_NORMAL2);
}

void LoraInit2(loraHandler2 *hlora)
{
	 HAL_GPIO_WritePin(LORA2_M0_PORT, LORA2_M0_PIN, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(LORA2_M1_PORT, LORA2_M1_PIN, GPIO_PIN_RESET);

	 while(!LoraCheckReady2())
	 			{

	 			}

	hlora->adressHigh = 0XAA;
	hlora->adressLow = 0XAA;

	hlora->reg0 = 0x62;
	hlora->reg1 = 0x00;
	hlora->reg2 = 0x00;
	if(LORA2_RSSI_ENABLED)
		hlora->reg3 = 0x80;
	else
		hlora->reg3 = 0x00;
	hlora->crypt_h = 0x00;
	hlora->crypt_l = 0x00;

	hlora->rxBufferIndex = 0;

	if(LORA2_SET_FACTORY_DEFAULT)
		LoraSetFactoryDefault2(hlora);
	else
		LoraConfig2(hlora);
}

