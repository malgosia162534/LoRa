/*
 * loraChannel.h
 *
 *  Created on: Aug 29, 2017
 *      Author: Malgosia
 */

#ifndef LORACHANNEL_H_
#define LORACHANNEL_H_
#include <omnetpp.h>
using namespace omnetpp;
class loraChannel {
private:
    int frequency;
    bool channelBusy;
    long sentMessages;
    long lostMessages;
    simtime_t lastPacket;
    int state;

public:
    loraChannel();
    int getFrequency();
    void setFrequency(int freq);

    bool getAvailability();
    void setAvailability(bool busy);

    long getSentMessages();
    void setSentMessages(long number);

    long getLostMessages();
    void setLostMessages(long number);

    double getLastPacket();
    void setLastPacket(simtime_t lastPck);

    int getChannelState();
    void setChannelState(int state);

    void printAllInformation();
};

#endif /* LORACHANNEL_H_ */
