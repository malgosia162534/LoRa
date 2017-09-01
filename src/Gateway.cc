#include "Gateway.h"

namespace lora {

Define_Module(Gateway);

/*Gateway::Gateway() {
 endRxEvent = nullptr;
 }

 Gateway::~Gateway() {
 cancelAndDelete(endRxEvent);
 }*/

void Gateway::initialize() {
    endReceivedMessageAck1 = new cMessage("end-receiving-channel-1");
    endReceivedMessageAck2 = new cMessage("end-receiving-channel-2");
    endReceivedMessageAck3 = new cMessage("end-receiving-channel-3");

    endReceivedMessageNoAck1 = new cMessage("end-receiving-channel-1");
    endReceivedMessageNoAck2 = new cMessage("end-receiving-channel-2");
    endReceivedMessageNoAck3 = new cMessage("end-receiving-channel-3");

    // Assign collision number on each channel to zero.
    collisionChannel_1 = 0;
    collisionChannel_2 = 0;
    collisionChannel_3 = 0;
    // Assign number of received packets to zero.
    receivedChannel_1 = 0;
    receivedChannel_2 = 0;
    receivedChannel_3 = 0;

    // In the beginning all three channels are available.
    channelBusy_1 = false;
    channelBusy_2 = false;
    channelBusy_3 = false;

    //Gateway position
    posX = (&par("posX"))->longValue();
    posY = (&par("posY"))->longValue();

    // Normally the packet object gets delivered to the destination module at the simulation time
    // that corresponds to finishing the reception of the message (ie. the arrival of its last bit).
    // It can be changed with setDeliverOnReceptionStart() method.
    gate("in")->setDeliverOnReceptionStart(true);

    receive_delay1 = getParentModule()->par("receive_delay1");
    receive_delay2 = getParentModule()->par("receive_delay2");

    radioDelay = 0.0;

    ackTime = 0.0;

    //Statistics
    channelStateSignal = registerSignal("channelState");
    emit(channelStateSignal, IDLE);

    collisionCH1 = registerSignal("collisionCH1");
    collisionCH2 = registerSignal("collisionCH2");
    collisionCH3 = registerSignal("collisionCH3");

    emit(collisionCH1, 0);
    emit(collisionCH2, 0);
    emit(collisionCH3, 0);

    receivedCH1 = registerSignal("receivedCH1");
    receivedCH2 = registerSignal("receivedCH2");
    receivedCH3 = registerSignal("receivedCH3");

    emit(receivedCH1, 0);
    emit(receivedCH2, 0);
    emit(receivedCH3, 0);

}

void Gateway::handleMessage(cMessage *msg) {
    // Randomly gateway will decide in which receive window will send confirmation.
    double temp = rand() / (RAND_MAX + 1.);
    // Gateway will answer in RX1
    if (temp > 0 and temp <= 0.5) {
        ackTime = 0.002;    //receive_delay1;
    }
    // Gateway will answer in RX2
    else {
        ackTime = 0.002;    //receive_delay2;
    }

    if (msg == endReceivedMessageAck1) {
        EV << "reception finished, sending ACK\n";
        channelBusy_1 = false;
        emit(channelStateSignal, IDLE);

        //Sending ACK message to ED
        char pkname[40] = "ack";
        LoraPck *pkt = new LoraPck(pkname);
        pkt->setFrequency(1);
        if (hasGUI()) {
            getDisplayString().setTagArg("i2", 0, "old/x_blank");
            getDisplayString().setTagArg("t", 0, "RECEIVED");
            getDisplayString().setTagArg("t", 2, "#808000");
        }
        sendDirect(pkt, radioDelay, ackTime, endDevice->gate("in"));
    } else if (msg == endReceivedMessageAck2) {
        EV << "reception finished, sending ACK\n";
        channelBusy_2 = false;
        emit(channelStateSignal, IDLE);

        //Sending ACK message to ED
        char pkname[40] = "ack";
        LoraPck *pkt = new LoraPck(pkname);
        pkt->setFrequency(2);
        if (hasGUI()) {
            getDisplayString().setTagArg("i2", 0, "old/x_blank");
            getDisplayString().setTagArg("t", 0, "RECEIVED");
            getDisplayString().setTagArg("t", 2, "#808000");
        }
        sendDirect(pkt, radioDelay, ackTime, endDevice->gate("in"));
    } else if (msg == endReceivedMessageAck3) {
        EV << "reception finished, sending ACK\n";
        channelBusy_3 = false;
        emit(channelStateSignal, IDLE);

        //Sending ACK message to ED
        char pkname[40] = "ack";
        LoraPck *pkt = new LoraPck(pkname);
        pkt->setFrequency(3);
        if (hasGUI()) {
            getDisplayString().setTagArg("i2", 0, "old/x_blank");
            getDisplayString().setTagArg("t", 0, "RECEIVED");
            getDisplayString().setTagArg("t", 2, "#808000");
        }
        sendDirect(pkt, radioDelay, ackTime, endDevice->gate("in"));
    } else if (msg == endReceivedMessageNoAck1) {
        EV << "reception finished\n";
        channelBusy_1 = false;
        emit(channelStateSignal, IDLE);
    } else if (msg == endReceivedMessageNoAck2) {
        EV << "reception finished\n";
        channelBusy_2 = false;
        emit(channelStateSignal, IDLE);
    } else if (msg == endReceivedMessageNoAck3) {
        EV << "reception finished\n";
        channelBusy_3 = false;
        emit(channelStateSignal, IDLE);
    } else {
        LoraPck *pkt = check_and_cast<LoraPck *>(msg);
        endDevice = pkt->getSenderModule();
        int channelNumber = pkt->getFrequency();
        ASSERT(pkt->isReceptionStart());
        simtime_t endReceptionTime = simTime() + pkt->getDuration();
        if (channelNumber == 1) {
            receivedChannel_1++;
            emit(receivedCH1, receivedChannel_1);
            if (!channelBusy_1) {
                EV << "started receiving\n";
                emit(channelStateSignal, TRANSMISSION);
                if (hasGUI()) {
                    getDisplayString().setTagArg("i2", 0, "old/x_yellow");
                    getDisplayString().setTagArg("t", 0, "RECEIVING");
                    getDisplayString().setTagArg("t", 2, "#808000");
                }
                recvStartTime = simTime();
                channelBusy_1 = true;

                if (pkt->getRequireACK()) {
                    scheduleAt(endReceptionTime, endReceivedMessageAck1);
                } else {
                    scheduleAt(endReceptionTime, endReceivedMessageNoAck1);
                }

            } else {
                EV
                          << "another frame arrived while receiving -- collision! from: "
                          << endDevice->getFullName() << endl;
                bubble("collision");
                collisionChannel_1++;
                emit(channelStateSignal, COLLISION);
                emit(collisionCH1, collisionChannel_1);
                /*if (endReceptionTime > endReceivedMessageAck1->getArrivalTime()) {
                 cancelEvent (endReceivedMessageAck1);
                 //scheduleAt(endReceptionTime, endReceivedMessage1);
                 }*/

                // update network graphics
            }
            channelBusy_1 = true;
        } else if (channelNumber == 2) {
            receivedChannel_2++;
            emit(receivedCH2, receivedChannel_2);
            if (!channelBusy_2) {
                EV << "started receiving\n";
                emit(channelStateSignal, TRANSMISSION);
                if (hasGUI()) {
                    getDisplayString().setTagArg("i2", 0, "old/x_yellow");
                    getDisplayString().setTagArg("t", 0, "RECEIVING");
                    getDisplayString().setTagArg("t", 2, "#808000");
                }
                recvStartTime = simTime();
                channelBusy_2 = true;

                if (pkt->getRequireACK()) {
                    scheduleAt(endReceptionTime, endReceivedMessageAck2);
                } else {
                    scheduleAt(endReceptionTime, endReceivedMessageNoAck2);
                }
            } else {
                EV
                          << "another frame arrived while receiving -- collision! from: "
                          << endDevice->getFullName() << endl;
                bubble("collision");
                collisionChannel_2++;
                emit(channelStateSignal, COLLISION);
                emit(collisionCH2, collisionChannel_2);
                /* if (endReceptionTime > endReceivedMessageAck2->getArrivalTime()) {
                 cancelEvent (endReceivedMessageAck2);
                 //scheduleAt(endReceptionTime, endReceivedMessage2);
                 }*/

                // update network graphics
            }
            channelBusy_2 = true;
        } else if (channelNumber == 3) {
            receivedChannel_3++;
            emit(receivedCH3, receivedChannel_3);
            if (!channelBusy_3) {
                EV << "started receiving\n";
                emit(channelStateSignal, TRANSMISSION);
                if (hasGUI()) {
                    getDisplayString().setTagArg("i2", 0, "old/x_yellow");
                    getDisplayString().setTagArg("t", 0, "RECEIVING");
                    getDisplayString().setTagArg("t", 2, "#808000");
                }
                recvStartTime = simTime();
                channelBusy_3 = true;

                if (pkt->getRequireACK()) {
                    scheduleAt(endReceptionTime, endReceivedMessageAck3);
                } else {
                    scheduleAt(endReceptionTime, endReceivedMessageNoAck3);
                }
            } else {
                EV
                          << "another frame arrived while receiving -- collision! from: "
                          << endDevice->getFullName() << endl;
                bubble("collision");
                collisionChannel_3++;
                emit(channelStateSignal, COLLISION);
                emit(collisionCH3, collisionChannel_3);
                /*  if (endReceptionTime > endReceivedMessageAck3->getArrivalTime()) {
                 cancelEvent (endReceivedMessageAck3);
                 // scheduleAt(endReceptionTime, endReceivedMessage3);
                 }*/

                // update network graphics
            }
            channelBusy_3 = true;
        } else {
            EV << "Wrong channel" << endl;
        }
        delete pkt;
    }
}
/*frequencyCollision, conditions
 |f1-f2| <= 120 kHz if f1 or f2 has bw 500
 |f1-f2| <= 60 kHz if f1 or f2 has bw 250
 |f1-f2| <= 30 kHz if f1 or f2 has bw 125*/
bool Gateway::frequencyCollision(LoraPck* pkt1, LoraPck* pkt2) {
    if (abs(pkt1->getFrequency() - pkt2->getFrequency()) <= 120
            and (pkt1->getBandwidth() == 500 or pkt2->getBandwidth() == 500))
        return true;
    else if (abs(pkt1->getFrequency() - pkt2->getFrequency()) <= 60
            and (pkt1->getBandwidth() == 250 or pkt2->getBandwidth() == 250))
        return true;
    else if (abs(pkt1->getFrequency() - pkt2->getFrequency()) <= 30)
        return true;
    return false;

}
bool Gateway::sfCollision(LoraPck* pkt1, LoraPck* pkt2) {
    if (pkt1->getSpreadingFactor() == pkt2->getSpreadingFactor())
        return true;
    return false;
}
bool Gateway::powerCollision(LoraPck* pkt1, LoraPck* pkt2) {
    int powerThreshold = 6;
    if (abs(pkt1->getRssi() - pkt2->getRssi()) < powerThreshold)
        return true;
    return false;

}
/*bool Gateway::timingCollision(LoraPck* pkt1, LoraPck* pkt2) {
 int Npream = 8;
 // we can lose at most (Npream - 5) * Tsym of our preamble
 double Tpream = pow(2, pkt1->getSpreadingFactor())
 / (1.0 * pkt1->getBandwidth()) * (Npream - 5);
 double pkt2_end = pkt2->getToA() + pkt2->getRectime();
 double pkt1_cs = simTime() + Tpream;
 if (pkt1_cs < pkt2_end)
 return true;
 return false;
 }
 */
void Gateway::refreshDisplay() const {
    char buf[80];
    sprintf(buf,
            "rcvd1: %ld  collision1: %ld \nrcvd2: %ld  collision2: %ld \nrcvd3: %ld  collision3: %ld ",
            receivedChannel_1, collisionChannel_1, receivedChannel_2,
            collisionChannel_2, receivedChannel_3, collisionChannel_3);
    getDisplayString().setTagArg("t", 0, buf);
}

void Gateway::finish() {
    EV << "\nDuration: " << simTime() << endl;
    recordScalar("duration", simTime());
    recordScalar("collisionsCH1", collisionChannel_1);
    recordScalar("collisionsCH2", collisionChannel_2);
    recordScalar("collisionsCH3", collisionChannel_3);
}

}
;
//class end
//namespace
