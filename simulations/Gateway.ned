package simulations;

simple Gateway
{
    parameters:
        @display("i=device/antennatower_l");
        
        //@signal[receiveBegin](type="long");  // increases with each new frame arriving to the server and drops to 0 if the channel becomes finally idle
       // @signal[receive](type="long");  // for successful receptions (non-collisions): 1 at the start of the reception, 0 at the end of the reception
      //  @signal[collision](type="long"); // the number of collided frames at the beginning of the collision period
     //   @signal[collisionLength](type="simtime_t");  // the length of the last collision period at the end of the collision period
       // @signal[channelState](type="long");

        int spreadingFactor;
    gates:
        input in @directIn;
}

