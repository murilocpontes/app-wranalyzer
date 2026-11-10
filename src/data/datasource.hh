#ifndef DATASOURCE_HH
#define DATASOURCE_HH

#include "domain/worldsnapshot.hh"

class DataSource {
public:
    DataSource() = default;
    virtual ~DataSource() = default;
    virtual bool open() = 0; // how establish connections
    virtual bool hasNext() = 0;
    virtual WorldSnapshot nextFrame() = 0; // updates next frame data
    virtual void close() = 0;

};


#endif