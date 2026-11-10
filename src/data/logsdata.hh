#ifndef SSLLOGSDATA_HH
#define SSLLOGSDATA_HH

#include "datasource.hh"

#include "zlib.h"
#include <QString>

class LogsData : public DataSource {
public:
    LogsData(QString &filePath);
    ~LogsData();

    bool open() override;
    WorldSnapshot nextFrame() override;
    bool hasNext() override;
    void close() override;
private:

    enum MessageId : qint32 {
        MessageBlank                = 0,
        MessageUnknown              = 1,
        MessageSslVision2010        = 2,
        MessageSslRefbox2013        = 3, // de onde serão retirados os ref states
        MessageSslVision2014        = 4,
        MessageSslVisionTracker2020 = 5, // de onde serão retiradas as posições 
        MessageIndex2021            = 6 
    };

    bool readRawMessage(qint64 &timestampNs, qint32 &messageId, QByteArray &payload);
    void parseReferee(const QByteArray &payload);
    bool parseTracker(qint64 timestampNs, const QByteArray &payload);
    bool advance();

    QString filePath_;
    gzFile file_ = nullptr;
    bool hasNextFrame_ = false;
    WorldSnapshot currentSnapshot_;
    std::string trackedSourceUuid_;
    double lastTimestamp_ = -1.0;
};

#endif