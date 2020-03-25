#include "RMR_QT.h"
#include <string> 

RMR_QT::RMR_QT(QWidget* parent)
	: QWidget(parent), robot("asd"),
	sim(std::string("../RMR_BASE/robotdata.txt"), std::string("../RMR_BASE/lidardata.txt"))
{
	ui.setupUi(this);
	this->setWindowTitle("aasfs");
}


void RMR_QT::on_pushButtonStart_clicked() {
	robot.init();
}

void RMR_QT::on_pushButtonForward_clicked() {
	//pohyb dopredu
	robot.translation(200);
}

void RMR_QT::on_pushButtonBackward_clicked() {
	//pohyb dopredu
	robot.translation(-200);
}

void RMR_QT::on_pushButtonLeft_clicked() {
	//Left
	robot.rotation(-1);
}

void RMR_QT::on_pushButtonRight_clicked() {
	//Right
	robot.rotation(1);
}

void RMR_QT::on_pushButtonStop_clicked() {
	//Stop
	robot.rotation(0);
	robot.translation(0);
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