#ifndef __VIRTUAL_SERIAL_H
#define __VIRTUAL_SERIAL_H

#include "AConfig.h"
#include "SerialEx.h"

#include "Stream.h"


using namespace std;

const uint16_t SERIAL_BUFFER_SIZE = 64;

struct ring_buffer
{
	unsigned char buffer[SERIAL_BUFFER_SIZE];
	volatile unsigned int head;
	volatile unsigned int tail;
};

class VirtualSerial : private CSerialEx, public Stream
{

protected:
	ring_buffer rx_buffer;
	HANDLE ghMutex;
	HANDLE ghReadEvent;
	HANDLE consoleThread;

public:
	
	inline void store_char(unsigned char c, ring_buffer *buffer)
	{
		int i = (unsigned int)(buffer->head + 1) % SERIAL_BUFFER_SIZE;

		// if we should be storing the received character into the location
		// just before the tail (meaning that the head would advance to the
		// current location of the tail), we're about to overflow the buffer
		// and so we don't write the character or advance the head.
		if (i != buffer->tail) {
			buffer->buffer[buffer->head] = c;
			buffer->head = i;
		}
	}



	void OnEvent(EEvent eEvent, EError eError);
	
	static DWORD WINAPI consoleThreadProc(LPVOID lpArg);
	DWORD consoleThreadProc(void);	
	
	int available(void);
	
	void begin(unsigned int baudrate);
	int read();
	size_t write(uint8_t);
	inline size_t write(unsigned long n) { return write((uint8_t)n); }
	inline size_t write(long n) { return write((uint8_t)n); }
	inline size_t write(unsigned int n) { return write((uint8_t)n); }
	inline size_t write(int n) { return write((uint8_t)n); }
	void flush();
	int peek();
	int _timedRead(unsigned long timeout, bool peekData=false);

	void lock();
	void unlock();

	int waitAvailable(unsigned long timeout);
};












#endif