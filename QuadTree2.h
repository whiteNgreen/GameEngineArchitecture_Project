#ifndef QUADTREE_H
#define QUADTREE_H

#include <iostream>
#include <vector>
#include <list>
#include <iterator>

namespace gsm
{
typedef std::pair<float, float> Point2D;

template <class ID, class T>
class QuadTree
{
public:
    QuadTree();
    QuadTree(const Point2D& v1, const Point2D& v2, const Point2D& v3, const Point2D& v4);
    void init(const Point2D& v1, const Point2D& v2, const Point2D& v3, const Point2D& v4);

    void subDivide(int n);
    QuadTree* insert(const Point2D& point, const ID& id, const T& object);
    QuadTree* find(const Point2D& point);
    const QuadTree* find(const Point2D& point) const;

    std::list<std::pair<ID, T>> getObjects() const;
    T getObject(ID id) const;


    T find(const ID& id) const;
    T& operator[](ID id);

    void print() const;
    void printAll() const;

    typename std::vector<T>::iterator begin()
    {
        s_allObjects.clear();
        traverseAll(s_allObjects);
        return s_allObjects.begin();
    }
    typename std::vector<T>::iterator end()
    {
        return s_allObjects.end();
    }

private:
    Point2D m_a;
    Point2D m_b;
    Point2D m_c;
    Point2D m_d;

    QuadTree* m_southWest{ nullptr };
    QuadTree* m_southEast{ nullptr };
    QuadTree* m_northEast{ nullptr };
    QuadTree* m_northWest{ nullptr };

    std::list<std::pair<ID, T>> m_Objects;

    std::vector<T> s_allObjects;

    void traverseAll(std::vector<T>& allObjects);

