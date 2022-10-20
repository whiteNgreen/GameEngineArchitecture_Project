#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "visualobject.h"

class BezierCurve : public VisualObject
{
public:
    BezierCurve(Shader* shader);

    void init() override;
    void draw() override;
    void draw(QMatrix4x4& projectionMatrix, QMatrix4x4& viewMatrix) override;

    std::vector<QVector3D> mPoints;

    QVector3D findBezierPoint(const std::vector<QVector3D>& points, float t);
};

#endif // BEZIERCURVE_H
