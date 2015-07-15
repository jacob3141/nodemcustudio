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

#pragma once

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerlua.h>
#include <QMainWindow>

#include <QtExtSerialPort/qextserialport.h>

#include <QCompleter>

namespace Ui {
class MainWindow;
}

class MainWindow :
    public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_connectPushButton_clicked();
    void on_serialSendComboBox_activated(QString command);
    void on_sendPushButton_clicked();

    void serialPortSurvey();
    void readSerial();

private:
    void addEditorWindow();

    Ui::MainWindow *ui;

    QsciScintilla *_editor;
    QsciLexerLua *_luaLexer;

    QextSerialPort *_serialPort;
    QCompleter *_serialCommandCompleter;
};
