/*
 * loraChannel.cc

 *
 *  Created on: Aug 29, 2017
 *      Author: Malgosia
 */
#include "loraChannel.h"
#include <omnetpp.h>
using namespace omnetpp;

loraChannel::loraChannel()
{
    //Setting initial values
        channelBusy=false;
        sentMessages=0;
        lostMessages=0;
        lastPacket=0.0;
        state=0;

}
int loraChannel::getFrequency(){
    return frequency;
}
void loraChannel::setFrequency(int freq)
{
    frequency=freq;
}
bool loraChannel::getAvailability()
{
    return channelBusy;
}
void loraChannel::setAvailability(bool avail)
{
    channelBusy=avail;
}
long loraChannel::getSentMessages(){
    return sentMessages;
}
void loraChannel::setSentMessages(long num)
{
    sentMessages=num;
}
long loraChannel::getLostMessages(){
    return lostMessages;
}
void loraChannel::setLostMessages(long num)
{
    lostMessages=num;
}
double loraChannel::getLastPacket(){
    return lastPacket.dbl();
}
void loraChannel::setLastPacket(simtime_t last)
{
    lastPacket=last;
}

int loraChannel::getChannelState(){
    return state;
}
void loraChannel::setChannelState(int sta)
{
    state=sta;
}

void loraChannel::printAllInformation()
{
    EV<< "frequency: "<<frequency <<" channelBusy: "<<channelBusy <<" sentMessages "<<sentMessages << " lostMessages " << lostMessages <<" lastPacket "<<lastPacket << " state "<<state<<endl;

}




