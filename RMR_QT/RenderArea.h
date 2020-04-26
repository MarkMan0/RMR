#pragma once

#include <QWidget>
#include "RobotManager.h"
#include <thread>
#include "MazeSolver.h"
#include "MotionController.h"

class RenderArea : public QWidget
{
	Q_OBJECT
public:
	using point_t = lidar::point_t;
private:
	std::shared_ptr<RobotManager> robot;
	//QRect getRect(const LidarPoint& point);
	void resetFcn();
	void paintMap();
	void paintRaw();
	void drawPoint(QPainter& painter, const point_t& p);
	void drawRobot();
	void paintSolution();
	void paintMaze();
	std::thread resetThread;
	std::atomic<bool> stopSignal = false;
	bool once = false;
	maze::MazeSolver solver;
	MC::MotionController &mc;
public:
	void solve();
	void follow();
	RenderArea(QWidget* parent, const std::shared_ptr<RobotManager>& _robot, MC::MotionController& _mc);
	~RenderArea();
	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

	bool paintMapNow = false;
	

public slots:
	void dummy() {
		update();
	}




protected:
	void paintEvent(QPaintEvent* event) override;
};
