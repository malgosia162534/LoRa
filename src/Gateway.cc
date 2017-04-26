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
    endReceivedMessage = new cMessage("end-receiving");
    ack1=new cMessage("ack1");

    // Assign collision number on each channel to zero.
    collisionChannel868_1 = 0;
    collisionChannel868_3 = 0;
    collisionChannel868_5 = 0;

    // In the beginning all three channels are available.
    channelBusy868_1 = false;
    channelBusy868_3 = false;
    channelBusy868_5 = false;

    // Normally the packet object gets delivered to the destination module at the simulation time
    // that corresponds to finishing the reception of the message (ie. the arrival of its last bit).
    // It can be changed with setDeliverOnReceptionStart() method.
    //gate("in")->setDeliverOnReceptionStart(true);

    receive_delay1 = getParentModule()->par("receive_delay1");
    receive_delay2 = getParentModule()->par("receive_delay2");

    // Declaring the gateway module
    gateway = getModuleByPath("gateway");
    if (!gateway)
        throw cRuntimeError("Gateway module not found");

    // Declaring the endDevice module
   /* endDevice = getModuleByPath("endDevice");
    if (!endDevice)
        throw cRuntimeError("EndDevice module not found");
*/
    ackTime = 0.0;

}

void Gateway::handleMessage(cMessage *msg) {
    if (msg == endReceivedMessage) {
        EV << "Here we will send ACK"<<endl;
    } else {
        bubble("ARRIV");

        cPacket *pkt = check_and_cast<cPacket *>(msg);

        //Gettin parameters from received message.
        //Used channel.
        channelNumber = msg->par("channelNumber").doubleValue();
        EV << "channel Number in Gateway -> " << channelNumber << endl;
        // TimeOnAir value for specific message.
        simtime_t duration = msg->par("timeOnAirValue").doubleValue();
        receiveEndTime = simTime() + duration;
        EV << "Gateway received the message\t at time" << receiveEndTime
                  << endl;

        // Randomly gateway will decide in which receive window will send confirmation.
        int temp = rand() / (RAND_MAX + 1.);
        // Gateway will answer in RX1
        if (temp > 0 and temp <= 0.5) {
            ackTime = receive_delay1;
        }
        // Gateway will answer in RX2
        else {
            ackTime = receive_delay2;
        }

        //Based on the channel in which End Device send a message we will decide about collisions and Rx windows.

        //End device uses channel 868_1 to transmit message.
        if (channelNumber == 868.1) {

            if (channelBusy868_1 == false) // Channel 868_1 is not busy we can receive the packet
                    {
                EV << "No collision\n";
                // We can set up RX windows.
//            receiveEndTime = simTime()+duration;
                channelBusy868_1 = true;
                scheduleAt(receiveEndTime, endReceivedMessage);

            } else // Channel 868_1 is busy. Collision happens.
            {
                EV << "Collision happened\n";
                collisionChannel868_1++;
                if (receiveEndTime > endReceivedMessage->getArrivalTime()) {
                    scheduleAt(receiveEndTime, endReceivedMessage);
                }
            }
        }
        //End device uses channel 868.3 to transmit message.
        else if (channelNumber == 868.3) {
            if (channelBusy868_3 == false) // Channel 868.3 is not busy we can receive the packet
                    {

            } else // Channel 868.3 is busy. Collision happens.
            {
                collisionChannel868_3++;
            }
        }
        //End device uses channel 868.5 to transmit message.
        else if (channelNumber == 868.5) {
            if (channelBusy868_5 == false) // Channel 868.5 is not busy we can receive the packet
                    {

            } else // Channel 868.5 is busy. Collision happens.
            {
                collisionChannel868_5++;
            }
        } else {
            EV << "Incorrect channel number\n";
        }
        delete pkt;
    }

}
void Gateway::receiveMessage() {

}
void Gateway::finish() {
    EV << "\nDuration: " << simTime() << endl;
}

}
;
//namespace
