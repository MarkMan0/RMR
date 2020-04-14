#pragma once

#include <QWidget>
#include "RobotManager.h"
#include <thread>

class RenderArea : public QWidget
{
	Q_OBJECT

private:
	std::shared_ptr<RobotManager> robot;
	//QRect getRect(const LidarPoint& point);
	void resetFcn();
	void paintMap();
	void paintRaw();
	void drawRobot();
	std::thread resetThread;

public:
	RenderArea(QWidget* parent, const std::shared_ptr<RobotManager>& _robot);
	~RenderArea();
	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

	

public slots:
	void dummy() {
		update();
	}




protected:
	void paintEvent(QPaintEvent* event) override;
};
