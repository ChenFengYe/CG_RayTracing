#include "mainwindow.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QMessageBox>
#include <QKeyEvent>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//setGeometry(300, 150, 800, 600);
	CreateScene();
	//DrawScene();
}

MainWindow::~MainWindow()
{
	if (picture_ != NULL)
	{
		delete picture_;
	}
}


void MainWindow::CameraPositionX(QString str)
{
	scene_.camera_position.x() = str.toDouble();
}

void MainWindow::CameraPositionY(QString str)
{
	scene_.camera_position.y() = str.toDouble();
}

void MainWindow::CameraPositionZ(QString str)
{
	scene_.camera_position.z() = str.toDouble();
}

void MainWindow::CameraLookAtX(QString str)
{
	scene_.camera_LookAt.x() = str.toDouble();
}

void MainWindow::CameraLookAtY(QString str)
{
	scene_.camera_LookAt.y() = str.toDouble();
}

void MainWindow::CameraLookAtZ(QString str)
{
	scene_.camera_LookAt.z() = str.toDouble();
}

void MainWindow::SampleNum(QString str)
{
	scene_.sample = str.toDouble();
}

void MainWindow::GetImage()
{
	if (picture_!=NULL)
	{
		delete picture_;
	}
	scene_.TakePicture();
	scene_.GetImage(picture_);
}


void MainWindow::paintEvent(QPaintEvent *paintevent)
{
	QPainter painter(this);
	if (picture_ != NULL)
	{
		painter.drawImage(QPoint(160,20), *picture_);
	}
	update();
}

void MainWindow::CreateScene()
{
	scene_.BuildScene();
}