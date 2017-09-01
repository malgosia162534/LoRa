//
// Generated file, do not edit! Created by nedtool 5.0 from loraPck.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "loraPck_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: no doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: no doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(LoraPck);

LoraPck::LoraPck(const char *name, int kind) : ::omnetpp::cPacket(name,kind)
{
    this->nodeId = 0;
    this->packetLength = 0;
    this->distance = 0;
    this->ToA = 0;
    this->spreadingFactor = 0;
    this->bandwidth = 0;
    this->gatewayID = 0;
    this->rectime = 0;
    this->arrivalTime = 0;
    this->rssi = 0;
    this->frequency = 0;
    this->requireACK = false;
    this->colllided = false;
    this->processed = false;
    this->lost = false;
}

LoraPck::LoraPck(const LoraPck& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

LoraPck::~LoraPck()
{
}

LoraPck& LoraPck::operator=(const LoraPck& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void LoraPck::copy(const LoraPck& other)
{
    this->nodeId = other.nodeId;
    this->packetLength = other.packetLength;
    this->distance = other.distance;
    this->ToA = other.ToA;
    this->spreadingFactor = other.spreadingFactor;
    this->bandwidth = other.bandwidth;
    this->gatewayID = other.gatewayID;
    this->rectime = other.rectime;
    this->arrivalTime = other.arrivalTime;
    this->rssi = other.rssi;
    this->frequency = other.frequency;
    this->requireACK = other.requireACK;
    this->colllided = other.colllided;
    this->processed = other.processed;
    this->lost = other.lost;
}

void LoraPck::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->nodeId);
    doParsimPacking(b,this->packetLength);
    doParsimPacking(b,this->distance);
    doParsimPacking(b,this->ToA);
    doParsimPacking(b,this->spreadingFactor);
    doParsimPacking(b,this->bandwidth);
    doParsimPacking(b,this->gatewayID);
    doParsimPacking(b,this->rectime);
    doParsimPacking(b,this->arrivalTime);
    doParsimPacking(b,this->rssi);
    doParsimPacking(b,this->frequency);
    doParsimPacking(b,this->requireACK);
    doParsimPacking(b,this->colllided);
    doParsimPacking(b,this->processed);
    doParsimPacking(b,this->lost);
}

void LoraPck::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->nodeId);
    doParsimUnpacking(b,this->packetLength);
    doParsimUnpacking(b,this->distance);
    doParsimUnpacking(b,this->ToA);
    doParsimUnpacking(b,this->spreadingFactor);
    doParsimUnpacking(b,this->bandwidth);
    doParsimUnpacking(b,this->gatewayID);
    doParsimUnpacking(b,this->rectime);
    doParsimUnpacking(b,this->arrivalTime);
    doParsimUnpacking(b,this->rssi);
    doParsimUnpacking(b,this->frequency);
    doParsimUnpacking(b,this->requireACK);
    doParsimUnpacking(b,this->colllided);
    doParsimUnpacking(b,this->processed);
    doParsimUnpacking(b,this->lost);
}

int LoraPck::getNodeId() const
{
    return this->nodeId;
}

void LoraPck::setNodeId(int nodeId)
{
    this->nodeId = nodeId;
}

int LoraPck::getPacketLength() const
{
    return this->packetLength;
}

void LoraPck::setPacketLength(int packetLength)
{
    this->packetLength = packetLength;
}

double LoraPck::getDistance() const
{
    return this->distance;
}

void LoraPck::setDistance(double distance)
{
    this->distance = distance;
}

double LoraPck::getToA() const
{
    return this->ToA;
}

void LoraPck::setToA(double ToA)
{
    this->ToA = ToA;
}

int LoraPck::getSpreadingFactor() const
{
    return this->spreadingFactor;
}

void LoraPck::setSpreadingFactor(int spreadingFactor)
{
    this->spreadingFactor = spreadingFactor;
}

int LoraPck::getBandwidth() const
{
    return this->bandwidth;
}

void LoraPck::setBandwidth(int bandwidth)
{
    this->bandwidth = bandwidth;
}

int LoraPck::getGatewayID() const
{
    return this->gatewayID;
}

void LoraPck::setGatewayID(int gatewayID)
{
    this->gatewayID = gatewayID;
}

::omnetpp::simtime_t LoraPck::getRectime() const
{
    return this->rectime;
}

void LoraPck::setRectime(::omnetpp::simtime_t rectime)
{
    this->rectime = rectime;
}

::omnetpp::simtime_t LoraPck::getArrivalTime() const
{
    return this->arrivalTime;
}

void LoraPck::setArrivalTime(::omnetpp::simtime_t arrivalTime)
{
    this->arrivalTime = arrivalTime;
}

double LoraPck::getRssi() const
{
    return this->rssi;
}

void LoraPck::setRssi(double rssi)
{
    this->rssi = rssi;
}

int LoraPck::getFrequency() const
{
    return this->frequency;
}

void LoraPck::setFrequency(int frequency)
{
    this->frequency = frequency;
}

bool LoraPck::getRequireACK() const
{
    return this->requireACK;
}

void LoraPck::setRequireACK(bool requireACK)
{
    this->requireACK = requireACK;
}

