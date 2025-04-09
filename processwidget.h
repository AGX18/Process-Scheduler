#ifndef PROCESSWIDGET_H
#define PROCESSWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>

#include "process.h"

class ProcessWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ProcessWidget(QWidget *parent);


private:
    QLabel* name;
    QSpinBox *arrivalSpinner;
    QSpinBox *burstSpinner;
    QSpinBox *prioritySpinner;
    static int counter;
    int id;
};

#endif // PROCESSWIDGET_H
