#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpinBox>
#include "ui_mainwindow.h"
#include "Scene.h"
class QLabel;
class QPushButton;
class QCheckBox;
class QGroupBox;
class RenderingWidget;
class QSpinbox;

class MainWindow : public QMainWindow
{
	Q_OBJECT


public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();


private:
	Ui::MainWindowClass ui;
	
	//void DrawScene();
	void CreateScene();
protected:
	void paintEvent(QPaintEvent *paintevent);

	// Basic
	QWidget							ImageWidget_;
	QMenu							*menu_file_;
	QToolBar						*toolbar_file_;
	QAction							*action_saveas_;
	QCheckBox						*checkbox_axes_;
	
	QGroupBox						*groupbox_render_;
	// Tool bar
	QSpinBox						*SpinBox_Sample;
	// Information
	QLabel							*label_operatorinfo_;

	RenderingWidget					*renderingwidget_;
	Scene							scene_;
	QImage							*picture_ = NULL;
private slots:
	void							CameraPositionX(QString str);
	void							CameraPositionY(QString str);
	void							CameraPositionZ(QString str);
	void							CameraLookAtX(QString str);
	void							CameraLookAtY(QString str);
	void							CameraLookAtZ(QString str);
	void							SampleNum(QString str);
	void							GetImage();
};

#endif // MAINWINDOW_H
