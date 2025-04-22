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
    processconfig->addWidget(addprocessbtn);
    processconfig->addWidget(resetprocessbtn);
    mainLayout->addLayout(processconfig);

    // Add to your main layout
    mainLayout->addWidget(processContainer);

    connect(addprocessbtn, &QPushButton::clicked, this, [this, processContainer, comboBox]() {
        qDebug() << "Add Process button clicked!";
        comboBox->setEnabled(false);
        processContainer->addProcess(this->scheduler);
    });

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
    connect(startBtn, &QPushButton::clicked, this, [processContainer, this](){
        QList<ProcessWidget*> processWidgets = processContainer->findChildren<ProcessWidget*>();
        int i = 0;
        for (ProcessWidget* widget : processWidgets) {
            if(this->scheduler=="Round Robin"){
                RoundRobin::addProcessRR(new Process(widget->getProcess()));
            }
            else if (scheduler == "Priorty Preemptive") {
                PreemptivePriorityScheduling* pps = new PreemptivePriorityScheduling(this, processes);
                connect(pps, &PreemptivePriorityScheduling::dataChanged, this, &MainWindow::visualizeProcesses);
                connect(pps, &PreemptivePriorityScheduling::ProcessFinished, this, &MainWindow::visualizeProcesses);
                pps->schedule();
            }
            processes.push_back(widget->getProcess());

        }

        visualizeProcesses();
    });

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
    return Scheduler::running_process;
}

void MainWindow::clearScreen()
{
    if (centralWidget()) {
        if (QLayout* layout = centralWidget()->layout()) {
            clearLayout(layout);
        }
        centralWidget()->deleteLater();
    }
    clearChildWidgets(this);
}

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

void MainWindow::clearChildWidgets(QWidget* parent)
{
    if (!parent) return;
    const auto children = parent->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
    for (QWidget* child : children) {
        if (child != centralWidget()) {
            child->deleteLater();
        }
    }
}

void MainWindow::visualizeProcesses()
{
    clearScreen();

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedHeight(100);
    mainLayout->addWidget(view);

    auto addRectangleToScene = [scene, this]() {
        static int x = 0;
        static int currentTime = 0;

        // نعرض النص Idle فقط إذا لم تكن هناك عملية قيد التنفيذ
        QString displayText = "Idle";
        int runningPid = -1;

        // إذا كانت هناك عملية قيد التنفيذ، نعرض معرّف العملية بدلاً من Idle
        Process* runningProcess = getcurrentrunningprocess();
        if (runningProcess != nullptr) {
            runningPid = runningProcess->getProcessNumber();
            displayText = QString("P%1").arg(runningPid);
        }

        // إنشاء المستطيل
        QGraphicsRectItem *rect = scene->addRect(x, 0, 50, 50, QPen(Qt::black, 2), QBrush(Qt::cyan));

        // إضافة نص الوقت في الزاوية العليا
        QGraphicsTextItem* timeText = scene->addText(QString::number(currentTime));
        QFont smallFont = timeText->font();
        smallFont.setPointSize(7);
        timeText->setFont(smallFont);
        timeText->setDefaultTextColor(Qt::black);
        timeText->setPos(x + 2, 0);

        // إضافة نص معرّف العملية أو Idle
        QGraphicsTextItem* pidText = scene->addText(displayText);
        QFont pidFont = pidText->font();
        pidFont.setPointSize(10);
        pidFont.setBold(true);
        pidText->setFont(pidFont);
        pidText->setDefaultTextColor(Qt::black);

        // تمركز النص داخل المستطيل
        QRectF rectBounds = rect->rect();
        QRectF textBounds = pidText->boundingRect();
        qreal centerX = x + (rectBounds.width() - textBounds.width()) / 2;
        qreal centerY = (rectBounds.height() - textBounds.height()) / 2;
        pidText->setPos(centerX, centerY);

        // تحديث الموضع للوقت التالي
        x += 55;
        currentTime++;
    };

    // إعداد المؤقت لإضافة المستطيلات كل ثانية
    QTimer *rectTimer = new QTimer(this);
    connect(rectTimer, &QTimer::timeout, addRectangleToScene);
    rectTimer->start(1000);

    // إنشاء الجدول لعرض العمليات
    QTableWidget *table = new QTableWidget(ProcessWidget::getCounter(), 6, this);
    table->setHorizontalHeaderLabels({"Process ID", "Arrival", "Burst", "Remaining", "Turnaround", "Waiting"});
    table->verticalHeader()->setVisible(false);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // ملء الجدول بالبيانات
    for (int row = 0; row < ProcessWidget::getCounter(); ++row) {
        QString value = QString("%1").arg(row);
        table->setItem(row, 0, new QTableWidgetItem(value));
    }

    for (int row = 0; row < ProcessWidget::getCounter(); ++row) {
        QString value = QString("%1").arg(processes[row].getArrivalTime());
        table->setItem(row, 1, new QTableWidgetItem(value));
    }

    for (int row = 0; row < ProcessWidget::getCounter(); ++row) {
        QString value = QString("%1").arg(processes[row].getBurstTime());
        table->setItem(row, 2, new QTableWidgetItem(value));
        table->setItem(row, 3, new QTableWidgetItem(value));
    }

    for (int row = 0; row < ProcessWidget::getCounter(); ++row) {
        QString value = QString("%1").arg(-1);
        table->setItem(row, 4, new QTableWidgetItem(value));
        table->setItem(row, 5, new QTableWidgetItem(value));
    }

    // إضافة لوحة لعرض الوقت
    QWidget *rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    QLabel *timerLabel = new QLabel("Time: 0", this);
    QFont font = timerLabel->font();
    font.setPointSize(14);
    font.setBold(true);
    timerLabel->setFont(font);
    rightLayout->addWidget(timerLabel);

    QTimer *stopwatch = new QTimer(this);
    int *timeCounter = new int(0);
    connect(stopwatch, &QTimer::timeout, this, [timerLabel, timeCounter]() {
        (*timeCounter)++;
        timerLabel->setText(QString("Time: %1").arg(*timeCounter));
    });
    stopwatch->start(1000);
    rightLayout->addWidget(table);

    mainLayout->addWidget(rightPanel);
    setCentralWidget(centralWidget);
}

