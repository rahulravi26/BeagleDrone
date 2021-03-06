#include "app/command.h"

typedef struct CommandLogMsgHeader {
    CommandTypeHeader type_header;
    unsigned char log_level;
} CommandLogMsgHeader;

void LogMessage(unsigned char level, const char *message, unsigned int size) {
    CommandLogMsgHeader hdr;
    hdr.type_header.type_id = COMMAND_TYPE_LOG;
    hdr.log_level = level;
    CommandSend((CommandTypeHeader*)&hdr, sizeof(CommandLogMsgHeader), message, size);
}

void LogCString(unsigned char level, const char *message) {
    unsigned int size = 0;
    while(message[size++]);
    LogMessage(level, message, size-1);
}
