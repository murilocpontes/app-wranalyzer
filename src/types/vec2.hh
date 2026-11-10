#ifndef VEC2_HH
#define VEC2_HH

#include <QString>

#include <cassert>
#include <cmath>

#include <eigen3/Eigen/Dense>

class Vec2 {
private:
    Eigen::Vector2f v_;
    bool valid_;

public:
    Vec2() : v_(0.0f, 0.0f), valid_(true) {}

    Vec2(float x, float y) : v_(x, y), valid_(true) {}

    Vec2(const Eigen::Vector2f &vec) : v_(vec), valid_(true) {}

    float x() const {
        return v_.x();
    }

    float y() const {
        return v_.y();
    }

    float &x() {
        return v_.x();
    }

    float &y() {
        return v_.y();
    }

    void setX(float val) {
        v_.x() = val;
    }

    void setY(float val) {
        v_.y() = val;
    }

    Vec2 &invalidate() {
        valid_ = false;
        return *this;
    }

    void setInvalid() {
        valid_ = false;
    }

    void setValid() {
        valid_ = true;
    }

    bool isValid() const {
        return valid_;
    }

    bool isZero() const {
        return v_.isZero();
    }

    const Eigen::Vector2f &eigen() const {
        return v_;
    }

    Eigen::Vector2f &eigen() {
        return v_;
    }

    Vec2 operator+(const Vec2 &other) const {
        return Vec2(v_ + other.v_);
    }

    Vec2 operator-(const Vec2 &other) const {
        return Vec2(v_ - other.v_);
    }

    Vec2 operator*(float scalar) const {
        return Vec2(v_ * scalar);
    }

    Vec2 operator/(float scalar) const {
        return Vec2(v_ / scalar);
    }

    Vec2 &operator+=(const Vec2 &other) {
        v_ += other.v_;
        return *this;
    }

    Vec2 &operator-=(const Vec2 &other) {
        v_ -= other.v_;
        return *this;
    }

    Vec2 &operator*=(float scalar) {
        v_ *= scalar;
        return *this;
    }

    Vec2 &operator/=(float scalar) {
        v_ /= scalar;
        return *this;
    }

    bool operator==(const Vec2 &other) const {
        return v_.isApprox(other.v_, 1e-6f);
    }

    bool operator!=(const Vec2 &other) const {
        return !(*this == other);
    }

    float operator[](int index) const {
        assert(index == 0 || index == 1);
        return v_[index];
    }

    float &operator[](int index) {
        assert(index == 0 || index == 1);
        return v_[index];
    }

    float norm() const {
        return v_.norm();
    }

    float squaredNorm() const {
        return v_.squaredNorm();
    }

    Vec2 normalized() const {
        return Vec2(v_.normalized());
    }

    void normalize() {
        v_.normalize();
    }

    float dot(const Vec2 &other) const {
        return v_.dot(other.v_);
    }

    Vec2 rotated90() const {
        return Vec2(-y(), x());
    }

    Vec2 rotatedMinus90() const {
        return Vec2(y(), -x());
    }
};

inline Vec2 operator*(float scalar, const Vec2 &v) {
    return v * scalar;
}

static const Vec2 NULL_VEC;

inline bool isNullVec(const Vec2 &vec) {
    return vec == NULL_VEC;
}

inline QString vec2ToString(const Vec2 &vec) {
    return QString("(%1, %2)").arg(vec[0]).arg(vec[1]);
}

inline bool isVecNaNorInf(const Vec2 &vec) {
    return std::isnan(vec.x()) || std::isnan(vec.y()) || std::isinf(vec.x()) || std::isinf(vec.y());
}

#endif