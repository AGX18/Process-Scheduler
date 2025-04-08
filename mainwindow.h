#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QComboBox *comboBox;
    QPushButton *startBtn;
    QPushButton *stopBtn;
    QLabel *averageWaitingTimelabel;
    QLabel *averageTurnaroundTimelabel;
    QLabel *averageWaitingTimeValue;
    QLabel *averageTurnaroundTimeValue;

};
#endif // MAINWINDOW_H
