#ifndef __LORA_END_DEVICE_H_
#define __LORA_END_DEVICE_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace lora {

class EndDevice : public cSimpleModule
{
  private:
    // parameters
    simtime_t radioDelay;
    double txRate;
    cPar *iaTime;
    cPar *pkLenBits;
    simtime_t slotTime;
    bool isSlotted;

    // state variables, event pointers etc
    cModule *gateway;
    cMessage *endTxEvent;
    enum { IDLE = 0, TRANSMIT = 1 } state;
    simsignal_t stateSignal;
    int pkCounter;

  public:
    EndDevice();
    virtual ~EndDevice();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;
    simtime_t getNextTransmissionTime();
};

}; //namespace

#endif

