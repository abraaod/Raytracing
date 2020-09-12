#ifndef _CAMERA_
#define _CAMERA_

#include <string>

class Camera
{
private:
    /* data */
    std::string type;

public:
    Camera();
    Camera(std::string type);
    ~Camera();
};

Camera::Camera(){

}

Camera::Camera(std::string type)
{
    this->type = type;
}

Camera::~Camera() = default;

#endif
