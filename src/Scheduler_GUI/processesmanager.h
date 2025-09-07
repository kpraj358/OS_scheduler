#ifndef PROCESSESTABLE_H
#define PROCESSESTABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QStackedWidget>
#include <QRadioButton>
#include <QHeaderView>
#include <QMessageBox>
#include "scheduleviewer.h"
#include <QTableWidgetItem>
#include <QSpinBox>
#include "../ScheduleList.h"
#include "../SJF.h"
#include "../FCFS.h"


class ProcessesManager : public QWidget
{
    Q_OBJECT
const int MAX_QSPINBOX =  1500;

public:
    explicit ProcessesManager(QStackedWidget *stackedWidget, Scheduler *scheduler, QWidget *parent = nullptr, int schedulerType = 0);
    ~ProcessesManager();
    void initialize(int schedulerType);
private:
    QTableWidgetItem *createCenteredQTableWidgetItem(const QString& text);
    QDoubleSpinBox *arrivalTimeInput;
    QDoubleSpinBox *burstTimeInput;
    QSpinBox *priorityInput;
    QLabel *arrivalTimeLabel;
    QLabel *burstTimeLabel;
    QLabel *priorityLabel;
    QPushButton *continueButton;
    QPushButton *backButton;
    QLineEdit *processNameInput;
    QPushButton *addProcessButton;
    QTableWidget *processesTable;
    QStackedWidget *stackedWidget;
    QRadioButton *preemptiveButton;
    QRadioButton *nonPreemptiveButton;
    QSpinBox *quantumTimeInput;
    Scheduler *scheduler;
    QMessageBox *msgBox;

signals:

private slots:
    void onAddProcessClicked();
    void onBackClicked();
    void onContinueClicked();



};

#endif // PROCESSESTABLE_H
