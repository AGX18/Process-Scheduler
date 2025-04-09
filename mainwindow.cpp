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

    // Configure layout spacing
    gridLayout->setHorizontalSpacing(10);  // Space between columns
    gridLayout->setVerticalSpacing(15);    // Space between rows
    gridLayout->setContentsMargins(10, 10, 10, 10);  // Margins around edges

    // Add widgets in 3-column layout
    int row = 0, col = 0;
    for (int i = 0; i < 9; i++) {  // Example with 9 items
        ProcessWidget *process = new ProcessWidget(container);

        gridLayout->addWidget(process, row, col);

        // Move to next position
        col++;
        if (col >= 2) {  // 2 items per row
            col = 0;
            row++;
        }
    }

    // Set up scroll area
    container->setLayout(gridLayout);
    scrollArea->setWidget(container);

    // Add to your main layout
    mainLayout->addWidget(scrollArea);



    // Set the central widget
    setCentralWidget(centralWidget);

}




MainWindow::~MainWindow()
{
    delete ui;
}
