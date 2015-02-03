#include <inttypes.h>

static const uint16_t MSG_ERROR = 0;
static const uint16_t MSG_OK = 1;
static const uint16_t MSG_PING = 2;

static const uint16_t MSG_pinMode = 10;
static const uint16_t MSG_digitalWrite = 20;
static const uint16_t MSG_analogWrite = 21;
static const uint16_t MSG_digitalRead = 30;
static const uint16_t MSG_analogRead = 31;



typedef struct pinMode_m
{
	uint8_t pin;
	uint8_t mode;
};

typedef struct digitalWrite_m
{
	uint8_t pin;
	uint8_t value;
};