#ifndef PROCESSWIDGET_H
#define PROCESSWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <string>
#include <QString>

#include "process.h"
#include <string>
class ProcessWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ProcessWidget(QWidget *parent , bool isPriority);
    Process getProcess();
    static void resetCounter();
    int getProcessID();
    int getArrivalTime();
    int getBurstTime();
    int getPriority();
    static int getCounter();

private:
    QLabel* name;
    QSpinBox *arrivalSpinner;
    QSpinBox *burstSpinner;
    QSpinBox *prioritySpinner;
    static int counter;
    bool isPriority;
    int id;
};

#endif // PROCESSWIDGET_H
