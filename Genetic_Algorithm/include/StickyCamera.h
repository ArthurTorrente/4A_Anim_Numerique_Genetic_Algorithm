#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "cinder/Camera.h"

class StickyCamera
{
public:
    StickyCamera();

    void setup(float fov, float aspectRatio, float _near, float _far, float cameraDistance);
    void update();

    float& getCameraDistance();
    float getCameraDistance() const;

    cinder::Quatf& getRotation();
    const cinder::Quatf& getRotation() const;

    cinder::Vec3f& getEye();
    cinder::Vec3f& getCenter();
    cinder::Vec3f& getUp();


protected:
    cinder::CameraPersp m_camera;

    cinder::Vec3f m_eye;
    cinder::Vec3f m_center;
    cinder::Vec3f m_up;

    float m_cameraDistance;
    cinder::Quatf m_rotation;
};

#endif //_CAMERA_H_