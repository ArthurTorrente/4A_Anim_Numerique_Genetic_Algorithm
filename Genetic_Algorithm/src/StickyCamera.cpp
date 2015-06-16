#include "StickyCamera.h"
#include "cinder/gl/gl.h"

StickyCamera::StickyCamera()
    : m_eye(0.0f, 0.0f, 0.0f),
    m_center(0.f, 0.f, 0.f),
    m_up(cinder::Vec3f::yAxis()),
    m_cameraDistance(0.0f),
    m_rotation(0.0f, 0.0f, 0.0f)
{}

void StickyCamera::setup(float fov, float aspectRatio, float _near, float _far, float cameraDistance)
{
    m_cameraDistance = cameraDistance;

    m_eye.set(0.f, 0.f, m_cameraDistance);

    m_camera.setPerspective(fov, aspectRatio, _near, _far);
    m_camera.lookAt(m_eye, m_center, m_up);
    
    cinder::gl::setMatrices(m_camera);
    cinder::gl::rotate(m_rotation);
}

void StickyCamera::update()
{
    m_eye.set(0.0f, 0.0f, m_cameraDistance);

    m_camera.lookAt(m_eye, m_center, m_up);

    cinder::gl::setMatrices(m_camera);
    cinder::gl::rotate(m_rotation);
}

float& StickyCamera::getCameraDistance()
{
    return m_cameraDistance;
}

float StickyCamera::getCameraDistance() const
{
    return m_cameraDistance;
}

cinder::Quatf& StickyCamera::getRotation()
{
    return m_rotation;
}

const cinder::Quatf& StickyCamera::getRotation() const
{
    return m_rotation;
}

cinder::Vec3f& StickyCamera::getEye()
{
    return m_eye;
}

cinder::Vec3f& StickyCamera::getCenter()
{
    return m_center;
}

cinder::Vec3f& StickyCamera::getUp()
{
    return m_up;
}