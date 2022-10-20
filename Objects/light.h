#ifndef LIGHT_H
#define LIGHT_H

#include "visualobject.h"

// Task: 3 Sun ----------------------------------------------------------------------------------------------
class Light : public VisualObject
{
public:
    Light(Shader* shader);
    Light(VisualObject* obj, Shader* shader);

    void init() override;
    void draw() override{}
    void draw(QMatrix4x4& projectionMatrix, QMatrix4x4& viewMatrix) override;

    GLfloat mAmbientStrength{0.3f};
    QVector3D mAmbientColor{0.3f, 0.3f, 0.3f};

    GLfloat mLightStrength{1.f};
    QVector3D mLightColor{0.9f, 0.9f, 0.7f};

    GLfloat mSpecularStrength{0.9f};
    GLint mSpecularExponent{4};

    QVector3D Position;
    void MoveTo(const QVector3D& Location);
};

#endif // LIGHT_H
