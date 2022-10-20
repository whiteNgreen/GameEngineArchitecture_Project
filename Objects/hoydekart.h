#ifndef HOYDEKART_H
#define HOYDEKART_H

#include <visualobject.h>
#include <unordered_map>
#include "StandAlone/LoggerFunctions.h"
#include "Objects/bakke.h"

//std::pair<int, int> indeksering;



class Rute : VisualObject
{
public:
    Rute(Shader* shader, int index, int X, int Y);
    ~Rute(){}

    int Xindex;
    int Yindex;
    int RuteIndex;

    /* Grensene i 2D */
    float Xmin;
    float Xmax;
    float Ymin;
    float Ymax;

    std::vector<QVector3D> mPunkter;
    float averageHeight{0.1f};
    QVector3D Center;

    void MakeCenter()
    {
        Center.setX((Xmin + Xmax)/2);
        Center.setY((Ymin + Ymax)/2);
        if (mPunkter.size() != 0)
        {
            Center.setZ(averageHeight);
        }
        else
        {
            averageHeight = 0.1f;
            Center.setZ(averageHeight);
        }

//        Log("Point size of: " + std::to_string(TriangleIndex) + " : " + std::to_string(mPunkter.size()));
    }
    void MakeRute()
    {

        mVertices.push_back(Vertex{Xmin, Ymin, 0.1}); // a
        mVertices.push_back(Vertex{Xmax, Ymin, 0.1}); // b
        mVertices.push_back(Vertex{Xmax, Ymax, 0.1}); // c
        mVertices.push_back(Vertex{Xmin, Ymax, 0.1}); // d

        mIndices = { 0, 1, 2, 3 };

        for (auto& it : mVertices)
        {
            it.Vertex::setNormal(1, 1, 1);
        }

        VisualObject::init();
    }

    /* Rendering av Rute */
    void draw(QMatrix4x4 &projectionMatrix, QMatrix4x4 &viewMatrix) override;
};



/* Brukes til utregningen av barysentriske koordinater */
struct TriangleObj
{
    TriangleObj(int tIndex, QVector3D a, QVector3D b, QVector3D c, QVector3D n)
    {
        TriangleIndex = tIndex;
        A = a; B = b; C = c;
        N = n;
    }
    int TriangleIndex{};

    QVector3D A;
    QVector3D B;
    QVector3D C;
    QVector3D N;

    int BarySentricCoordinate(const QVector3D ObjectPosition,
                              QVector3D& TheBarysentricCoordinates,
                              QVector3D& SurfacePosition,
                              QVector3D& SurfaceNormal)
    {
        QVector3D Baryc;

        QVector3D ab = B - A;   ab.setZ(0);
        QVector3D ac = C - A;   ac.setZ(0);

        QVector3D n{ QVector3D::crossProduct(ab, ac) };
        float areal_abc{ n.length() };

        // u
        QVector3D p{ B.x() - ObjectPosition.x(), B.y() - ObjectPosition.y(), 0.f };
        QVector3D q{ C.x() - ObjectPosition.x(), C.y() - ObjectPosition.y(), 0.f };
//        n = QVector3D::crossProduct(p, q);
        n = QVector3D::crossProduct(q, p);
        Baryc.setX(n.z()/areal_abc);

        // v
        p = QVector3D{ C.x() - ObjectPosition.x(), C.y() - ObjectPosition.y(), 0.f };
        q = QVector3D{ A.x() - ObjectPosition.x(), A.y() - ObjectPosition.y(), 0.f };
//        n = QVector3D::crossProduct(p, q);
        n = QVector3D::crossProduct(q, p);
        Baryc.setY(n.z()/areal_abc);

        // w
        p = QVector3D{ A.x() - ObjectPosition.x(), A.y() - ObjectPosition.y(), 0.f };
        q = QVector3D{ B.x() - ObjectPosition.x(), B.y() - ObjectPosition.y(), 0.f };
//        n = QVector3D::crossProduct(p, q);
        n = QVector3D::crossProduct(q, p);
        Baryc.setZ(n.z()/areal_abc);

        TheBarysentricCoordinates = Baryc;
        if (Baryc.x() >= 0.f && Baryc.y() >= 0.f && Baryc.z() >= 0.f)
        {
            /* Returns the objects intended 3D position on the surface */
            SurfacePosition = {
                A.x() * Baryc.x() + B.x() * Baryc.y() + C.x() * Baryc.z(),
                A.y() * Baryc.x() + B.y() * Baryc.y() + C.y() * Baryc.z(),
                A.z() * Baryc.x() + B.z() * Baryc.y() + C.z() * Baryc.z(),
            };

            /* Returns the normal of the surface */
            SurfaceNormal = N;
            return TriangleIndex;
        }

        SurfacePosition = {0,0,0};
        SurfaceNormal = {0,0,0};
        return 0;
    }

    QVector3D GetCoordinateWithBaryc(const QVector3D Baryc)
    {
        return {
                A.x() * Baryc.x() + B.x() * Baryc.y() + C.x() * Baryc.z(),
                A.y() * Baryc.x() + B.y() * Baryc.y() + C.y() * Baryc.z(),
                A.z() * Baryc.x() + B.z() * Baryc.y() + C.z() * Baryc.z(),
        };
    }
    void PrintTriangle()
    {
        Log(" ---- Printing Triangle : " + std::to_string(TriangleIndex) + " ---- ");
        LogVector("A", A);
        LogVector("B", B);
        LogVector("C", C);
        LogVector("N", N);
    }
};



class HoydeKart : public Bakke
{
private:
    std::vector<QVector3D> mPunktdata;

public:
    /* Blir litt midlertidig nå.
     * Går igjennom alle TriangleObj uten å sjekke hvilken quad
     *  den skal være inni først
     *  */
//    int BarySentricCoordinate(const QVector3D ObjectPosition,
//                              QVector3D& TheBarysentricCoordinates,
//                              QVector3D& SurfacePosition,
//                              QVector3D& SurfaceNormal) override;
    virtual int BarySentricCoordinate(const QVector3D ObjectPosition, QVector3D& TheBarysentricCoordinates, QVector3D& SurfacePosition, QVector3D& SurfaceNormal) override;

    std::vector<TriangleObj> mTriangles;
    std::vector<int> mTriangleIndexes;

public: /* ----- Lage Terrain Mesh ----- */
    HoydeKart(Shader* shader,
              float scale, unsigned int RuteResolution = 3,
              bool ReadComplex = false, int PointCloudResolution = 100);
        /* PointCloudResolution, hvilke n'te punkt skal vi lese inn
         * = 1: Den sanne oppløsningen. Leser inn alle punkter
         * = 2: Hvert andre punkt
         * = 100: Hvert hundrede punkt
         * */
    ~HoydeKart(){}

    /* Max & Min verdier for hoydedataen */
    float Xmax{}, Ymax{}, Zmax{};
    float Xmin{}, Ymin{}, Zmin{};

    void ReadComplexPointCloud(std::string file, int PointRes);
    void WriteSimplePointCloud();
    void ReadSimplePointCloud();

    void WriteMeshToFile();
    void GetMeshFromFile();

    QVector3D GetCenter();

    std::vector<Rute*> mRuter;

//    void CalculateNormal()

//    std::unordered_map<std::pair<int, int>, Rute> mRuter2;

public: /* Rendering */
    virtual void draw(QMatrix4x4& projectionMatrix, QMatrix4x4& viewMatrix) override;
};

#endif // HOYDEKART_H
