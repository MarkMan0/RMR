#pragma once

#include <QtWidgets/QWidget>
#include "ui_RMR_QT.h"

#include "RobotManager.h"
#include "MotionController.h"

class RMR_QT : public QWidget
{
	Q_OBJECT

public:
	RMR_QT(QWidget *parent = Q_NULLPTR);

private:
	Ui::RMR_QTClass ui;

	RobotManager robot;

private slots:
	void on_pushButtonStart_clicked();
	void on_pushButtonForward_clicked();
	void on_pushButtonBackward_clicked();
	void on_pushButtonLeft_clicked();
	void on_pushButtonRight_clicked();
	void on_pushButtonStop_clicked();
	void on_pushButtonSquare_clicked();
};
