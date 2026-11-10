#ifndef BALLSTATE_HH
#define BALLSTATE_HH

#include "types/vec2.hh"

class BallState {
public:
    BallState() = default;
    BallState(const Vec2 &pos, const Vec2 &vel) : pos_(pos), vel_(vel) {}

    const Vec2 &pos() const { return pos_; }
    const Vec2 &vel() const { return vel_; }

    void setPos(const Vec2 &pos) { pos_ = pos; }
    void setVel(const Vec2 &vel) { vel_ = vel; }

private:
    Vec2 pos_{0.0f, 0.0f};
    Vec2 vel_{0.0f, 0.0f};
};

#endif