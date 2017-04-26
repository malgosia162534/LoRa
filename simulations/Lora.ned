package simulations;

network Lora
{
    parameters:
        int numHosts=3;  // number of hosts
        //int numGatewats; 	//number of gateways

        //double bandwidth @unit(Hz);	//bandwidth - 125 kHz, 250 kHz
        double receive_delay1 @unit(s) = 1s + uniform(-20e-6 s,20e-6 s); // receive delay1
        double receive_delay2 @unit(s) = receive_delay1 + 1 s;	// receive delay2

        @display("bgi=background/mapa_piotrowo,s;bgb=792,386;bgu=m");
    submodules:
        gateway: Gateway {
            parameters:
                spreadingFactor = uniform(6, 13);
              	
                @display("t= $spreadingFactor");
        }
         endDevice[numHosts]: EndDevice {
            parameters:
                PHYpayload = uniform(0 byte,10 byte); // in uniform have to decide the unit of number
                
        }
}

