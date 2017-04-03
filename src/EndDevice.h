#ifndef __LORA_END_DEVICE_H_
#define __LORA_END_DEVICE_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace lora {

class EndDevice: public cSimpleModule {
private:
    // parameters
    simtime_t radioDelay;
    double txRate;
    cPar *iaTime;
    cPar *packetLength;
    simtime_t slotTime;
    bool isSlotted;

    // state variables, event pointers etc
    cModule *gateway;
    cMessage *endTxEvent;
    enum {
        IDLE = 0, TRANSMIT = 1
    } state;
    simsignal_t stateSignal;
    int pkCounter;

    //ToA
    int NP,SW,PL,CRC,IH,DE;
    double CR;

public:
    EndDevice();
    virtual ~EndDevice();
    double timeOnAir();

protected:
    // Initialize is called at the beginning of the simulation.
    // To bootstrap the process, one of the modules needs
    // to send the first message. In our case it will be EndDevice.
    virtual void initialize() override;

    // The handleMessage() method is called whenever a message arrives
    // at the module.
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;
    simtime_t getNextTransmissionTime();
};

}
;
//namespace

#endif

