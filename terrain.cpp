#include "terrain.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "logger.h"

// Task 2: Terrain ----------------------------------------------------------------------------------------------
Terrain::Terrain(Shader* shader)
{
    m_shader = shader;

    mLogger = Logger::getInstance();

    std::string heightmap = "../GEA_Project/Assets/HeightmapEksamen2.png";
    int width, height, nrChannels;

//    stbi_set_flip_vertically_on_load(true);
    uint8_t* data = stbi_load(heightmap.c_str(), &width, &height, &nrChannels, 0);
    if(stbi_failure_reason())
        mLogger->logText(stbi_failure_reason());

    if (data)
    {
        m_width = width;
        m_height = height;

        /* Function only works on images with a single channel */
        if (nrChannels == 1)
        {
            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    float z = data[(x * width) + (y)];
                    mVertices.push_back(Vertex{QVector3D{static_cast<float>(x), static_cast<float>(y), z/8}, QVector2D{static_cast<float>(x)/width, static_cast<float>(y)/height}});
                }
            }

            /* When A, B, C, D is mentioned
             * D  ________ C
             *   |       |
             *   |       |
             *   |       |
             * A |_______| B
             * */
            // Calculating the normals
            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    QVector3D v1 = mVertices[((x + 1) * width) + (y + 1)].getPosition() - mVertices[(x * width) + y].getPosition(); //  c - a
                    QVector3D v2 = mVertices[((x + 1) * width) + y].getPosition() - mVertices[(x * width) + y].getPosition();       //  b - a

                    QVector3D norm = norm.crossProduct(v2, v1);
                    norm.normalize();
                    mVertices[x*width+y].setNormal(norm);

                    /* Changing the calculation for the last row in x direction, last row in y dircetion and last vertex on the terrain */

                    // Last row in x direction
                    if (x == width - 1)
                    {
                        QVector3D v1 = mVertices[(x * width) + (y + 1)].getPosition() - mVertices[(x * width) + y].getPosition();   //  c - b
                        QVector3D v2 = mVertices[((x - 1) * width) + y].getPosition() - mVertices[(x * width) + y].getPosition();   //  a - b

                        QVector3D norm = norm.crossProduct(v1, v2);
                        norm.normalize();
                        mVertices[x*width+y].setNormal(norm);
                    }

                    // Last row in y direction
                    if (y == height - 1)
                    {
                        QVector3D v1 = mVertices[((x + 1) * width) + y].getPosition() - mVertices[(x * width) + y].getPosition();   //  c - d
                        QVector3D v2 = mVertices[(x * width) + (y - 1)].getPosition() - mVertices[(x * width) + y].getPosition();   //  a - d

                        QVector3D norm = norm.crossProduct(v2, v1);
                        norm.normalize();
                        mVertices[x*width+y].setNormal(norm);
                    }

                    // For the very last vertex
                    if (x == width - 1 && y == height - 1)
                    {
                        QVector3D v1 = mVertices[((x - 1) * width) + y].getPosition() - mVertices[(x * width) + y].getPosition();         //  d - c
                        QVector3D v2 = mVertices[((x - 1) * width) + (y - 1)].getPosition() - mVertices[(x * width) + y].getPosition();   //  a - c

                        QVector3D norm = norm.crossProduct(v1, v2);
                        norm.normalize();
                        mVertices[x*width+y].setNormal(norm);
                    }
                }
            }


            /* Creating indices for the terrain */
            for (int x = 0; x < width-1; x++)
            {
                for (int y = 0; y < height-1; y++)
                {
                    mIndices.push_back((x * height) + y);          // a
                    mIndices.push_back(((x+1) * height) + y);      // b
                    mIndices.push_back(((x+1) * height) + y + 1);  // c

                    mIndices.push_back((x * height) + y);          // a
                    mIndices.push_back(((x+1) * height) + y + 1);  // c
                    mIndices.push_back((x * height) + y + 1);      // d
                }
            }
        }
        stbi_image_free(data);
    }
    else{
        mLogger->logText("ERROR: Failed to load heightmap", LogType::REALERROR);
    }

    /* Move terrain so game is set in origin */
    mMatrix.translate(-128, -128, 0);
}

