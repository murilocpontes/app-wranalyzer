#ifndef WORLDSNAPSHOT_HH
#define WORLDSNAPSHOT_HH

#include <QList>
#include "domain/ballstate.hh"
#include "domain/gameevent.hh"
#include "domain/robotstate.hh"
#include "types/refstates.hh"

class WorldSnapshot {
public:
    WorldSnapshot() = default;

    double timestamp() const { return timestamp_; }
    void setTimestamp(double t) { timestamp_ = t; }

    const BallState &ball() const { return ball_; }
    void setBall(const BallState &ball) { ball_ = ball; }

    const QList<RobotState> &robots() const { return robots_; }
    void clearRobots() { robots_.clear(); }
    void addRobot(const RobotState &robot) { robots_.append(robot); }

    RefereeStates::State refState() const { return refState_; }
    void setRefState(RefereeStates::State state) { refState_ = state; }

    const PlayerID &yellowGoalkeeper() const { return yellowGoalkeeper_; }
    const PlayerID &blueGoalkeeper() const { return blueGoalkeeper_; }
    void setGoalkeepers(const PlayerID &yellowGk, const PlayerID &blueGk) {
        yellowGoalkeeper_ = yellowGk;
        blueGoalkeeper_ = blueGk;
    }

    const Vec2 &ballPlacementPos() const { return ballPlacementPos_; }
    void setBallPlacementPos(const Vec2 &pos) { ballPlacementPos_ = pos; }

    const QList<GameEvent> &events() const { return events_; }
    void clearEvents() { events_.clear(); }
    void addEvent(const GameEvent &ev) { events_.append(ev); }

private:
    double timestamp_{0.0};
    BallState ball_;
    QList<RobotState> robots_;

    RefereeStates::State refState_{RefereeStates::HALT};
    PlayerID yellowGoalkeeper_;
    PlayerID blueGoalkeeper_;
    Vec2 ballPlacementPos_{0.0f, 0.0f};
    QList<GameEvent> events_;
};

#endif