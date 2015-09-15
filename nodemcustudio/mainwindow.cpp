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

    QStringList serialCommandList;
    serialCommandList
    << "node.restart"
    << "node.dsleep"
    << "node.info"
    << "node.chipid"
    << "node.flashid"
    << "node.heap"
    << "node.key"
    << "node.led"
    << "node.input"
    << "node.output"
    << "node.readvdd33"
    << "node.compile"
    << "node.setcpufreq"
    << "node.restore"
    << "file.remove"
    << "file.open"
    << "file.close"
    << "file.readline"
    << "file.writeline"
    << "file.read"
    << "file.write"
    << "file.flush"
    << "file.seek"
    << "file.list"
    << "file.format"
    << "file.rename"
    << "file.fsinfo"
    << "wifi.setmode"
    << "wifi.getmode"
    << "wifi.getchannel"
    << "wifi.setphymode"
    << "wifi.getphymode"
    << "wifi.startsmart"
    << "wifi.stopsmart"
    << "wifi.sleeptype"
    << "wifi.sta.getconfig"
    << "wifi.sta.config"
    << "wifi.sta.connect"
    << "wifi.sta.disconnect"
    << "wifi.sta.autoconnect"
    << "wifi.sta.getip"
    << "wifi.sta.setip"
    << "wifi.sta.getmac"
    << "wifi.sta.setmac"
    << "wifi.sta.getap"
    << "wifi.sta.status"
    << "wifi.sta.getbroadcast"
    << "wifi.sta.eventMonReg"
    << "wifi.sta.eventMonStart"
    << "wifi.sta.eventMonStop"
    << "wifi.ap.config"
    << "wifi.ap.getip"
    << "wifi.ap.setip"
    << "wifi.ap.getmac"
    << "wifi.ap.setmac"
    << "wifi.ap.getbroadcast"
    << "tmr.delay"
    << "tmr.now"
    << "tmr.alarm"
    << "tmr.stop"
    << "tmr.wdclr"
    << "tmr.time"
    << "gpio.mode"
    << "gpio.read"
    << "gpio.write"
    << "gpio.trig"
    << "pwm.setup"
    << "pwm.close"
    << "pwm.start"
    << "pwm.stop"
    << "pwm.setclock"
    << "pwm.getclock"
    << "pwm.setduty"
    << "pwm.getduty"
    << "net.createServer"
    << "net.createConnection"
    << "net.server:listen"
    << "net.server:close"
    << "net.socket:connect"
    << "net.socket:send"
    << "net.socket:on"
    << "net.socket:close"
    << "net.socket:dns"
    << "i2c.setup"
    << "i2c.start"
    << "i2c.stop"
    << "i2c.address"
    << "i2c.write"
    << "i2c.read"
    << "adc.read"
    << "adc.readvdd33"
    << "uart.setup"
    << "uart.on"
    << "uart.write"
    << "ow.setup"
    << "ow.reset"
    << "ow.skip"
    << "ow.select"
    << "ow.write"
    << "ow.write_bytes"
    << "ow.read"
    << "ow.read_bytes"
    << "ow.depower"
    << "ow.reset_search"
    << "ow.target_search"
    << "ow.search"
    << "ow.crc8"
    << "ow.check_crc16"
    << "ow.crc16"
    << "bit.bnot"
    << "bit.band"
    << "bit.bor"
    << "bit.bxor"
    << "bit.lshift"
    << "bit.rshift"
    << "bit.arshift"
    << "bit.bit"
    << "bit.set"
    << "bit.clear"
    << "bit.isset"
    << "bit.isclear"
    << "spi.setup"
    << "spi.send"
    << "spi.recv"
    << "mqtt.Client"
    << "mqtt.client:lwt"
    << "mqtt.client:connect"
    << "mqtt.client:close"
    << "mqtt.client:publish"
    << "mqtt.client:subscribe"
    << "mqtt.client:on"
    << "ws2812.writergb"
    << "cjson.encode"
    << "cjson.decode"
    << "crypto.hash"
    << "crypto.hmac"
    << "crypto.mask"
    << "crypto.toHex"
    << "crypto.toBase64"
    << "u8g.ssd1306_128x64_i2c"
    << "u8g.ssd1306_128x64_spi"
    << "u8g.pcd8544_84x48"
    << "u8g.disp:begin"
    << "u8g.disp:drawBitmap"
    << "u8g.disp:drawBox"
    << "u8g.disp:drawCircle"
    << "u8g.disp:drawDisc"
    << "u8g.disp:drawEllipse"
    << "u8g.disp:drawFilledEllipse"
    << "u8g.disp:drawFrame"
    << "u8g.disp:drawHLine"
    << "u8g.disp:drawLine"
    << "u8g.disp:drawPixel"
    << "u8g.disp:drawRBox"
    << "u8g.disp:drawRFrame"
    << "u8g.disp:drawStr"
    << "u8g.disp:drawStr90"
    << "u8g.disp:drawStr180"
    << "u8g.disp:drawStr270"
    << "u8g.disp:drawTriangle"
    << "u8g.disp:drawVLine"
    << "u8g.disp:drawXBM"
    << "u8g.disp:firstPage"
    << "u8g.disp:getColorIndex"
    << "u8g.disp:getFontAscent"
    << "u8g.disp:getFontDescent"
    << "u8g.disp:getFontLineSpacing"
    << "u8g.disp:getHeight"
    << "u8g.disp:getMode"
    << "u8g.disp:getWidth"
    << "u8g.disp:getStrWidth"
    << "u8g.disp:nextPage"
    << "u8g.disp:setColorIndex"
    << "u8g.disp:setDefaultBackgroundColor"
    << "u8g.disp:setDefaultForegroundColor"
    << "u8g.disp:setFont"
    << "u8g.disp:setFontLineSpacingFactor"
    << "u8g.disp:setFontPosBaseline"
    << "u8g.disp:setFontPosBottom"
    << "u8g.disp:setFontPosCenter"
    << "u8g.disp:setFontPosTop"
    << "u8g.disp:setFontRefHeightAll"
    << "u8g.disp:setFontRefHeightExtendedText"
    << "u8g.disp:setFontRefHeightText"
    << "u8g.disp:setRot90"
    << "u8g.disp:setRot180"
    << "u8g.disp:setRot270"
    << "u8g.disp:setScale2x2"
    << "u8g.disp:sleepOn"
    << "u8g.disp:sleepOff"
    << "u8g.disp:undoRotation"
    << "u8g.disp:undoScale"
    << "dht.read"
    << "dht.read11"
    << "dht.readxx";

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

void MainWindow::on_disconnectPushButton_clicked() {
  _serialPort->close();
  ui->serialTextEdit->setEnabled(false);
  ui->serialSendComboBox->setEnabled(false);
  ui->sendPushButton->setEnabled(false);
  ui->connectPushButton->setEnabled(true);
  ui->disconnectPushButton->setEnabled(false);

}

void MainWindow::on_serialSendComboBox_activated(QString command) {
    QByteArray data = command.toLatin1();
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
    ui->serialTextEdit->textCursor().insertText(QString::fromLatin1(data));
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

