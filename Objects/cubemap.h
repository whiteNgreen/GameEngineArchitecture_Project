#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "visualobject.h"

class CubeMap : public VisualObject
{
public:
    CubeMap(Shader* shader);

    void init() override;
    void draw() override{}
    void draw(QMatrix4x4& projectionMatrix, QMatrix4x4& viewMatrix) override;

};

#endif // CUBEMAP_H
