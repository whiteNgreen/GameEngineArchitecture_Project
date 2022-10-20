#include "objmesh.h"

#include <iostream>
#include <sstream>
#include <fstream>

ObjMesh::ObjMesh(std::string filename, Shader* shader)
{
    mLogger->getInstance();
    readFile(filename);
    mMatrix.setToIdentity();

    m_shader = shader;
    init();
}

void ObjMesh::init()
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

void ObjMesh::draw()
{
//    glUniformMatrix4fv(m_shader->mMmatrixUniform, 1, GL_FALSE, mMatrix.constData());
    m_shader->setUniformMatrix("mMatrix", mMatrix);

    glBindVertexArray( mVAO );
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void ObjMesh::readFile(std::string filename)
{
    // Open file
    std::string path = "../GEA_Project/Assets/" + filename;
    std::ifstream fileIn;
    fileIn.open(path, std::ifstream::in);
    if(!fileIn)
        mLogger->logText("Could not open file for reading: " + filename);

    std::string oneLine;
    std::string oneWord;

    std::vector<QVector3D> tempVertices;
    std::vector<QVector3D> tempNormals;
    std::vector<QVector2D> tempUVs;

    GLuint temp_index = 0;

    while (std::getline(fileIn, oneLine))
    {
        std::stringstream sStream;
        sStream << oneLine;

        oneWord = "";
        sStream >> oneWord;

        if (oneWord == "#")
        {
            // Ignore
            continue;
        }
        if (oneWord == "")
        {
            // Ignore
            continue;
        }
        if (oneWord == "mtllib")
        {
            // Ignore
            continue;
        }
        if (oneWord == "o")
        {
            // Ignore
            continue;
        }
        if (oneWord == "usemtl")
        {
            // Ignore
            continue;
        }
        if (oneWord == "s")
        {
            // Ignore
            continue;
        }

        if (oneWord == "v")
        {
            QVector3D tempVertex;
            sStream >> oneWord;
            tempVertex.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempVertex.setY(std::stof(oneWord));
            sStream >> oneWord;
            tempVertex.setZ(std::stof(oneWord));

            tempVertices.push_back(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            QVector2D tempUV;
            sStream >> oneWord;
            tempUV.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempUV.setY(std::stof(oneWord));

            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            QVector3D tempNormal;
            sStream >> oneWord;
            tempNormal.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempNormal.setY(std::stof(oneWord));
            sStream >> oneWord;
            tempNormal.setZ(std::stof(oneWord));

            tempNormals.push_back(tempNormal);

            continue;
        }
        if (oneWord == "f")
        {
            int index{0}, normal{0}, uv{0};
            for (int i = 0; i < 3; i++)
            {
                sStream >> oneWord;
                std::stringstream tempWord(oneWord);
                std::string segment;
                std::vector<std::string> segmentArray;
                while (std::getline(tempWord, segment, '/'))
                {
                    segmentArray.push_back(segment);
                }
                index = std::stoi(segmentArray[0]);
                if (segmentArray[1] != "")
                    uv = std::stoi(segmentArray[1]);
                normal = std::stoi(segmentArray[2]);

                --index;
                --uv;
                --normal;

                Vertex tempVert{tempVertices[index], tempNormals[normal], tempUVs[uv]};
                mVertices.push_back(tempVert);

                mIndices.push_back(temp_index++);
            }
            continue;
        }
    }
    fileIn.close();
}
