#ifndef SIMPLEOBJECT_H
#define SIMPLEOBJECT_H

#include "visualobject.h"

/* A general class with several primitives available for models */

enum class Type{
    Cube,
    Plane,
    Triangle,
    Axis
};

class SimpleObject : public VisualObject
{
public:
    SimpleObject();
    SimpleObject(Type type, Shader* shader);
    SimpleObject(Type, float size, Shader* shader);

    void init() override;
    void draw() override;
    void draw(QMatrix4x4& projectionMatrix, QMatrix4x4& viewMatrix) override;


    void PlaceObject(QVector3D location);
    void PlaceObject(float x, float y);
    void PlaceObject(float x, float y, float z);

    void setName(std::string name){ m_name = name; };

    void rotateToCamera(QVector3D cameraPos);

    virtual bool onCollision(VisualObject* collisionObject)/* override*/;

    virtual void MoveToHeight(QVector3D Zvec);

protected:
    void constructCube(float i);
    void constructPlane(float i);
    void constructTriangle(float i);
    void constructAxis();


private:
    Type m_type;


    QVector3D up{0, 0, 1};
    float prevAngle{};
};

#endif // SIMPLEOBJECT_H
