#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>

#include "renderwindow.h"
#include "logger.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    //this sets up what's in the mainwindow.ui - the GUI
    ui->setupUi(this);
    init(); //initializes parts of the program
}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}

void MainWindow::init()
{
    //This will contain the setup of the OpenGL surface we will render into
    QSurfaceFormat format;

    //OpenGL v 4.1 - (Ole Flatens Mac does not support higher than this - sorry!)
    //you can try other versions, but then have to update RenderWindow and Shader
    //to inherit from other than QOpenGLFunctions_4_1_Core
    //(The 3DProgramming course does not use anything from higher than 4.1, so please don't change it)
    format.setVersion(4, 1);
    //Using the main profile for OpenGL - no legacy code permitted
    format.setProfile(QSurfaceFormat::CoreProfile);
    //A QSurface can be other types than OpenGL
    format.setRenderableType(QSurfaceFormat::OpenGL);

    //This should activate OpenGL debug Context used in RenderWindow::startOpenGLDebugger().
    //This line (and the startOpenGLDebugger() and checkForGLerrors() in RenderWindow class)
    //can be deleted, but it is nice to have some OpenGL debug info!
    format.setOption(QSurfaceFormat::DebugContext);

    // The surface will need a depth buffer - (not requiered to set in glfw-tutorials)
    format.setDepthBufferSize(24);

    //Just prints out what OpenGL format we try to get
    // - this can be deleted
    qDebug() << "Requesting surface format: " << format;

    //We have a surface format for the OpenGL window, so let's make it:
    mRenderWindow = new RenderWindow(format, this);

    //Check if renderwindow did initialize, else prints error and quit
    if (!mRenderWindow->context()) {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderWindow;
        return;
    }

    //The OpenGL RenderWindow got made, so continuing the setup:
    //We put the RenderWindow inside a QWidget so we can put in into a
    //layout that is made in the .ui-file
    // (Qt has a QOpenGLWidget but that forces us to use more Qt specific OpenGL-stuff
    //  So we make a renderwindow manually to get more freedom)
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow);
    //OpenGLLayout is made in the mainwindow.ui-file!
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();

    //feed in MainWindow to the logger - has to be done, else logger will crash program
    Logger::getInstance()->setMainWindow(this);;

    grabKeyboard();
}

void MainWindow::OnStart()
{
//    /* Justere på teksten i gamemode boksen */
//    {
//        ui->TextMode->setText(t_GameMode);
//        ui->TextMode->setAlignment(Qt::AlignCenter);
//        ui->TextMode->setFontWeight(500);
//    }
}

void MainWindow::SetBallStartPositionText(const QVector3D startPos)
{
    ui->BallStartPositionSetX->setValue(startPos.x());
    ui->BallStartPositionSetY->setValue(startPos.y());
    ui->BallStartPositionSetZ->setValue(startPos.z());
}

void MainWindow::SetBallStartVelocityText(const QVector3D startVel)
{
    ui->BallStartVelocitySetX->setValue(startVel.x());
    ui->BallStartVelocitySetY->setValue(startVel.y());
    ui->BallStartVelocitySetZ->setValue(startVel.z());
}

void MainWindow::SetBallCurrentPositionText(const QVector3D Position)
{
    ui->BallCurrentPositionX->setValue(Position.x());
    ui->BallCurrentPositionY->setValue(Position.y());
    ui->BallCurrentPositionZ->setValue(Position.z());
}

void MainWindow::SetElapsedTime(const float Time)
{
    ui->ElapsedTime->setText(QString::fromStdString(std::to_string(Time)) + "s");
    ui->ElapsedTime->setAlignment(Qt::AlignCenter);
}

void MainWindow::SetLightStartValues(const QVector3D lightPosition)
{
    ui->LightPositionX->setValue(lightPosition.x());
    ui->LightPositionY->setValue(lightPosition.y());
    ui->LightPositionZ->setValue(lightPosition.z());
}

bool MainWindow::GetUsingPhongShader()
{
    return ui->UsingPhongShader->checkState();
}



//File menu Exit closes the program
void MainWindow::on_fileExit_triggered()
{
    close();       //Shuts down the whole program
}

// Task 6: GUI elements ----------------------------------------------------------
static bool editor{true};
//void MainWindow::on_GameMode_toggled(bool checked)
//{
//    mRenderWindowContainer->setFocus();
//    if (checked)
//    {
//        ui->GameMode->setText("Editor Mode");
//        t_GameMode = "Game";
//        editor = false;
//    }
//    else
//    {
//        ui->GameMode->setText("Game Mode");
//        t_GameMode = "Editor";
//        editor = true;
//    }
//    on_PauseButton_clicked(checked);
//    mRenderWindow->togglePlay(checked);
//}

void MainWindow::on_PauseButton_clicked(bool checked)
{
    /* If GameMode is the editor. The pause button shows Simulate, to allow for the game
     * to run in editor mode */
    if (editor && !checked)
    {
        t_Paused = "Simulate";
    }
    ui->PauseButton->setText(t_Paused);
    if (checked)
    {
        ui->NextFrameButton->setText("");
        t_Paused = "Simulating";
    }
    else
    {
        ui->NextFrameButton->setText("Next\nFrame");
        t_Paused = "Pause";
    }

    /* Pauser spillet i Rendervinduet */
    mRenderWindow->togglePause(checked);
}


