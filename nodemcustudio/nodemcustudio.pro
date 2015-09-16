#///////////////////////////////////////////////////////////////////////////////
#//                                                                           //
#//    This file is part of NodeMCU Studio.                                   //
#//    Copyright (C) 2015 Jacob Dawid, jacob@omg-it.works                     //
#//                                                                           //
#//    NodeMCU Studio is free software: you can redistribute it and/or modify //
#//    it under the terms of the GNU General Public License as published by   //
#//    the Free Software Foundation, either version 3 of the License, or      //
#//    (at your option) any later version.                                    //
#//                                                                           //
#//    NodeMCU Studio is distributed in the hope that it will be useful,      //
#//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
#//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
#//    GNU General Public License for more details.                           //
#//                                                                           //
#//    You should have received a copy of the GNU General Public License      //
#//    along with NodeMCU Studio. If not, see <http://www.gnu.org/licenses/>. //
#//                                                                           //
#///////////////////////////////////////////////////////////////////////////////

QT  += core gui webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = nodemcustudio
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

include(../pods.pri)

RESOURCES += \
    resources.qrc

LIBS += -lqscintilla2 -lqextserialport

TRANSLATIONS = translations/Russian.ts\
    translations/English.ts\
    translations/French.ts\
    translations/Spanish.ts\
    translations/German.ts\
    translations/Dutch.ts\
    translations/Chinese.ts
