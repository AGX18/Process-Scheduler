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
        processContainer->addProcess(this->scheduler);

    });

    // connect()
    connect(comboBox, &QComboBox::currentTextChanged, this, [this](const QString &choice) {
        this->scheduler = choice;
        qDebug() << this->scheduler;
    });


    connect(resetprocessbtn, &QPushButton::clicked, this, [this, processContainer, comboBox]() {
        processContainer->clearAllProcesses();
        comboBox->setEnabled(true);
        ProcessWidget::resetCounter();

    });



    // start button
    // rerender
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::visualizeProcesses);


    // Set the central widget
    setCentralWidget(centralWidget);

}




MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::clearScreen()
{
    // 1. Clear the central widget
    if (centralWidget()) {
        // Remove layout items first
        if (QLayout* layout = centralWidget()->layout()) {
            clearLayout(layout);
        }

        // Delete the central widget itself
        centralWidget()->deleteLater();
    }

    // 2. Clear any remaining top-level widgets
    clearChildWidgets(this);

}

// Helper function to recursively clear a layout
void MainWindow::clearLayout(QLayout* layout)
{
    if (!layout) return;

    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        else if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout);
        }
        delete item;
    }
}

// Helper function to clear all child widgets
void MainWindow::clearChildWidgets(QWidget* parent)
{
    if (!parent) return;

    const auto children = parent->findChildren<QWidget*>(
        QString(), Qt::FindDirectChildrenOnly);

    for (QWidget* child : children) {
        if (child != centralWidget()) {  // Skip central widget if still present
            child->deleteLater();
        }
    }
}

void MainWindow::visualizeProcesses()
{
    // first clear the screen
    clearScreen();





    // Create new central widget for new screen
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);


    // add new screen widgets here


    // now we get to the part where we visualize the scheduling of the process.

    // Scheduler* choosenScheduler;
    // // assign the choosenScheduler
    // switch (this->scheduler) {

    // }

    // QThread schedulingThread;
    // schedulingThread.setObjectName("Scheduling Thread");
    // choosenScheduler->moveToThread(&schedulingThread);
    // QObject::connect(&schedulingThread, &QThread::started, choosenScheduler, &Scheduler::schedule);

    setCentralWidget(centralWidget);

}
