#include "RMR_QT.h"
#include <string> 

RMR_QT::RMR_QT(QWidget* parent)
	: QWidget(parent), robot(std::make_shared<RobotManager>("127.0.0.1")),
	mc(robot)
{
	ui.setupUi(this);
	area = new RenderArea(this, robot);

	ui.renderAreaCont->addWidget(area);
	
	this->setWindowTitle("aasfs");
}


void RMR_QT::on_pushButtonStart_clicked() {
	robot->init();
	mc.init();
}

void RMR_QT::on_pushButtonForward_clicked() {
	//pohyb dopredu
	mc.moveForward(100);
}

void RMR_QT::on_pushButtonBackward_clicked() {
	//pohyb dopredu
	mc.moveForward(-100);
}

void RMR_QT::on_pushButtonLeft_clicked() {
	//Left
	mc.rotateTo(90);
}

void RMR_QT::on_pushButtonRight_clicked() {
	//Right
	mc.rotateTo(-90);
}

void RMR_QT::on_pushButtonStop_clicked() {
	//Stop;
	//TODO: stop via mc
	robot->stop();
}

void RMR_QT::on_pushButtonSquare_clicked()
{
	mc.arcToXY(1000, 0);
	mc.arcToXY(1000, 1000);
	mc.arcToXY(0, 1000);
	mc.arcToXY(0, 0);
}

void RMR_QT::on_pushButtonPoint_clicked() {
	bool ok;
	double x = ui.textEditX->toPlainText().toDouble(&ok);
	if (!ok) return;
	double y = ui.textEditY->toPlainText().toDouble(&ok);
	if (!ok) return;

	mc.arcToXY(x, y);
}