static bool bPressedNextFrameButton{false};
void MainWindow::on_NextFrameButton_pressed()
{
    if (!bPressedNextFrameButton)
    {
        mRenderWindow->GoNextFrame();

        bPressedNextFrameButton = true;
    }
}

void MainWindow::on_NextFrameButton_released()
{
    bPressedNextFrameButton = false;
}



void MainWindow::on_ResetButton_pressed()
{
//    ui->PauseButton->setText("Simulate");
//    on_PauseButton_clicked(true);
//    ui->PauseButton->toggle();
//    on_PauseButton_clicked(true);
    mRenderWindow->Reset();
//    ui->ElapsedTime->setText("0.02s");
//    ui->ElapsedTime->setAlignment(Qt::AlignCenter);
}


void MainWindow::on_BallStartPositionSetX_valueChanged(double arg1)
{
    mRenderWindow->ChangeBallStartPositionX(arg1);
}

void MainWindow::on_BallStartPositionSetY_valueChanged(double arg1)
{
    mRenderWindow->ChangeBallStartPositionY(arg1);
}

void MainWindow::on_BallStartPositionSetZ_valueChanged(double arg1)
{
    mRenderWindow->ChangeBallStartPositionZ(arg1);
}


void MainWindow::on_BallStartVelocitySetX_valueChanged(double arg1)
{
    mRenderWindow->ChangeBallStartVelocityX(arg1);
}

void MainWindow::on_BallStartVelocitySetY_valueChanged(double arg1)
{
    mRenderWindow->ChangeBallStartVelocityY(arg1);
}

void MainWindow::on_BallStartVelocitySetZ_valueChanged(double arg1)
{
    mRenderWindow->ChangeBallStartVelocityZ(arg1);
}


void MainWindow::on_LagNedbor_pressed()
{
    int antall = ui->DroperAntall->value();
    float lengde = ui->DroperTilfeldigLengdeXY->value();
    float hoyde = ui->DroperHoydefraTerrain->value();

    if (ui->DropercheckBoxTilfeldigHoyde->checkState())
    {
        float tilfeldigHoyde = ui->DroperTilfeldigHoyde->value();
        mRenderWindow->MakeNedbor(antall, lengde, hoyde, true, tilfeldigHoyde);
        return;
    }
    mRenderWindow->MakeNedbor(antall, lengde, hoyde);
}


void MainWindow::on_LagNedbor_2_pressed()
{
    mRenderWindow->SlettNedbor();
}


void MainWindow::on_DropercheckBoxTilfeldigHoyde_toggled(bool checked)
{
    ui->TilfeldigHoydeTekst->setEnabled(checked);
    ui->DroperTilfeldigHoyde->setEnabled(checked);
}


void MainWindow::on_UsingPhongShader_toggled(bool checked)
{
    mRenderWindow->UsingPhongShader(checked);

}


void MainWindow::on_LightPositionZ_valueChanged(double arg1)
{
    float z = arg1;
    mRenderWindow->MoveLight({0,0,z});
}


void MainWindow::on_LightPositionY_valueChanged(double arg1)
{
    float y = arg1;
    mRenderWindow->MoveLight({0,y,0});
}


void MainWindow::on_LightPositionX_valueChanged(double arg1)
{
    float x = arg1;
    mRenderWindow->MoveLight({x,0,0});
}


void MainWindow::on_HeightCurveSlider_Step_valueChanged(int value)
{
    const int dpi = 10;
    float v = value;
    v /= dpi;
    mRenderWindow->HeightCurve_ChangeStep(v);
}


void MainWindow::on_HeightCurveSlider_Thickness_valueChanged(int value)
{
    const int dpi = 100;
    float v = value;
    v /= dpi;
    mRenderWindow->HeightCurve_ChangeThickness(v);
}




void MainWindow::on_HeightCurves_Button_toggled(bool checked)
{
    mRenderWindow->ShowHeightCurves(checked);

    ui->HeightCurveSlider_Step->setEnabled(checked);
    ui->HeightCurveSlider_Thickness->setEnabled(checked);
    ui->HeightCurveText_Step->setEnabled(checked);
    ui->HeightCurveText_Thickness->setEnabled(checked);

    if (checked)
    {
        ui->HeightCurves_Button->setText("Disable\nHeight Curves");
    }
    else
    {
        ui->HeightCurves_Button->setText("Show\nHeight Curves");
    }
}


void MainWindow::on_BSpline_ShowCurves_toggled(bool checked)
{
    mRenderWindow->ShowSplineCurves(checked);
    ui->BSpline_ShowPoints->setEnabled(checked);
}


void MainWindow::on_BSpline_ShowPoints_toggled(bool checked)
{
    mRenderWindow->ShowSplinePoints(checked);
}

