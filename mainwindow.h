#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTcpSocket>
#include <QObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString host = "172.16.131.170";
    int port = 1026;
    QString voltage = "5.321";
    QString amperage = "1.0005";
    bool hostSet = false;
    bool portSet = false;
    int minAmperage = 0.0000;
    int maxAmperage = 15000;
    int minVoltage = 0;
    int maxVoltage = 12000;
    int step = 1;
    int exponentAmperage = 10000;
    int exponentVoltage = 1000;

private:
    Ui::MainWindow *ui;
    QTcpSocket* socket;
    void connectToHostSlot();
    void sendToHostSlot();
    void readyReadSlot();
    void clearErr();
    void setVol();
    void setAmp();
    void checkErrSlot();
    void setHost();
    void setPort();
    void setDefault();
    void dialValChangedSlot();
    void getVol();
    void getAmp();
    void power();

private slots:
    void on_connButton_clicked();
    void on_setDefaulButton_clicked();
    void on_sendCommandButton_clicked();
    void on_checkErrorsButton_clicked();
    void on_dial_valueChanged(int value);
    void on_powerButton_clicked();
    void on_getAmperageButton_clicked();
    void on_getVoltageButton_clicked();
    void on_editHost_returnPressed();
    void on_editPort_returnPressed();
    void on_commandInput_returnPressed();
    void on_voltageInput_returnPressed();
    void on_amperageInput_returnPressed();
};
#endif // MAINWINDOW_H
