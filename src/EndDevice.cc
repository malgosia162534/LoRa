#include "EndDevice.h"

namespace lora {

Define_Module(EndDevice);

/*EndDevice::EndDevice() {
 //endTxEvent = nullptr;
 }

 EndDevice::~EndDevice() {
 //cancelAndDelete(endTxEvent);
 }*/

void EndDevice::initialize() {
    //Setting on parameters
    //For ToA calculation
    NP = 8;
    SW = 8;
    CRC = 1; // uplink transmission
    IH = 0; //uplink transmission
    // CRC = 0; // downlink transmission
    // IH = 0; // downlink transmission
    DE = 2; // optimization algorithm enabled
    // DE = 0; // optimiztion algorithm disabled
    CR = 3 / 4;    // value for coding rate

    // Declaring the gateway module
    gateway = getModuleByPath("gateway");
    if (!gateway)
        throw cRuntimeError("Gateway module not found");

    // Declaring the endDevice module
    /*endDevice = getModuleByPath("endDevice");
    if (!endDevice)
        throw cRuntimeError("EndDevice module not found");
*/

    // Parameters taken from GATEWAY module
    SF = gateway->par("spreadingFactor");
    BW = gateway->par("bandwidth");

    // Channel random selection.
    int value = intrand(3);
    if (value == 1) {
        channelNumber = 868.10;
    } else if (value == 2) {
        channelNumber = 868.30;
    } else if (value == 3) {
        channelNumber = 868.50;
    } else {
        EV << "\nIncorrect channel number\n";
    }

    numberOfSentMessages = 0;
    stateED = IDLE;
    // Parameters taken from ENDDEVICE module
    PL=(&par("PHYpayload"))->longValue();

    //iaTime is a value how often packets will be generated
    iaTime = &par("iaTime");

    //Date rate value depending on SF and BW (in bps)
    // if (BW == 125000) { // 125kHz
    if (SF == 12)
        DR = 250;
    else if (SF == 11)
        DR = 440;
    else if (SF == 10)
        DR = 980;
    else if (SF == 9)
        DR = 1760;
    else if (SF == 8)
        DR = 3125;
    else if (SF == 7)
        DR = 5470;
    else {
        EV << "Inappropriate Spreading Factor value" << endl;
    }

    payloadData = new cMessage("Payload");
    gotACK = false;

    scheduleAt(getNextTransmissionTime(), payloadData);

}

void EndDevice::handleMessage(cMessage *msg) {
    if (msg == payloadData) {
        ToA = timeOnAir();

        // End Device is IDLE and can send message to Gateway
        if (stateED == IDLE) {
            // When End Device is in IDLE mode, we know that ifrom previous message it received ACK.
            gotACK = false;
            bubble("IDLE");
            char pkname[40];
            sprintf(pkname, "pk-%d-#%d", getId(), numberOfSentMessages);
            EV << "generating packet " << pkname << endl;
            EV << "Spreading factor:  " << SF << " \t payload " << PL << endl;
            EV << "Time of arrival for initial values  is " << ToA << endl;
            simtime_t gettime = simTime() + ToA;
            EV << "Gettime value: " << gettime << endl;

            simtime_t radioDelay = 0.0;

            stateED = BUSY;
            cPacket *pkt = new cPacket(pkname);

            // Adding parameters to message. Allows to read parameters in module which receive the message.
            pkt->addPar("timeOnAirValue");
            pkt->par("timeOnAirValue").setDoubleValue(ToA);

            pkt->addPar("channelNumber");
            pkt->par("channelNumber").setDoubleValue(868.1);
            //pkt->par("channelNumber").setDoubleValue(channelNumber);
            //Sending packet directly to Gateway input
            sendDirect(pkt, radioDelay, ToA, gateway->gate("in"));
            scheduleAt(gettime, payloadData);
            bubble("TRANSMITTING!");
            numberOfSentMessages++;

        }
        // End Device is BUSY - End Device is waiting for ACK from gateway
        else if (stateED == BUSY) {

            bubble("BUSY");
            //End Device cannot handle message. It is still waiting for ACK from Gateway.
            scheduleAt(getNextTransmissionTime(), payloadData);

        } else {
            throw cRuntimeError("Invalid End Device state");
        }
    } else // Received message is a ACK message from Gateway.
    {
        bubble("ACK");
        EV <<"ACK message from gateway\n";
    }
}

simtime_t EndDevice::getNextTransmissionTime() {
    simtime_t t = simTime() + iaTime->doubleValue();
    EV << "Transmission time -> " << t << endl;
    return t;
}

/*void EndDevice::refreshDisplay() const {
 getDisplayString().setTagArg("t", 2, "#808000");
 if (state == IDLE) {
 getDisplayString().setTagArg("i", 1, "");
 getDisplayString().setTagArg("t", 0, "");
 } else if (state == TRANSMIT) {
 getDisplayString().setTagArg("i", 1, "yellow");
 getDisplayString().setTagArg("t", 0, "TRANSMIT");
 }
 }*/
double EndDevice::timeOnAir() {
    double part1 = NP + 4.25;
    double temp1 = ceil(
            ((8 * PL - 4 * SF - 28 + 16 * CRC - 20 * IH) / (4 * (SF - 2 * DE)))
                    * (CR + 4));

    double temp2 = fmax(temp1, 0) + SW;
    ToA = (pow(2, SF) / BW) * (part1 + temp2);

    return ToA;

}
double EndDevice::tOffset() {
    double dutyCycle = 0.01;
    double timeOffset = (ToA / dutyCycle) - ToA;
    return timeOffset;
}
void EndDevice::refreshDisplay() const {
    char buffer[40];
    sprintf(buffer, "Sent: %ld", numberOfSentMessages);
    getDisplayString().setTagArg("t", 0, buffer);
}
}
;
//namespace
