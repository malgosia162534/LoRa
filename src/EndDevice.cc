#include "EndDevice.h"

namespace lora {

Define_Module(EndDevice);

void EndDevice::initialize() {
    channel1.setFrequency(1);
    channel2.setFrequency(2);
    channel3.setFrequency(3);
    //Log-shadow parameters
    txPow = this->getParentModule()->par("txPow"); // transmission power
    GL = this->getParentModule()->par("GL");
    gamma = this->getParentModule()->par("gamma");
    d0 = this->getParentModule()->par("d0");
    LpLd0 = this->getParentModule()->par("LpLd0");
    minsensitivity = this->getParentModule()->par("minsensitivity");

    //Simulation parameter from ini file
    SF = this->getParentModule()->par("spreadingFactor");
    CR = this->getParentModule()->par("CR");
    BW = this->getParentModule()->par("bandwidth");
    packetSize = (&par("packetSize"))->longValue();

    numGateways = this->getParentModule()->par("numGateways");
    // Declaring the gateway module
    EV << "gateways " << numGateways << endl;
    for (int i = 0; i < numGateways; i++) {
        char gatewayName[15];
        sprintf(gatewayName, "gateway[%d]", i);
        gateway = getModuleByPath(gatewayName);
    }

    numberOfSentMessages = 0;
    numberOfLostMessages = 0;

    stateCH1 = IDLE;
    stateCH2 = IDLE;
    stateCH3 = IDLE;

    //iaTime is a value how often packets will be generated
    iaTime = &par("iaTime");

    ToA = airtime(SF, CR, packetSize, BW);

    payloadData = new cMessage("payloadData");
    sentMessageNoAck = new cMessage("sentMessageNoAck");
    sentMessageAck = new cMessage("sentMessageAck");

    gotACK = false;
    gotACKCH1 = false;
    gotACKCH2 = false;
    gotACKCH3 = false;

    firstMsg = true; //It is a first message.
    lastPacketCH1 = 0.0;
    lastPacketCH2 = 0.0;
    lastPacketCH3 = 0.0;

    //EndDevice position
    posX = (&par("posX"))->longValue();
    posY = (&par("posY"))->longValue();
    EV << "posX: " << posX << " posY: " << posY << endl;
    scheduleAt(simTime() + iaTime->doubleValue(), payloadData);
    if (hasGUI()) {
        getDisplayString().setTagArg("i2", 0, "status/green");
    }

    //Statistics setup
    CH1state = registerSignal("state");
    emit(CH1state, stateCH1);
    usedChannel = registerSignal("usedChannel");
    emit(usedChannel, 0);

    Sentmessages = registerSignal("Sentmessages");
    emit(Sentmessages, numberOfSentMessages);
    Lostmessages = registerSignal("Lostmessages");
    emit(Lostmessages, numberOfLostMessages);
}

void EndDevice::handleMessage(cMessage *msg) {

    if (msg == payloadData) {
        frequency = chooseChannel();
        EV << "Frequency ->"<<frequency<<endl;
        if (frequency == 0) // There is no free channel. We have to reschedule message.
                {
            cancelEvent(payloadData);
            EV << "Reschedule payload\n";
            scheduleAt(simTime() + dutyCycle(), payloadData);
        } else if (frequency == 1) {
            //We will transmit on channel_1
            manageMessage(channel1);
        } else if (frequency == 2) {
            //We will transmit on channel_2
            manageMessage(channel2);

        } else if (frequency == 3) {
            //We will transmit on channel_3
            manageMessage(channel3);

        } else {
            EV << "Incorrect channel\n";
        }

    } else { //ACK message from GW
        LoraPck *pkt = check_and_cast<LoraPck *>(msg);
        int channelNumber = pkt->getFrequency();
        if (channelNumber == 1) {
            channel1.setChannelState(0);
        } else if (channelNumber == 2) {
            channel2.setChannelState(0);
        } else if (channelNumber == 3) {
            channel3.setChannelState(0);
        } else {
            EV << "Incorrect channel ";
        }

    }

}
void EndDevice::manageMessage(loraChannel channel) {

    // When End Device is in IDLE mode, we know that from previous message it received ACK.
    gotACK = false;
    channel.setChannelState(1);
    emit(CH1state, channel.getChannelState());
    emit(usedChannel, channel.getFrequency());
    LoraPck * pkt = setPacket();

    //Sending packet directly to Gateway input
    simtime_t gettime = simTime() + ToA;

    simtime_t radioDelay = 0.0;
    //bool setDistance = false;
    int temp = 0;
    long actSent = channel.getSentMessages();
    long actLost = channel.getLostMessages();
    channel.setSentMessages(++actSent);
    for (int i = 0; i < numGateways; i++) {
        char gatewayName[15];
        sprintf(gatewayName, "gateway[%d]", i);
        gateway = getModuleByPath(gatewayName);
        double distance = distanceToGW(gateway->par("posX").longValue(),
                gateway->par("posY").longValue());
        if (checkRSSI(pkt, distance) == true) //packet will be lost
                {
            temp++;
            EV << "temp: " << temp << endl;
        } else {

            LoraPck* copy = pkt->dup();
            sendDirect(copy, radioDelay, ToA, gateway->gate("in"));
        }
        if (temp == numGateways) {
            EV << "Lost number" << numberOfLostMessages << endl;
            channel.setLostMessages(++actLost);
        }
    }
    if (pkt->getRequireACK() == false) {
        //This packet does not require ACK
        channel.setChannelState(0);
    } else {
        //This packet does not require ACK
        channel.setChannelState(2); //Channel state BUSY - waiting for ACK
    }
    delete pkt;
    if (hasGUI()) {
        getDisplayString().setTagArg("i2", 0, "status/yellow");
    }
    scheduleAt(getNextTransmissionTime(), payloadData);

    /* emit(Sentmessages, numberOfSentMessages);
     emit(Lostmessages, numberOfLostMessages);*/

}
int EndDevice::chooseChannel() {
    int channel = 0;
    if (firstMsg == true) {
        channel1.setLastPacket(simTime() + dutyCycle());
        firstMsg = false;
        channel = 1;
    } else {
        double min;
        if (channel1.getAvailability() == 0 && channel2.getAvailability() == 0
                && channel3.getAvailability() == 0) //All channels are IDLE
                        {
            min = fmin(fmin(channel1.getLastPacket(), channel2.getLastPacket()),
                    channel3.getLastPacket());
            if (min == channel1.getLastPacket()) {
                channel1.setLastPacket(simTime() + dutyCycle());
                channel = 1;
            } else if (min == channel2.getLastPacket()) {
                channel2.setLastPacket(simTime() + dutyCycle());
                channel = 2;
            } else {
                channel3.setLastPacket(simTime() + dutyCycle());
                channel = 3;
            }

        } else if (channel1.getAvailability() == 0
                && channel2.getAvailability() == 0
                && channel3.getAvailability() != 0) { // CH1 and CH2 free
            min = fmin(channel1.getLastPacket(), channel2.getLastPacket());

            if (min == channel1.getLastPacket()) {
                channel1.setLastPacket(simTime() + dutyCycle());
                channel = 1;
            } else {
                channel2.setLastPacket(simTime() + dutyCycle());
                channel = 2;
            }
        } else if (channel1.getAvailability() == 0
                && channel3.getAvailability() == 0
                && channel2.getAvailability() == 0) { //CH1 and CH3 free
            min = fmin(channel1.getLastPacket(), channel3.getLastPacket());

            if (min == channel1.getLastPacket()) {
                channel1.setLastPacket(simTime() + dutyCycle());
                channel = 1;
            } else {
                channel3.setLastPacket(simTime() + dutyCycle());
                channel = 3;
            }
        } else if (channel3.getAvailability() == 0
                && channel2.getAvailability() == 0
                && channel1.getAvailability() == 0) { //CH2 and CH3 free
            min = fmin(channel3.getLastPacket(), channel2.getLastPacket());

            if (min == channel3.getLastPacket()) {
                channel3.setLastPacket(simTime() + dutyCycle());
                channel = 3;
            } else {
                channel2.setLastPacket(simTime() + dutyCycle());
                channel = 2;
            }
        } else if (channel1.getAvailability() != 0
                && channel2.getAvailability() != 0
                && channel3.getAvailability() == 0) { //CH3 is free
            channel3.setLastPacket(simTime() + dutyCycle());
            channel = 3;

        } else if (channel1.getAvailability() != 0
                && channel2.getAvailability() == 0
                && channel3.getAvailability() != 0) { //CH2 is free
            channel2.setLastPacket(simTime() + dutyCycle());
            channel = 2;

        } else if (channel1.getAvailability() == 0
                && channel2.getAvailability() != 0
                && channel3.getAvailability() != 0) { //CH1 is free
            channel1.setLastPacket(simTime() + dutyCycle());
            channel = 1;

        } else // All busy
        {
            EV << "All channels busy" << endl;
            channel = 0;

        }

    }
    return channel;
}

LoraPck* EndDevice::setPacket() {

    char pkname[40];
    sprintf(pkname, "pk-%d-#%d", getId(), numberOfSentMessages);
//Creating LoRa Packet
    LoraPck *pkt = new LoraPck(pkname);
//Setting all needed parameters
    pkt->setNodeId(1); // !!for now
    pkt->setPacketLength(packetSize);
    pkt->setToA(airtime(SF, CR, packetSize, BW));
    pkt->setSpreadingFactor(SF);
    pkt->setBandwidth(BW);
    pkt->setGatewayID(8); // !!for now
//Determine if this packet needs ACK
    double temp = rand() / (RAND_MAX + 1.);
    if (temp > 0 and temp <= 0.5) {
        //Packet require ACK
        pkt->setRequireACK(true);
    }
    else {
        pkt->setRequireACK(true);
    }
//pkt->setRectime(simTime() + timeOnAir());
// pkt->setArrivalTime(simTime() + timeOnAir());

    pkt->setFrequency(frequency);

    return pkt;

}
bool EndDevice::checkRSSI(LoraPck* pkt, double distance) {
    pkt->setDistance(distance);
//Log-shadow
    Lp1 = LpLd0 + 10 * gamma * log10(pkt->getDistance() / d0);
    Prx = txPow + GL - Lp1;
    pkt->setRssi(Prx);
    EV << "Distance: " << pkt->getDistance() << ";  packet rssi: " << Prx
              << endl; //pkt->getRssi();
// Verify whether packet won't be lost
    if (pkt->getRssi() < minsensitivity) {
        pkt->setLost(true);
        EV << "Packet lost: " << pkt->getLost() << endl;
        return true;
    } else {
        pkt->setLost(false);
        EV << "Packet not lost: " << pkt->getLost() << endl;
        return false;
    }

}
double EndDevice::airtime(int sf, int cr, int pl, int bw) {
    IH = 0; //implicit header disable (H=0) or not (H=1)
    DE = 0; //low data rate optimization enabled (=1) or not (=0)
    NPream = 8;
    if (bw == 125 and (sf == 11 or sf == 12))
        // low data rate optimization mandated for BW125 with SF11 and SF12
        DE = 1;
    if (sf == 6)
        //can only have implicit header with SF6
        IH = 1;
    double Tsym = pow(2.0, sf) / bw; //value in seconds
    double Tpream = (NPream + 4.25) * Tsym;
    double payloadSymbNB = 8
            + fmax(
                    ceil(
                            (8.0 * pl - 4.0 * sf + 28 + 16 - 20 * IH)
                                    / (4.0 * (sf - 2.0 * DE))) * (CR + 4), 0);
    double Tpayload = payloadSymbNB * Tsym;
    EV << "Tsym " << Tsym << " Tpream->" << Tpream << " Tpayload " << Tpayload
              << " airtime in ms: " << 1000 * (Tpream + Tpayload) << endl;
    return (Tpream + Tpayload) * 0.001; // ToA in seconds
}
double EndDevice::distanceToGW(int posAx, int posAy) {

    double x, y;
    x = abs(this->posX - posAx);
    y = abs(this->posY - posAy);
    double hypotenuse = sqrt(x * x + y * y);
    return hypotenuse;

}

simtime_t EndDevice::getNextTransmissionTime() {
    simtime_t t = simTime() + iaTime->doubleValue() + dutyCycle();
    EV << "Transmission time -> " << t << endl;
    return t;
}

double EndDevice::dutyCycle() {
    double dutyCycle = 0.01;
    double timeOffset = (ToA / dutyCycle) - ToA;
    EV << "Offset: " << timeOffset << endl;
    return timeOffset;
}

void EndDevice::refreshDisplay() const {
    /* char buf[40];
     sprintf(buf, "state: %ld ", stateCH1);
     getDisplayString().setTagArg("t", 0, buf);*/
}
void EndDevice::finish() {
    EV << "\nDuration: " << simTime() << endl;

}

}
;
//namespace
