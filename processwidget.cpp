#include "processwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

int ProcessWidget::counter = 0;

// ProcessWidget::ProcessWidget(QWidget *parent = nullptr): QWidget(parent) {
//     name = new QLabel("Process " + QString::number(counter), this);
//     arrivalSpinner = new QSpinBox(this);
//     burstSpinner = new QSpinBox(this);
//     prioritySpinner = new QSpinBox(this);

//     // Configure spin boxes
//     arrivalSpinner->setRange(0, 1000000);
//     burstSpinner->setRange(1, 1000000);
//     prioritySpinner->setRange(0, 100);

//     // Layout setup
//     QHBoxLayout *layout = new QHBoxLayout(this);


//     QVBoxLayout *arrival = new QVBoxLayout(this);

//     QVBoxLayout *burst = new QVBoxLayout(this);

//     QVBoxLayout *priority = new QVBoxLayout(this);




//     arrival->addWidget(new QLabel("Arrival:"));
//     arrival->addWidget(arrivalSpinner);
//     burst->addWidget(new QLabel("Burst:"));
//     burst->addWidget(burstSpinner);
//     priority->addWidget(new QLabel("Priority:"));
//     priority->addWidget(prioritySpinner);


//     layout->addWidget(name);
//     layout->addLayout(arrival);
//     layout->addLayout(burst);
//     layout->addLayout(priority);

//     counter++;  // Increment counter after creating the widget
//     setLayout(layout);



// }



ProcessWidget::ProcessWidget(QWidget *parent = nullptr, bool isPriority = false): QWidget(parent) {
    name = new QLabel("Process " + QString::number(counter), this);
    arrivalSpinner = new QSpinBox(this);
    burstSpinner = new QSpinBox(this);



    // Configure spin boxes
    arrivalSpinner->setRange(0, 1000000);
    burstSpinner->setRange(1, 1000000);

    // Layout setup
    QHBoxLayout *layout = new QHBoxLayout(this);


    QVBoxLayout *arrival = new QVBoxLayout(this);

    QVBoxLayout *burst = new QVBoxLayout(this);





    arrival->addWidget(new QLabel("Arrival:"));
    arrival->addWidget(arrivalSpinner);
    burst->addWidget(new QLabel("Burst:"));
    burst->addWidget(burstSpinner);



    layout->addWidget(name);
    layout->addLayout(arrival);
    layout->addLayout(burst);

    if (isPriority) {
        prioritySpinner = new QSpinBox(this);
        prioritySpinner->setRange(0, 100);
        QVBoxLayout *priority = new QVBoxLayout(this);
        priority->addWidget(new QLabel("Priority:"));
        priority->addWidget(prioritySpinner);
        layout->addLayout(priority);
    }

    counter++;  // Increment counter after creating the widget
    setLayout(layout);



}


Process* ProcessWidget::getProcess() {
    int arrivalTime = arrivalSpinner->value();
    int burstTime = burstSpinner->value();
    int priority = prioritySpinner->value();
    // TODO: set isPriority based on the value of the scheduler

    int processID = id;

    return new Process(id, arrivalTime, burstTime, priority);
}


void ProcessWidget::resetCounter() {
    counter = 0;
}
