///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of NodeMCU Studio.                                   //
//    Copyright (C) 2015 Jacob Dawid, jacob@omg-it.works                     //
//                                                                           //
//    NodeMCU Studio is free software: you can redistribute it and/or modify //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    NodeMCU Studio is distributed in the hope that it will be useful,      //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with NodeMCU Studio. If not, see <http://www.gnu.org/licenses/>. //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QMessageBox>
#include <QLineEdit>
#include <QMdiSubWindow>

// Own includes
#include "mainwindow.h"

// UIC includes
#include "ui_mainwindow.h"

#include <QDebug>

#define BOOKMARK_MARKER 1
#define DEBUGGER_MARKER 2
#define BREAKPOINT_MARKER 3

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    _serialPort = new QextSerialPort;
    connect(_serialPort, SIGNAL(readyRead()), this, SLOT(readSerial()));
    serialPortSurvey();

    QStringList serialCommandList;
    serialCommandList << "=node.heap()";

    _serialCommandCompleter = new QCompleter(serialCommandList, this);
    ui->serialSendComboBox->lineEdit()->setCompleter(_serialCommandCompleter);

    addEditorWindow();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_connectPushButton_clicked() {
    _serialPort->setPortName(ui->deviceComboBox->currentText());

    _serialPort->setBaudRate((BaudRateType)ui->baudRateComboBox->currentText().toInt());
    _serialPort->setDataBits((DataBitsType)ui->dataBitsComboBox->currentText().toInt());

    switch(ui->parityComboBox->currentIndex()) {
    case 0: _serialPort->setParity(PAR_NONE); break;
    case 1: _serialPort->setParity(PAR_ODD); break;
    case 2: _serialPort->setParity(PAR_EVEN); break;
    #if defined(Q_OS_WIN) || defined(qdoc)
    case 3: _serialPort->setParity(PAR_MARK); break;
    #endif
    case 4: _serialPort->setParity(PAR_SPACE); break;
    default:
        qDebug() << "Parity not supported on this OS.";
        break;
    }

    switch(ui->stopBitsComboBox->currentIndex()) {
    case 0: _serialPort->setStopBits(STOP_1); break;
    #if defined(Q_OS_WIN) || defined(qdoc)
    case 1: _serialPort->setStopBits(STOP_1_5); break;
    #endif
    case 2: _serialPort->setStopBits(STOP_2); break;
    default:
        qDebug() << "Stop bits not supported on this OS.";
    }

    _serialPort->setDtr(ui->enableDTRCheckBox->isChecked());
    _serialPort->setRts(ui->enableRTSCheckBox->isChecked());

    _serialPort->setTimeout((long)ui->timeoutSpinBox->value());

    _serialPort->open(QextSerialPort::ReadWrite);
    if(_serialPort->isOpen()) {
        ui->serialTextEdit->setEnabled(true);
        ui->serialSendComboBox->setEnabled(true);
        ui->sendPushButton->setEnabled(true);

        ui->connectPushButton->setEnabled(false);
        ui->disconnectPushButton->setEnabled(true);
    } else {
        QMessageBox::critical(this, tr("Connection failed"), tr("Reason: ") + _serialPort->errorString());
    }
}

void MainWindow::on_serialSendComboBox_activated(QString command) {
    QByteArray data = command.toAscii();
    data.append("\r\n");
    _serialPort->write(data);

    ui->serialSendComboBox->lineEdit()->clear();
}

void MainWindow::on_sendPushButton_clicked() {
    on_serialSendComboBox_activated(ui->serialSendComboBox->currentText());
}

void MainWindow::serialPortSurvey() {
    ui->deviceComboBox->addItem("/dev/ttyUSB0");
}

void MainWindow::readSerial() {
    QByteArray data = _serialPort->readAll();
    ui->serialTextEdit->moveCursor(QTextCursor::End);
    ui->serialTextEdit->textCursor().insertText(QString::fromAscii(data));
    ui->serialTextEdit->moveCursor(QTextCursor::End);
}

void MainWindow::addEditorWindow() {
    _editor = new QsciScintilla;
    _luaLexer = new QsciLexerLua;
    _editor->setLexer(_luaLexer);
    _luaLexer->setFont(QFont("Courier New"));

    // symbols
    _editor->setMarginType(1, QsciScintilla::SymbolMargin);
    _editor->setMarginWidth(1, "xx");
    _editor->setMarginSensitivity (1, true);
    _editor->markerDefine(QsciScintilla::RightTriangle, BOOKMARK_MARKER);
    _editor->setMarkerBackgroundColor(QColor (0,0,232), BOOKMARK_MARKER);
    _editor->markerDefine(QsciScintilla::Circle, BREAKPOINT_MARKER);
    _editor->setMarkerBackgroundColor(QColor (192,0,0), BREAKPOINT_MARKER);
    _editor->markerDefine(QsciScintilla::RightTriangle, DEBUGGER_MARKER);
    _editor->setMarkerBackgroundColor(QColor (255,255,0), DEBUGGER_MARKER);

    // line numbers
    _editor->setMarginsForegroundColor(QColor (96, 96, 96));
    _editor->setMarginsBackgroundColor(QColor (232, 232, 220));
    _editor->setMarginType(2, QsciScintilla::TextMargin);
    _editor->setMarginWidth(2, "xxxx");
    _editor->setMarginLineNumbers(2, true);

    // other features
    _editor->setBraceMatching(QsciScintilla::StrictBraceMatch);
    _editor->setAutoIndent(true);
    _editor->setIndentationWidth(2);
    _editor->setIndentationsUseTabs(false);

    _editor->setCaretLineVisible(true);
    _editor->setCaretLineBackgroundColor(QColor (248, 248, 248));

    _editor->setUtf8(true);

    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(_editor);
    subWindow->setWindowIcon(QIcon("://icons/text-x-generic-template.svg"));
    subWindow->showMaximized();
}

