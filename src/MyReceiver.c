/* MyReceiver.c
 *
 * Responsible for storing, updating and clearing
 * a command received from USART.
 * 
 * It will echo the command back on the USART.
*/
#include "MyReceiver.h"
#include "MyTerminalUART.h"
#include <string.h>

static ReceiverEcode_e receiveState = RCVR_NOT_RECEIVED;
static void resetGlobals(void);

#define MAX_CHARACTERS 21
#define RETURN_KEY  '\r'

static struct
{
    char character[MAX_CHARACTERS];
    unsigned int index;
} inbuf;

void MyReceiver_Init(void)
{
    resetGlobals();
    MyTerminalUART_Init();
}

ReceiverEcode_e MyReceiver_Receive(void)
{
    unsigned char data = 0;
    data = MyTerminalUART_Read();
    if (data != 0)  // received data
    {
        if (data == RETURN_KEY)
        {
            receiveState = RCVR_DONE;
        }
        else
        {
            if (inbuf.index < (MAX_CHARACTERS - 1))
            {
                // max char limit not reached
                inbuf.character[inbuf.index++] = data;
                receiveState = RCVR_RECEIVED;
            }
        }
    }
    return receiveState;
}

// Writes a line to the terminal
void MyReceiver_Transmit(const char* buf)
{
    if (buf)
    {
        MyTerminalUART_WriteString(buf);
        MyTerminalUART_Write('\r');
    }
}

char* MyReceiver_GetBuffer(void)
{
    return inbuf.character;
}

void MyReceiver_Clear(void)
{
    resetGlobals();
}

/***************************** Private *******************/

static void resetGlobals(void)
{
    receiveState = RCVR_NOT_RECEIVED;
    memset(&inbuf, 0, sizeof(inbuf));
}
