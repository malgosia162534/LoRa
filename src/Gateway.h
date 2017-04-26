#ifndef __LORA_GATEWAY_H_
#define __LORA_GATEWAY_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace lora {

class Gateway: public cSimpleModule {
private:
    bool channelBusy868_1; // boolean value if channel1 is currently busy -> channelBusy1 = true, or not -> channelBusy1 = false
    bool channelBusy868_3; // boolean value if channel2 is currently busy -> channelBusy2 = true, or not -> channelBusy2 = false
    bool channelBusy868_5; // boolean value if channel3 is currently busy -> channelBusy3 = true, or not -> channelBusy3 = false
    double channelNumber;
    // Modules declaration
    cModule *endDevice;
    cModule *gateway;

    // Counter to collisions on each channel.
    int collisionChannel868_1;
    int collisionChannel868_3;
    int collisionChannel868_5;

    // Messages declaration
    cMessage *endReceivedMessage;
    cMessage *ack1;
    cMessage *ack2;

    // Receive_dalay values
    // First receive window RX1 uses the same frequency channel as the uplink
    // and a data rate that is a function of the data rate used  for the uplink.
    // By default RECEIVE_DELAY1 last 1s <- specification 7.1.8
    simtime_t receive_delay1;

    // Second receive window RX2 uses a fixed configurable frequency and data rate
    // and opens RECEIVE_DELAY2 which lasts RECEIVE_DELAY1 + 1 second
    simtime_t receive_delay2;

    //Simulation time of receive of a packet
    simtime_t receiveEndTime;

    //Time when gateway will send confirmation
    simtime_t ackTime;

    /*
     // state variables, event pointers
     bool channelBusy;
     cMessage *endRxEvent;

     long currentCollisionNumFrames;
     long receiveCounter;
     simtime_t recvStartTime;
     enum {
     IDLE = 0, TRANSMISSION = 1, COLLISION = 2
     };
     simsignal_t channelStateSignal;

     // statistics
     simsignal_t receiveBeginSignal;
     simsignal_t receiveSignal;
     simsignal_t collisionLengthSignal;
     simsignal_t collisionSignal;
     */
public:
    /*Gateway();
     virtual ~Gateway();*/

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    void receiveMessage();
};

}
;
//namespace

#endif

