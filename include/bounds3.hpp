#ifndef _BOUNDS3_
#define _BOUNDS3_

#include "vec.hpp"
#include <limits>
using Point = Vec;

class Bounds3 {
    public:

        Point pMin;
        Point pMax;
        
        Bounds3(){
            float minNum = std::numeric_limits<float>::lowest();
            float maxNum = std::numeric_limits<float>::max();
            pMin = Point(maxNum,maxNum,maxNum);
            pMax = Point(minNum,minNum,minNum);
        }

        Bounds3(const Point &p) : pMin(p), pMax(p) {}

        Bounds3(const Point &p1, const Point &p2):
                        pMin(std::min(p1.v1, p2.v1), std::min(p1.v2, p2.v2),
                             std::min(p1.v3, p2.v3)),
                        pMax(std::max(p1.v1, p2.v1), std::max(p1.v2, p2.v2),
                             std::max(p1.v3, p2.v3)) {}

        const Point &operator[](int i) const;
        Point &operator[](int i);
        
        bool operator==(const Bounds3 &b) const {
            return b.pMin == pMin && b.pMax == pMax;
        }

        bool operator!=(const Bounds3 &b) const {
            return b.pMin != pMin || b.pMax != pMax;
        }

        Point corner(int corner) const {
            return Point((*this)[(corner & 1)].v1,
                         (*this)[(corner & 2) ? 1 : 0].v2,
                         (*this)[(corner & 4) ? 1 : 0].v3);
        }



        Vec diagonal() {
            Vec p = pMax - pMin;
            return p;
        }
        
        float surfaceArea(){
            Vec d = diagonal();
            return 2 * (d.v1 * d.v2 + d.v1 * d.v3 + d.v2 * d.v3);
        }

        float volume(){
            Vec d = diagonal();
            return d.v1 * d.v2 * d.v3;
        }
        
        int maximumExtent(){
            Vec d = diagonal();
            if(d.v1 > d.v2 && d.v1 > d.v3)
                return 0;
            else if(d.v2 > d.v3)
                return 1;
            else
                return 2;
        }

        Vec offset(Point &p) {
            Vec o = p - pMin;
            if(pMax.v1 > pMin.v1) o.v1 /= pMax.v1 - pMin.v1;
            if(pMax.v2 > pMin.v2) o.v2 /= pMax.v2 - pMin.v2;
            if(pMax.v3 > pMin.v3) o.v3 /= pMax.v3 - pMin.v3;
            return o;
        }

        void boundingSphere(Point *center, float *radius) {
            *center = (pMin + pMax) / 2;
            *radius = inside(*center, *this) ? distance(*center, pMax) : 0;
        }

        bool intersect_p(const Ray & ray, float *hitt0 = nullptr, float *hiit1 = nullptr) const;
        inline bool interset_p(const Ray & ray, const Vec & invDir, const int dirIsneg[3]) const;

        bool inside(const Point &p, const Bounds3 &b) {
            return (p.v1 >= b.pMin.v1 && p.v1 <= b.pMax.v1 &&
                p.v2 >= b.pMin.v2 && p.v2 <= b.pMax.v2 &&
                p.v3 >= b.pMin.v3 && p.v3 <= b.pMax.v3);
        }

        float distance(Point &p1, Point &p2) {
            return (p1 - p2).length();
        }

        Point lerp(Point &t){
            return Point(lerp_f(t.v1, pMin.v1, pMax.v1),
                         lerp_f(t.v2, pMin.v2, pMax.v2),
                         lerp_f(t.v3, pMin.v3, pMax.v3));
        }

        float lerp_f(float t, float v1, float v2){
            return (1-t) * v1 + t * v2;
        }
};

#endif