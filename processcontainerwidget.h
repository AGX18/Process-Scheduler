#ifndef PROCESSCONTAINERWIDGET_H
#define PROCESSCONTAINERWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include "processwidget.h"
#include "process.h"
#include <vector>

class ProcessContainerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProcessContainerWidget(QWidget *parent = nullptr);
    void addProcess(QString scheduler);
    void clearAllProcesses();  // New function to clear all processes

protected:
    void resizeEvent(QResizeEvent *event) override;  // For custom resize handling

private:
    QScrollArea *m_scrollArea;
    QWidget *m_container;
    QGridLayout *m_gridLayout;
    int m_row;
    int m_col;
};

#endif // PROCESSCONTAINERWIDGET_H
