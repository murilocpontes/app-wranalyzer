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


    static RefereeStates::State convertRefCommand(ssl::Referee::Command cmd,
                                                Colors::Color ourColor = Colors::BLUE) {
        bool weAreYellow = (ourColor == Colors::YELLOW);

        switch (cmd) {
        case ssl::Referee::HALT:
            return RefereeStates::HALT;
        case ssl::Referee::STOP:
            return RefereeStates::STOP;
        case ssl::Referee::NORMAL_START:
        case ssl::Referee::FORCE_START:
            return RefereeStates::RUN;
        case ssl::Referee::TIMEOUT_YELLOW:
        case ssl::Referee::TIMEOUT_BLUE:
            return RefereeStates::TIMEOUT;

        case ssl::Referee::PREPARE_KICKOFF_YELLOW:
            return weAreYellow ? RefereeStates::OUR_PREPARE_KICKOFF : RefereeStates::THEIR_PREPARE_KICKOFF;
        case ssl::Referee::PREPARE_KICKOFF_BLUE:
            return weAreYellow ? RefereeStates::THEIR_PREPARE_KICKOFF : RefereeStates::OUR_PREPARE_KICKOFF;

        case ssl::Referee::BALL_PLACEMENT_YELLOW:
            return weAreYellow ? RefereeStates::OUR_BALL_PLACEMENT : RefereeStates::THEIR_BALL_PLACEMENT;
        case ssl::Referee::BALL_PLACEMENT_BLUE:
            return weAreYellow ? RefereeStates::THEIR_BALL_PLACEMENT : RefereeStates::OUR_BALL_PLACEMENT;

        case ssl::Referee::PREPARE_PENALTY_YELLOW:
            return weAreYellow ? RefereeStates::OUR_PREPARE_PENALTY : RefereeStates::THEIR_PREPARE_PENALTY;
        case ssl::Referee::PREPARE_PENALTY_BLUE:
            return weAreYellow ? RefereeStates::THEIR_PREPARE_PENALTY : RefereeStates::OUR_PREPARE_PENALTY;

        case ssl::Referee::DIRECT_FREE_YELLOW:
        case ssl::Referee::INDIRECT_FREE_YELLOW:
            return weAreYellow ? RefereeStates::OUR_FREEKICK : RefereeStates::THEIR_FREEKICK;
        case ssl::Referee::DIRECT_FREE_BLUE:
        case ssl::Referee::INDIRECT_FREE_BLUE:
            return weAreYellow ? RefereeStates::THEIR_FREEKICK : RefereeStates::OUR_FREEKICK;

        default:
            return RefereeStates::HALT;
        }
    }

    static ::GameEvent convertGameEvent(const ssl::GameEvent &protoEv) {
        ::GameEvent::Type type = ::GameEvent::UNKNOWN;

        switch (protoEv.type()) {
        case ssl::GameEvent::BOT_CRASH_UNIQUE:
            type = ::GameEvent::BOT_CRASH_UNIQUE; break;
        case ssl::GameEvent::BOT_CRASH_DRAWN:
            type = ::GameEvent::BOT_CRASH_DRAWN; break;
        case ssl::GameEvent::BOT_PUSHED_BOT:
            type = ::GameEvent::BOT_PUSHED_BOT; break;
        case ssl::GameEvent::BOT_TOO_FAST_IN_STOP:
            type = ::GameEvent::BOT_TOO_FAST_IN_STOP; break;
        case ssl::GameEvent::DEFENDER_IN_DEFENSE_AREA:
            type = ::GameEvent::DEFENDER_IN_DEFENSE_AREA; break;
        case ssl::GameEvent::DEFENDER_TOO_CLOSE_TO_KICK_POINT:
            type = ::GameEvent::DEFENDER_TOO_CLOSE_TO_KICK_POINT; break;
        case ssl::GameEvent::ATTACKER_TOO_CLOSE_TO_DEFENSE_AREA:
            type = ::GameEvent::ATTACKER_TOO_CLOSE_TO_DEFENSE_AREA; break;
        case ssl::GameEvent::ATTACKER_DOUBLE_TOUCHED_BALL:
            type = ::GameEvent::ATTACKER_DOUBLE_TOUCHED_BALL; break;
        case ssl::GameEvent::BOT_DRIBBLED_BALL_TOO_FAR:
            type = ::GameEvent::BOT_DRIBBLED_BALL_TOO_FAR; break;
        case ssl::GameEvent::BOT_KICKED_BALL_TOO_FAST:
            type = ::GameEvent::BOT_KICKED_BALL_TOO_FAST; break;
        case ssl::GameEvent::BOUNDARY_CROSSING:
            type = ::GameEvent::BOUNDARY_CROSSING; break;
        case ssl::GameEvent::BALL_LEFT_FIELD_TOUCH_LINE:
            type = ::GameEvent::BALL_LEFT_FIELD_TOUCH_LINE; break;
        case ssl::GameEvent::BALL_LEFT_FIELD_GOAL_LINE:
            type = ::GameEvent::BALL_LEFT_FIELD_GOAL_LINE; break;
        case ssl::GameEvent::AIMLESS_KICK:
            type = ::GameEvent::AIMLESS_KICK; break;
        case ssl::GameEvent::PLACEMENT_SUCCEEDED:
            type = ::GameEvent::PLACEMENT_SUCCEEDED; break;
        case ssl::GameEvent::PLACEMENT_FAILED:
            type = ::GameEvent::PLACEMENT_FAILED; break;
        case ssl::GameEvent::GOAL:
            type = ::GameEvent::GOAL; break;
        default:
            type = ::GameEvent::UNKNOWN; break;
        }

        return ::GameEvent(type, Colors::NONE);
    }


};


#endif