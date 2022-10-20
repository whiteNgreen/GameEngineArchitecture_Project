#ifndef OBJMESH_H
#define OBJMESH_H

#include "visualobject.h"
#include "logger.h"

class ObjMesh : public VisualObject
{
public:
    ObjMesh(std::string filename, Shader* shader);

    virtual void init() override;
    virtual void draw() override;


private:
    void readFile(std::string filename);
    Logger* mLogger;
};

#endif // OBJMESH_H
