#include "EndDevice.h"

namespace lora {

Define_Module(EndDevice);

EndDevice::EndDevice() {
    endTxEvent = nullptr;
}

EndDevice::~EndDevice() {
    cancelAndDelete(endTxEvent);
}

void EndDevice::initialize() {
    //stateSignal = registerSignal("state");
    gateway = getModuleByPath("gateway");
    if (!gateway)
        throw cRuntimeError("gateway not found");

    //txRate = par("txRate");
    //radioDelay = par("radioDelay");
    iaTime = &par("iaTime");
    packetLength = &par("packetLengthBits");

    //slotTime = par("slotTime");
    //isSlotted = slotTime > 0;
    //WATCH(slotTime);
    //WATCH(isSlotted);

    endTxEvent = new cMessage("send/endTx");
    state = IDLE;
   // emit(stateSignal, state);
    pkCounter = 0;
    WATCH((int& )state);
    WATCH(pkCounter);

    scheduleAt(getNextTransmissionTime(), endTxEvent);
}

void EndDevice::handleMessage(cMessage *msg) {
    ASSERT(msg == endTxEvent);

    if (state == IDLE) {
        // generate packet and schedule timer when it ends
        char pkname[40];
        sprintf(pkname, "pk-%d-#%d", getId(), pkCounter++);
        EV << "generating packet " << pkname << endl;

        state = TRANSMIT;
      //  emit(stateSignal, state);

        cPacket *pk = new cPacket(pkname);
        pk->setBitLength(packetLength->longValue());
        simtime_t duration = pk->getBitLength()/2.0;
        sendDirect(pk, radioDelay, duration, gateway->gate("in"));

        scheduleAt(simTime() + duration, endTxEvent);
    } else if (state == TRANSMIT) {
        // endTxEvent indicates end of transmission
        state = IDLE;
      //  emit(stateSignal, state);

        // schedule next sending
        scheduleAt(getNextTransmissionTime(), endTxEvent);
    } else {
        throw cRuntimeError("invalid state");
    }
}

simtime_t EndDevice::getNextTransmissionTime() {
    simtime_t t = simTime() + iaTime->doubleValue();

    if (!isSlotted)
        return t;
    else
        // align time of next transmission to a slot boundary
        return slotTime * ceil(t / slotTime);
}

void EndDevice::refreshDisplay() const {
    getDisplayString().setTagArg("t", 2, "#808000");
    if (state == IDLE) {
        getDisplayString().setTagArg("i", 1, "");
        getDisplayString().setTagArg("t", 0, "");
    } else if (state == TRANSMIT) {
        getDisplayString().setTagArg("i", 1, "yellow");
        getDisplayString().setTagArg("t", 0, "TRANSMIT");
    }
}
double EndDevice::timeOnAir(){

}
}
;
//namespace
