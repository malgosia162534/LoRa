package simulations;

network Lora
{
    parameters:
        int numHosts;  // number of hosts
        double txRate @unit(bps);  // transmission rate
        double slotTime @unit(ms);  // zero means no slots (pure Aloha)
        @display("bgi=background/mapa_piotrowo,s;bgb=792,386;bgu=m");
    submodules:
        gateway: Gateway {
            @display("p=437,101");
        }
        endDevice: EndDevice {// endDevice[numHosts]: EndDevice {
            txRate = txRate;
            slotTime = slotTime;
            @display("p=166,149");
        }
}

