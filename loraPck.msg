//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

//
// TODO generated message class
//
packet LoraPck {
    int nodeId;
    int packetLength;
    double distance;
    double ToA;
    int spreadingFactor;
    int bandwidth;
    int gatewayID;
    simtime_t rectime;
    simtime_t arrivalTime;
    double rssi;
    int frequency;
    bool requireACK;
    bool colllided;
    bool processed;
    bool lost; 
}
