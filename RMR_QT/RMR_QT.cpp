#include "RMR_QT.h"
#include <string> 


RMR_QT::RMR_QT(QWidget* parent)
	: QWidget(parent), robot(std::make_shared<RobotManager>("127.0.0.1")),
	mc(robot)
{
	ui.setupUi(this);
	area = std::make_unique<RenderArea>(this, robot);

	ui.renderAreaCont->addWidget(area.get());
	
	this->setWindowTitle("aasfs");
}


void RMR_QT::on_pushButtonStart_clicked() {
	try {
		robot->init();
		mc.init();
	}
	catch (const std::runtime_error& e) {
		ui.label_errors->setText(e.what());
	}
}

RMR_QT::~RMR_QT() {
}

double RMR_QT::getNumFromLineEdit(const QLineEdit* le)
{
	bool ok;
	double x = le->displayText().toDouble(&ok);
	if (ok) {
		return x;
	}
	else {
		throw std::runtime_error("Not a number");
	}
}

void RMR_QT::setErrTxt(const QString& err) {
	ui.label_errors->setText(err);
}

void RMR_QT::on_pushButtonForward_clicked() {
	try {
		double d = getNumFromLineEdit(ui.lineEdit_Distance);
		mc.moveForward(d);
	}
	catch (const std::runtime_error&) {
		setErrTxt("Distance not a number");
	}
}

void RMR_QT::on_pushButtonBackward_clicked() {
	try {
		double d = getNumFromLineEdit(ui.lineEdit_Distance);
		mc.moveForward(-d);
	}
	catch (const std::runtime_error&) {
		setErrTxt("Distance not a number");
	}
}

void RMR_QT::on_pushButtonLeft_clicked() {
	try {
		double d = getNumFromLineEdit(ui.lineEdit_AngleRel);
		mc.rotateTo(robot->getPosition().theta + d);
	}
	catch (const std::runtime_error&) {
		setErrTxt("Angle not a number");
	}
}

void RMR_QT::on_pushButtonRight_clicked() {
	try {
		double d = getNumFromLineEdit(ui.lineEdit_AngleRel);
		mc.rotateTo(robot->getPosition().theta - d);
	}
	catch (const std::runtime_error&) {
		setErrTxt("Angle not a number");
	}
}

void RMR_QT::on_pushButtonStop_clicked() {
	//Stop;
	//TODO: stop via mc
	robot->stop();
}

void RMR_QT::on_pushButtonPoint_clicked() {

	try {
		double x = getNumFromLineEdit(ui.lineEdit_X);
		double y = getNumFromLineEdit(ui.lineEdit_Y);
		mc.arcToXY(x, y);
	}
	catch (const std::runtime_error&) {
		setErrTxt("X/Y not a number");
	}

}

void RMR_QT::on_pushButtonTurn_clicked() {
	try {
		double angle = getNumFromLineEdit(ui.lineEdit_Angle);
		mc.rotateTo(angle);
	}
	catch (const std::runtime_error&) {
		setErrTxt("Angle not a number");
	}
}


void RMR_QT::on_pushButtonEraseMap_clicked() {
	robot->getMap().erase();
}

void RMR_QT::on_pushButtonSolve_clicked() {
	area->solve();
}
