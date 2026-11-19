QT += core gui widgets
CONFIG += c++17
CONFIG -= app_bundle

TEMPLATE = app
TARGET = ShoppingList

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    shoppinglistmodel.cpp

HEADERS += \
    mainwindow.h \
    shoppinglistmodel.h
