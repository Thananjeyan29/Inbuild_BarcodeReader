#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <wiringPi.h>
#include <wiringSerial.h>
#include <QDebug>
#include <unistd.h>

#define S0_PIN 21 //Tx
#define S1_PIN 22 //Rx
QSerialPort uart_fd;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wiringPiSetup();
    // Set the pins to OUTPUT mode
       pinMode(S0_PIN, OUTPUT);
       pinMode(S1_PIN, OUTPUT);
       // Set pins to select channel 0 (01 in binary)
           digitalWrite(S0_PIN, HIGH);
           digitalWrite(S1_PIN, LOW);

              // Initialize the UART connection
           uart_fd.setPortName("/dev/ttyS0"); // Replace with your port name (e.g., COM1, /dev/ttyS0)
           uart_fd.setBaudRate(QSerialPort::Baud9600); // Set the baud rate
           uart_fd.setDataBits(QSerialPort::Data8); // Set data bits
           uart_fd.setParity(QSerialPort::NoParity); // Set parity
           uart_fd.setStopBits(QSerialPort::OneStop); // Set stop bits
           uart_fd.setFlowControl(QSerialPort::NoFlowControl); // Set flow control

           // Connect the serial port when the application starts
           if (uart_fd.open(QIODevice::ReadWrite)) {
               qDebug() << "Serial port opened successfully.";
           } else {
               qDebug() << "Failed to open serial port.";
           }


}

MainWindow::~MainWindow()
{
   // serialClose(uart_fd);
   // uart_fd.close();

    delete ui;
}


//void MainWindow::on_start_clicked()
//{
//    qDebug() << "Sending start command...";
//    int result = write(uart_fd, &startCommand, 1);
//    if (result == -1) {
//        qDebug() << "Error sending start command: " << strerror(errno);
//    } else {
//        qDebug() << "Start command sent successfully.";
//    }

//}



void MainWindow::on_start_clicked()
{
    qDebug() << "Sending start command...";
      uart_fd.write(START_SCAN_COMMAND);

      // Connect the readyRead signal to the handleResponse slot
      connect(&uart_fd, &QSerialPort::readyRead, this, &MainWindow::handleResponse);
}

void MainWindow::on_stop_clicked()
{
    qDebug() << "Sending stop command...";
        uart_fd.write(STOP_SCAN_COMMAND);

        // Disconnect the readyRead signal if it was connected during scanning
        disconnect(&uart_fd, &QSerialPort::readyRead, this, &MainWindow::handleResponse);

}

void MainWindow::handleResponse()
{
    // Read the response
    QByteArray response = uart_fd.readAll();
    qDebug() << "Response received: " << response.toHex();

    // Check if the response indicates that the scanner has started scanning
    if (response.startsWith(QByteArray::fromHex("02")))
    {
        qDebug() << "Scanner has started scanning.";
    }
    else
    {
        // Search for the markers before and after the barcode data
        QByteArray startMarker = "31j";
        QByteArray endMarker = "\x02";

        int startIndex = response.indexOf(startMarker);
        int endIndex = response.indexOf(endMarker, startIndex + startMarker.length());

        if (startIndex != -1 && endIndex != -1 && endIndex > startIndex)
        {
            // Extract the barcode data between the markers
            QByteArray barcodeData = response.mid(startIndex + startMarker.length(), endIndex - startIndex - startMarker.length());

            // Convert the extracted data to a QString
            QString extractedBarcode = QString::fromUtf8(barcodeData);
            qDebug() << "Extracted Barcode Data: " << extractedBarcode;
        }
    }

    // Disconnect the readyRead signal from the handleResponse slot
    disconnect(&uart_fd, &QSerialPort::readyRead, this, &MainWindow::handleResponse);
}

