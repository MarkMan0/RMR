#include "RMR_QT.h"

RMR_QT::RMR_QT(QWidget *parent)
	: QWidget(parent), robot("asd")
{
	ui.setupUi(this);
	this->setWindowTitle("aasfs");
}


void RMR_QT::on_pushButton_clicked() {
	robot.init();
}