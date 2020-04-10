#pragma once

#include <QtWidgets/QWidget>
#include <memory>
#include "ui_RMR_QT.h"

#include "RobotManager.h"
#include "MotionController.h"
#include "Simulator.h"

class RMR_QT : public QWidget
{
	Q_OBJECT

public:
	RMR_QT(QWidget *parent = Q_NULLPTR);

private:
	Ui::RMR_QTClass ui;

	std::shared_ptr<RobotManager> robot;
	MC::MotionController mc;
	Simulator sim;

private slots:
	void on_pushButtonStart_clicked();
	void on_pushButtonForward_clicked();
	void on_pushButtonBackward_clicked();
	void on_pushButtonLeft_clicked();
	void on_pushButtonRight_clicked();
	void on_pushButtonStop_clicked();
	void on_pushButtonSquare_clicked();
	void on_pushButtonPoint_clicked();
};
