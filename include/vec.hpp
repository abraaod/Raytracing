#ifndef _VEC_
#define _VEC_

#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>

class Vec
{
public:
    float v1; // x
    float v2; // y
    float v3; // z
    float v4; // a

    Vec()
    {
        v1 = 0.0;
        v2 = 0.0;
        v3 = 0.0;
        v4 = 0.0;
    }

    Vec(float v1, float v2, float v3)
    {
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
        this->v4 = 0;
    }

    Vec(float v1, float v2, float v3, float v4)
    {
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
        this->v4 = v4;
    }

    Vec(std::vector<std::string> vetor)
    {
        this->v1 = std::stof(vetor[0]);
        this->v2 = std::stof(vetor[1]);
        this->v3 = std::stof(vetor[2]);
        this->v4 = std::stof(vetor[3]);
    }

    Vec(std::string vetor)
    {
        if(vetor != ""){
        std::istringstream iss(vetor);
        std::vector<std::string> splited((std::istream_iterator<std::string>(iss)),
                                         std::istream_iterator<std::string>());
        float v1_;
        std::istringstream(splited[0]) >> v1_;
        float v2_;
        std::istringstream(splited[1]) >> v2_;
        float v3_;
        std::istringstream(splited[2]) >> v3_;
        // int v4_;
        // std::istringstream(splited[3]) >> v4_;
        
        this->v1 = v1_;
        this->v2 = v2_;
        this->v3 = v3_;
        } else{
            v1 = 0.0;
        v2 = 0.0;
        v3 = 0.0;
        v4 = 0.0;
        }
    }

    Vec(std::vector<float> vetor)
    {
        this->v1 = vetor[0];
        this->v2 = vetor[1];
        this->v3 = vetor[2];
        this->v4 = vetor[3];
    }

    void print()
    {
        std::cout << "(" << v1 << ", " << v2 << ", " << v3 << ", " << v4 << ")\n";
    }

    bool operator!=(const Vec &v_) const
    {
        if ((v1 != v_.v1) && (v2 != v_.v2) && (v3 != v_.v3) && (v4 != v_.v4))
        {
            return true;
        }

        return false;
    }

    bool isBlack()
    {
        if (v1 == 0 && v2 == 0 && v3 == 0 && v4 == 0)
        {
            return true;
        }

        return false;
    }

    Vec operator+(const Vec &v_)
    {
        float v_v1 = v1 + v_.v1;
        float v_v2 = v2 + v_.v2;
        float v_v3 = v3 + v_.v3;
        float v_v4 = v4 + v_.v4;

        return Vec(v_v1, v_v2, v_v3, v_v4);
    }

    Vec operator-()
    {
        return Vec(-v1, -v2, -v3);
    }

    Vec operator-(const Vec &v_)
    {
        float v_v1 = v1 - v_.v1;
        float v_v2 = v2 - v_.v2;
        float v_v3 = v3 - v_.v3;
        float v_v4 = v4 - v_.v4;

        return Vec(v_v1, v_v2, v_v3, v_v4);
    }

    Vec operator*(const float &v_) const
    {
        float v_v1 = v1 * v_;
        float v_v2 = v2 * v_;
        float v_v3 = v3 * v_;
        float v_v4 = 0.0;

        return Vec(v_v1, v_v2, v_v3, v_v4);
    }

    Vec operator*(const Vec &v_) const
    {
        float v_v1 = v1 * v_.v1;
        float v_v2 = v2 * v_.v2;
        float v_v3 = v3 * v_.v3;

        return Vec(v_v1, v_v2, v_v3, 0.0);
    }

    std::string toRGB()
    {
        int v1_ = static_cast<int>(v1 * 255);
        int v2_ = static_cast<int>(v2 * 255);
        int v3_ = static_cast<int>(v3 * 255);
        return std::to_string(v1_) + " " + std::to_string(v2_) + " " + std::to_string(v3_);
    }

    std::string toRGBA()
    {
        int v1_ = static_cast<int>(v1 * 255);
        int v2_ = static_cast<int>(v2 * 255);
        int v3_ = static_cast<int>(v3 * 255);
        return std::to_string(v1_) + " " + std::to_string(v2_) + " " + std::to_string(v3_) + " 255";
    }

    float length() const
    {
        return sqrt(v1 * v1 + v2 * v2 + v3 * v3);
    }

    Vec &operator/=(const Vec &v)
    {
        v1 /= v.v1;
        v2 /= v.v2;
        v3 /= v.v3;
        return *this;
    }

    Vec &operator/=(const float t)
    {
        float k = 1.0 / t;
        v1 *= k;
        v2 *= k;
        v3 *= k;
        return *this;
    }
};

// Normalização do vetor
inline Vec normalize(const Vec &v)
{
    float sq_v1 = v.v1 * v.v1;
    float sq_v2 = v.v2 * v.v2;
    float sq_v3 = v.v3 * v.v3;

    float root = sqrt(sq_v1 + sq_v2 + sq_v3);

    float r_v1 = v.v1 / root;
    float r_v2 = v.v2 / root;
    float r_v3 = v.v3 / root;

    return Vec(r_v1, r_v2, r_v3);
}

inline float magnitude(const Vec &v){
    float sq_v1 = v.v1 * v.v1;
    float sq_v2 = v.v2 * v.v2;
    float sq_v3 = v.v3 * v.v3;

    float root = sqrt(sq_v1 + sq_v2 + sq_v3);
    return root;
}

inline Vec cross(const Vec &v1, const Vec &v2){
    return Vec((v1.v2 * v2.v3 - v1.v3 * v2.v2), 
    ((v1.v3 * v2.v1 - v1.v1 * v2.v3)),
    (v1.v1 * v2.v2 - v1.v2 * v2.v1));
}

inline float dot(const Vec &v1, const Vec &v2)
{
    return v1.v1 * v2.v1 + v1.v2 * v2.v2 + v1.v3 * v2.v3;
}

inline Vec operator/(const Vec &v1, const Vec &v2)
{
    return Vec(v1.v1 / v2.v1, v1.v2 / v2.v2, v1.v3 / v2.v3);
}

inline Vec operator/(Vec v, float t)
{
    return Vec(v.v1 / t, v.v2 / t, v.v3 / t);
}

inline Vec unique_vector(Vec v)
{
    return v / v.length();
}

#endif
