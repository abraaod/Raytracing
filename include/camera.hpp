#ifndef _CAMERA_
#define _CAMERA_

#include <string>
#include <math.h>
#include "./vec.hpp"
#include "./ray.hpp"

class Camera
{
private:

public:

    virtual Ray generate_ray(int x, int y) {}
    virtual Ray generate_ray(float x, float y) {}

    std::string type;

    float l;
    float r;
    float b;
    float t;

    float fovy = 0;

    Vec gaze;
    Vec w;
    Vec u;
    Vec v;
    Vec e;

    float u_;
    float v_;

    int height;
    int width;

    Camera(std::string type, std::string screen, Vec lookat, Vec lookfrom, Vec vup) {}
    Camera(std::string type, float fovy, Vec lookat, Vec lookfrom, Vec vup) {}
    void initializeScreen(std::string screen);
    void initializeFrame(Vec lookat, Vec lookfrom, Vec vup);
    void setHeightWidth(int height, int width);
    virtual ~Camera() = default;
};

void Camera::initializeScreen(std::string screen){
    if(screen != ""){
        std::istringstream iss(screen);
            std::vector<std::string> splited((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());

            std::istringstream(splited[0]) >> l;
            std::istringstream(splited[1]) >> r;
            std::istringstream(splited[2]) >> b;
            std::istringstream(splited[3]) >> t;

            std::cout << l << " " << r << " " << b << " "  << t << std::endl;
    }
}

void Camera::initializeFrame(Vec lookat, Vec lookfrom, Vec vup){
    this->gaze = lookat - lookfrom;
    this->w = normalize(gaze);
    this->u = normalize(cross(vup, w));
    this->v = normalize(cross(w, u));
    this->e = lookfrom;

    // u.print();
    // v.print();
}

void Camera::setHeightWidth(int height, int width){
    
    this->height = height;
    this->width = width;

    if(fovy > 0){
        float ratio = (float) width/ (float) height;
        float h = tan((float) fovy/2.0);

        this->l = -ratio * h;
        this->r = ratio * h;
        this->t = h;
        this->b = -h;
    }
}

class PerspectiveCamera : public Camera{
    
    public:

        PerspectiveCamera(std::string type, float fovy, Vec lookat, Vec lookfrom, Vec vup) : Camera(type, fovy, lookat, lookfrom, vup){
            this->type = type;
            this->fovy = fovy;
            initializeFrame(lookat, lookfrom, vup);
        }

        Ray generate_ray(int x, int y){
            u_ = l + (r - l) * ((float(x)+0.5)/(float)width);
            v_ = b + (t - b) * ((float(y)+0.5)/(float)height);
            Ray r(w + u*u_ + v*v_, e);
            return r;
        }

        Ray generate_ray(float x, float y){
            u_ = l + (r - l) * ((x+0.5)/width);
            v_ = b + (t - b) * ((y+0.5)/height);
            Ray r(w + u*u_ + v*v_, e);
            return r;
        }

        virtual ~PerspectiveCamera() = default;
};

class OrthograficCamera : public Camera{
    public:
        
        OrthograficCamera(std::string type, std::string screen, Vec lookat, Vec lookfrom, Vec vup) : Camera(type, screen, lookat, lookfrom, vup){
            this->type = type;
            initializeScreen(screen);
            initializeFrame(lookat, lookfrom, vup);
        }

        Ray generate_ray(int x, int y){
            u_ = l + (r - l) * ((x+0.5)/width);
            v_ = b + (t - b) * ((y+0.5)/height);
            //std::cout  << u_ << " " << v_ << std::endl;
            Ray r(e + u*u_ + v*v_, w);
            return r;

        }

        Ray generate_ray(float x, float y){
            u_ = l + (r - l) * ((x+0.5)/width);
            v_ = b + (t - b) * ((y+0.5)/height);
            Ray r(e + u*u_ + v*v_, w);
            return r;
        }

        virtual ~OrthograficCamera() = default;
};

#endif