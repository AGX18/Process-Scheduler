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


    this->scheduler = comboBox->itemText(0);


    QPushButton *startBtn = new QPushButton("Start", this);


     QHBoxLayout *headerLayout = new QHBoxLayout;

    headerLayout->addWidget(startBtn);
    headerLayout->addWidget(comboBox);


    mainLayout->addLayout(headerLayout);

    ProcessContainerWidget *processContainer = new ProcessContainerWidget(this);

    QPushButton *addprocessbtn = new QPushButton("Add process", this);
    QPushButton *resetprocessbtn = new QPushButton("Reset", this);
    QHBoxLayout *processconfig = new QHBoxLayout(centralWidget);
    processconfig -> addWidget(addprocessbtn);
    processconfig -> addWidget(resetprocessbtn);
    mainLayout-> addLayout(processconfig);


    // Add to your main layout
    mainLayout->addWidget(processContainer);

    /**
     * @brief Scheduler
     *
     *  FCFS : 0
        SJF Preemptive : 1
        SJF nonpreemptive : 2
        Priority preemptive :3
        priority nonpreemptive:4
        round robin :5
     */
    // std::vector<bool> Scheduler(6, 0);

    connect(addprocessbtn, &QPushButton::clicked, this, [this, processContainer, comboBox]() {
        qDebug() << "Add Process button clicked!";
        comboBox->setEnabled(false);
        processContainer->addProcess();

    });

    // connect()
    connect(comboBox, &QComboBox::currentTextChanged, this, [this](const QString &choice) {
        this->scheduler = choice;
        qDebug() << this->scheduler;
    });


    connect(resetprocessbtn, &QPushButton::clicked, this, [this, processContainer, comboBox]() {
        processContainer->clearAllProcesses();
        comboBox->setEnabled(false);
        ProcessWidget::resetCounter();

    });

    // Set the central widget
    setCentralWidget(centralWidget);

}




MainWindow::~MainWindow()
{
    delete ui;
}
