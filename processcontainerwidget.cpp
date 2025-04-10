#include "processcontainerwidget.h"
#include <QVBoxLayout>

ProcessContainerWidget::ProcessContainerWidget(QWidget *parent)
    : QWidget(parent), m_row(0), m_col(0)
{
    // Set size policy to expand in both directions
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Main layout with zero margins
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create scroll area
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_scrollArea->setFrameShape(QFrame::NoFrame);  // Clean look

    // Create container widget
    m_container = new QWidget();
    m_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);

    // Create grid layout for container
    m_gridLayout = new QGridLayout(m_container);
    m_gridLayout->setHorizontalSpacing(25);
    m_gridLayout->setVerticalSpacing(15);
    m_gridLayout->setContentsMargins(15, 15, 15, 15);

    // Final setup
    m_scrollArea->setWidget(m_container);
    mainLayout->addWidget(m_scrollArea);
}

void ProcessContainerWidget::addProcess(QString scheduler)
{
    bool isPriority = false;

    if (scheduler == "Priorty Preemptive" | scheduler == "Priorty non-Preemptive") {
        isPriority = true;
    }

    // TODO: set the boolean value passed on the type of the scheduler
    ProcessWidget *process = new ProcessWidget(m_container, isPriority);
    m_gridLayout->addWidget(process, m_row, m_col);

    // Update position
    m_col++;
    if (m_col >= 2) {  // 2 items per row
        m_col = 0;
        m_row++;
    }

    // Adjust container size
    m_container->adjustSize();
}

void ProcessContainerWidget::clearAllProcesses()
{
    // Remove all widgets from layout
    QLayoutItem *item;
    while (item = m_gridLayout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    // Reset counters
    m_row = 0;
    m_col = 0;
}

void ProcessContainerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // Optional: Add custom resize handling if needed
}
