#ifndef _BOUNDS3_
#define _BOUNDS3_

#include "vec.hpp"
#include <limits>
#include <algorithm>
using Point = Vec;

class Bounds3
{
public:
    Point pMin;
    Point pMax;

    Bounds3()
    {
        float minNum = std::numeric_limits<float>::lowest();
        float maxNum = std::numeric_limits<float>::max();
        pMin = Point(maxNum, maxNum, maxNum);
        pMax = Point(minNum, minNum, minNum);
    }

    Bounds3(const Point &p) : pMin(p), pMax(p) {}

    Bounds3(const Point &p1, const Point &p2) : pMin(std::min(p1.v1, p2.v1), std::min(p1.v2, p2.v2),
                                                     std::min(p1.v3, p2.v3)),
                                                pMax(std::max(p1.v1, p2.v1), std::max(p1.v2, p2.v2),
                                                     std::max(p1.v3, p2.v3)) {}

    const Point &operator[](int i) const;
    Point &operator[](int i);

    bool operator==(const Bounds3 &b) const
    {
        return b.pMin == pMin && b.pMax == pMax;
    }

    bool operator!=(const Bounds3 &b) const
    {
        return b.pMin != pMin || b.pMax != pMax;
    }

    Point corner(int corner) const
    {
        return Point((*this)[(corner & 1)].v1,
                     (*this)[(corner & 2) ? 1 : 0].v2,
                     (*this)[(corner & 4) ? 1 : 0].v3);
    }

    Vec diagonal()
    {
        Vec p = pMax - pMin;
        return p;
    }

    float surfaceArea()
    {
        Vec d = diagonal();
        return 2 * (d.v1 * d.v2 + d.v1 * d.v3 + d.v2 * d.v3);
    }

    float volume()
    {
        Vec d = diagonal();
        return d.v1 * d.v2 * d.v3;
    }

    int maximumExtent()
    {
        Vec d = diagonal();
        if (d.v1 > d.v2 && d.v1 > d.v3)
            return 0;
        else if (d.v2 > d.v3)
            return 1;
        else
            return 2;
    }

    Vec offset(Point &p)
    {
        Vec o = p - pMin;
        if (pMax.v1 > pMin.v1)
            o.v1 /= pMax.v1 - pMin.v1;
        if (pMax.v2 > pMin.v2)
            o.v2 /= pMax.v2 - pMin.v2;
        if (pMax.v3 > pMin.v3)
            o.v3 /= pMax.v3 - pMin.v3;
        return o;
    }

    void boundingSphere(Point *center, float *radius)
    {
        *center = (pMin + pMax) / 2;
        *radius = inside(*center, *this) ? distance(*center, pMax) : 0;
    }

    bool intersect_p(Ray &ray, float *hitt0 = nullptr, float *hitt1 = nullptr)
    {
        float t0 = ray.tmin;
        float t1 = ray.tmax;

        float invRayDir_v1 = 1.f / ray.getDirection().v1;
        float tNear_v1 = invRayDir_v1 * (pMin.v1 - ray.getOrigin().v1);
        float tFar_v1 = (pMax.v1 - ray.getOrigin().v1) * invRayDir_v1;

        if(tNear_v1 > tFar_v1) std::swap(tNear_v1, tFar_v1);
        t0 = tNear_v1 > t0 ? tNear_v1 : t0;
        t1 = tFar_v1 < t1 ? tFar_v1 : t1;
        if(t0 > t1) return false;

        float invRayDir_v2 = 1.f / ray.getDirection().v2;
        float tNear_v2 = invRayDir_v2 * (pMin.v2 - ray.getOrigin().v2);
        float tFar_v2 = (pMax.v2 - ray.getOrigin().v2) * invRayDir_v2;

        if(tNear_v2 > tFar_v2) std::swap(tNear_v2, tFar_v2);
        t0 = tNear_v2 > t0 ? tNear_v2 : t0;
        t1 = tFar_v2 < t1 ? tFar_v2 : t1;
        if(t0 > t1) return false;

        float invRayDir_v3 = 1.f / ray.getDirection().v3;
        float tNear_v3 = invRayDir_v3 * (pMin.v3 - ray.getOrigin().v3);
        float tFar_v3 = (pMax.v3 - ray.getOrigin().v3) * invRayDir_v3;

        if(tNear_v3 > tFar_v3) std::swap(tNear_v3, tFar_v3);
        t0 = tNear_v3 > t0 ? tNear_v3 : t0;
        t1 = tFar_v3 < t1 ? tFar_v3 : t1;
        if(t0 > t1) return false;

        if (hitt0)
            *hitt0 = t0;
        if (hitt1)
            *hitt1 = t1;
        return true;
    }
    inline bool interset_p(const Ray &ray, const Vec &invDir, const int dirIsneg[3]) const;

    bool inside(const Point &p, const Bounds3 &b)
    {
        return (p.v1 >= b.pMin.v1 && p.v1 <= b.pMax.v1 &&
                p.v2 >= b.pMin.v2 && p.v2 <= b.pMax.v2 &&
                p.v3 >= b.pMin.v3 && p.v3 <= b.pMax.v3);
    }

    float distance(Point &p1, Point &p2)
    {
        return (p1 - p2).length();
    }

    Point lerp(Point &t)
    {
        return Point(lerp_f(t.v1, pMin.v1, pMax.v1),
                     lerp_f(t.v2, pMin.v2, pMax.v2),
                     lerp_f(t.v3, pMin.v3, pMax.v3));
    }

    float lerp_f(float t, float v1, float v2)
    {
        return (1 - t) * v1 + t * v2;
    }
};

//Realiza a união de 2 bounds
Bounds3 unionBounds(const Bounds3 &b1, const Bounds3 &b2)
{
    return Bounds3(Point(std::min(b1.pMin.v1, b2.pMin.v1),
                         std::min(b1.pMin.v2, b2.pMin.v2),
                         std::min(b1.pMin.v3, b2.pMin.v3)),
                   Point(std::max(b1.pMax.v1, b2.pMax.v1),
                         std::max(b1.pMax.v2, b2.pMax.v2),
                         std::max(b1.pMax.v3, b2.pMax.v3)));
}

Bounds3 unionBounds(const Bounds3 &b, const Point &p)
{
    return Bounds3(Point(std::min(b.pMin.v1, p.v1),
                         std::min(b.pMin.v2, p.v2),
                         std::min(b.pMin.v3, p.v3)),
                   Point3(std::max(b.pMax.v1, p.v1),
                          std::max(b.pMax.v2, p.v2),
                          std::max(b.pMax.v3, p.v3)));
}

#endif