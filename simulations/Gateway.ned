package simulations;

simple Gateway
{
    parameters:
        int posX = intuniform(1,1000);
        int posY = intuniform(1,1000);
        int spreadingFactor = intuniform(6, 13);
        double bandwidth @unit(Hz) = 125kHz;	//bandwidth - 125 kHz, 250 kHz
        @display("i=device/antennatower_l;i2=old/x_blank");

        //Signals for collision
        @signal[collisionCH1](type="long");
        @signal[collisionCH2](type="long");
        @signal[collisionCH3](type="long");
        //Statistics for collision
        @statistic[collision1](title="collisionCH1"; record=vector; source="collisionCH1");
        @statistic[collision2](title="collisionCH2"; record=vector; source="collisionCH2");
        @statistic[collision3](title="collisionCH3"; record=vector; source="collisionCH3");
        //Statistics and signals for channel State
        @signal[channelState](type="long");
        @statistic[serverChannelState](source="channelState"; title="Channel state"; enum="IDLE=0,TRANSMISSION=1,COLLISION=2"; record=histogram);
        //Signals for received messages
        @signal[receivedCH1](type="long");
        @signal[receivedCH2](type="long");
        @signal[receivedCH3](type="long");
        //Statistics for collision
        @statistic[received1](title="receivedCH1"; record=vector; source="receivedCH1");
        @statistic[received2](title="receivedCH2"; record=vector; source="receivedCH2");
        @statistic[received3](title="receivedCH3"; record=vector; source="receivedCH3");

    gates:
        input in @directIn;
}

