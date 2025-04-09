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



    QGraphicsScene* scene = new QGraphicsScene(this);


    QGraphicsView* view = new QGraphicsView(scene);

    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);  // Ensure it's visible
    view->verticalScrollBar();

    view->show();



    mainLayout->addWidget(view);

    // Set the central widget
    setCentralWidget(centralWidget);

}




MainWindow::~MainWindow()
{
    delete ui;
}
