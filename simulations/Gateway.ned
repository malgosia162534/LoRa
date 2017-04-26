package simulations;

simple Gateway
{
    parameters:
        
        int spreadingFactor;
        double bandwidth @unit(Hz)=125kHz;	//bandwidth - 125 kHz, 250 kHz
        @display("i=device/antennatower_l");
    gates:
        input in @directIn;
}

