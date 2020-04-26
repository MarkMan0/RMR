#pragma once

#include <QtWidgets/QWidget>
#include <memory>
#include "ui_RMR_QT.h"

#include "RobotManager.h"
#include "MotionController.h"

#include "RenderArea.h"

class RMR_QT : public QWidget
{
	Q_OBJECT

public:
	RMR_QT(QWidget *parent = Q_NULLPTR);
	~RMR_QT();

private:
	Ui::RMR_QTClass ui;
	std::unique_ptr<RenderArea> area;
	std::shared_ptr<RobotManager> robot;
	MC::MotionController mc;

	double getNumFromLineEdit(const QLineEdit*);
	void setErrTxt(const QString&);

private slots:
	void on_pushButtonStart_clicked();

	void on_pushButtonForward_clicked();
	void on_pushButtonBackward_clicked();
	void on_pushButtonLeft_clicked();
	void on_pushButtonRight_clicked();

	void on_pushButtonStop_clicked();
	void on_pushButtonPoint_clicked();
	void on_pushButtonTurn_clicked();
	void on_pushButtonDiscover_clicked();

	void on_radioButtonLinear_clicked();
	void on_radioButtonBug_clicked();
	void on_radioButtonMap_clicked();
};
