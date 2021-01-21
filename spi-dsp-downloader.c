/* code program untuk komunikasi spi ke dsp */

#include "main.h"

// definisi data ram pada dsp
#include "program_data.h"

// alamat memori dsp
#define CORE_ADDR 0x081C 
#define PROG_ADDR 0x0400
#define PARM_ADDR 0x0000

void clock()
{
	HAL_GPIO_WritePin(SPI_CLK_GPIO_Port, SPI_CLK_Pin, GPIO_PIN_SET);
	for(int i=0; i<2; i++);
	HAL_GPIO_WritePin(SPI_CLK_GPIO_Port, SPI_CLK_Pin, GPIO_PIN_RESET);
	for(int i=0; i<2; i++);
}

void spiWrite(unsigned char data)
{
	for (unsigned int i = 0; i < 8; i ++) 
	{
    HAL_GPIO_WritePin(SPI_DATA_GPIO_Port, SPI_DATA_Pin, (data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    clock();
    data = data << 1;
  }
}

void SPI_init()
{
	HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPI_CLK_GPIO_Port, SPI_CLK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPI_DATA_GPIO_Port, SPI_DATA_Pin, GPIO_PIN_RESET);
	HAL_Delay(5);
	for(int i = 0; i < 3; i ++)
	{
		HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_RESET);
		HAL_Delay(5);
		HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_SET);
		HAL_Delay(5);
	}
}

// load program data
void LoadProgram()
{  
  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_RESET);

  spiWrite(0x00);
  spiWrite(0x04);
  spiWrite(0x00);
	// kirim data array
  for(int index = 0; index < sizeof(Program); index ++){
    uint8_t data = Program[index];
    spiWrite(data);
  }
	
  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_SET);
}

// load parameter data
void LoadParameter()
{ 
  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_RESET);
	
  spiWrite(0x00);
  spiWrite(0x00);
  spiWrite(0x00);
	// kirim data array
  for(int index = 0; index < sizeof(Parameter); index ++){
    uint8_t data = Parameter[index];
    spiWrite(data);
  }
	
  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_SET);
}

// mute dsp
void mute()
{  
  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_RESET);

  spiWrite(0x00);
  spiWrite(0x08);
  spiWrite(0x1C);
  spiWrite(0x00);
  spiWrite(0x18);
  
  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_SET);
}

// unmute dsp
void unmute()
{  
  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_RESET);

  spiWrite(0x00);
  spiWrite(0x08);
  spiWrite(0x1C);
  spiWrite(0x00);
  spiWrite(0x1C);
  
  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_SET);
}

// kirim config data
void Config()
{  
  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_RESET);

  spiWrite(0x00);
  spiWrite(0x08);
  spiWrite(0x1C);

  for(int index = 0; index < sizeof(HWConfig); index ++){
    uint8_t data = HWConfig[index];
    spiWrite(data);
  }
  
  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_SET);
}

// safeload address
void SafeLoadAddrWrite(uint16_t SafeAddr, uint16_t dataAddr)
{
  unsigned char SubAddr0 = SafeAddr >> 8;
  unsigned char SubAddr1 = SafeAddr & 0xff;

  unsigned char byte3 = dataAddr >> 8;
  unsigned char byte4 = dataAddr & 0xff;

  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_RESET);

  spiWrite(0x00);

  spiWrite(SubAddr0);
  spiWrite(SubAddr1);
  spiWrite(byte3);
  spiWrite(byte4);

  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_SET);
}

// safeload parameter
void SafeLoadParamWrite(uint16_t SafeAddr, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4)
{
  unsigned char SubAddr0 = SafeAddr >> 8;
  unsigned char SubAddr1 = SafeAddr & 0xff;

  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_RESET);

  spiWrite(0x00);

  spiWrite(SubAddr0);
  spiWrite(SubAddr1);

  spiWrite(0x00);

  spiWrite(data1);
  spiWrite(data2);
  spiWrite(data3);
  spiWrite(data4);

  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_SET);
}

// safeload flag
void SafeLoadFlag()
{
  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_RESET);

  spiWrite(0x00);
  spiWrite(0x08);
  spiWrite(0x1C);
  spiWrite(0x00);
  spiWrite(0x3C);

  HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_SET);
}
