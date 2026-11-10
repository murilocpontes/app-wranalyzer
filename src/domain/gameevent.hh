#ifndef GAMEEVENT_HH
#define GAMEEVENT_HH

#include "types/color.hh"
#include "types/playerid.hh"
#include "types/vec2.hh"

class GameEvent{
public:
    enum Type{
        UNKNOWN = 0,
        BOT_CRASH_UNIQUE,
        BOT_CRASH_DRAWN,
        BOT_PUSHED_BOT,
        BOT_TOO_FAST_IN_STOP,
        DEFENDER_IN_DEFENSE_AREA,
        DEFENDER_TOO_CLOSE_TO_KICK_POINT,
        ATTACKER_TOO_CLOSE_TO_DEFENSE_AREA,
        ATTACKER_DOUBLE_TOUCHED_BALL,
        BOT_DRIBBLED_BALL_TOO_FAR,
        BOT_KICKED_BALL_TOO_FAST,
        BOUNDARY_CROSSING,
        BALL_LEFT_FIELD_TOUCH_LINE,
        BALL_LEFT_FIELD_GOAL_LINE,
        AIMLESS_KICK,
        PLACEMENT_SUCCEEDED,
        PLACEMENT_FAILED,
        GOAL
    };
    GameEvent() = default;
    GameEvent(Type type, Colors::Color byTeam, const PlayerID &primaryBot = PlayerID(),
              const Vec2 &location = Vec2(0.0f, 0.0f))
        : type_(type), byTeam_(byTeam), primaryBot_(primaryBot), location_(location){}
    Type type() const { return type_; }
    Colors::Color byTeam() const { return byTeam_; }
    const PlayerID &primaryBot() const { return primaryBot_; }
    const Vec2 &location() const { return location_; }
private:
    Type type_{UNKNOWN};
    Colors::Color byTeam_{Colors::NONE};
    PlayerID primaryBot_;
    Vec2 location_{0.0f, 0.0f};
};
#endif