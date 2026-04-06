// ◦ Xyz ◦
#pragma once

#include <immintrin.h>
#include <cmath>
// TODO: Временно
#include <glm/vec3.hpp>

namespace mystd
{
    struct alignas(16) Vec3
    {
        __m128 m;

        Vec3() : m(_mm_setzero_ps()) {}

        Vec3(float x, float y, float z)
            : m(_mm_set_ps(0.0f, z, y, x)) {
        }

        explicit Vec3(__m128 v) : m(v) {}

        inline float X() const { return _mm_cvtss_f32(m); }

        inline float Y() const
        {
            return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1)));
        }

        inline float Z() const
        {
            return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2)));
        }

        static inline Vec3 Load(const float* p)
        {
            return Vec3(_mm_loadu_ps(p));
        }

        inline void Store(float* p) const
        {
            _mm_storeu_ps(p, m);
        }

        // arithmetic

        inline Vec3 operator+(const Vec3& v) const
        {
            return Vec3(_mm_add_ps(m, v.m));
        }

        inline Vec3 operator-(const Vec3& v) const
        {
            return Vec3(_mm_sub_ps(m, v.m));
        }

        inline Vec3 operator*(float s) const
        {
            return Vec3(_mm_mul_ps(m, _mm_set1_ps(s)));
        }

        inline Vec3 operator/(float s) const
        {
            return Vec3(_mm_mul_ps(m, _mm_set1_ps(1.0f / s)));
        }

        inline Vec3& operator+=(const Vec3& v)
        {
            m = _mm_add_ps(m, v.m);
            return *this;
        }

        inline Vec3& operator-=(const Vec3& v)
        {
            m = _mm_sub_ps(m, v.m);
            return *this;
        }

        inline Vec3& operator*=(float s)
        {
            m = _mm_mul_ps(m, _mm_set1_ps(s));
            return *this;
        }

        // dot

        inline float Dot(const Vec3& v) const
        {
            __m128 dp = _mm_dp_ps(m, v.m, 0x71);
            return _mm_cvtss_f32(dp);
        }

        // length

        inline float Length() const
        {
            __m128 dp = _mm_dp_ps(m, m, 0x71);
            dp = _mm_sqrt_ps(dp);
            return _mm_cvtss_f32(dp);
        }

        // normalize (fast)

        inline Vec3 Normalized() const
        {
            __m128 dp = _mm_dp_ps(m, m, 0x7F);
            __m128 inv = _mm_rsqrt_ps(dp);
            return Vec3(_mm_mul_ps(m, inv));
        }

        // cross

        inline Vec3 Cross(const Vec3& v) const
        {
            __m128 a = m;
            __m128 b = v.m;

            __m128 a_yzx = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 b_yzx = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));

            __m128 c = _mm_sub_ps(
                _mm_mul_ps(a, b_yzx),
                _mm_mul_ps(a_yzx, b)
            );

            return Vec3(_mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1)));
        }

        static inline float Distance(const Vec3& a, const Vec3& b)
        {
            __m128 diff = _mm_sub_ps(b.m, a.m);
            __m128 dp = _mm_dp_ps(diff, diff, 0x71);
            dp = _mm_sqrt_ps(dp);
            return _mm_cvtss_f32(dp);
        }
    };
}
