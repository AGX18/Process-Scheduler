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


    QTextEdit *ProcessesInput = new QTextEdit();
    ProcessesInput->setPlaceholderText("Enter your Intial Processes");
    ProcessesInput->setAcceptRichText(false);  // For plain text only

    QPushButton *startBtn = new QPushButton("Start", this);
    QPushButton *stopBtn = new QPushButton("Stop", this);


    QLabel *averageWaitingTimelabel = new QLabel("Average Waiting Time:", this);
    QLabel *averageTurnaroundTimelabel = new QLabel("Average Turnaround Time:",this);

    QLabel *averageWaitingTimeValue = new QLabel("NA", this);
    QLabel *averageTurnaroundTimeValue = new QLabel("NA", this);



    QHBoxLayout *buttonLayout = new QHBoxLayout;

    QHBoxLayout *headerLayout = new QHBoxLayout;

    QVBoxLayout *outputLabelLayout = new QVBoxLayout;

    QHBoxLayout *averageWaitingLayout = new QHBoxLayout;

    averageWaitingLayout->addWidget(averageWaitingTimelabel);
    averageWaitingLayout->addWidget(averageWaitingTimeValue);


    QHBoxLayout *averageTurnaroundLayout = new QHBoxLayout;

    averageTurnaroundLayout->addWidget(averageTurnaroundTimelabel);
    averageTurnaroundLayout->addWidget(averageTurnaroundTimeValue);


    buttonLayout->addWidget(startBtn);
    buttonLayout->addWidget(stopBtn);
    buttonLayout->addWidget(comboBox);



    outputLabelLayout->addLayout(averageTurnaroundLayout);
    outputLabelLayout->addLayout(averageWaitingLayout);


    headerLayout->addLayout(buttonLayout);

    headerLayout->addLayout(outputLabelLayout);


    // Add widgets to layout
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(ProcessesInput);

    // Set the central widget
    setCentralWidget(centralWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}
