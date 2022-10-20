#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "visualobject.h"

class TriangleSurface : public VisualObject
{
private:
    QVector3D NormalVector;
public:
    TriangleSurface();
    ~TriangleSurface() {}

    void MakeTriangle(QVector3D v1, QVector3D v2, QVector3D v3);
    void ChangeTriangleLocation(QVector3D p1, QVector3D p2, QVector3D p3);
    void CalculateNormalOfTriangle(QVector3D AlignmentVector);

    QVector3D GetTriangleNormal() const { return NormalVector; }

    void draw() override
    {
        VisualObject::draw();
    }
};

#endif // TRIANGLESURFACE_H
