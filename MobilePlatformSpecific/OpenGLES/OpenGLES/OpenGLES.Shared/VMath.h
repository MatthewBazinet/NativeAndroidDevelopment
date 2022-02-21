#pragma once
struct vec3
{
    float x;
    float y;
    float z;

    vec3() : x(0.0f), y(0.0f), z(0.0f) { }
    vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) { }
    explicit vec3(float S) : x(S), y(S), z(S) { }
    vec3 operator - () const { return vec3(-x, -y, -z); }
    vec3 operator + (const vec3& rhs) const { return vec3(x + rhs.x, y + rhs.y, z + rhs.z); }
    vec3 operator * (const vec3& rhs) const { return vec3(x * rhs.x, y * rhs.y, z * rhs.z); }
    vec3 operator - (const vec3& rhs) const { return vec3(x - rhs.x, y - rhs.y, z - rhs.z); }
    vec3 operator * (const float s)  const { return vec3(x * s, y * s, z * s); }
    vec3 operator / (const float s)  const { return vec3(x / s, y / s, z / s); }

    vec3& operator += (const vec3& rhs) { *this = *this + rhs; return *this; }
    vec3& operator *= (const vec3& rhs) { *this = *this * rhs; return *this; }
    vec3& operator -= (const vec3& rhs) { *this = *this - rhs; return *this; }

    float& operator [] (unsigned int i) { return (&x)[i]; }
    const float& operator [] (unsigned int i) const { return (&x)[i]; }
};

 struct vec2
 {
     float x;
     float y;

     vec2() : x(0.0f), y(0.0f) { }
     vec2(float X, float Y) : x(X), y(Y) { }
     explicit vec2(float S) : x(S), y(S) { }
     vec2 operator + (const vec2 & rhs) const { return vec2(x + rhs.x, y + rhs.y); }
     vec2 operator * (const vec2 & rhs) const { return vec2(x * rhs.x, y * rhs.y); }
     vec2 operator - (const vec2 & rhs) const { return vec2(x - rhs.x, y - rhs.y); }
     vec2 operator * (const float s)  const { return vec2(x * s, y * s); }
     vec2 operator / (const float s)  const { return vec2(x / s, y / s); }

     vec2 & operator *= (const float s) { *this = *this * s; return *this; }
     vec2 & operator += (const vec2 & rhs) { *this = *this + rhs; return *this; }
     vec2 & operator *= (const vec2 & rhs) { *this = *this * rhs; return *this; }
     vec2 & operator -= (const vec2 & rhs) { *this = *this - rhs; return *this; }

     float& operator [] (unsigned int i) { return (&x)[i]; }
     const float& operator [] (unsigned int i) const { return (&x)[i]; }
 };
