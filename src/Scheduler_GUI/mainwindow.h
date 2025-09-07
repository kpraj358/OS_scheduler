#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QStandardItem>
#include "processesmanager.h"
#include <QStackedWidget>
#include "../SJF.h"
#include "../FCFS.h"
#include "../RR.h"
#include "../Priority.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLabel *header;
    QComboBox *schedulerSelect;
    QPushButton *continueButton;
    QStackedWidget *stackedWidget;

private slots:
    void onContinueButtonClicked();
};
#endif // MAINWINDOW_H
