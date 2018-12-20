#include "Sim800.h"
#include <string.h>

bool flag = false;

char fonaNotificationBuffer[64];          //for notifications from the FONA
char smsBuffer[250];

class FonaEventListener : public Adafruit_FONA::EventListener
{
    virtual void onRing()
    {
        led1 = 0; // 0 = LED ON on the "nRF51 DK" board
    }

    virtual void onNoCarrier()
    {
        led1 = 1; // 1 = LED OFF on the "nRF51 DK" board
    }

    virtual void onSmsReceived()
    {
        flag = true;                
    }
};
FonaEventListener fonaEventListener;

void Sim800::setup()
{
    // See if the FONA is responding
    if (! fona.begin(9600)) {
        pc.printf("Couldn't find FONA\r\n");
        while (1);
    }
    fona.setEventListener(&fonaEventListener);
    pc.printf("FONA is OK\r\n");
    
    // Print SIM card IMEI number.
    char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
    uint8_t imeiLen = fona.getIMEI(imei);
    if (imeiLen > 0) {
        pc.printf("SIM card IMEI: %s\r\n", imei);
    }

    //Sim setup time
    wait(15);

    uint16_t sign = fona.getRSSI();
    pc.printf("Signal: %d \r\n", sign);

    // Clear sms buffer
    if(clearSmsBuffer()){
        pc.printf("Sms Buffer successfully emptyed!\n\r");
    }

    wait(1);
    pc.printf("Sim card and module ready!\n\r\n\r");
}


bool Sim800::clearSmsBuffer(){
    int x = 0;
    int8_t smsnum = fona.getNumSMS();
    if(smsnum < 0){
        pc.printf("Could not read the number of SMS's\r\n");
        return false;
    } else if(smsnum == 0){
        pc.printf("No sms's in the Buffer!\r\n");
        return false;
    } else {
        for(int i = 1; i <= smsnum; i++){
            if(fona.deleteSMS(i)) x++;
            else break;
        }
        if (x == smsnum) 
            return true;
        else 
            return false;
    }
}

void Sim800::checkForMessages()
{
    if(flag){

        char telSender[100];
        char text[250];
        
        //Get sms index
        int8_t smsnum = fona.getSmsIndex();
        
        //Get sms sender
        fona.getSMSSender(smsnum, telSender, 50);
        pc.printf("Sender: %s \r\n", telSender);

        /**
         * Get sms text
         * 
         * smsnum - sms index
         * text - text
         * smslen - number of bytes in a text
         */ 
        uint16_t smslen;
        if(! fona.readSMS(smsnum, text, 250, &smslen)) { // pass in buffer and max len!
                    pc.printf("Failed!\r\n");
        }

        pc.printf("***** SMS #%d (%d) bytes *****\r\n", smsnum, smslen); 
        pc.printf("%s\r\n", text);
        pc.printf("*****\r\n");

        /**
         * Compare if what we got is from receiving device
         */
        if(strstr(text, "OKSIM") != NULL){

            pc.printf("Command from reciving device received!\r\n");

            wait_ms(100);

            /**
             * Send response back to the sender
             */
            while(! fona.sendSMS(telSender, "OK\0")){
                pc.printf("Sending ...\r\n");
                wait_ms(500);
            }
            pc.printf("Response sent!\r\n");

        }
        
        /**
         * Delete sms with index
         */
        if (fona.deleteSMS(smsnum)){
            pc.printf("Sms with number %d deleted!\r\n\r\n", smsnum);
        }

        pc.printf("*********************************************************\r\n\r\n");

        //Flag to false
        flag = false;
    }
}



void Sim800::flushSerial() 
{
    while (pc.readable()) 
        pc.getc();
}
 
char Sim800::readBlocking() 
{
    while (!fona.readable());
    return fona.getc();
}
