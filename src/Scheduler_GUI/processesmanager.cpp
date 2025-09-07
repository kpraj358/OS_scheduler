#include "processesmanager.h"

ProcessesManager::ProcessesManager(QStackedWidget *stackedWidget, Scheduler* scheduler, QWidget *parent, int schedulerType)
    : QWidget{parent}
{
    this -> scheduler = scheduler;
    this -> stackedWidget = stackedWidget;
    initialize(schedulerType);
}

ProcessesManager::~ProcessesManager() {
    delete scheduler;
}
void ProcessesManager::initialize(int schedulerType) {

    msgBox = new QMessageBox(QMessageBox::Warning, "Warning", "", QMessageBox::Close);
    setStyleSheet("QSpinBox, QLineEdit, QDoubleSpinBox{ background-color: #F8FCEB; }");
    processNameInput = new QLineEdit();
    processNameInput -> setPlaceholderText("Process Name");

    burstTimeInput = new QDoubleSpinBox();
    arrivalTimeInput = new QDoubleSpinBox();
    burstTimeInput -> setMaximum(MAX_QSPINBOX);
    // Process should atleast 1 busrt time
    burstTimeInput -> setMinimum(1);
    arrivalTimeInput -> setMaximum(MAX_QSPINBOX);
    addProcessButton = new QPushButton("Add Process");

    arrivalTimeLabel = new QLabel("Arrival Time");
    burstTimeLabel = new QLabel("Burst Time");


    QVBoxLayout  *inputBox = new QVBoxLayout();
    inputBox -> setAlignment(Qt::AlignTop);

    inputBox -> addWidget(processNameInput);
    inputBox -> addWidget(arrivalTimeLabel);
    inputBox -> addWidget(arrivalTimeInput);
    inputBox -> addWidget(burstTimeLabel);
    inputBox -> addWidget(burstTimeInput);

    priorityLabel = new QLabel("Priority");
    priorityInput = new QSpinBox();
    priorityInput -> setMaximum(MAX_QSPINBOX);
    inputBox -> addWidget(priorityLabel);
    inputBox -> addWidget(priorityInput);
    if(schedulerType != Scheduler::PRIORITY) {
        priorityInput -> hide();
        priorityLabel -> hide();
    }


    inputBox -> addWidget(addProcessButton);

    preemptiveButton = new QRadioButton("Preemptive");
    nonPreemptiveButton = new QRadioButton("Non Preemptive");
    nonPreemptiveButton -> setChecked(true);
    inputBox -> addWidget(preemptiveButton);
    inputBox -> addWidget(nonPreemptiveButton);

    if(schedulerType != Scheduler::PRIORITY && schedulerType != Scheduler::SJF) {
        preemptiveButton -> hide();
        nonPreemptiveButton -> hide();
    }
    QLabel *quantumLabel = new QLabel("Quantum");
    quantumTimeInput = new QSpinBox();
    quantumTimeInput -> setMaximum(1500);
    quantumTimeInput -> setMinimum(1);
    inputBox -> addWidget(quantumLabel);
    inputBox -> addWidget(quantumTimeInput);
    if(schedulerType != Scheduler::RB) {
        quantumTimeInput -> hide();
        quantumLabel -> hide();
    }

    processesTable = new QTableWidget();
    processesTable -> setFrameStyle(QFrame::NoFrame);
    processesTable -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    processesTable -> setShowGrid(false);
    processesTable -> verticalHeader() -> setVisible(false);
    QStringList colNames = {"Name", "Arrival Time", "BurstTime", "Priority"};

    processesTable -> setColumnCount(colNames.size());
    processesTable -> setHorizontalHeaderLabels(colNames);
    processesTable -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
    processesTable -> setSelectionMode(QAbstractItemView::NoSelection);
    if(schedulerType != Scheduler::PRIORITY) {
        processesTable -> setColumnHidden(3, true);
    }
    QHBoxLayout *top = new QHBoxLayout();

    top -> addLayout(inputBox);
    top -> addWidget(processesTable);

    continueButton = new QPushButton("Continue");
    backButton = new QPushButton("Back");

    QHBoxLayout *bottom = new QHBoxLayout();
    bottom -> addWidget(backButton);
    bottom -> addWidget(continueButton);
    bottom -> setAlignment(Qt::AlignRight);
    QVBoxLayout *container = new QVBoxLayout();
    container -> addLayout(top);
    container -> addLayout(bottom);
    setLayout(container);


    connect(addProcessButton, SIGNAL (clicked()), this, SLOT (onAddProcessClicked()));
    connect(backButton, SIGNAL (clicked()), this, SLOT (onBackClicked()));
    connect(continueButton, SIGNAL (clicked()), this, SLOT (onContinueClicked()));

}

void ProcessesManager::onAddProcessClicked() {
    // Check if no process name is provided
    if(processNameInput -> text() == "") {
        msgBox->setText("Please enter a process name");
        msgBox -> exec();
        return;
    }

    // Getting values from input fields
    double arrivalTime = arrivalTimeInput -> value();
    double burstTime = burstTimeInput -> value();
    int priority = priorityInput ->value();
    QString processName = processNameInput -> text();

    // Checks if process name is already present
    for(int i = 0 ; i < processesTable -> rowCount(); ++i) {
        if(processesTable -> item(i, 0) -> text() == processName) {
            msgBox->setText("Process name is already present");
            msgBox -> exec();
            return;
        }
    }



    // Adding process to the schedule
    scheduler -> addProcess(Process(processName.toStdString(), arrivalTime, burstTime, priority));

    // Adding new row to table
    int rows = processesTable -> rowCount();

    processesTable -> insertRow(rows);
    processesTable -> setItem(rows, 0, createCenteredQTableWidgetItem(processName));
    processesTable -> setItem(rows, 1, createCenteredQTableWidgetItem(QString::number(arrivalTime)));
    processesTable -> setItem(rows, 2, createCenteredQTableWidgetItem(QString::number(burstTime)));
    processesTable -> setItem(rows, 3, createCenteredQTableWidgetItem(QString::number(priority)));

    // Resting input fields
    arrivalTimeInput -> setValue(0);
    burstTimeInput -> setValue(0);
    priorityInput -> setValue(0);
    processNameInput -> setText("");

}

void ProcessesManager::onContinueClicked() {
    // Checks if there are any processes
    if(scheduler -> processCount() == 0) {
        msgBox -> setText("Please add a process first");
        msgBox -> exec();
        return;
    }
    scheduler -> setPreemptive(preemptiveButton -> isChecked());
    scheduler -> setQuantum(quantumTimeInput -> value());
    stackedWidget -> addWidget(new ScheduleViewer(scheduler->schedule(), nullptr, stackedWidget));
    stackedWidget -> setCurrentIndex(stackedWidget -> count() - 1);

}
void ProcessesManager::onBackClicked() {
    stackedWidget -> removeWidget(this);
    delete this;
}

QTableWidgetItem *ProcessesManager::createCenteredQTableWidgetItem(const QString& text) {
    QTableWidgetItem *tableItem = new QTableWidgetItem(text);
    tableItem -> setTextAlignment(Qt::AlignCenter);
    tableItem -> setBackground(QBrush(QColor("#F8FCEB")));


    return tableItem;
}
