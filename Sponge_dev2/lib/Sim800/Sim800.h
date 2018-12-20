#ifndef SIM800_H
#define SIM800_H 0

#include <stdio.h>
#include "mbed.h"
#include <ctype.h>
#include "Adafruit_FONA.h"
#include "ATCmdParser.h"
#include "UARTSerial.h"

#define FONA_RST    D11
#define FONA_TX     PA_9   //PA_2
#define FONA_RX     PA_10   //PA_3
#define FONA_RI     D4
#define FONA_BAUD   9600

/* Costants */
#define EVENT_SYNCSTART "SYNCSTART+"
#define EVENT_IS_AVAILABLE "AVAILABLE+"
#define EVENT_YES "YES+"
#define EVENT_NO "NO+"
#define EVENT_GETSMS "+CMTI:"
#define EVENT_GETCALL "+CLIP:"
#define EVENT_OK "OK"
#define EVENT_READSMS "+CMGR:"


extern Serial pc;
extern DigitalOut led1;

class Sim800
{
    private:
        Adafruit_FONA fona;
        char replybuffer[255];
        int smsIndex;
        char c;
        char buffer[250];
        int bufCounter;
        
        
        
    
    public:
        Sim800(): fona(FONA_TX, FONA_RX, FONA_RST, FONA_RI){}

        /**
         * Setup 
         * 
         * Setup Adafruit FONA
         * Delete messages in the buffer
         */
        void setup();

        void checkForMessages();
        void flushSerial();
        char readBlocking();
        bool clearSmsBuffer();
        
    };

#endif






 

 