#ifndef AABBFENCE_H
#define AABBFENCE_H

//#include "visualobject.h"
#include "simpleobject.h"

class AABBfence : public SimpleObject
{
public:
    AABBfence();
    AABBfence(VisualObject* obj, Shader* shader);

    void init() override;
    void draw() override;

    std::vector<QVector2D> getAABBcollider();
};

#endif // AABBFENCE_H
