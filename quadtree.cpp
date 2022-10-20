#include "quadtree.h"

//static std::string print;

Quadtree::Quadtree(Shader* shader)
{
    m_a = {-20, -20};
    m_b = {20, -20};
    m_c = {20, 20};
    m_d = {-20, 20};

    mVertices.push_back(Vertex{m_a.first, m_a.second, 0,      0.6, 0.6, 1});
    mVertices.push_back(Vertex{m_b.first, m_b.second, 0,      0.6, 0.6, 1});
    mVertices.push_back(Vertex{m_c.first, m_c.second, 0,      0.6, 0.6, 1});
    mVertices.push_back(Vertex{m_d.first, m_d.second, 0,      0.6, 0.6, 1});

    mIndices = {0,1,2,3};

    if (!m_parent)
        address = "Master";

    m_shader = shader;
//    init();

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

    subDivide(3);
}

Quadtree::Quadtree(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4, Quadtree* parent, Shader* shader)
    : m_a{v1}, m_b{v2}, m_c{v3}, m_d{v4}, m_parent{parent}
{
    mVertices.push_back(Vertex{m_a.first, m_a.second, 0,      0.6, 0.6, 1});
    mVertices.push_back(Vertex{m_b.first, m_b.second, 0,      0.6, 0.6, 1});
    mVertices.push_back(Vertex{m_c.first, m_c.second, 0,      0.6, 0.6, 1});
    mVertices.push_back(Vertex{m_d.first, m_d.second, 0,      0.6, 0.6, 1});

    mIndices = {0,1,2,3};

    m_shader = shader;

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

Quadtree::~Quadtree()
{
    m_shader = nullptr;
    m_parent = nullptr;

    for (const auto& it : mObjects){
        mObjects.erase(it.first);
    }

    if (m_sv){
        delete m_sv;
        m_sv = nullptr;
    }
    if (m_so){
        delete m_so;
        m_so = nullptr;
    }
    if (m_no){
        delete m_no;
        m_no = nullptr;
    }
    if (m_nv){
        delete m_nv;
        m_nv = nullptr;
    }
}

void Quadtree::subDivide(int n)
{
    if (n>0)
    {
        Point2D v1 = Point2D{(m_a.first+m_b.first)/2, (m_a.second+m_b.second)/2};
        Point2D v2 = Point2D{(m_b.first+m_c.first)/2, (m_b.second+m_c.second)/2};
        Point2D v3 = Point2D{(m_c.first+m_d.first)/2, (m_c.second+m_d.second)/2};
        Point2D v4 = Point2D{(m_d.first+m_a.first)/2, (m_d.second+m_a.second)/2};
        Point2D m  = Point2D{(m_a.first+m_c.first)/2, (m_a.second+m_c.second)/2};

        if (!m_sv){
            m_sv = new Quadtree(m_a, v1, m, v4, this, m_shader);
            m_sv->address += address + "->sv";
//            qDebug() << qPrintable(QString::fromStdString(m_sv->address));
            m_sv->subDivide(n-1);
        }
        if (!m_so){
            m_so = new Quadtree(v1, m_b, v2, m, this, m_shader);
            m_so->address += address + "->so";
            m_so->subDivide(n-1);
        }
        if (!m_no){
            m_no = new Quadtree(m, v2, m_c, v3, this, m_shader);
            m_no->address += address + "->no";
            m_no->subDivide(n-1);
        }
        if (!m_nv){
            m_nv = new Quadtree(v4, m, v3, m_d, this, m_shader);
            m_nv->address += address + "->nv";
            m_nv->subDivide(n-1);
        }
    }
}

bool Quadtree::isLeaf() const
{
    return m_sv == nullptr && m_so == nullptr && m_no == nullptr && m_nv == nullptr;
}

void Quadtree::insertObj(std::string name, VisualObject *obj)
{
    Point2D P = obj->getPosition2D();
    if (outsideQuad(P))
    {
        return;
    }

    Point2D m = Point2D{(m_a.first+m_c.first)/2, (m_a.second+m_c.second)/2};

    if (isLeaf())
    {
        if (mObjects.find(name) == mObjects.end()){
//            print += ": " + name + " inserted";
//            qDebug() << qPrintable(QString::fromStdString(print));
            mObjects.insert({name, obj});
            mObjects.size();
//            qDebug() << "P" << P;
        }
//        print = "";
        return;
    }
    else
    {
        if (P.first <= m.first)
        {
            if (P.second <= m.second)
            {
                if (m_sv){
//                    print += "m_sv-> ";
                    m_sv->insertObj(name, obj);
                }
            }
            else if (P.second > m.second)
            {
                if (m_nv){
//                    print += "m_nv-> ";
                    m_nv->insertObj(name, obj);
                }
            }
        }
        else if (P.first > m.first)
        {
            if (P.second <= m.second)
            {
                if (m_so){
//                    print += "m_so-> ";
                    m_so->insertObj(name, obj);
                }
            }
            else if (P.second > m.second)
            {
                if (m_no){
//                    print += "m_no-> ";
                    m_no->insertObj(name, obj);
                }
            }
        }
    }
//    print = "";
}


void Quadtree::removeObj(std::string name)
{
//    qDebug() << "Removed: " << qPrintable(QString::fromStdString(name)) << "from: " << qPrintable(QString::fromStdString(address));
    mObjects.erase(name);
}

void Quadtree::insertObject(std::string name, VisualObject *obj)
{
    mObjects.insert({name, obj});
}

bool Quadtree::findObject(std::string name)
{
    if (mObjects.find(name) != mObjects.end())
        return true;
    else
        return false;
}


void Quadtree::traverseObject(std::string name, VisualObject *obj)
{
    Point2D P = obj->getPosition2D();

    /* Each object has a pointer to the quad that they are in. If the object points to nullptr
     * it is placed within the quadtree */
    if (!obj->m_quad)
    {
        obj->m_quad = objectPlacement(P);
        if (obj->m_quad)
        {
            obj->m_quad->insertObject(name, obj);
        }
    }
    else
    {
        /* if object is outside of its designated quad, it is removed from that quad
         * and the objects m_quad pointer is set to nullptr */
        if (obj->m_quad->outsideQuad(P))
        {
            obj->m_quad->removeObj(name);
            obj->m_quad = nullptr;
        }
        else
        {
            /* If the object IS within its quad, it will check for broad-phase collision */
            /* Start broad-phase collision check */
            if (obj->m_quad->mObjects.size() > 1)
            {
                for (const auto& it : obj->m_quad->mObjects)
                {
                    // Kollisjon for pickups
                    if (it.second != obj /*&& it.second->m_team == obj->m_team*/)
                    {
//                        it.second->sphereCollision(obj);
                        if (it.second->destroy)
                        {
                            obj->m_quad->removeObj(it.first);
                        }
                    }
                    // Kollisjon for Bomber
                    if (obj->m_name[0] == 'B' && (it.first == "Player" || it.first == "NPC"))
                    {
//                        obj->destroy = it.second->sphereCollision(obj);
                        if (obj->destroy)
                        {
                            mObjects.erase(obj->m_name);
                            obj->destroy = true;
                            obj->m_quad->mObjects.erase(obj->m_name);
                        }
                    }
                }
            }
        }
    }
}

Quadtree *Quadtree::objectPlacement(Point2D P)
{
    Quadtree* quad{nullptr};

    if (outsideQuad(P))
        return nullptr;

    if (isLeaf())
        return this;
    else
    {
        Point2D M = Point2D{(m_a.first+m_c.first)/2, (m_a.second+m_c.second)/2};
        if (P.second >= M.second)
        {
            if (P.first >= M.first)
                quad = m_no->objectPlacement(P);
            else
                quad = m_nv->objectPlacement(P);
        }
        else
        {
            if (P.first >= M.first)
                quad = m_so->objectPlacement(P);
            else
                quad = m_sv->objectPlacement(P);
        }
    }
    return quad;
}

bool Quadtree::outsideQuad(Point2D P)
{
    return P.first < m_a.first || P.second < m_a.second ||
            P.first > m_b.first || P.second < m_b.second ||
            P.first > m_c.first || P.second > m_c.second ||
            P.first < m_d.first || P.second > m_d.second;
}


void Quadtree::draw(QMatrix4x4& projectionMatrix, QMatrix4x4& viewMatrix)
{
//    glUniformMatrix4fv(m_shader->mMmatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glUseProgram(m_shader->getProgram());
    m_shader->setUniformMatrix("mMatrix", mMatrix);
    m_shader->setUniformMatrix("pMatrix", projectionMatrix);
    m_shader->setUniformMatrix("vMatrix", viewMatrix);

    glBindVertexArray( mVAO );
    glDrawElements(GL_LINE_LOOP, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    if (m_sv)
        m_sv->draw(projectionMatrix, viewMatrix);
    if (m_so)
        m_so->draw(projectionMatrix, viewMatrix);
    if (m_no)
        m_no->draw(projectionMatrix, viewMatrix);
    if (m_nv)
        m_nv->draw(projectionMatrix, viewMatrix);
}
