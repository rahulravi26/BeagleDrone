#include "tests/test_help.h"
#include "app/buffer.h"
#include "app/communication.h"

#include <stdio.h>
#include <assert.h>

void RingBufferPushCString(RingBuffer *b, const char *str) {
    while(*str)
        RingBufferPush(b, *str++);
}

typedef struct state {
    void (*int_handler)(void);
    RingBuffer in_buff;
    char in_buff_data[COMMUNICATION_UART_IN_BUFF_SIZE];
} state_t;

static state_t state;

static void state_zero(void) {
    state.int_handler = 0;
    RingBufferInit(&state.in_buff, state.in_buff_data,
                   COMMUNICATION_UART_IN_BUFF_SIZE);
}

void LogCString(unsigned char level, const char *message) {
    printf("LOG - Level: %d. Msg: %s\n", level, message);
}

int UartGetCharNonBlocking(unsigned int baseAdd, char *data) {
    return RingBufferPop(&state.in_buff, data);
}

unsigned int UARTCharPutNonBlocking(unsigned int baseAdd,
                                    unsigned char byteWrite) {
}

static unsigned int int_enable_base_add;
static unsigned int int_enable_int_flag;
void UARTIntEnable(unsigned int baseAdd, unsigned int intFlag) {
    int_enable_base_add = baseAdd;
    int_enable_int_flag = intFlag;
}

static unsigned int int_disable_base_add;
static unsigned int int_disable_int_flag;
void UARTIntDisable(unsigned int baseAdd, unsigned int intFlag) {
    int_disable_base_add = baseAdd;
    int_disable_int_flag = intFlag;
}

static unsigned int init_base_add;
static unsigned int init_baud_rate;
void UartInit(unsigned int baseAdd, unsigned int baudRate) {
    init_base_add = baseAdd;
    init_baud_rate = baudRate;
}

void UartSetInterruptHandler(unsigned int baseAdd, void (*fnHandler)(void)) {
    state.int_handler = fnHandler;
}

static const char *command_handle_raw_last_data;

void CommandHandleRaw(const char *data)
{
    command_handle_raw_last_data = data;
}

void test_communication_init(void) {
    state_zero();
    CommunicationInit();
    assert(state.int_handler);
    state.int_handler();
}

void test_communication_receive(void) {
    state_zero();
    CommunicationInit();
    RingBufferPushCString(&state.in_buff, "\x7d\x68\x69\xc\x7f\x7d");
    state.int_handler();
    assert(command_handle_raw_last_data);
}

int main(int argc, char **argv) {
    TestInfo tests[] = {
        { "Communication Initialization", test_communication_init },
        { "Communication receive", test_communication_receive },
        { 0, 0 }
    };

    run_tests(tests);
}
