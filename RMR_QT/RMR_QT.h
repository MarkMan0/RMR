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
	void on_pushButton_clicked();
};
