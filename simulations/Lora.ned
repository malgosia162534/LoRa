package simulations;

network Lora
{
    parameters:
        int txPow; // transmission power
        int GL;
      	double gamma;
      	double d0;
      	double LpLd0;
      	int nodeId=0;
        
        //Experiment parameters
        int spreadingFactor;
        int CR;
        int bandwidth;
        double minsensitivity;
        
        int numHosts;  // number of hosts
        int numGateways; 	//number of gateways

        //double bandwidth @unit(Hz);	//bandwidth - 125 kHz, 250 kHz
        double receive_delay1 @unit(s) = 1s + uniform(-20e-6 s,20e-6 s); // receive delay1
        double receive_delay2 @unit(s) = receive_delay1 + 1 s;	// receive delay2

        @display("bgi=background/green,s;bgb=100000,140000;bgu=m");
    submodules:
        gateway[numGateways]: Gateway {
            parameters:
                @display("t=$spreadingFactor");//;p=$posX,$posY");
        }
        endDevice[numHosts]: EndDevice {
        }
}

