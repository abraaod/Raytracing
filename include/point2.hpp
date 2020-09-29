#ifndef _POINT2_
#define _POINT2_

class Point2{
    public:
        int u;
        int v;

        Point2(int u = 0, int v = 0) : u{u}, v{v} {};
        ~Point2() = default;
};

#endif