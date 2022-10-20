#include "trianglesurface.h"

TriangleSurface::TriangleSurface()
{

}

void TriangleSurface::MakeTriangle(QVector3D v1, QVector3D v2, QVector3D v3)
{
    mVertices.push_back(Vertex{v1});
    mVertices.push_back(Vertex{v2});
    mVertices.push_back(Vertex{v3});
}

void TriangleSurface::ChangeTriangleLocation(QVector3D p1, QVector3D p2, QVector3D p3)
{
    mVertices[0].setPosition(p1);
    mVertices[1].setPosition(p2);
    mVertices[2].setPosition(p3);

    mIndices = { 2, 1, 0 };
}

void TriangleSurface::CalculateNormalOfTriangle(QVector3D AlignmentVector)
{
    QVector3D v1{ mVertices[1].getPosition() - mVertices[0].getPosition() };
    QVector3D v2{ mVertices[2].getPosition() - mVertices[0].getPosition() };

    QVector3D normal = QVector3D::crossProduct(v1, v2);
    normal.normalize();

    /*
     * Alignment Vector er som oftest Up vectoren (0, 0, 1)
     * Sjekker her som den faktisk peker oppover, vet at i dette prosjektet så langt
     *  så skal ikke overflater kunne peke nedover
     */
    if (QVector3D::dotProduct(normal, AlignmentVector) < 0.f){ normal *= -1.f; }

    NormalVector = normal;
    for (auto& it : mVertices)
    {
        it.setNormal(NormalVector);
    }
}



