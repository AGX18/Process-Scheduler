#ifndef PROCESSCONTAINERWIDGET_H
#define PROCESSCONTAINERWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include "processwidget.h"

class ProcessContainerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessContainerWidget(QWidget *parent = nullptr);

    void addProcess(QString scheduler);
    void clearAllProcesses();
    QVector<ProcessWidget*> getAllProcessWidgets() const;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QScrollArea *m_scrollArea;
    QWidget *m_container;
    QGridLayout *m_gridLayout;
    int m_row;
    int m_col;
};

#endif // PROCESSCONTAINERWIDGET_H
