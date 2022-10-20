#ifndef QUADTREE_H
#define QUADTREE_H

#include "visualobject.h"
//#include "vertex.h"
#include "Shaders/shader.h"

#include <unordered_map>

typedef std::pair<float, float> Point2D;

/* Very simple quadtree
 * It comes pre-subdivided and will not subdivide further during runtime due to it being simplified
 * for now. It checks the broad-phase collision and will launch the objects viable for narrow-phase
 * collision to check for collision on the objects sphereCollider
 *
 * Each object within the quadtree has a pointer directly to the quad they are within (m_quad) */

class Quadtree : public VisualObject
{
private:
    Point2D m_a;
    Point2D m_b;
    Point2D m_c;
    Point2D m_d;

    Quadtree* m_sv{nullptr};
    Quadtree* m_so{nullptr};
    Quadtree* m_no{nullptr};
    Quadtree* m_nv{nullptr};
    Quadtree* m_parent{nullptr};

    std::unordered_map<std::string, VisualObject*> mObjects;
    std::vector<VisualObject*> mObjectsVector{nullptr};

    bool outsideQuad(Point2D P);

    /* Gives the address in nv->so and so on, Starting with Master. example Master->so->nv */
    std::string address;

//    Shader* m_shader{nullptr};

public:
    Quadtree(Shader* shader);
    Quadtree(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4, Quadtree* parent, Shader* shader);
    ~Quadtree();

//    virtual void init(/*GLuint matrixUniform*/) override;
    void draw()override {}
    void draw(QMatrix4x4& projectionMatrix, QMatrix4x4& viewMatrix) override;

    void subDivide(int n);

    bool isLeaf() const;

    /* Used to insert the trophies during initialization */
    void insertObj(std::string name, VisualObject* obj);


    void removeObj(std::string name);
    void insertObject(std::string name, VisualObject* obj);
    bool findObject(std::string name);
    void traverseObject(std::string name, VisualObject* obj);
    Quadtree* objectPlacement(Point2D P);

};

#endif // QUADTREE_H
