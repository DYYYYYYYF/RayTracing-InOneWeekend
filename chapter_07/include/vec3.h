#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

inline double random_double(){
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max){
    return min + (max - min) * random_double();
}



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

    inline static vec3 random(){
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max){
        return vec3(random_double(min, max),random_double(min, max),random_double(min, max));
    }

};

using point3 = vec3;  // 3D point
using color = vec3; // RGB color

//Util Functions
inline std::ostream& operator<<(std::ostream &out, const vec3 &v){
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v){
    return vec3(u.e[0]+v.e[0], u.e[1]+v.e[1], u.e[2]+v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v){
    return vec3(u.e[0]-v.e[0], u.e[1]-v.e[1], u.e[2]-v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v){
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t){
    return t * v;
}

inline vec3 operator/(const vec3 v, double t){
    return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3 &v){
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v){
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1], 
                u.e[2] * v.e[0] - u.e[0] * v.e[2], 
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v){
    return v / v.length();
}


inline vec3 random_in_unit_sphere(){
    while(true){
        auto p = vec3::random(-1, 1);
        if(p.length_squared() >= 1) continue;
        return p;
    }
}

inline vec3 random_unit_vector(){
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3& normal){
    vec3 in_unit_sphere = random_in_unit_sphere();
    if(dot(in_unit_sphere, normal) > 0.0) return in_unit_sphere;
    else return -in_unit_sphere;
}

#endif

