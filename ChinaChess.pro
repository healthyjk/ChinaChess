#-------------------------------------------------
#
# Project created by QtCreator 2013-05-15T11:26:16
#
#-------------------------------------------------

QT       += core gui
QT       += phonon
TARGET = ChinaChess
TEMPLATE = app
CONFIG += release

SOURCES += main.cpp\
        chessmainwindow.cpp \
    chessgame.cpp \
    chessmovegenerate.cpp \
    chessman.cpp \
    searchengine.cpp \
    chessevaluation.cpp \
    chessdefine.cpp \
    negascout_tt_hh_engine.cpp \
    transpositiontable.cpp \
    historyheuristic.cpp \
    stopwatch.cpp \
    mysplashscreen.cpp \
    configdialog.cpp

HEADERS  += chessmainwindow.h \
    chessgame.h \
    chessdefine.h \
    chessmovegenerator.h \
    chessman.h \
    searchengine.h \
    chessevaluation.h \
    negascout_tt_hh_engine.h \
    transpositiontable.h \
    historyheuristic.h \
    stopwatch.h \
    mysplashscreen.h \
    configdialog.h

CONFIG+=thread


