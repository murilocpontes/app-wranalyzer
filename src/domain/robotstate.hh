#ifndef ROBOTSTATE_HH
#define ROBOTSTATE_HH

#include "types/angle.hh"
#include "types/playerid.hh"
#include "types/vec2.hh"

class RobotState {
public:
    RobotState() = default;

    RobotState(const PlayerID &id, const Vec2 &pos, const Vec2 &vel,
               const WRAngle &theta, float omega = 0.0f,
               const Vec2 &telemetryVel = Vec2(0.0f, 0.0f))
        : id_(id), pos_(pos), vel_(vel), theta_(theta),
          omega_(omega), telemetryVel_(telemetryVel) {}

    const PlayerID &id() const { return id_; }
    const Vec2 &pos() const { return pos_; }
    const Vec2 &vel() const { return vel_; }
    const WRAngle &theta() const { return theta_; }
    float omega() const { return omega_; }
    const Vec2 &telemetryVel() const { return telemetryVel_; }

private:
    PlayerID id_;
    Vec2 pos_{0.0f, 0.0f};
    Vec2 vel_{0.0f, 0.0f};
    WRAngle theta_{0.0f};
    float omega_{0.0f};
    Vec2 telemetryVel_{0.0f, 0.0f};
};

#endif