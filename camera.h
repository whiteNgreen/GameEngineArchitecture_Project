#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

class Camera
{
public:
    Camera();

    void update();

    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;

    QVector3D mPosition;
    QVector3D mLookAt;

    void move( const float x, const float y, const float z );

    void Pitch(float i);
    void Yaw(float i);

    void updateForwardVector();
    void updateLeftVector();

    void followPlayer(QVector3D playerPosition, QVector3D cameraPosition);
    bool isFollowingPlayer();
    void setFollowPlayer(bool b);

    QVector3D getForwardVector(){ return mForward; }
    QVector3D getRightVector(){ return mRight; }
    QVector3D getUpVector(){ return mUp; };


private:
    QVector3D mForward{0, 1, 0};
    QVector3D mUp{0, 0, 1};
    QVector3D mRight{1, 0, 0};

    QVector3D globalUp{0, 0, 1};

    float pitch{-90.f};
    float yaw{-45.f};

    float cameraRotationSpeed = 0.2f;
    float cameraMovementSpeed = 0.5f;

    bool bFollowPlayer{false};
};

#endif // CAMERA_H
