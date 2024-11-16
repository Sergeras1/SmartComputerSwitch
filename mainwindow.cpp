#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QTimer>
#include <QTimeEdit>
#include<QMessageBox>
#include <qt_windows.h>
#include<QDir>
#include<QSettings>
#include<QRadioButton>
//#include <QThread>

QTime timeSet;
QString TimeEditStr;
//SmartComputerSwitch w;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    autoRun();

    ui->setupUi(this);
    ui->timeLabel->setText(QTime::currentTime().toString("hh:mm:ss"));

    /*
     * Фоновое управление через поток
     * moveToThread(&process);
    connect(&process, &QThread::started, &w, MainWindow::backProcess);
    connect(&process, &QThread::finished, &w, QObject::deleteLater);

    process.start();

    connect(&w, &QMainWindow::destroyed, [&]() {
       process.quit();
       process.wait();
    });*/

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimer()));
    timer->start(1000);
    setSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::powerOff()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    // Получаем маркер (token) для этого процесса.
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) GetLastError();

    // Получаем LUID для привилегии выключения (shutdown).
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Получаем привилегию выключения (shutdown) для этого процесса.
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

    // Выключаем или перезагружаем в соответствие с ExitFlags
    if (!ExitWindowsEx(EWX_SHUTDOWN, 0)) GetLastError();

    close();
}

void MainWindow::autoRun()
{
    name = "SmartComputerSwitch";
#ifdef Q_OS_WIN32
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    settings.setValue(name, QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
    settings.sync();
#endif
}

void MainWindow::setSettings()
{
    QSettings settings("Rubin", "SmartComputerSwitch");
    QString saveTime = settings.value("timeSettings", "").toString();
    QTime enTime = ui->timeEdit->time();
    QString saveTimeEdit = settings.value("timeSettingsEdit", enTime).toString();
    enTime = QTime::fromString(saveTimeEdit, "h:mm");
    ui->timeEdit->setTime(enTime);
    ui->timeInfoLabel->setText(saveTime);

    bool isSaveChecked = settings.value("checkRB", false).toBool();
    ui->radioButton->setChecked(isSaveChecked);
}

void MainWindow::saveSettings()
{
    QSettings settings("Rubin", "SmartComputerSwitch");
    settings.setValue("timeSettings", TimeEditStr);
    settings.setValue("timeSettingsEdit", ui->timeEdit->text());
    settings.setValue("checkRB", ui->radioButton->isChecked());

}
void MainWindow::slotTimer() {
    QMessageBox msg;

    ui->timeLabel->setText(QTime::currentTime().toString("hh:mm:ss"));


    if(TimeEditStr == QTime::currentTime().toString("hh:mm")) {
       msg.setText("Компьютер");
       msg.exec();
       //powerOff();
    }

}


void MainWindow::on_pushButton_clicked() {
    QString setTimeEdit = ui->timeEdit->text();
    timeSet = QTime::fromString(setTimeEdit, "h:mm");
    TimeEditStr = timeSet.toString("hh:mm");
    ui->timeInfoLabel->setText(TimeEditStr);
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->timeInfoLabel->setText("");
    TimeEditStr = "";
}


void MainWindow::on_radioButton_clicked(bool checked)
{
    if(checked){
        saveSettings();
    }
    else {
        QSettings settings("Rubin", "SmartComputerSwitch");
        settings.clear();
    }

}

/*void MainWindow::backProcess()
{
    while(true) {
        slotTimer();
    }
}
*/


