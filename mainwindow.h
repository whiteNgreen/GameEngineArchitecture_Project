#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class RenderWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    ///Slot called from the File->Exit menu in the mainwindow.ui file
    void on_fileExit_triggered();

//    void on_GameMode_toggled(bool checked);

    void on_PauseButton_clicked(bool checked);


    void on_NextFrameButton_released();

    void on_NextFrameButton_pressed();

    void on_ResetButton_pressed();

//    void on_PauseButton_toggled(bool checked);

    void on_BallStartPositionSetX_valueChanged(double arg1);

    void on_BallStartPositionSetY_valueChanged(double arg1);

    void on_BallStartPositionSetZ_valueChanged(double arg1);

    void on_BallStartVelocitySetX_valueChanged(double arg1);

    void on_BallStartVelocitySetY_valueChanged(double arg1);

    void on_BallStartVelocitySetZ_valueChanged(double arg1);

    void on_LagNedbor_pressed();

    void on_LagNedbor_2_pressed();

    void on_DropercheckBoxTilfeldigHoyde_toggled(bool checked);

    void on_UsingPhongShader_toggled(bool checked);

    void on_LightPositionZ_valueChanged(double arg1);

    void on_LightPositionY_valueChanged(double arg1);

    void on_LightPositionX_valueChanged(double arg1);

    void on_HeightCurveSlider_Step_valueChanged(int value);

    void on_HeightCurveSlider_Thickness_valueChanged(int value);

    void on_HeightCurves_Button_toggled(bool checked);

    void on_BSpline_ShowCurves_toggled(bool checked);

    void on_BSpline_ShowPoints_toggled(bool checked);

private:
    ///called from the constructor. Initializes different parts of the program.
    void init();

    Ui::MainWindow *ui{nullptr};                //the GUI of the app
    QWidget *mRenderWindowContainer{nullptr};   //Qt container for the RenderWindow
    RenderWindow *mRenderWindow{nullptr};       //The class that actually renders OpenGL

    //Logger class uses private ui pointer from this class
    friend class Logger;

    QString t_GameMode{"Editor Mode"};
    QString t_Paused{"Paused"};

public:
    Ui::MainWindow* GetUi() const { return ui; }
    void OnStart();
    void SetBallStartPositionText(const QVector3D startPos);
    void SetBallStartVelocityText(const QVector3D startVel);
    void SetBallCurrentPositionText(const QVector3D Position);
    void SetElapsedTime(const float Time);
    void SetLightStartValues(const QVector3D lightPosition);

    bool GetUsingPhongShader();
};

#endif // MAINWINDOW_H
