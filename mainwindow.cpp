#include "mainwindow.h"
#include "./ui_mainwindow.h"

int row = 0;
int col = 0;

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



    // QGraphicsScene* scene = new QGraphicsScene(this);


    // QGraphicsView* view = new QGraphicsView(scene);

    // view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);  // Ensure it's visible
    // view->verticalScrollBar();

    // view->show();

    // Create scroll area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    // Create container widget
    QWidget *container = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(container);
    // Set up scroll area
    scrollArea->setWidget(container);

    // Configure layout spacing
    gridLayout->setHorizontalSpacing(25);  // Space between columns
    gridLayout->setVerticalSpacing(15);    // Space between rows
    gridLayout->setContentsMargins(10, 10, 10, 10);  // Margins around edges


    QPushButton *addprocessbtn = new QPushButton("Add process", this);
    QPushButton *resetprocessbtn = new QPushButton("Reset", this);
    QHBoxLayout *processconfig = new QHBoxLayout(centralWidget);
    processconfig -> addWidget(addprocessbtn);
    processconfig -> addWidget(resetprocessbtn);
    mainLayout-> addLayout(processconfig);
    // Configure grid layout properties
    gridLayout->setSpacing(15);  // Space between items
    gridLayout->setContentsMargins(10, 10, 10, 10);  // Margins around the grid


    // Add to your main layout
    mainLayout->addWidget(scrollArea);

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

    connect(addprocessbtn, &QPushButton::clicked, this, [this, container, gridLayout, comboBox]() {
        qDebug() << "Add Process button clicked!";
        comboBox->setEnabled(false);
        ProcessWidget *process = new ProcessWidget(container, true);
        gridLayout->addWidget(process, row, col);
        //     // Move to next position
        col++;
        if (col >= 2) {  // 2 items per row
            col = 0;
            row++;
        }

    });

    // connect()
    connect(comboBox, &QComboBox::currentTextChanged, this, [this](const QString &choice) {
        this->scheduler = choice;
        qDebug() << this->scheduler;
    });


    // connect(resetprocessbtn, &QPushButton::clicked, this, [this, &container, &gridLayout, scrollArea, comboBox]() {
    //     delete container;
    //     container = new QWidget();
    //     gridLayout = new QGridLayout(container);
    //     // Set up scroll area
    //     scrollArea->setWidget(container);
    //     comboBox->setEnabled(false);
    //     row = 0;
    //     col = 0;
    // });

    // Set the central widget
    setCentralWidget(centralWidget);

}




MainWindow::~MainWindow()
{
    delete ui;
}
