#include "logsdata.hh"

#include <cstring>
#include <QtEndian>

#include "3rdparty/proto/pb/ssl_gc_referee_message.pb.h"
#include "3rdparty/proto/pb/ssl_vision_wrapper_tracked.pb.h"
#include "types/color.hh"


LogsData::LogsData(QString &filePath) : filePath_(filePath) {}

LogsData::~LogsData() {
    close();
}


bool LogsData::open() {
    if(file_ != nullptr){
        close();
    }

    file_ = gzopen(filePath_.toUtf8().constData(), "rb");
    if(file_ == nullptr){
        return false;
    }

    char header[12];
    if(gzread(file_, header, 12) != 12 || std::memcmp(header, "SSL_LOG_FILE", 12) != 0){
        close();
        return false;
    }

    qint32 versionBigEndian = 0;
    if(gzread(file_, &versionBigEndian, sizeof(versionBigEndian)) != sizeof(versionBigEndian)){
        close();
        return false;
    }

    trackedSourceUuid_.clear();
    lastTimestamp_ = -1.0;

    advance();

    return true;
}

void LogsData::close(){
    if(file_ != nullptr){
        gzclose(file_);
        file_ = nullptr;
    }
}

bool LogsData::advance() {
    qint64 timestamp = 0;
    qint32 messageId = 0;
    QByteArray payload;

    while(readRawMessage(timestamp, messageId, payload)){
        if(messageId == MessageSslRefbox2013){
            parseReferee(payload);
        } 
        else if(messageId == MessageSslVisionTracker2020){
            if(parseTracker(timestamp, payload)){
                hasNextFrame_ = true;
                return true;
            }
        }
    }

    hasNextFrame_ = false;
    return false;
}

bool LogsData::hasNext() {
    return hasNextFrame_;
}

WorldSnapshot LogsData::nextFrame() {
     WorldSnapshot frameToReturn = currentSnapshot_;
    advance(); 
    return frameToReturn;
}

bool LogsData::readRawMessage(qint64 &timestampNs, qint32 &messageId, QByteArray &payload){
    if(file_ == nullptr){
        return false;
    }
    qint64 rawTimestamp = 0;
    qint32 rawMessageId = 0;
    qint32 rawLength = 0;

    if(gzread(file_, &rawTimestamp, sizeof(rawTimestamp)) != sizeof(rawTimestamp)) 
        return false;
    if(gzread(file_, &rawMessageId, sizeof(rawMessageId)) != sizeof(rawMessageId)) 
        return false;
    if(gzread(file_, &rawLength, sizeof(rawLength)) != sizeof(rawLength)) 
        return false;

    timestampNs = qFromBigEndian(rawTimestamp);
    messageId = qFromBigEndian(rawMessageId);
    qint32 length = qFromBigEndian(rawLength);

    if(length < 0){
        return false;
    }

    if(messageId != MessageSslRefbox2013 && messageId != MessageSslVisionTracker2020){
        return gzseek(file_, length, SEEK_CUR);
    }

    payload.resize(length);
    if(length > 0 && gzread(file_, payload.data(), length) != length)
        return false;
        
    return true;
}

bool LogsData::parseTracker(qint64 timestampNs, const QByteArray &payload){
    ssl::TrackerWrapperPacket packet;

    if(!packet.ParseFromArray(payload.data(), payload.size())){
        return false;
    }
    if(!packet.has_tracked_frame()){
        return false;
    }

    // Na primeira iteração pós abertura, guarda uuid para não misturar dados de 2 trackers (caso haja)
    if(trackedSourceUuid_.empty()){
        trackedSourceUuid_ = packet.uuid();
    } else if (packet.uuid() != trackedSourceUuid_) {
        return false;
    }

    const ssl::TrackedFrame &frame = packet.tracked_frame();

    if(frame.timestamp() <= lastTimestamp_){
        return false;
    }

    lastTimestamp_ = frame.timestamp();
    currentSnapshot_.setTimestamp(lastTimestamp_);

    if (frame.balls_size() > 0){
        const ssl::TrackedBall &b = frame.balls(0);
        Vec2 ballPos(b.pos().x(), b.pos().y());
        Vec2 ballVel = b.has_vel() ? Vec2(b.vel().x(), b.vel().y()) : Vec2(0.0f, 0.0f);
        currentSnapshot_.setBall(BallState(ballPos, ballVel));
    }

    currentSnapshot_.clearRobots();
    for (int i = 0; i < frame.robots_size(); ++i) {
        const ssl::TrackedRobot &r = frame.robots(i);
        Colors::Color teamColor = (r.robot_id().team() == ssl::Team::YELLOW)
                                      ? Colors::YELLOW
                                      : Colors::BLUE;
        PlayerID id(teamColor, static_cast<quint8>(r.robot_id().id()));
        Vec2 pos(r.pos().x(), r.pos().y());
        Vec2 vel = r.has_vel() ? Vec2(r.vel().x(), r.vel().y()) : Vec2(0.0f, 0.0f);
        WRAngle orientation(r.orientation());
        float omega = r.has_vel_angular() ? r.vel_angular() : 0.0f;
        currentSnapshot_.addRobot(RobotState(id, pos, vel, orientation, omega));
    }
    return true;
}

void LogsData::parseReferee(const QByteArray &payload) {
    ssl::Referee ref;
    if (!ref.ParseFromArray(payload.constData(), payload.size())) {
        return;
    }

    // Atualiza comando do juiz, IDs dos goleiros e eventos do AutoRef no snapshot atual
    currentSnapshot_.setRefState(convertRefCommand(ref.command()));
    currentSnapshot_.setGoalkeepers(
        PlayerID(Colors::YELLOW, static_cast<quint8>(ref.yellow().goalkeeper())),
        PlayerID(Colors::BLUE,   static_cast<quint8>(ref.blue().goalkeeper()))
    );

    // Eventos do AutoReferee (faltas, colisões, ball placement)
    currentSnapshot_.clearEvents();
    for (int i = 0; i < ref.game_events_size(); ++i) {
        currentSnapshot_.addEvent(convertGameEvent(ref.game_events(i)));
    }
}