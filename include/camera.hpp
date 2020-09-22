#ifndef _CAMERA_
#define _CAMERA_

#include <string>
#include "./vec.hpp"
#include "./ray.hpp"

class Camera
{
private:

public:

    Ray generate_ray(int x, int y);
    Ray generate_ray(float x, float y);

    std::string type;

    float l;
    float r;
    float b;
    float t;

    float fovy;

    Vec gaze;
    Vec w;
    Vec u;
    Vec v;
    Vec e;

    float u_;
    float v_;

    int height;
    int width;

    explicit Camera(std::string type, std::string screen, Vec lookat, Vec lookfrom, Vec vup);
    explicit Camera(std::string type, float fovy, Vec lookat, Vec lookfrom, Vec vup);
    void initializeScreen(std::string screen);
    void initializeFrame(Vec lookat, Vec lookfrom, Vec vup);
    void setHeightWidth(int height, int width);
    ~Camera();
};

Camera::Camera(std::string type, std::string screen, Vec lookat, Vec lookfrom, Vec vup)
{
    this->type = type;
    initializeScreen(screen);
    initializeFrame(lookat, lookfrom, vup);
}

Camera::Camera(std::string type, float fovy, Vec lookat, Vec lookfrom, Vec vup)
{
    this->type = type;
    initializeFrame(lookat, lookfrom, vup);
}

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

    u.print();
    v.print();
}

void Camera::setHeightWidth(int height, int width){
    this->height = height;
    this->width = width;
}

Camera::~Camera() = default;

// class PerspectiveCamera : public Camera{
    
//     public:
//         Ray generate_ray(int x, int y){

//         }

//         Ray generate_ray(float x, float y){

//         }
// };

class OrthograficCamera : public Camera{
    public:
        using Camera::Camera;
        Ray generate_ray(int x, int y){
            u_ = l + (r - l) * ((x+0.5)/width);
            v_ = b + (t - b) * ((y+0.5)/height);
            std::cout  << u_ << " " << v_ << std::endl;
            Ray r(e + u*u_ + v*v_, w);
            return r;

        }

        Ray generate_ray(float x, float y){
            u_ = l + (r - l) * ((x+0.5)/width);
            v_ = b + (t - b) * ((y+0.5)/height);
            Ray r(e + u*u_ + v*v_, w);
            return r;
        }
};

#endif