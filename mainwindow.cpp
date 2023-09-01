#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->history->setText("host : not set\nport : not set");
    ui->history->append(QTime::currentTime().toString() + " :No connection");
    socket = new QTcpSocket(this);

    ui->connButton->setEnabled(false);

    ui->expectedVoltageLabel->setText("Voltage: " + voltage);
    ui->expectedAmperageLabel->setText("Amperage: " + amperage);

    QObject::connect(socket, &QTcpSocket::readyRead, [=] () {
        qDebug("ready read!");
        QByteArray dataArr = socket->readAll();
        ui->commandsResponse->append(QTime::currentTime().toString() + ": " + dataArr);
    });

    ui->powerButton->setEnabled(false);
    ui->commandInput->setReadOnly(true);
    ui->voltageInput->setReadOnly(true);
    ui->amperageInput->setReadOnly(true);
    ui->powerButton->setEnabled(false);
    ui->amperageRadioButton->setEnabled(false);
    ui->voltageRadioButton->setEnabled(false);
    ui->checkErrorsButton->setEnabled(false);
    ui->sendCommandButton->setEnabled(false);
    ui->getVoltageButton->setEnabled(false);
    ui->getAmperageButton->setEnabled(false);
    ui->getAmperageButton->setEnabled(false);
    ui->getVoltageButton->setEnabled(false);
    ui->dial->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendToHostSlot() {
    socket->write(ui->commandInput->text().toLocal8Bit() + "\n");
    ui->commandInput->setText("");
}

void MainWindow::on_connButton_clicked()
{
    socket->connectToHost(host, port);
    if (socket->waitForConnected(1000)) {
        ui->history->append(QTime::currentTime().toString() + ": Connected");
        qDebug("Connected!");
    } else {
        ui->history->append(QTime::currentTime().toString() + ": Not connected");
        qDebug("Error occured while connecting");
        return;
    }

    ui->powerButton->setEnabled(true);
}

void MainWindow::on_setDefaulButton_clicked()
{
    port = 1026;
    host = "172.16.131.170";
    ui->editHost->setText(host);
    ui->editPort->setText(QString::number(port));
    ui->setDefaulButton->setEnabled(false);
    portSet = true;
    hostSet = true;
    ui->history->append(QTime::currentTime().toString() + " host: " + host);
    ui->history->append(QTime::currentTime().toString() + " port: " + QString::number(port));
    ui->connButton->setEnabled(true);
}

void MainWindow::on_sendCommandButton_clicked()
{
    sendToHostSlot();
}

void MainWindow::on_checkErrorsButton_clicked()
{
    QString errTxt = ":SYST:ERR?\n";
    socket->write(errTxt.toLocal8Bit());
}

void MainWindow::on_dial_valueChanged(int value)
{
    qDebug() << "value: " << ui->dial->value();
    if (ui->amperageRadioButton->isChecked()) {
        ui->dial->setMinimum(minAmperage);
        ui->dial->setMaximum(maxAmperage);
        ui->dial->setSingleStep(step);
        amperage = QString::number((float) ui->dial->value() / exponentAmperage);
        QString cmd = ":SOUR2:CURR " + amperage + "\n";
        ui->expectedAmperageLabel->setText("Amperage: " + amperage);
        qDebug() << cmd;
        socket->write(cmd.toLocal8Bit());
    } else if (ui->voltageRadioButton->isChecked()) {
        ui->dial->setMinimum(minVoltage);
        ui->dial->setMaximum(maxVoltage);
        ui->dial->setSingleStep(step);
        voltage = QString::number((float) ui->dial->value() / exponentVoltage);
        QString cmd = ":SOUR2:VOLT " + voltage + "\n";
        ui->expectedVoltageLabel->setText("Voltage: " + voltage);
        qDebug() << cmd;
        socket->write(cmd.toLocal8Bit());
    }
}

void MainWindow::on_powerButton_clicked()
{
    if (ui->powerButton->text() == "TURN ON") {
        QString cmd = ":OUTP:STAT ON";
        socket->write(cmd.toLocal8Bit());
        ui->amperageInput->setReadOnly(false);
        ui->voltageInput->setReadOnly(false);
        ui->commandInput->setReadOnly(false);
        ui->amperageRadioButton->setEnabled(true);
        ui->voltageRadioButton->setEnabled(true);
        ui->checkErrorsButton->setEnabled(true);
        ui->sendCommandButton->setEnabled(true);
        ui->getAmperageButton->setEnabled(true);
        ui->getVoltageButton->setEnabled(true);
        ui->dial->setEnabled(true);
        ui->commandInput->setText("*IDN?");
        ui->powerButton->setText("TURN OFF");
    } else if (ui->powerButton->text() == "TURN OFF") {
        QString cmd = ":OUTP:STAT OFF";
        socket->write(cmd.toLocal8Bit());
        ui->amperageInput->setReadOnly(true);
        ui->voltageInput->setReadOnly(true);
        ui->commandInput->setReadOnly(true);
        ui->amperageRadioButton->setEnabled(false);
        ui->voltageRadioButton->setEnabled(false);
        ui->checkErrorsButton->setEnabled(false);
        ui->sendCommandButton->setEnabled(false);
        ui->getAmperageButton->setEnabled(false);
        ui->getVoltageButton->setEnabled(false);
        ui->dial->setEnabled(false);
        ui->commandInput->setText("");
        ui->powerButton->setText("TURN ON");
    }
}


void MainWindow::on_getAmperageButton_clicked()
{
    QString cmd = ":SOUR2:CURR?";
    socket->write(cmd.toLocal8Bit());
    QString response = socket->readAll();
    ui->currentAmperageLabel->setText(response);
}

void MainWindow::on_getVoltageButton_clicked()
{
    QString cmd = ":SOUR2:VOLT?";
    socket->write(cmd.toLocal8Bit());
    QString response = socket->readAll();
    ui->currentVoltageLabel->setText(response);
}


void MainWindow::on_editHost_returnPressed()
{
    host = ui->editHost->text();
    ui->history->append(QTime::currentTime().toString() + " host: " + host);
    ui->setDefaulButton->setEnabled(true);
    hostSet = true;
    if (hostSet && portSet)
        ui->connButton->setEnabled(true);
}


void MainWindow::on_editPort_returnPressed()
{
    port = ui->editPort->text().toInt();
    ui->history->append(QTime::currentTime().toString() + " port: " + QString::number(port));
    ui->setDefaulButton->setEnabled(true);
    portSet = true;
    if (hostSet && portSet)
        ui->connButton->setEnabled(true);
}


void MainWindow::on_commandInput_returnPressed()
{
    sendToHostSlot();
}

void MainWindow::on_voltageInput_returnPressed()
{
    voltage = ui->voltageInput->text();
    QString cmd = ":SOUR2:VOLT " + voltage + "\n";
    ui->expectedVoltageLabel->setText("Voltage: " + voltage);
    socket->write(cmd.toLocal8Bit());
    ui->voltageInput->setText("");
}

void MainWindow::on_amperageInput_returnPressed()
{
    amperage = ui->amperageInput->text();
    QString cmd = ":SOUR2:CURR " + amperage + "\n";
    ui->expectedAmperageLabel->setText("Amperage: " + amperage);
    socket->write(cmd.toLocal8Bit());
    ui->amperageInput->setText("");
}

