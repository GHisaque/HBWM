#pragma once

#ifndef GIP_MATH_H
#define GIP_MATH_H

#include <math.h>
#include "../Types.h"

#define PI 3.141592653589793
#define TAU PI*2
#define PHI 1.618033988749895
#define EU_NUMBR 2.718281828459045
#define EU_CONST 0.577215664901532

static inline float clamp(float x, float min_val, float max_val) {
    if (x > max_val) return max_val;
    if (x < min_val) return min_val;
    return x;
}

static inline float deg_to_rad(float degrees) {
    return degrees * (PI / 180.f);
}

static inline float rad_to_deg(float radians) {
    return radians * (180.f / PI);
}

// --- Vectors ---

typedef struct {
    float x, y;
} vec2;

static inline vec2 vec2_make(float x, float y) { return (vec2){x, y}; }
static inline vec2 vec2_add(vec2 a, vec2 b) { return (vec2){a.x + b.x, a.y + b.y}; }
static inline vec2 vec2_sub(vec2 a, vec2 b) { return (vec2){a.x - b.x, a.y - b.y}; }
static inline vec2 vec2_mul(vec2 a, vec2 b) { return (vec2){a.x * b.x, a.y * b.y}; }
static inline vec2 vec2_div(vec2 a, vec2 b) { return (vec2){a.x / b.x, a.y / b.y}; }

static inline float vec2_len_squ(vec2 v) { return (v.x * v.x)+(v.y * v.y); }
static inline float vec2_len(vec2 v) { return sqrtf(vec2_len_squ(v)); }
static inline vec2 vec2_norm(vec2 v) {
    float mag = vec2_len(v);
    if (mag <= .00001f) return (vec2){0, 0};
    return (vec2){
        v.x / mag,
        v.y / mag
    };
}

static inline vec2 vec2_neg(vec2 v) { return (vec2){-v.x, -v.y}; }

// --- RGBA ---
typedef struct {
    uchar r, g, b, a;
} RGBA;

static inline RGBA rgba(uchar r, uchar g, uchar b, uchar a) {
    return (RGBA){r, g, b, a};
}

static inline RGBA rgb(uchar r, uchar g, uchar b) {
    return (RGBA){r, g, b, 255};
}

static inline uint rgba_to_base10(RGBA c) {
    return ((uint)c.a << 24) | ((uint)c.r << 16) | ((uint)c.g << 8) | (uint)c.b;
}

static inline RGBA rgba_blend(RGBA base, RGBA c) {
    if (c.a <= 0) return base;
    if (c.a >= 255) return c;
    
    float fa = c.a / 255.0f;
    float ba = base.a / 255.0f;
    float out_a = fa + ba * (1.0f - fa);

    if (out_a <= 0.0f) return (RGBA){0, 0, 0, 0};

    float out_r = (c.r * fa + base.r * ba * (1.0f - fa)) / out_a;
    float out_g = (c.g * fa + base.g * ba * (1.0f - fa)) / out_a;
    float out_b = (c.b * fa + base.b * ba * (1.0f - fa)) / out_a;

    return (RGBA){
        (uchar)clamp(out_r, 0.0f, 255.0f),
        (uchar)clamp(out_g, 0.0f, 255.0f),
        (uchar)clamp(out_b, 0.0f, 255.0f),
        (uchar)clamp(out_a * 255.0f, 0.0f, 255.0f)
    };
}

#endif