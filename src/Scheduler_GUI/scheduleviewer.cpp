#include "scheduleviewer.h"

ScheduleViewer::ScheduleViewer(const ScheduleList& scheduleList, QWidget *parent, QStackedWidget* stackedWidget)
    : QWidget{parent}
{
    this -> stackedWidget = stackedWidget;


    backButton = new QPushButton("Back");
    container = new QVBoxLayout(this);
    QScrollArea *scrollable = new QScrollArea();

    QTableWidget* processMetrics = new QTableWidget();

    setupProcessesMetrics(processMetrics, scheduleList);
    scrollable ->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollable -> setStyleSheet("padding:0; margin:0; border:0;");
    QHBoxLayout* grantLayout = new QHBoxLayout();
    grantLayout -> setAlignment(Qt::AlignCenter);

    setupGrantChart(grantLayout, scheduleList.getTimeline());
    QWidget* grantChart = new QWidget();
    grantChart -> setLayout(grantLayout);
    scrollable -> setWidget(grantChart);
    scrollable -> setAlignment(Qt::AlignCenter);

//    container -> addLayout(grantLayout);
    container -> addWidget(scrollable, 0, Qt::AlignTop);
    container -> addWidget(processMetrics, 0);
    container -> addWidget(backButton, 0, Qt::AlignRight | Qt::AlignBottom);

    connect(backButton, SIGNAL (clicked()), this, SLOT (onBackClicked()));
}

void ScheduleViewer::setupGrantChart(QHBoxLayout* layout, const std::vector<TimeFrame>& timeLine) {
    layout -> setSpacing(0);

    QVBoxLayout* column;
    for(int i = 0; i < timeLine.size(); ++i) {
        layout -> addLayout(makeGrantChartBlock(timeLine[i], (i + 1 == timeLine.size()) ));
    }

}
void ScheduleViewer::setupGrantChart(QTableWidget* grantChart, const std::vector<TimeFrame>& timeLine) {
    grantChart -> setRowCount(2);
    grantChart -> setColumnCount(timeLine.size() + 1);
    grantChart -> setShowGrid(false);
    grantChart -> setMaximumHeight(this -> height() * 0.2);
    grantChart -> setFocusPolicy(Qt::NoFocus);
    grantChart -> setSelectionMode(QAbstractItemView::NoSelection);
    grantChart -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    grantChart -> setFrameStyle(QFrame::NoFrame);

    grantChart -> verticalHeader() -> setVisible(false);
    grantChart -> horizontalHeader() -> setVisible(false);


    int heightMainRow = grantChart -> height() * 0.6;
    grantChart -> setRowHeight(0, heightMainRow);
    grantChart -> setRowHeight(1,  grantChart -> height() * 0.2);
    grantChart -> setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grantChart -> setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    auto backgroundColor = QBrush(QColor("#DDDDDD"));
    for(int i = 0; i < timeLine.size(); ++i) {
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(timeLine[i].getName()));

        item -> setBackground(backgroundColor);
        item -> setTextAlignment(Qt::AlignCenter);
        grantChart -> setItem(0, i, item);

        grantChart -> setColumnWidth(i, std::min(150. ,(timeLine[i].end - timeLine[i].start) * 5));
        QTableWidgetItem* time = new QTableWidgetItem(QString::number(timeLine[i].start));

        grantChart -> setItem(1, i, time);
    }
    grantChart -> setItem(1, timeLine.size(), new QTableWidgetItem(QString::number(timeLine[timeLine.size() - 1].end)));
}
void ScheduleViewer::setupProcessesMetrics(QTableWidget* processTable, const ScheduleList& scheduleList) {
//    processTable -> setMaximumWidth(this -> width() * 0.75);
    processTable -> setRowCount(scheduleList.numberOfProcesses() + 1);
    processTable -> setColumnCount(3);
    processTable -> setHorizontalHeaderLabels({"", "Waiting Time", "TurnAround Time"});
    processTable -> setFocusPolicy(Qt::NoFocus);
    processTable -> setSelectionMode(QAbstractItemView::NoSelection);
    processTable -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    processTable -> setShowGrid(false);
    processTable -> verticalHeader() -> setVisible(false);
    processTable -> setFrameStyle(QFrame::NoFrame);
    processTable -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);

    const auto & process = scheduleList.getProcesses();
    double totalWaitingTime = 0;
    double totalTurnAroundTime = 0;



    for(int i = 0; i < process.size(); ++i) {
        PMetric pmetric = scheduleList.getProcessMetric(process[i]);
        totalWaitingTime += pmetric.waitingTime;
        totalTurnAroundTime += pmetric.turnAroundTime;
        QTableWidgetItem* waitingTime = makeCell(QString::number(pmetric.waitingTime));
        QTableWidgetItem* turnAroundTime = makeCell(QString::number(pmetric.turnAroundTime));
        QTableWidgetItem *name = makeCell(QString::fromStdString(process[i]->getName()));

        processTable -> setItem(i, 0, name);
        processTable->setItem(i, 1, waitingTime);
        processTable->setItem(i, 2, turnAroundTime);
    }


    QTableWidgetItem *avgWaitItem = makeCell(QString::number((totalWaitingTime) / scheduleList.numberOfProcesses()),
                                             "#303841", "#EEEEEE");
    QTableWidgetItem *avgTurnAroundItem = makeCell(QString::number((totalTurnAroundTime) / scheduleList.numberOfProcesses()),
                                                   "#303841", "#EEEEEE");
    QTableWidgetItem *name = makeCell("Average", "#303841", "#EEEEEE");

    processTable -> setItem(scheduleList.numberOfProcesses(), 0, name);
    processTable -> setItem(scheduleList.numberOfProcesses(), 1, avgWaitItem);
    processTable -> setItem(scheduleList.numberOfProcesses(), 2, avgTurnAroundItem);
}
void ScheduleViewer::onBackClicked() {
    stackedWidget -> removeWidget(this);
    delete this;
}

