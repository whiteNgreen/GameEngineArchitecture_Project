#include "camera.h"

Camera::Camera() : mPosition(-2, -4, 25), mLookAt(-2,-2,0)
{
    mViewMatrix.setToIdentity();
    mProjectionMatrix.setToIdentity();

    /* Setter pitch og yaw verdier basert på mPosition og mLookAt */
    {
        QVector3D V{ mLookAt - mPosition };
        V.normalize();

        /* pitch */
        float P = QVector3D::dotProduct(QVector3D(0,0,1), V);
        pitch = acosf(P) * (180/M_PI) * -1.f;

        /* yaw */
        QVector2D F{ V.toVector2D() * -1.f };
        F.normalize();
        float Y = QVector2D::dotProduct(QVector2D(1,0), F);
        yaw = acosf(Y) * (180/M_PI);

        /* Sjekker om yaw er til høyre eller venstre */
        {
            float D = QVector2D::dotProduct(QVector2D(0,1), F);
            if (D < 0.f) { yaw *= -1.f; }
        }
    }

    updateForwardVector();
}

void Camera::update()
{
    mViewMatrix.setToIdentity();
    mProjectionMatrix.setToIdentity();

    mForward.normalize();
    mUp.normalize();
    mRight.normalize();


    if (!bFollowPlayer)
        mLookAt = mPosition + mForward;

    mViewMatrix.lookAt(mPosition, mLookAt, globalUp);
}

void Camera::move( const float x, const float y, const float z )
{
    mPosition += mRight * x * cameraMovementSpeed;
    mPosition += mForward * y * cameraMovementSpeed;
    mPosition += globalUp * z * cameraMovementSpeed;
}

void Camera::Pitch(float i)
{
    pitch -= i * cameraRotationSpeed;

    if(pitch > -1.f)
        pitch = -1.f;
    if(pitch < -179.f)
        pitch = -179.f;

    updateForwardVector();
}

void Camera::Yaw(float i)
{
    yaw -= i * cameraRotationSpeed;
    updateForwardVector();
}

void Camera::updateForwardVector()
{
    float pitchRad = pitch * (M_PI/180);
    float yawRad = yaw * (M_PI/180);


    // Rotate LEFT Yaw
    mRight = QVector3D(0,1,0);
    QVector3D dir;
    dir.setX((mRight.x() * cosf(yawRad)) - (mRight.y() * sinf(yawRad)));
    dir.setY((mRight.x() * sinf(yawRad)) + (mRight.y() * cosf(yawRad)));
    dir.setZ(mRight.z());
    mRight = dir;
    mRight.normalize();

//    // Rotate UP Pitch
    mUp = QVector3D(1,0,0);
    dir = QVector3D(0,0,0);
    dir.setX((mUp.x() * cosf(pitchRad)) + (mUp.z() * sinf(pitchRad)));
    dir.setY(mUp.y());
    dir.setZ((mUp.x() * -sinf(pitchRad)) + (mUp.z() * cosf(pitchRad)));
    mUp = dir;
//    // Rotate UP Yaw
    QVector3D dir2;
    dir2.setX((mUp.x() * cosf(yawRad)) - (mUp.y() * sinf(yawRad)));
    dir2.setY((mUp.x() * sinf(yawRad)) + (mUp.y() * cosf(yawRad)));
    dir2.setZ(mUp.z());
    mUp = dir2;
    mUp.normalize();


    // Set FORWARD vector
    mForward = mForward.crossProduct(mUp, mRight);
    mForward.normalize();

    updateLeftVector();
}

void Camera::updateLeftVector()
{
    mRight = mRight.crossProduct(mForward, mUp);
    mRight.normalize();
}

void Camera::followPlayer(QVector3D playerPosition, QVector3D cameraPosition)
{
    mLookAt = playerPosition;
    mPosition = cameraPosition;

//    bFollowPlayer = true;
}

bool Camera::isFollowingPlayer()
{
    return bFollowPlayer;
}

void Camera::setFollowPlayer(bool b)
{
    bFollowPlayer = b;
}


