#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <fstream>
#include <QMatrix4x4>

class Vertex
{
    friend std::ostream& operator << (std::ostream&, const Vertex&);
    friend std::istream& operator >> (std::istream& is, Vertex& v);
//    {
//        // Trenger fire temporære variabler som kun skal lese inn paranteser og komma
//        char dum, dum2, dum3, dum4;
//        is >> dum >> v.m_xyz[0] >> dum2 >> v.m_xyz[1] >> dum3 >> v.m_xyz[2] >> dum4;
//        is >> dum >> v.m_normal[0] >> dum2 >> v.m_normal[1] >> dum3 >> v.m_normal[2] >> dum4;
//        is >> dum >> v.m_uv[0] >> dum2 >> v.m_uv[1] >> dum3;
//        return is;
//    }

private:
    float m_xyz[3];
    float m_normal[3];
    float m_uv[2];

public:
    Vertex();
    Vertex(float x, float y, float z);
    Vertex(float x, float y, float z, float r, float g, float b);
    Vertex(float x, float y, float z, float r, float g, float b, float u, float v);
    Vertex(QVector3D Position, QVector3D Normals, QVector2D UVs);
    Vertex(QVector3D Position, QVector3D Normals);
    Vertex(QVector3D Position, QVector2D UVs);
    Vertex(QVector3D Position);
    ~Vertex(){}


    void setPosition(float x, float y, float z);
    void setPosition(QVector3D NewPosition);
    void setNormal(float r, float g, float b);
    void setNormal(QVector3D normal);
    void setUV(float U, float V);

    void SubtractPosition(float x, float y, float z);
    void SubtractPosition(const QVector3D Subraction);

    void ScalePosition(float scale);

    QVector3D getPosition();
    QVector3D getNormal();
    QVector2D getUV();

};

#endif // VERTEX_H
