#ifndef __LORA_GATEWAY_H_
#define __LORA_GATEWAY_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace lora {

class Gateway : public cSimpleModule
{
  private:
    // state variables, event pointers
    bool channelBusy;
    cMessage *endRxEvent;

    long currentCollisionNumFrames;
    long receiveCounter;
    simtime_t recvStartTime;
    enum { IDLE = 0, TRANSMISSION = 1, COLLISION = 2 };
    simsignal_t channelStateSignal;

    // statistics
    simsignal_t receiveBeginSignal;
    simsignal_t receiveSignal;
    simsignal_t collisionLengthSignal;
    simsignal_t collisionSignal;

  public:
    Gateway();
    virtual ~Gateway();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void refreshDisplay() const override;
};

}; //namespace

#endif

