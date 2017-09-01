#ifndef __LORA_GATEWAY_H_
#define __LORA_GATEWAY_H_

#include <omnetpp.h>
#include <loraPck_m.h>
using namespace omnetpp;

namespace lora {

class Gateway: public cSimpleModule {
private:
    bool channelBusy_1; // boolean value if channel1 is currently busy -> channelBusy1 = true, or not -> channelBusy1 = false
    bool channelBusy_2; // boolean value if channel2 is currently busy -> channelBusy2 = true, or not -> channelBusy2 = false
    bool channelBusy_3; // boolean value if channel3 is currently busy -> channelBusy3 = true, or not -> channelBusy3 = false
    double channelNumber;
    // Modules declaration
    cModule *endDevice;
    cModule *gateway;
    int numGateways;
    cQueue packetQueue;
    // Position of Gateway
    int posX;
    int posY;

    // Counter to collisions on each channel.
    long collisionChannel_1;
    long collisionChannel_2;
    long collisionChannel_3;
    // Counter to number of received packets.
    long receivedChannel_1;
    long receivedChannel_2;
    long receivedChannel_3;

    // Messages declaration without ACK
    cMessage *endReceivedMessageNoAck1;
    cMessage *endReceivedMessageNoAck2;
    cMessage *endReceivedMessageNoAck3;
    // Messages declaration with ACK
    cMessage *endReceivedMessageAck1;
    cMessage *endReceivedMessageAck2;
    cMessage *endReceivedMessageAck3;

    // Radio delay value.
    simtime_t radioDelay;
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
    simtime_t endReceptionTime;
    simtime_t recvStartTime;
    //Statistics
    enum {
        IDLE = 0, TRANSMISSION = 1, COLLISION = 2
    } channelState;

    simsignal_t channelStateSignal;
    simsignal_t collisionCH1;
    simsignal_t collisionCH2;
    simsignal_t collisionCH3;

    simsignal_t receivedCH1;
    simsignal_t receivedCH2;
    simsignal_t receivedCH3;

public:

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void refreshDisplay() const override;
    //Collisions check
    bool frequencyCollision(LoraPck* pkt1, LoraPck* pkt2);
    bool sfCollision(LoraPck* pkt1, LoraPck* pkt2);
    bool powerCollision(LoraPck* pkt1, LoraPck* pkt2);
    bool timingCollision(LoraPck* pkt1, LoraPck* pkt2);
    void checkRelation();
    void manageMessage(long numCollisions, long numSent, bool busy);
    bool calculateDistance(int a, int b, int c, int d);
};

}
;
//namespace

#endif

