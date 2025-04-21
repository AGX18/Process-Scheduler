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

    QSpinBox* timeQ = nullptr;
    QLabel* label = nullptr;

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
    connect(comboBox, &QComboBox::currentTextChanged, this, [this, timeQ, label, headerLayout](const QString &choice) mutable{
        this->scheduler = choice;
        qDebug() << this->scheduler;
        if (scheduler == "Round Robin") {
            if (timeQ == nullptr) {
                timeQ = new QSpinBox(this);
                label = new QLabel("Time Quantum", this);
                headerLayout->addWidget(label);
                headerLayout->addWidget(timeQ);
                connect(timeQ, &QSpinBox::valueChanged, [this](int value) {
                    this->timeQuantum = value;
                    qDebug() << this->timeQuantum;
                });
            }
        } else {
            if (timeQ != nullptr) {
                timeQ->deleteLater();
                timeQ = nullptr;
            }
            if (label != nullptr) {
                label->deleteLater();
                label = nullptr;
            }
        }
    });


    connect(resetprocessbtn, &QPushButton::clicked, this, [this, processContainer, comboBox]() {
        processContainer->clearAllProcesses();
        comboBox->setEnabled(true);
        ProcessWidget::resetCounter();

    });



    // start button
    // rerender
    connect(startBtn, &QPushButton::clicked, this, [processContainer, this](){
        /**
         * add all the processes
        */
        // std::vector<Process*>* processes = new std::vector<Process*>();
        QList<ProcessWidget*> processWidgets = processContainer->findChildren<ProcessWidget*>();
        int i = 0;
        for (ProcessWidget* widget : processWidgets) {
            if(this->scheduler=="Round Robin"){
                RoundRobin::addProcessRR(new Process(widget->getProcess()));
            }
            processes.push_back(widget->getProcess());

        }


        visualizeProcesses();


    });


    // connect(startBtn, &QPushButton::clicked, this
    // visualizeProcesses();


    // Set the central widget
    setCentralWidget(centralWidget);

}




MainWindow::~MainWindow()
{
    if (schedulingThread && schedulingThread->isRunning()) {
        schedulingThread->quit();
        schedulingThread->wait();
    }
    delete ui;

}