    bool isLeaf() const;
};

template<class ID, class T>
QuadTree<ID, T>::QuadTree()
{
    m_a = {-10,-10};
    m_b = {0,-10};
    m_c = {0,0};
    m_d = {-10,0};
}

template<class ID, class T>
QuadTree<ID, T>::QuadTree(const Point2D& v1, const Point2D& v2, const Point2D& v3, const Point2D& v4)
    :m_a{ v1 }, m_b{ v2 }, m_c{ v3 }, m_d{ v4 }
{

}

template<class ID, class T>
void QuadTree<ID, T>::init(const Point2D& v1, const Point2D& v2, const Point2D& v3, const Point2D& v4)
{
    m_a = { v1 };
    m_b = { v2 };
    m_c = { v3 };
    m_d = { v4 };
}

template<class ID, class T>
void QuadTree<ID, T>::subDivide(int n)
{
    if (n > 0)
    {
        Point2D v1 = Point2D((m_a.first + m_b.first) / 2, (m_a.second + m_b.second) / 2);
        Point2D v2 = Point2D((m_b.first + m_c.first) / 2, (m_b.second + m_c.second) / 2);
        Point2D v3 = Point2D((m_c.first + m_d.first) / 2, (m_c.second + m_d.second) / 2);
        Point2D v4 = Point2D((m_d.first + m_a.first) / 2, (m_d.second + m_a.second) / 2);
        Point2D m = Point2D((m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2);

        m_southWest = new QuadTree(m_a, v1, m, v4);
        m_southWest->subDivide(n - 1);
        m_southEast = new QuadTree(v1, m_b, v2, m);
        m_southEast->subDivide(n - 1);
        m_northEast = new QuadTree(m, v2, m_c, v3);
        m_northEast->subDivide(n - 1);
        m_northWest = new QuadTree(v4, m, v3, m_d);
        m_northWest->subDivide(n - 1);
    }
}

template<class ID, class T>
T QuadTree<ID, T>::getObject(ID id) const
{
    std::list<std::pair<ID, T>> tempObjects = m_Objects;
    while (!tempObjects.empty())
    {
        if (tempObjects.front().first == id)
            return tempObjects.front().second;
        else
            tempObjects.pop_front();
    }
    return T{};
}

template<class ID, class T>
std::list<std::pair<ID, T> > QuadTree<ID, T>::getObjects() const
{
    return m_Objects;
}

template<class ID, class T>
bool QuadTree<ID, T>::isLeaf() const
{
    bool bIsLeaf = (m_southWest == nullptr && m_southEast == nullptr && m_northWest == nullptr && m_northEast == nullptr);
    return bIsLeaf;
}

template<class ID, class T>
QuadTree<ID, T>* QuadTree<ID, T>::insert(const Point2D& point, const ID& id, const T& object)
{
    if (isLeaf())
    {
        m_Objects.push_back(std::pair<ID, T>(id, object));
        return this;
    }
    else
    {
        QuadTree* ptrToReturn;
        Point2D m = Point2D((m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2);

        if (point.second < m.second)
        {
            if (point.first < m.first && m_southWest)
                ptrToReturn = m_southWest->insert(point, id, object);
            //return nullptr;
            else if (point.first >= m.first && m_southEast)
                ptrToReturn = m_southEast->insert(point, id, object);
            //return nullptr;
        }
        else
        {
            if (point.first < m.first && m_northWest)
                ptrToReturn = m_northWest->insert(point, id, object);
            //return nullptr;
            else if (point.first >= m.first && m_northEast)
                ptrToReturn = m_northEast->insert(point, id, object);
            //return nullptr;
        }
        return ptrToReturn;
    }
    //return nullptr;
}

template<class ID, class T>
QuadTree<ID, T>* QuadTree<ID, T>::find(const Point2D& point)
{
    if (isLeaf())
        return this;
    else
    {
        Point2D m = Point2D((m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2);

        if (point.second < m.second)
        {
            if (point.first < m.first && m_southWest)
                m_southWest->find(point);
            else if (point.first >= m.first && m_southEast)
                m_southEast->find(point);
        }
        else
        {
            if (point.first < m.first && m_northWest)
                m_northWest->find(point);
            else if (point.first >= m.first && m_northEast)
                m_northEast->find(point);
        }
    }
}

template<class ID, class T>
const QuadTree<ID, T>* QuadTree<ID, T>::find(const Point2D& point) const
{
    if (isLeaf())
        return this;
    else
    {
        Point2D m = Point2D((m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2);

        if (point.second < m.second)
        {
            if (point.first < m.first && m_southWest)
                m_southWest->find(point);
            else if (point.first >= m.first && m_southEast)
                m_southEast->find(point);
        }
        else
        {
            if (point.first < m.first && m_northWest)
                m_northWest->find(point);
            else if (point.first >= m.first && m_northEast)
                m_northEast->find(point);
        }
    }
}

template<class ID, class T>
T QuadTree<ID, T>::find(const ID& id) const
{
    if (isLeaf() && m_Objects.size() > 0)
    {
        for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it)
            if ((*it).first == id)
                return (*it).second;

        return nullptr;
    }
    else if (m_southWest)
        return m_southWest->find(id);
    else if (m_southEast)
        return m_southEast->find(id);
    else if (m_northWest)
        return m_northWest->find(id);
    else if (m_northEast)
        return m_northEast->find(id);
}

template<class ID, class T>
T& QuadTree<ID, T>::operator[](ID id)
{
    if (isLeaf())
    {
        for (auto it : m_Objects)
            if (it.first == id)
                return it.second;
    }
}

template<class ID, class T>
void QuadTree<ID, T>::print() const
{
    Point2D center = Point2D{ (m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2 };

    std::cout << "Center= (" << center.first << ", " << center.second << " ) m_Objects.size: " << m_Objects.size() << '\n';
    for (auto it = m_Objects.begin(); it != m_Objects.end(); it++)
        std::cout << it->first << ", " << it->second->getName() << std::endl;
}

template<class ID, class T>
void QuadTree<ID, T>::printAll() const
{
    print();
    if (m_northWest) m_northWest->printAll();
    if (m_northEast) m_northEast->printAll();
    if (m_southEast) m_southEast->printAll();
    if (m_southWest) m_southWest->printAll();
}

template<class ID, class T>
void QuadTree<ID, T>::traverseAll(std::vector<T>& allObjects)
{
    for (auto it : m_Objects) allObjects.push_back(it.second);
    if (m_southWest) m_southWest->traverseAll(allObjects);
    if (m_southEast) m_southEast->traverseAll(allObjects);
    if (m_northWest) m_northWest->traverseAll(allObjects);
    if (m_northEast) m_northEast->traverseAll(allObjects);
}

}
#endif // QUADTREE_H
