#ifndef _CAMERA_
#define _CAMERA_

#include <string>
#include "./vec.hpp"
#include "./ray.hpp"

class Camera
{
private:
    /* data */
    std::string type;

    Ray generate_ray(int x, int y);
    Ray generate_ray(float x, float y);
    
    Vec gaze;
    Vec vup;
    Vec fovy;

public:
    Camera(std::string type);
    ~Camera();
};

Camera::Camera(std::string type)
{
    this->type = type;
}

Camera::~Camera() = default;

class PerspectiveCamera : public Camera{

};

class OrthograficCamera : public Camera{

};

#endif