#ifndef LORA_H
#define LORA_H

#include "stdint.h"
//INCLUDE HAL LIBRARIES
#include "stm32f4xx_hal.h"

//aux -> self check devam ediyor mu/ gonderilmemis data var mi
//aux 0 -> transmission buffer bos degil
//aux 2ms up olmadan mod degismis sayilmaz

//m0 m1 -> 0 0 ise transmission mode
//m0 m1 -> 1 1 configration mode
//setting mode -> only 9600 8N1 supported
//pin defines 
//FILL PIN DEFINES
#define LORA_M1_PORT GPIOA
#define LORA_M1_PIN	GPIO_PIN_7
#define LORA_M0_PORT GPIOA
#define LORA_M0_PIN	GPIO_PIN_5
#define LORA_AUX_PORT GPIOC
#define LORA_AUX_PIN GPIO_PIN_5


//register defines 
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

//factory default c0 00 00 62 00 17

//config defines 
#define LORA_SET_FACTORY_DEFAULT 1
#define LORA_RSSI_ENABLED 0
#define LORA_UART_TX_TIMEOUT 500
#define LORA_UART_RX_TIMEOUT 500

typedef enum LoraModes
{
	LORA_CONFIG,
	LORA_NORMAL
}LoraModes;

typedef enum LoraState
{
	LORA_NOT_READY = 0,
	LORA_READY = 1
}LoraState;

typedef struct loraHandler
{
    uint32_t adressHigh; //address regs
    uint32_t adressLow;

    uint32_t reg0; //uart register 9600 8N1
    uint32_t reg1; //anlamadim bos biraktim
    uint32_t reg2; //channel control register
    uint32_t reg3; //rssi ve transmission method register -> sadece rssi aktif et??

    uint32_t crypt_h; //crypting regs
    uint32_t crypt_l;

    //uart handler of the lora
    UART_HandleTypeDef * huart;
} loraHandler;

LoraState LoraCheckReady();

void LoraChangeMode(loraHandler *hlora, LoraModes mode);

void LoraWrite(loraHandler *hlora,uint8_t *buf,uint32_t len);

void LoraRead(loraHandler *hlora,uint8_t *buf,uint32_t len);

void LoraConfig(loraHandler *hlora);

void LoraInit(loraHandler *hlora);

#endif
