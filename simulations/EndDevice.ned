package simulations;

simple EndDevice
{
    parameters:

        volatile int PHYpayload @unit(byte);  // packet length in bits
        volatile double iaTime @unit(s);  // packet interarrival time
        volatile int channelNumber = default(2);
        @display("i=device/pc_s");
    gates:
        input in @directIn;


}

