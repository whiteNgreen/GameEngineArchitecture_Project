#ifndef TERRAIN_H
#define TERRAIN_H

#include "visualobject.h"
#include "texture.h"


// Task 2: Terrain ----------------------------------------------------------------------------------------------
class Terrain : public VisualObject
{

public:

    Terrain(Shader* shader);

    void init() override;
    void draw() override{}
    void draw(QMatrix4x4& projectionMatrix, QMatrix4x4& viewMatrix) override;

    std::vector<std::vector <float>> heights;

    /* Function called in renderwindow::render
     * will call BarySentricCoordinates if the 2Dvector is within
     * the terrain and return a 3Dvector of the world position
     * on the triangle it is over.
     * Is used on movement for Player and NPC
     * Is used on spawning of trophies */
    QVector3D getPositionOnTerrain(QVector2D point2d);

private:

    class Logger *mLogger{nullptr};

    int m_width, m_height;


    QVector3D BarySentricCoordinates(QVector2D point2d, int x, int y);
};

#endif // TERRAIN_H
