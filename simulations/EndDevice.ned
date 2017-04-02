package simulations;

simple EndDevice
{
    parameters:
        @signal[state](type="long");
        
        
        double txRate @unit(bps);         // transmission rate
        double radioDelay @unit(s);       // propagation delay of radio link
        volatile int packetLengthBits @unit(bit);  // packet length in bits
        volatile double iaTime @unit(s);  // packet interarrival time
        double slotTime @unit(s);         // zero means no slots (pure Aloha)
        @display("i=device/pc_s");

        //parameters to calculate ToA
        int NP @unit(byte);	//number of preamble symbols in BYTE
        int SW @unit(bit);	//length of synchronization word in BIT
        int PL @unit(byte);	//number of PHY payload in BYTE
        int CRC;	//presence of CRC --> CRC=1 in uplink and CRC=0 in downlink
        int IH;		//presence of PHY header --> IH=0 in uplink and IH=1 in downlink
        double CR;		//coding rate value,eg. 1/4, 3/4
        int DE;		//data rate optimization algorithm --> DE=2 when algorithm enabled, DE=0 otherwise
}

