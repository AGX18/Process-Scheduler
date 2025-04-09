#include "processwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

int ProcessWidget::counter = 0;

ProcessWidget::ProcessWidget(QWidget *parent = nullptr): QWidget(parent) {
    name = new QLabel("Process " + QString::number(counter), this);
    arrivalSpinner = new QSpinBox(this);
    burstSpinner = new QSpinBox(this);
    prioritySpinner = new QSpinBox(this);

    // Configure spin boxes
    arrivalSpinner->setRange(0, 1000000);
    burstSpinner->setRange(1, 1000000);
    prioritySpinner->setRange(0, 100);

    // Layout setup
    QHBoxLayout *layout = new QHBoxLayout(this);


    QVBoxLayout *arrival = new QVBoxLayout(this);

    QVBoxLayout *burst = new QVBoxLayout(this);

    QVBoxLayout *priority = new QVBoxLayout(this);





    arrival->addWidget(new QLabel("Arrival:"));
    arrival->addWidget(arrivalSpinner);
    burst->addWidget(new QLabel("Burst:"));
    burst->addWidget(burstSpinner);
    priority->addWidget(new QLabel("Priority:"));
    priority->addWidget(prioritySpinner);


    layout->addWidget(name);
    layout->addLayout(arrival);
    layout->addLayout(burst);
    layout->addLayout(priority);

    counter++;  // Increment counter after creating the widget
    setLayout(layout);

}


