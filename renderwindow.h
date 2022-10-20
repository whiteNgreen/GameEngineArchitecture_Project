#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>

#include <unordered_map>

#include "camera.h"
#include "texture.h"
#include "simpleobject.h"
#include "terrain.h"
#include "Objects/light.h"
#include "objmesh.h"
#include "quadtree.h"
#include "Objects/cubemap.h"
#include "beziercurve.h"
#include "aabbfence.h"
#include "octahedronball.h"
#include "Objects/bakke.h"
#include "Objects/hoydekart.h"
#include "Objects/b_spline.h"

class QOpenGLContext;
class Shader;
class MainWindow;

/// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using "glad" and "glw" from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;  //gets called when app is shown and resized

    bool mRotate{true};     //Check if triangle should rotate

    /* ---- Funksjoner for GUI ---- */
    void togglePlay(bool bPlay);
    void togglePause(bool bPause);
    void GoNextFrame();
    void Reset();
    void ChangeBallStartPosition();

    void ChangeBallStartPositionX(const float x);
    void ChangeBallStartPositionY(const float y);
    void ChangeBallStartPositionZ(const float z);

    void ChangeBallStartVelocity();
    void ChangeBallStartVelocityX(const float x);
    void ChangeBallStartVelocityY(const float y);
    void ChangeBallStartVelocityZ(const float z);


    void MakeNedbor(int antallDroper, float MaxLengdefraSenter, float HoydefraTerrain, bool RandomHeight = false, float RandomHeightDifference = 0);
    void SlettNedbor();
    void ShowSplineCurves(bool b);
    void ShowSplinePoints(bool b);
    bool bShowingSplinePoints{ true };

    void UsingPhongShader(bool b);
    void MoveLight(const QVector3D Move);

    void ShowHeightCurves(bool b);
    bool bShowingHeightCurves{ false };

    void HeightCurve_ChangeStep(float Step);
    void HeightCurve_ChangeThickness(float value);

private slots:
    void render();          //the actual render - function

private:
    void init();            //initialize things we need before rendering
    bool bSimulate{};           // Determines if the game is paused or not
    bool bGoNextFrame{};    // If the game should calculate the next frame or not
    bool bGameMode{};       // Editor mode or Gaming mode
    bool bEndGame{};        // Game is done or not

    QOpenGLContext *mContext{nullptr};  //Our OpenGL context
    bool mInitialized{false};

    Texture* mTexture[4]{nullptr};
    std::unordered_map<std::string, Texture*> mTextureMap;

    Shader *mShaderProgram[4]{nullptr};    //holds pointer the GLSL shader program

    Shader* plainShader{nullptr};
    Shader* textureShader{nullptr};
    Shader* phongShader{nullptr};
    Shader* cubeMapShader{nullptr};
    Shader* heightcurvesShader{ nullptr };

    Shader* currentShader{ nullptr };
    void PhongShaderUpdate();


    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;               //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};  //helper class to get some clean debug info from OpenGL
    class Logger *mLogger{nullptr};         //logger - Output Log in the application

    ///Helper function that uses QOpenGLDebugLogger or plain glGetError()
    void checkForGLerrors();

    void calculateFramerate();          //as name says

    ///Starts QOpenGLDebugLogger if possible
    void startOpenGLDebugger();

private:    // Objekter i scenen

    std::unordered_map<std::string, VisualObject*> mMap;
    std::vector<VisualObject*> mSceneObjects;

    Quadtree* quadtree{nullptr};

    SimpleObject* Axis{nullptr};

/* ------ VISUALISERING OG SIMULERING ------- */
    float ElapsedTime{};
    /* Liten test overflate */
    Bakke* Bakken{nullptr};

    /* En Rullende Ball */
    QVector3D StartPosition{};
    QVector3D StartVelocity{};
    OctahedronBall* Ball{nullptr};

    /* --- NEDBØR --- */
//    std::vector<OctahedronBall*> mNedbor;
    std::vector<std::shared_ptr<OctahedronBall>> mNedbor;

//    void BallPhysics(OctahedronBall* object, Bakke* surface, float DeltaTime)
//    {
//        object->CalculatePhysics(surface, DeltaTime);
//        object->Update(DeltaTime);
//    }

    BSpline* splineTest{ nullptr };

    /* Hoydekart */
    HoydeKart* BigArea{nullptr};

//    std::vector<VisualObject*> mVisualObjects;

    SimpleObject* billboard{nullptr};

//    AABBfence* fence1{nullptr};
//    AABBfence* fence2{nullptr};

    ObjMesh* mCameraMesh{nullptr};

    Terrain* terrain{nullptr};          // Task 2: Terrain ---------------------------------

//    ObjMesh* monkey{nullptr};
//    ObjMesh* CubeCube{nullptr};

    BezierCurve* curve{nullptr};
    std::unordered_map<std::string, VisualObject*> mBombs;

    Light* light{nullptr};              // Task 3: Sun ------------------------------------

    CubeMap* cubemap{nullptr};


private:    // Kamera og keyboard


    void handleInput();         // Camera and player movement
    void handleSingleInput(int key);
    Camera* mCamera{nullptr};
    float mAspectratio{1.f};

    std::unordered_map<int, bool> mKeyboard;
    float forward;
    float right;
    float up;
    std::unordered_map<int, bool> mMouse;
    bool mLeftMouse;
    bool mRightMouse;
    bool mMiddleMouse;
    float lastMouseX;
    float lastMouseY;

protected:
    //The QWindow that we inherit from have these functions to capture
    // - mouse and keyboard.
    // Uncomment to use (you also have to make the definitions of
    // these functions in the cpp-file to use them of course!)
    //
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;              //the only one we use now
    void keyReleaseEvent(QKeyEvent *event) override;
    //    void wheelEvent(QWheelEvent *event) override{}
};

inline float random_between_two_floats(float min, float max)
{
    return (min + 1) + (((float) rand()) / (float) RAND_MAX) * (max - (min + 1));
}

inline void PhysicsCalc(OctahedronBall* object, Bakke* surface, float DeltaTime)
{
    object->CalculatePhysics(surface, DeltaTime);
    object->Update(DeltaTime);
}

#endif // RENDERWINDOW_H
