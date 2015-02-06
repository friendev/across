
#include "SPI.h"

#ifdef ARDUINO

void initSPI()
{
	pinMode(SS, OUTPUT);
	digitalWrite(SS, HIGH);
	pinMode(MOSI, OUTPUT);
	pinMode(SCK, OUTPUT);
	pinMode(MISO, INPUT);

	digitalWrite(MOSI, HIGH);
	digitalWrite(MOSI, LOW);
	digitalWrite(SCK, LOW);

	SPCR = bit(SPE) | bit(MSTR); // 8 MHz @ 16
	bitSet(SPSR, SPI2X);

}

static void xferSPI(byte data) 
{
	SPDR = data;
	while (!(SPSR&(1 << SPIF)))
		;
}

void SPISend(uint8_t cspin, uint16_t length, uint8_t* data)
{
	SPISendReceive(cspin,length,data,0,NULL);
}
void SPIReceive(uint8_t cspin, uint16_t length, uint8_t* data)
{
	SPISendReceive(cspin,0,NULL,length,data);
}
void SPISendReceive(uint8_t cspin, uint16_t sendLength, uint8_t* sendData, uint16_t receiveLength, uint8_t* receiveData)
{
	cli();
	digitalWrite(cspin, LOW);

	while (sendLength--)
		xferSPI(*sendData++);

	while (receiveLength--)
	{
		xferSPI(0x00);
		*receiveData++ = SPDR;
	}

	digitalWrite(cspin, HIGH);
	sei();
}

#else

#include "VirtualHardware/ACRPCCLient.h"

void initSPI()
{
	if (!ACRPCCLient::hostLinkEnabled)
		printf("ACross_initSPI\n");
	else
	{
	
		invoke_RPC_NO_PARMS(ACross_initSPI);
	}
}

void SPISend(uint8_t cspin, uint16_t length, uint8_t* data)
{
		SPISendReceive(cspin, length, data, 0, NULL);
}

void SPIReceive(uint8_t cspin, uint16_t length, uint8_t* data)
{
		SPISendReceive(cspin, 0, NULL, length, data);
}

void SPISendReceive(uint8_t cspin, uint16_t sendLength, uint8_t* sendData, uint16_t receiveLength, uint8_t* receiveData)
{
	if (!ACRPCCLient::hostLinkEnabled)
		printf("ACross_SPISendReceive(cspin=%d, sendLength=%d, sendData=%p, receiveLength=%d, receiveData=%p)\n",
			cspin,sendLength,sendData,receiveLength,receiveData);
	else
	{
		invoke_RPC(ACRoss_SPISendReceive, cspin, sendLength, sendData, sendLength, receiveLength, receiveData, receiveLength);
	}
}



#endif