bool LoraPck::getColllided() const
{
    return this->colllided;
}

void LoraPck::setColllided(bool colllided)
{
    this->colllided = colllided;
}

bool LoraPck::getProcessed() const
{
    return this->processed;
}

void LoraPck::setProcessed(bool processed)
{
    this->processed = processed;
}

bool LoraPck::getLost() const
{
    return this->lost;
}

void LoraPck::setLost(bool lost)
{
    this->lost = lost;
}

class LoraPckDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    LoraPckDescriptor();
    virtual ~LoraPckDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(LoraPckDescriptor);

LoraPckDescriptor::LoraPckDescriptor() : omnetpp::cClassDescriptor("LoraPck", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

LoraPckDescriptor::~LoraPckDescriptor()
{
    delete[] propertynames;
}

bool LoraPckDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LoraPck *>(obj)!=nullptr;
}

const char **LoraPckDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *LoraPckDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int LoraPckDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 15+basedesc->getFieldCount() : 15;
}

unsigned int LoraPckDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<15) ? fieldTypeFlags[field] : 0;
}

const char *LoraPckDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "nodeId",
        "packetLength",
        "distance",
        "ToA",
        "spreadingFactor",
        "bandwidth",
        "gatewayID",
        "rectime",
        "arrivalTime",
        "rssi",
        "frequency",
        "requireACK",
        "colllided",
        "processed",
        "lost",
    };
    return (field>=0 && field<15) ? fieldNames[field] : nullptr;
}

int LoraPckDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeId")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetLength")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "distance")==0) return base+2;
    if (fieldName[0]=='T' && strcmp(fieldName, "ToA")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "spreadingFactor")==0) return base+4;
    if (fieldName[0]=='b' && strcmp(fieldName, "bandwidth")==0) return base+5;
    if (fieldName[0]=='g' && strcmp(fieldName, "gatewayID")==0) return base+6;
    if (fieldName[0]=='r' && strcmp(fieldName, "rectime")==0) return base+7;
    if (fieldName[0]=='a' && strcmp(fieldName, "arrivalTime")==0) return base+8;
    if (fieldName[0]=='r' && strcmp(fieldName, "rssi")==0) return base+9;
    if (fieldName[0]=='f' && strcmp(fieldName, "frequency")==0) return base+10;
    if (fieldName[0]=='r' && strcmp(fieldName, "requireACK")==0) return base+11;
    if (fieldName[0]=='c' && strcmp(fieldName, "colllided")==0) return base+12;
    if (fieldName[0]=='p' && strcmp(fieldName, "processed")==0) return base+13;
    if (fieldName[0]=='l' && strcmp(fieldName, "lost")==0) return base+14;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *LoraPckDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "double",
        "double",
        "int",
        "int",
        "int",
        "simtime_t",
        "simtime_t",
        "double",
        "int",
        "bool",
        "bool",
        "bool",
        "bool",
    };
    return (field>=0 && field<15) ? fieldTypeStrings[field] : nullptr;
}

const char **LoraPckDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *LoraPckDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int LoraPckDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    LoraPck *pp = (LoraPck *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string LoraPckDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LoraPck *pp = (LoraPck *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getNodeId());
        case 1: return long2string(pp->getPacketLength());
        case 2: return double2string(pp->getDistance());
        case 3: return double2string(pp->getToA());
        case 4: return long2string(pp->getSpreadingFactor());
        case 5: return long2string(pp->getBandwidth());
        case 6: return long2string(pp->getGatewayID());
        case 7: return simtime2string(pp->getRectime());
        case 8: return simtime2string(pp->getArrivalTime());
        case 9: return double2string(pp->getRssi());
        case 10: return long2string(pp->getFrequency());
        case 11: return bool2string(pp->getRequireACK());
        case 12: return bool2string(pp->getColllided());
        case 13: return bool2string(pp->getProcessed());
        case 14: return bool2string(pp->getLost());
        default: return "";
    }
}

bool LoraPckDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    LoraPck *pp = (LoraPck *)object; (void)pp;
    switch (field) {
        case 0: pp->setNodeId(string2long(value)); return true;
        case 1: pp->setPacketLength(string2long(value)); return true;
        case 2: pp->setDistance(string2double(value)); return true;
        case 3: pp->setToA(string2double(value)); return true;
        case 4: pp->setSpreadingFactor(string2long(value)); return true;
        case 5: pp->setBandwidth(string2long(value)); return true;
        case 6: pp->setGatewayID(string2long(value)); return true;
        case 7: pp->setRectime(string2simtime(value)); return true;
        case 8: pp->setArrivalTime(string2simtime(value)); return true;
        case 9: pp->setRssi(string2double(value)); return true;
        case 10: pp->setFrequency(string2long(value)); return true;
        case 11: pp->setRequireACK(string2bool(value)); return true;
        case 12: pp->setColllided(string2bool(value)); return true;
        case 13: pp->setProcessed(string2bool(value)); return true;
        case 14: pp->setLost(string2bool(value)); return true;
        default: return false;
    }
}

const char *LoraPckDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *LoraPckDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    LoraPck *pp = (LoraPck *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