Process* MainWindow::getcurrentrunningprocess() {
    if (MainWindow::processes.empty()) return nullptr;
    return Scheduler::running_process;  // Return a pointer to the first process
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
    // === GANTT CHART AREA ===
    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedHeight(100); // Thin look

    mainLayout->addWidget(view); // Add scene to the layout

    // Example: Function to add a rectangle every second (you'll call this via QTimer)
    // auto addRectangleToScene = [scene]() {
    //     static int x = 0;
    //     QGraphicsRectItem *rect = scene->addRect(x, 0, 50, 50, QPen(Qt::black, 2), QBrush(Qt::blue));
    //     x += 60;
    // };
    auto addRectangleToScene = [scene, this]() {
        static int x = 0;
        static int currentTime = 0;

        // Decide whether a process is running — if not, show idle
        QString displayText = "Idle";
        int runningPid = -1;

        if (getcurrentrunningprocess() != nullptr) {
            Process* runningProcess = getcurrentrunningprocess();
            int runningPid = runningProcess->getProcessNumber();
            displayText = QString("P%1").arg(runningPid);
        }

        // Create rectangle
        QGraphicsRectItem *rect = scene->addRect(x, 0, 50, 50, QPen(Qt::black, 2), QBrush(Qt::cyan));

        // Time label in small font (top-left)
        QGraphicsTextItem* timeText = scene->addText(QString::number(currentTime));
        QFont smallFont = timeText->font();
        smallFont.setPointSize(7);
        timeText->setFont(smallFont);
        timeText->setDefaultTextColor(Qt::black);
        timeText->setPos(x + 2, 0);

        // Process ID or Idle label (centered)
        QGraphicsTextItem* pidText = scene->addText(displayText);
        QFont pidFont = pidText->font();
        pidFont.setPointSize(10);
        pidFont.setBold(true);
        pidText->setFont(pidFont);
        pidText->setDefaultTextColor(Qt::black);

        // Center it inside the rectangle
        QRectF rectBounds = rect->rect();
        QRectF textBounds = pidText->boundingRect();
        qreal centerX = x + (rectBounds.width() - textBounds.width()) / 2;
        qreal centerY = (rectBounds.height() - textBounds.height()) / 2;
        pidText->setPos(centerX, centerY);

        // Prepare for next tick
        x += 55;
        currentTime++;
    };


    QTimer *rectTimer = new QTimer(this);
    connect(rectTimer, &QTimer::timeout, addRectangleToScene);
    rectTimer->start(1000); // 1 second interval

    // === TABLE AREA ===
    QTableWidget *table = new QTableWidget(ProcessWidget::getCounter(), 6, this); // 'counter' is your row count
    table->setHorizontalHeaderLabels({"Process ID", "Arrival", "Burst", "Remaining", "Turnaround", "Waiting"});
    table->verticalHeader()->setVisible(false);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // First column : Process ID -> does not change
    for (int row = 0; row < ProcessWidget::getCounter(); ++row) {
        QString value = QString("%1").arg(row);
        table->setItem(row, 0, new QTableWidgetItem(value));
    }

    // Second column : Arrival Time
    for (int row = 0; row < ProcessWidget::getCounter(); ++row) {
        QString value = QString("%1").arg(processes[row].getArrivalTime());
        table->setItem(row, 1, new QTableWidgetItem(value));
    }


    // Third and Fourth column : Burst Time and Remaining
    for (int row = 0; row < ProcessWidget::getCounter(); ++row) {
        QString value = QString("%1").arg(processes[row].getBurstTime());
        table->setItem(row, 2, new QTableWidgetItem(value));
        table->setItem(row, 3, new QTableWidgetItem(value));
    }


    // Fifth column : Turnaround Time
    for (int row = 0; row < ProcessWidget::getCounter(); ++row) {
        QString value = QString("%1").arg(-1);
        table->setItem(row, 4, new QTableWidgetItem(value));
        table->setItem(row, 5, new QTableWidgetItem(value));
    }





    // === RIGHT SIDE PANEL ===
    QWidget *rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    // Timer Label
    QLabel *timerLabel = new QLabel("Time: 0", this);
    QFont font = timerLabel->font();
    font.setPointSize(14);
    font.setBold(true);
    timerLabel->setFont(font);
    rightLayout->addWidget(timerLabel);

    // Update label every second
    QTimer *stopwatch = new QTimer(this);
    int *timeCounter = new int(0); // Or use a class member instead
    connect(stopwatch, &QTimer::timeout, this, [timerLabel, timeCounter]() {
        (*timeCounter)++;
        timerLabel->setText(QString("Time: %1").arg(*timeCounter));
    });
    stopwatch->start(1000);

    // Arrival Time Widget
    QSpinBox *arrivalSpin = new QSpinBox(this);
    arrivalSpin->setRange(0, 1000);
    QLabel *arrivalLabel = new QLabel("Arrival Time", this);

    // Burst Time Widget
    QSpinBox *burstSpin = new QSpinBox(this);
    burstSpin->setRange(1, 1000);
    QLabel *burstLabel = new QLabel("Burst Time", this);

    QPushButton* liveMode = new QPushButton("Activate Live Mode", this);


    rightLayout->addWidget(liveMode);
    QPushButton* addProcess;



    connect(liveMode, &QPushButton::clicked, [this, liveMode, rightLayout, addProcess, table]() mutable {
        if (liveMode != nullptr) {
            liveMode->deleteLater();
            liveMode = nullptr;
        }
        auto havePriority = [](QString scheduler) {
            if (scheduler == "Priorty Preemptive" | scheduler == "Priorty non-Preemptive") {
                return true;
            }
            return false;
        };
        ProcessWidget *liveProcess = new ProcessWidget(this, havePriority(this->scheduler));
        addProcess = new QPushButton("add Process", this);
        rightLayout->addWidget(addProcess);
        rightLayout->addWidget(liveProcess);
        // rightLayout->addStretch(); // Push everything up

        connect(addProcess, &QPushButton::clicked, [this, rightLayout, havePriority, liveProcess, table]() mutable {
            int row = ProcessWidget::getCounter() - 1;
            qDebug() << row;
            table->insertRow(row);
            QString value = QString("%1").arg(row);
            table->setItem(row, 0, new QTableWidgetItem(value));

            // Arrival Time
            value = QString("%1").arg(liveProcess->getArrivalTime());
            table->setItem(row, 1, new QTableWidgetItem(value));


            value = QString("%1").arg(liveProcess->getBurstTime());
            table->setItem(row, 2, new QTableWidgetItem(value));
            table->setItem(row, 3, new QTableWidgetItem(value));

            value = QString("%1").arg(-1);
            table->setItem(row, 4, new QTableWidgetItem(value));
            table->setItem(row, 5, new QTableWidgetItem(value));


            // emit the info
            emit sendNewProcessInfo(new Process(row, liveProcess->getArrivalTime(), liveProcess->getBurstTime(), liveProcess->getPriority()));

            liveProcess->deleteLater();
            liveProcess = new ProcessWidget(this, havePriority(this->scheduler));
            rightLayout->addWidget(liveProcess);
        });

    });




    // rightLayout->addWidget(arrivalLabel);
    // rightLayout->addWidget(arrivalSpin);
    // rightLayout->addWidget(burstLabel);
    // rightLayout->addWidget(burstSpin);
    // rightLayout->addStretch(); // Push everything up

    // === Combine Table + Right Panel ===
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(table, 3);       // Takes 3x space
    bottomLayout->addWidget(rightPanel, 1);  // Takes 1x space

    mainLayout->addLayout(bottomLayout); // Add bottom layout to main vertical layout

    // Step 3: Set everything
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);


    // now we get to the part where we visualize the scheduling of the process.

    Scheduler* choosenScheduler;
    RoundRobin *RR;
    // assign the choosenScheduler
    if (this->scheduler == "Round Robin") {
        qDebug() << "round robin";
        RR = new RoundRobin(nullptr, this->processes, this->timeQuantum);
        choosenScheduler=RR;
        connect (this,&MainWindow::sendNewProcessInfo,RR,&RoundRobin::addNewProcessRR);
    }

    this->schedulingThread = new QThread(this);
    schedulingThread->setObjectName("Scheduling Thread");
    choosenScheduler->moveToThread(this->schedulingThread);
    QObject::connect(schedulingThread, &QThread::started, choosenScheduler, &Scheduler::schedule);

    // TODO: don't forget to connect the signals to the scheduler datachanged, ProcessFinished
    // and sendNewProcess
    QObject::connect(this, &MainWindow::sendNewProcessInfo, choosenScheduler, &Scheduler::addNewProcess, Qt::QueuedConnection);

    QObject::connect(choosenScheduler, &Scheduler::dataChanged, [table](int processID) {
        qDebug() << "decrementing";
        QTableWidgetItem* item = table->item(processID, 3);
        if (item) {
            int value = item->text().toInt();
            item->setText(QString::number(value - 1));
        }
    });

    // ProcessFinished
    QObject::connect(choosenScheduler, &Scheduler::ProcessFinished, [table](int processID,int waitingTime, int TurnaroundTime) {
        qDebug() << "finished";
        QTableWidgetItem* TurnarounIitem = table->item(processID, 4);
        if (TurnarounIitem) {
            TurnarounIitem->setText(QString::number(TurnaroundTime));
        }

        QTableWidgetItem* waitingItem = table->item(processID, 5);
        if (waitingItem) {
            waitingItem->setText(QString::number(waitingTime));
        }
    });

    connect(this->schedulingThread, &QThread::finished, choosenScheduler, &QObject::deleteLater);


    schedulingThread->start();

}
