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
	using PointInt = BasePoint<int>;
private:
	const double scale = 1.0 / 15.0;
	const int offset = 50;
	std::shared_ptr<RobotManager> robot;
	void resetFcn();
	void paintMap();
	void drawPoint(QPainter& painter, const point_t& p);
	void paintRobot();
	void paintSolution();
	void paintMaze();
	std::thread resetThread;
	std::atomic<bool> stopSignal = false;
	bool once = false;
	const maze::MazeSolver& solver;

	PointInt applyOffsets(const Point& p) const;

public:
	RenderArea(QWidget* parent, const std::shared_ptr<RobotManager>& _robot, const maze::MazeSolver& _solver);
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
