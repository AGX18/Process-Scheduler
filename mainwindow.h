#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QSlider>
#include <QVBoxLayout>
#include <QResizeEvent>
#include "processwidget.h"
#include <QScrollArea>
#include <QGridLayout>
#include <vector>
#include <string>
#include "processcontainerwidget.h"
#include "scheduler.h"
#include <QThread>
#include <QDebug>
#include "process.h"
#include <QTimer>
#include <QTableWidget>
#include <QHeaderView>
#include "roundrobin.h"
#include "priority_scheduler.h"
#include "fcfs_scheduler.h"
#include "sjf_scheduler.h"

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
    // QComboBox *comboBox;
    // QPushButton *startBtn;
    int timeQuantum;

    QThread* schedulingThread;

    QString scheduler;

    std::vector<Process> processes;


    void clearScreen();

    // Helper function to recursively clear a layout
    void clearLayout(QLayout* layout);

    // Helper function to clear all child widgets
    void clearChildWidgets(QWidget* parent);

    bool havePriority(QString scheduler) {
        if (scheduler == "Priorty Preemptive" | scheduler == "Priorty non-Preemptive") {
            return true;
        }
        return false;
    }

signals:
    void sendNewProcessInfo(Process* process);


public slots:
    void visualizeProcesses();


};
#endif // MAINWINDOW_H
