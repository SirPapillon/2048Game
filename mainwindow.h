#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void OpenMenu(int won);
    void ClearScore();
    int StartNewPoint();
    void NewGame();
    void CreateLabelAnimation(int target_index);
    void ResizeLabelAnimation(int target_index);
    void as();
    void setValue_on_Gridindex(int index,int value,int source_index);
    void remove_source_label(int source_index);
    void set_Config_onLabel_byIndex(QLabel * target_lbl, int index,int isCreate);
    void goRight();
    void goLeft();
    void goDown();
    void goTop();
    void setScore(int score,int target_index);
    void AnimationC(int source_index,int target_index);
    QLabel *indexToQLabel(int index);

private slots:
    void on_pushButton_clicked();

    void on_newGame_btn_clicked();
    void keyPressEvent(QKeyEvent *event);
    void on_try_again_btn_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