QTableWidgetItem* ScheduleViewer::makeCell(const QString& text, const QString& bckColor, const QString& frgColor) {
    QTableWidgetItem *cell= new QTableWidgetItem(text);
    cell -> setTextAlignment(Qt::AlignCenter);
    cell -> setBackground(QBrush(QColor(bckColor)));
    cell -> setForeground(QBrush(QColor(frgColor)));
    return cell;
}

QVBoxLayout* ScheduleViewer::makeGrantChartBlock(const TimeFrame& timeframe, bool isEnd) {
    QVBoxLayout* col = new QVBoxLayout();

    QLabel* processBlock = new QLabel(QString::fromStdString(timeframe.getName()));
    processBlock -> setAlignment(Qt::AlignCenter);
    QString bckColor = (timeframe.isIdle()) ? "#EEEDDE" :"#DDDDDD";
    processBlock -> setStyleSheet("background-color: " + bckColor + "; color: black; border-left: 1px dotted black; padding: 10px");
    int width = std::min( (timeframe.end - timeframe.start) * 20, 70.0);
    int height = 60;
    processBlock -> setMinimumSize(width, height);

    QHBoxLayout* timeLayout = new QHBoxLayout();
    QString timeBlockStyle = "background-color: #E6E6FA; color: black; padding-right:2px";
    QLabel* startTimeBlock = new QLabel(QString::number(timeframe.start));
    startTimeBlock -> setAlignment(Qt::AlignTop | Qt::AlignLeft);
    startTimeBlock -> setStyleSheet(timeBlockStyle);
    startTimeBlock -> setMinimumSize(width, 20);
    timeLayout -> addWidget((startTimeBlock));
    if(isEnd) {
        QLabel* endTimeBlock = new QLabel(QString::number(timeframe.end));
        endTimeBlock -> setStyleSheet(timeBlockStyle);
        endTimeBlock -> setAlignment(Qt::AlignTop | Qt::AlignRight);
        timeLayout -> addWidget(endTimeBlock);
    }

    col -> addWidget(processBlock);
    col -> addLayout(timeLayout);

    return col;
}
