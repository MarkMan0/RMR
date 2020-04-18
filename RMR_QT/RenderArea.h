#pragma once

#include <QWidget>
#include "RobotManager.h"
#include <thread>
#include "MazeSolver.h"

class RenderArea : public QWidget
{
	Q_OBJECT

private:
	std::shared_ptr<RobotManager> robot;
	//QRect getRect(const LidarPoint& point);
	void resetFcn();
	void paintMap();
	void paintRaw();
	void drawPoint(QPainter& painter, const Point& p);
	void drawRobot();
	void paintSolution();
	void paintMaze();
	std::thread resetThread;
	std::atomic<bool> stopSignal = false;
	bool once = false;
	maze::MazeSolver solver;

public:
	void solve();
	RenderArea(QWidget* parent, const std::shared_ptr<RobotManager>& _robot);
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
