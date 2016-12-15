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
#include <QDateTime>
#include <QBuffer>

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

	this->ui.pushButton->setDisabled(true);
	
	scene_.TakePicture();
	scene_.GetImage(picture_);

	this->ui.pushButton->setDisabled(false);
}

void MainWindow::SaveImage()
{
	if (picture_ != NULL && !picture_->isNull())
	{
		QDateTime time = QDateTime::currentDateTime();
		std::cout << time.toString().data() << std::endl;
		bool a = picture_->save("D:\\Desktop\\MainWindow\\RsultImage\\" + time.toString("dd.MM.yyyy") + ".png", "PNG", -1);
	}
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