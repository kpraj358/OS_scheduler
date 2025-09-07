#ifndef SCHEDULEVIEWER_H
#define SCHEDULEVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include <QPushButton>
#include "../ScheduleList.h"
#include "../TimeFrame.h"
#include <QString>
#include <QDebug>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <math.h>
#include <QScrollArea>

class ScheduleViewer : public QWidget
{
    Q_OBJECT
public:
    explicit ScheduleViewer(const ScheduleList& scheduleList, QWidget *parent = nullptr, QStackedWidget *stackedWidget = nullptr);
private:
    QStackedWidget *stackedWidget;
    QPushButton *backButton;
    QVBoxLayout *container;
    QVBoxLayout* makeGrantChartBlock(const TimeFrame& timeframe, bool isEnd);
    void setupGrantChart(QHBoxLayout* grantChart, const std::vector<TimeFrame>& timeLine);
    void setupGrantChart(QTableWidget* grantChart, const std::vector<TimeFrame>& timeLine);
    void  setupProcessesMetrics(QTableWidget* processTable, const ScheduleList& scheduleList);
    QTableWidgetItem* makeCell(const QString& text, const QString& bckColor = "#F8FCEB", const QString& frgColor="#393939");
private slots:
    void onBackClicked();
signals:


};

#endif // SCHEDULEVIEWER_H
