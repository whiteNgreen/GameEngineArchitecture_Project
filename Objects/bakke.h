#ifndef BAKKE_H
#define BAKKE_H

#include <visualobject.h>


class Bakke : public VisualObject
{
private:

public:
    Bakke(){}
    Bakke(Shader* shader);
    ~Bakke(){}

    void CreateTriangle(const QVector3D v1,const QVector3D v2,const QVector3D v3);

    virtual int BarySentricCoordinate(const QVector3D ObjectPosition, QVector3D& TheBarysentricCoordinates, QVector3D& SurfacePosition, QVector3D& SurfaceNormal, const int TriangleIndex);
    virtual int BarySentricCoordinate(const QVector3D ObjectPosition, QVector3D& TheBarysentricCoordinates, QVector3D& SurfacePosition, QVector3D& SurfaceNormal);
    QVector3D GetCoordinateWithBaryc(const QVector3D& Baryc, const int index);
    QVector3D GetNormalFromIndex(const int Index);

    unsigned int mTriangleIndex{0};
};

#endif // BAKKE_H
