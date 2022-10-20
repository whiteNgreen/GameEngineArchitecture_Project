#include "bakke.h"

Bakke::Bakke(Shader* shader)
{
    m_shader = shader;
    mMatrix.setToIdentity();
}

void Bakke::CreateTriangle(QVector3D v1, QVector3D v2, QVector3D v3)
{
    mVertices.push_back(Vertex{v1});
    mVertices.push_back(Vertex{v2});
    mVertices.push_back(Vertex{v3});


    /* Regner ut normal til triangelet */
    QVector3D n1{ v2 - v1 };
    QVector3D n2{ v3 - v1 };
    QVector3D normal{ QVector3D::crossProduct(n1, n2) };
    normal.normalize();


    const unsigned int m = mVertices.size();

    mVertices[m-3].setNormal(normal);
    mVertices[m-2].setNormal(normal);
    mVertices[m-1].setNormal(normal);


    /* Setter indeksen */
    mIndices.push_back(m-3);
    mIndices.push_back(m-2);
    mIndices.push_back(m-1);

    mTriangleIndex++;
}

int Bakke::BarySentricCoordinate(const QVector3D ObjectPosition, QVector3D& TheBarysentricCoordinates, QVector3D& SurfacePosition, QVector3D &SurfaceNormal, const int TriangleIndex)
{
    int Vertex = (TriangleIndex * 3);

    /* Punktene a, b, c */
    QVector3D a = mVertices[Vertex - 3].getPosition();   a.setZ(0.f);
    QVector3D b = mVertices[Vertex - 2].getPosition();   b.setZ(0.f);
    QVector3D c = mVertices[Vertex - 1].getPosition();   c.setZ(0.f);

    QVector3D Baryc;

    QVector3D ab = b - a;
    QVector3D ac = c - a;

    QVector3D n{ QVector3D::crossProduct(ab, ac) };
    float areal_abc{ n.length() };

    // u
    QVector3D p{ b.x() - ObjectPosition.x(), b.y() - ObjectPosition.y(), 0.f };
    QVector3D q{ c.x() - ObjectPosition.x(), c.y() - ObjectPosition.y(), 0.f };
    n = QVector3D::crossProduct(p, q);
    Baryc.setX(n.z()/areal_abc);

    // v
    p = QVector3D{ c.x() - ObjectPosition.x(), c.y() - ObjectPosition.y(), 0.f };
    q = QVector3D{ a.x() - ObjectPosition.x(), a.y() - ObjectPosition.y(), 0.f };
    n = QVector3D::crossProduct(p, q);
    Baryc.setY(n.z()/areal_abc);

    // w
    p = QVector3D{ a.x() - ObjectPosition.x(), a.y() - ObjectPosition.y(), 0.f };
    q = QVector3D{ b.x() - ObjectPosition.x(), b.y() - ObjectPosition.y(), 0.f };
    n = QVector3D::crossProduct(p, q);
    Baryc.setZ(n.z()/areal_abc);

    TheBarysentricCoordinates = Baryc;
    if (Baryc.x() >= 0.f && Baryc.y() >= 0.f && Baryc.z() >= 0.f)
    {
        /* Returns the objects intended 3D position on the surface */
        QVector3D a = mVertices[Vertex - 3].getPosition();
        QVector3D b = mVertices[Vertex - 2].getPosition();
        QVector3D c = mVertices[Vertex - 1].getPosition();
        SurfacePosition = {
            a.x() * Baryc.x() + b.x() * Baryc.y() + c.x() * Baryc.z(),
            a.y() * Baryc.x() + b.y() * Baryc.y() + c.y() * Baryc.z(),
            a.z() * Baryc.x() + b.z() * Baryc.y() + c.z() * Baryc.z(),
    };

        /* Returns the normal of the surface */
        SurfaceNormal = mVertices[Vertex - 3].getNormal();
        return TriangleIndex;
    }

    SurfacePosition = {0,0,0};
    return 0;
}

int Bakke::BarySentricCoordinate(const QVector3D ObjectPosition, QVector3D &TheBarysentricCoordinates, QVector3D &SurfacePosition, QVector3D &SurfaceNormal)
{
    return BarySentricCoordinate(ObjectPosition, TheBarysentricCoordinates, SurfacePosition, SurfaceNormal, 0);
}

QVector3D Bakke::GetCoordinateWithBaryc(const QVector3D &Baryc, const int index)
{
    /* Antar at ingen av verdiene på Baryc er negative */
    int Vertex = (index * 3);

    /* Punktene a, b, c */
    QVector3D a = mVertices[Vertex - 3].getPosition();
    QVector3D b = mVertices[Vertex - 2].getPosition();
    QVector3D c = mVertices[Vertex - 1].getPosition();

    return {
            a.x() * Baryc.x() + b.x() * Baryc.y() + c.x() * Baryc.z(),
            a.y() * Baryc.x() + b.y() * Baryc.y() + c.y() * Baryc.z(),
            a.z() * Baryc.x() + b.z() * Baryc.y() + c.z() * Baryc.z(),
    };
}

QVector3D Bakke::GetNormalFromIndex(const int Index)
{
    int Vertex = (Index * 3);

    return mVertices[Vertex-3].getNormal();
}



