/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QWidget *centralWidget;
    QLineEdit *lineEdit;
    QLabel *label;
    QWidget *Imagewidget;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_7;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(971, 639);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(10, 50, 101, 20));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 30, 91, 16));
        Imagewidget = new QWidget(centralWidget);
        Imagewidget->setObjectName(QStringLiteral("Imagewidget"));
        Imagewidget->setGeometry(QRect(220, 20, 551, 381));
        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(10, 120, 31, 21));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 100, 91, 16));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 180, 91, 16));
        lineEdit_4 = new QLineEdit(centralWidget);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(50, 120, 31, 21));
        lineEdit_5 = new QLineEdit(centralWidget);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(90, 120, 31, 21));
        lineEdit_3 = new QLineEdit(centralWidget);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(10, 200, 31, 21));
        lineEdit_6 = new QLineEdit(centralWidget);
        lineEdit_6->setObjectName(QStringLiteral("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(50, 200, 31, 21));
        lineEdit_7 = new QLineEdit(centralWidget);
        lineEdit_7->setObjectName(QStringLiteral("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(90, 200, 31, 21));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(20, 250, 75, 23));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 971, 23));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        retranslateUi(MainWindowClass);
        QObject::connect(lineEdit, SIGNAL(textChanged(QString)), MainWindowClass, SLOT(SampleNum(QString)));
        QObject::connect(lineEdit_2, SIGNAL(textChanged(QString)), MainWindowClass, SLOT(CameraPositionX(QString)));
        QObject::connect(lineEdit_3, SIGNAL(textChanged(QString)), MainWindowClass, SLOT(CameraLookAtX(QString)));
        QObject::connect(lineEdit_4, SIGNAL(textChanged(QString)), MainWindowClass, SLOT(CameraPositionY(QString)));
        QObject::connect(lineEdit_5, SIGNAL(textChanged(QString)), MainWindowClass, SLOT(CameraPositionZ(QString)));
        QObject::connect(lineEdit_6, SIGNAL(textChanged(QString)), MainWindowClass, SLOT(CameraLookAtY(QString)));
        QObject::connect(lineEdit_7, SIGNAL(textChanged(QString)), MainWindowClass, SLOT(CameraLookAtZ(QString)));
        QObject::connect(pushButton, SIGNAL(clicked()), MainWindowClass, SLOT(GetImage()));

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", 0));
        lineEdit->setText(QApplication::translate("MainWindowClass", "5", 0));
        label->setText(QApplication::translate("MainWindowClass", "Anti-aliasing:", 0));
        lineEdit_2->setText(QApplication::translate("MainWindowClass", "0.0", 0));
        label_3->setText(QApplication::translate("MainWindowClass", "Camera Position", 0));
        label_4->setText(QApplication::translate("MainWindowClass", "Camera LookAt", 0));
        lineEdit_4->setText(QApplication::translate("MainWindowClass", "0.0", 0));
        lineEdit_5->setText(QApplication::translate("MainWindowClass", "-2.0", 0));
        lineEdit_3->setText(QApplication::translate("MainWindowClass", "0.0", 0));
        lineEdit_6->setText(QApplication::translate("MainWindowClass", "0.0", 0));
        lineEdit_7->setText(QApplication::translate("MainWindowClass", "1.0", 0));
        pushButton->setText(QApplication::translate("MainWindowClass", "Run", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
