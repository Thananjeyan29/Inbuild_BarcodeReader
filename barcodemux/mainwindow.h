#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <wiringPi.h>
#include <QThread>
#include <QDebug>
#include <mcp3004.h>
#include <wiringPiSPI.h>
#include <softPwm.h>
#include <QtSerialPort/QSerialPort>
const char START_SCAN_ADDRESS = 0x02;   // Replace with your actual start address
const char START_SCAN_COMMAND = 0x01;   // Replace with your actual start command
const char STOP_SCAN_ADDRESS = 0x03;    // Replace with your actual stop address
const char STOP_SCAN_COMMAND = 0x04;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    // Declare a signal to indicate that a start response has been received
    void startResponseReceived(QByteArray response);

private slots:
    void on_start_clicked();

    void on_stop_clicked();
    // Declare a slot to handle response data when it arrives
    void handleResponse();


private:
    Ui::MainWindow *ui;\
    QString scannedBarcode;

// Define the start and stop commands as QByteArray
const QByteArray START_SCAN_COMMAND = QByteArray::fromHex("7E 00 08 01 00 02 01 AB CD");
const QByteArray STOP_SCAN_COMMAND = QByteArray(1, 0x04); // Replace with your actual stop command
};
#endif // MAINWINDOW_H
