/*
 * lora2.h
 *
 *  Created on: Feb 12, 2024
 *      Author: egeme
 */

#ifndef INC_LORA2_H_
#define INC_LORA2_H_
#include "stdint.h"
#include "stm32f4xx_hal.h"

#define LORA2_M1_PORT GPIOA
#define LORA2_M1_PIN	GPIO_PIN_8
#define LORA2_M0_PORT GPIOA
#define LORA2_M0_PIN	GPIO_PIN_9
#define LORA2_AUX_PORT GPIOD
#define LORA2_AUX_PIN GPIO_PIN_2

#define LORA_REG_ADDH 0X00 //module address high
#define LORA_REG_ADDL 0X01 //module address lows
#define LORA_REG_REG0 0X02 //uart serial config register
#define LORA_REG_REG1 0X03 //default birakmak en iyisi
#define LORA_REG_REG2 0X04 //channel control register
//actual freq = 410.125 + CH*1M -> channel freq
#define LORA_REG_REG3 0X05 //rssi and transmission method registers
#define LORA_REG_CRYPT_H 0X06 //encryption high
#define LORA_REG_CRYPT_L 0X07 //encryption low

//command defines
#define LORA_WRITE_REG_CMD 0xC0
#define LORA_READ_REG_CMD 0xC1

//config defines 
#define LORA2_SET_FACTORY_DEFAULT 1
#define LORA2_RSSI_ENABLED 0
#define LORA2_UART_TX_TIMEOUT 200
#define LORA2_UART_RX_TIMEOUT 30
#define LORA2_UART_RX_BUFFER_SIZE 32 //bytes

typedef enum LoraModes2
{
	LORA_CONFIG2,
	LORA_NORMAL2
}LoraModes2;

typedef enum LoraState2
{
	LORA_NOT_READY2,
	LORA_READY2
}LoraState2;

typedef struct loraHandler2
{
    uint32_t adressHigh; //address regs
    uint32_t adressLow;

    uint32_t reg0; //uart register 9600 8N1
    uint32_t reg1; //anlamadim bos biraktim
    uint32_t reg2; //channel control register
    uint32_t reg3; //rssi ve transmission method register -> sadece rssi aktif et??

    uint32_t crypt_h; //crypting regs
    uint32_t crypt_l;

    uint8_t rxBuffer[LORA2_UART_RX_BUFFER_SIZE];

    uint8_t rxBufferIndex;

    //uart handler of the lora
    UART_HandleTypeDef * huart;
} loraHandler2;

LoraState2 LoraCheckReady2();

void LoraChangeMode2(loraHandler2 *hlora, LoraModes2 mode);

void LoraWrite2(loraHandler2 *hlora,uint8_t *buf,uint32_t len);

void LoraRead2(loraHandler2 *hlora,uint8_t *buf,uint32_t len);

void LoraConfig2(loraHandler2 *hlora);

void LoraInit2(loraHandler2 *hlora);

void LoraSetFactoryDefault2(loraHandler2 *hlora);

void LoraReadCallback2(loraHandler2 *hlora,uint8_t *buf);

#endif /* INC_LORA2_H_ */
