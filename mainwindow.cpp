#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create a central widget and layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QComboBox *comboBox = new QComboBox(this);
    // Add items to the drop-down menu
    comboBox->addItem("FCFS");
    comboBox->addItem("SJF Preemptive");
    comboBox->addItem("SJF non-Preemptive");
    comboBox->addItem("Priorty Preemptive");
    comboBox->addItem("Priorty non-Preemptive");
    comboBox->addItem("Round Robin");

    QPushButton *startBtn = new QPushButton("Start", this);
    QPushButton *stopBtn = new QPushButton("Stop", this);


    QLabel *averageWaitingTimelabel = new QLabel("Average Waiting Time:", this);
    QLabel *averageTurnaroundTimelabel = new QLabel("Average Turnaround Time:",this);

    QLabel *averageWaitingTimeValue = new QLabel("NA", this);
    QLabel *averageTurnaroundTimeValue = new QLabel("NA", this);

    // Add widgets to layout
    mainLayout->addWidget(comboBox);
    mainLayout->addWidget(startBtn);
    mainLayout->addWidget(stopBtn);
    mainLayout->addWidget(averageWaitingTimelabel);
    mainLayout->addWidget(averageWaitingTimeValue);
    mainLayout->addWidget(averageTurnaroundTimelabel);
    mainLayout->addWidget(averageTurnaroundTimeValue);

    // Set the central widget
    setCentralWidget(centralWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}
