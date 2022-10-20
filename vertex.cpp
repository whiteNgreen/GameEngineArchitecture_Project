#include "vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(float x, float y, float z)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;

    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float u, float v)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;

    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;

    m_uv[0] = u;
    m_uv[1] = v;
}

Vertex::Vertex(QVector3D Position, QVector3D Normals, QVector2D UVs)
{
    m_xyz[0] = Position.x();
    m_xyz[1] = Position.y();
    m_xyz[2] = Position.z();

    m_normal[0] = Normals.x();
    m_normal[1] = Normals.y();
    m_normal[2] = Normals.z();

    m_uv[0] = UVs.x();
    m_uv[1] = UVs.y();
}

Vertex::Vertex(QVector3D Position, QVector3D Normals)
{
    m_xyz[0] = Position.x();
    m_xyz[1] = Position.y();
    m_xyz[2] = Position.z();

    m_normal[0] = Normals.x();
    m_normal[1] = Normals.y();
    m_normal[2] = Normals.z();
}

Vertex::Vertex(QVector3D Position, QVector2D UVs)
{
    m_xyz[0] = Position.x();
    m_xyz[1] = Position.y();
    m_xyz[2] = Position.z();

    m_uv[0] = UVs.x();
    m_uv[1] = UVs.y();
}

Vertex::Vertex(QVector3D Position)
{
    m_xyz[0] = Position.x();
    m_xyz[1] = Position.y();
    m_xyz[2] = Position.z();
}

void Vertex::setPosition(float x, float y, float z)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
}

void Vertex::setPosition(QVector3D NewPosition)
{
    m_xyz[0] = NewPosition.x();
    m_xyz[1] = NewPosition.y();
    m_xyz[2] = NewPosition.z();
}

void Vertex::setNormal(float r, float g, float b)
{
    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;
}

void Vertex::setNormal(QVector3D normal)
{
    m_normal[0] = normal.x();
    m_normal[1] = normal.y();
    m_normal[2] = normal.z();
}

void Vertex::setUV(float U, float V)
{
    m_uv[0] = U;
    m_uv[1] = V;
}

void Vertex::SubtractPosition(float x, float y, float z)
{
//    QVector3D Subtraction{ x, y, z };
//    QVector3D OldPos{ m_xyz[0], m_xyz[1], m_xyz[2] };
//    QVector3D NewPos = OldPos - Subtraction;

    m_xyz[0] -= x;
    m_xyz[1] -= y;
    m_xyz[2] -= z;
//    m_xyz[0] = NewPos.x();
//    m_xyz[1] = NewPos.y();
//    m_xyz[2] = NewPos.z();
}

void Vertex::SubtractPosition(const QVector3D Subtraction)
{
    QVector3D OldPos{ m_xyz[0], m_xyz[1], m_xyz[2] };
    QVector3D NewPos = OldPos - Subtraction;

    m_xyz[0] = NewPos.x();
    m_xyz[1] = NewPos.y();
    m_xyz[2] = NewPos.z();
}

void Vertex::ScalePosition(float scale)
{
    m_xyz[0] *= scale;
    m_xyz[1] *= scale;
    m_xyz[2] *= scale;
}

QVector3D Vertex::getPosition()
{
    return {m_xyz[0], m_xyz[1], m_xyz[2]};
}

QVector3D Vertex::getNormal()
{
    return {m_normal[0], m_normal[1], m_normal[2]};
}

QVector2D Vertex::getUV()
{
    return {m_uv[0], m_uv[1]};
}

std::ostream& operator<<(std::ostream& os, const Vertex& v){
    os << std::fixed;
    os << "(" << v.m_xyz[0] << "," << v.m_xyz[1] << "," << v.m_xyz[2] << ")";
    os << "(" << v.m_normal[0] << "," << v.m_normal[1] << "," << v.m_normal[2] << ")";
    os << "(" << v.m_uv[0] << "," << v.m_uv[1] << ")";
    return os;
}

std::istream& operator >>(std::istream& is, Vertex& v){
    // Trenger fire temporære variabler som kun skal lese inn paranteser og komma
    char dum, dum2, dum3, dum4;
    is >> dum >> v.m_xyz[0] >> dum2 >> v.m_xyz[1] >> dum3 >> v.m_xyz[2] >> dum4;
    is >> dum >> v.m_normal[0] >> dum2 >> v.m_normal[1] >> dum3 >> v.m_normal[2] >> dum4;
    is >> dum >> v.m_uv[0] >> dum2 >> v.m_uv[1] >> dum3;
    return is;
}