void Terrain::init()
{
//    MatrixUniform = matrixUniform;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0 )  );          // array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glGenBuffers ( 1, &mEAB );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Terrain::draw(QMatrix4x4& projectionMatrix, QMatrix4x4& viewMatrix)
{
//    glUniformMatrix4fv(m_shader->mMmatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glUseProgram(m_shader->getProgram());
    m_shader->setUniformMatrix("mMatrix", mMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glBindVertexArray( mVAO );
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

/* The function uses a QVector2D to check the position of said vector on the terrain.
 * Determining if the point is within the terrain to begin with.
 * Its QVector3D return is used to move objects on top the terrain using the returned
 * vectors z-axis. */
QVector3D Terrain::getPositionOnTerrain(QVector2D point2d)
{
    /* Check if object is within the terrain, keeping the terrains location translation
     * in mind. (see bottom of Terrain::Terrain) */
    if (point2d.x() >= mMatrix.column(3).x() && point2d.y() >= mMatrix.column(3).y() &&
            point2d.x() < m_width+mMatrix.column(3).x() && point2d.y() < m_height+mMatrix.column(3).y())
    {
        float x = (point2d.x() - mMatrix.column(3).x());
        float y = (point2d.y() - mMatrix.column(3).y());

        /* Passing in the floats x and y as integers to remove the decimals and simplify the act of
         * referencing the specific vertices within the array mVertices the 2Dvector is within */
        return BarySentricCoordinates(point2d, x, y);
    }
    else // Point is outside of terrain return 0
    {
        return {0,0,0};
    }
}

/* The actual calculation of the barysentric coordinates of the 2Dvector point2d */
QVector3D Terrain::BarySentricCoordinates(QVector2D point2d, int x, int y)
{
    /* Calculating the barysentriccoordinates within the quad
     * x   , y      // a
     * x+1 , y      // b
     * x   , y+1    // c
     * x+1 , y+1    // d
     */
    /* Quad A, B, C, D
     * D  ________ C
     *   |       |
     *   |       |
     *   |       |
     * A |_______| B
     * */
    /* Gets the specific quad the point is within to avoid having to loop through the array mVertices
     * The 2D position of the translation to mMatrix is added to the vertices 2D position to take
     * into account the objects potential translation */
    QVector2D a = mVertices[(x*m_width)+y].getPosition().toVector2D()        + mMatrix.column(3).toVector2D();
    QVector2D b = mVertices[((x+1)*m_width)+y].getPosition().toVector2D()    + mMatrix.column(3).toVector2D();
    QVector2D c = mVertices[((x+1)*m_width)+(y+1)].getPosition().toVector2D()+ mMatrix.column(3).toVector2D();
    QVector2D d = mVertices[(x*m_width)+y+1].getPosition().toVector2D()      + mMatrix.column(3).toVector2D();

    QVector3D baryc;

    /* Starts calculating on the triangle
     * A, B, C
     * x   , y      // a
     * x+1 , y      // b
     * x+1 , y+1    // c
     */
    QVector2D ab = b - a;
    QVector2D ac = c - a;

    QVector3D n = n.crossProduct(QVector3D(ab, 0), QVector3D(ac, 0));
    float areal_abc = n.length();

    // u
    QVector2D p{ b.x() - point2d.x(), b.y() - point2d.y() };
    QVector2D q{ c.x() - point2d.x(), c.y() - point2d.y() };
    n = n.crossProduct(QVector3D(p, 0), QVector3D(q,0));
    baryc.setX(n.z()/areal_abc);

    // v
    p = { c.x() - point2d.x(), c.y() - point2d.y() };
    q = { a.x() - point2d.x(), a.y() - point2d.y() };
    n = n.crossProduct(QVector3D(p, 0), QVector3D(q,0));
    baryc.setY(n.z()/areal_abc);

    // w
    p = { a.x() - point2d.x(), a.y() - point2d.y() };
    q = { b.x() - point2d.x(), b.y() - point2d.y() };
    n = n.crossProduct(QVector3D(p, 0), QVector3D(q,0));
    baryc.setZ(n.z()/areal_abc);


    if (baryc.x() >= 0 && baryc.y() >= 0 && baryc.z() >= 0)
        return {0,0, mVertices[(x*m_width)+y].getPosition().z() * baryc.x() +   //  a
        mVertices[(x+1)*m_width + y].getPosition().z() * baryc.y() +            //  b
        mVertices[(x+1)*m_width + y+1].getPosition().z() * baryc.z()};          //  c


    /* Not within the first triangle, so checks the next one
     * A, C, D
     * x   , y      // a
     * x+1 , y+1    // c
     * x   , y+1    // d
     */
    QVector2D ad = d - a;

    n = n.crossProduct(QVector3D(ad, 0), QVector3D(ac,0));
    float areal_acd = n.length();

    // u
    p = { c.x() - point2d.x(), c.y() - point2d.y() };
    q = { d.x() - point2d.x(), d.y() - point2d.y() };
    n = n.crossProduct(QVector3D(p, 0), QVector3D(q,0));
    baryc.setX(n.z()/areal_acd);

    // v
    p = { a.x() - point2d.x(), a.y() - point2d.y() };
    q = { c.x() - point2d.x(), c.y() - point2d.y() };
    n = n.crossProduct(QVector3D(p, 0), QVector3D(q,0));
    baryc.setY(n.z()/areal_acd);

    // w
    p = { d.x() - point2d.x(), d.y() - point2d.y() };
    q = { a.x() - point2d.x(), a.y() - point2d.y() };
    n = n.crossProduct(QVector3D(p, 0), QVector3D(q,0));
    baryc.setZ(n.z()/areal_acd);


    if (baryc.x() >= -0.01 && baryc.y() >= -0.01 && baryc.z() >= -0.01)
        return {0,0, mVertices[(x*m_width)+y].getPosition().z() * baryc.x() +   //  a
        mVertices[(x*m_width) + y+1].getPosition().z() * baryc.y() +            //  c
        mVertices[((x+1)*m_width) + y+1].getPosition().z() * baryc.z()};        //  d


    return {0,0,0};
}
