#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QRadioButton>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QAction>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTimer *timer;
    QRadioButton *rb;
    QString name;

    void powerOff();
    void autoRun();
    void saveSettings();
    void setSettings();
private slots:
    void slotTimer();

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_radioButton_clicked(bool checked);
};
#endif // MAINWINDOW_H
