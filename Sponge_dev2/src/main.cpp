#include "mbed.h"
#include <Sim800.h>


#define PC_BAUD     9600 

Serial pc(USBTX, USBRX);
Sim800 sim;
DigitalOut led1(LED1);

void setupPcSerial() {
    wait(1);
    pc.baud(PC_BAUD);
    pc.printf("\n\n\rPC Serial connection established at 9600 baud.\n\r");
    wait(1);
}

/* Setup
 * 
 * Sets up all the sensors and PC serial.
 */
void setup() {
    setupPcSerial();
    sim.setup();
} 

int main() {

  //Setup UART, SPI, SIM
  setup(); 

  while(1) {
    //led1 = !led1;
    sim.checkForMessages();
    //wait(1);
  }
}