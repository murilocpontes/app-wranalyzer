#ifndef WORLDSNAPSHOT_HH
#define WORLDSNAPSHOT_HH

#include <QList>
#include "domain/ballstate.hh"
#include "domain/robotstate.hh"
#include "3rdparty/proto/pb/ssl_gc_referee_message.pb.h"

class WorldSnapshot {
public:
    WorldSnapshot() = default;

    // Timestamp (em segundos)
    double timestamp() const { return timestamp_; }
    void setTimestamp(double t) { timestamp_ = t; }

    // Bola
    const BallState &ball() const { return ball_; }
    void setBall(const BallState &ball) { ball_ = ball; }

    // Robôs
    const QList<RobotState> &robots() const { return robots_; }
    void clearRobots() { robots_.clear(); }
    void addRobot(const RobotState &robot) { robots_.append(robot); }

    // Dados do Referee (gameState, goleiros e ponto de placement)
    ssl::Referee::Command refereeCommand() const { return refereeCommand_; }
    void setRefereeCommand(ssl::Referee::Command cmd) { refereeCommand_ = cmd; }

    const PlayerID &yellowGoalkeeper() const { return yellowGoalkeeper_; }
    const PlayerID &blueGoalkeeper() const { return blueGoalkeeper_; }
    void setGoalkeepers(const PlayerID &yellowGk, const PlayerID &blueGk) {
        yellowGoalkeeper_ = yellowGk;
        blueGoalkeeper_ = blueGk;
    }

    const Vec2 &designatedPosition() const { return designatedPosition_; }
    void setDesignatedPosition(const Vec2 &pos) { designatedPosition_ = pos; }

    // Eventos do AutoReferee (colisões, faltas, placement)
    const QList<ssl::GameEvent> &events() const { return events_; }
    void clearEvents() { events_.clear(); }
    void addEvent(const ssl::GameEvent &ev) { events_.append(ev); }

private:
    double timestamp_{0.0};
    BallState ball_;
    QList<RobotState> robots_;

    ssl::Referee::Command refereeCommand_{ssl::Referee::HALT};
    PlayerID yellowGoalkeeper_;
    PlayerID blueGoalkeeper_;
    Vec2 designatedPosition_{0.0f, 0.0f};
    QList<ssl::GameEvent> events_;
};

#endif