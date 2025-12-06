QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Remove Node.cpp and priorityQ.cpp from SOURCES!
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ..\driver.cpp

# Add all header files
HEADERS += \
    mainwindow.h \
    ..\driver.h \
    ..\Node.h \
    ..\priorityQ.h

FORMS += \
    mainwindow.ui

# Default rules for deployment
qnc.target = .PHONY
QMAKE_EXTRA_TARGETS += qnc
