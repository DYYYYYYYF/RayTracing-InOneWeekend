#pragma once
#include <iostream>
#include <cmath>

namespace rt{
class vec3{
public:
    double e[3];

public:
    vec3();
    vec3(double e0, double e1, double e2);

    double x() const;
    double y() const;
    double z() const;

    vec3 operator-() const;
    double operator[](int i) const;
    double& operator[](int i);

    vec3& operator+=(const vec3 &v);
    vec3& operator*=(const double t);
    vec3& operator/=(const double t);

    double length() const;
    double length_squared() const;
};

using point3 = vec3;  // 3D point
using color = vec3; // RGB color

//Util Functions
inline std::ostream& operator<<(std::ostream &out, const vec3 &V);
inline vec3 operator+(const vec3 &u, const vec3 &v);
inline vec3 operator-(const vec3 &u, const vec3 &v);
inline vec3 operator*(double t, const vec3 &v);
inline vec3 operator*(const vec3 &v, double t);
inline vec3 operator/(const vec3 v, double t);

inline double dot(vec3 &u, vec3 &v);
inline vec3 cross(vec3 &u, vec3 &v);
inline vec3 unit_vector(vec3 v);
}
