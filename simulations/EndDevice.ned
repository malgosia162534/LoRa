package simulations;

simple EndDevice
{
    parameters:
        int posX = intuniform(1,1000);
        int posY = intuniform(1,1000);
        int packetSize;  // packet length in bits
        volatile double iaTime @unit(s);  // packet interarrival time

        @display("i=old/ball_vs");//i2=old/x_blank");

        //Signals and statistics for EndDevice state
        @signal[state](type="long");
        @statistic[CH1state](source="state"; title="Channel_1 state"; enum="IDLE=0,BUSY=1,TRANSMITTING=2"; record=vector);
        //Signals and statistics for EndDevice used channel
        @signal[usedChannel](type="long");
        @statistic[usedCh](title="UsedChannel"; record=vector; source="usedChannel");
        
        //Signals and statistics about sent and lost messages
        @signal[Sentmessages](type="long");
        @statistic[SentmessagesStats](title="Sentmessages"; record=vector,vector; source="Sentmessages");
        @signal[Lostmessages](type="long");
        @statistic[LostmessagesStats](title="Lostmessages"; record=vector,vector; source="Lostmessages");



    gates:
        input in @directIn;


}

