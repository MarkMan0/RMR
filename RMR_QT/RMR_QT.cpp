#include "RMR_QT.h"
#include <string> 

RMR_QT::RMR_QT(QWidget* parent)
	: QWidget(parent), robot(std::make_shared<RobotManager>("127.0.0.1")),
	mc(robot),
	sim(std::string("../RMR_BASE/robotdata.txt"), std::string("../RMR_BASE/lidardata.txt"))
{
	ui.setupUi(this);
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
	// TODO: Add your control notification handler code here
	//Square
//	CString dest;
//	this->GetDlgItemTextW(IDC_EDIT7, dest);
//	int length = _ttoi(dest);
//	mc.moveForward(length);
//	mc.rotateTo(-90);
//	mc.moveForward(length);
//	mc.rotateTo(-180);
//	mc.moveForward(length);
//	mc.rotateTo(-270);
//	mc.moveForward(length);
//	mc.rotateTo(-360);
}

void RMR_QT::on_pushButtonPoint_clicked() {
	bool ok;
	double x = ui.textEditX->toPlainText().toDouble(&ok);
	if (!ok) return;
	double y = ui.textEditY->toPlainText().toDouble(&ok);
	if (!ok) return;

	mc.arcToXY(x, y);
}
