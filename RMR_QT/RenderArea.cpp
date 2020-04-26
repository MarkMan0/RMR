#include "RenderArea.h"

#include <QPainter>

RenderArea::PointInt RenderArea::applyOffsets(const Point& p) const {
	const int w = width(), h = height();
	PointInt ret;
	ret.x = (scale * (p.x)) + offset;
	ret.y = h - (scale * (p.y)) - offset;

	return ret;
}

RenderArea::RenderArea(QWidget* parent, const std::shared_ptr<RobotManager>& _robot, const maze::MazeSolver& _solver)
	: QWidget(parent),
	  robot(_robot),
	  solver(_solver)
{
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
	resetThread = std::thread(&RenderArea::resetFcn, this);
}

RenderArea::~RenderArea() {
	stopSignal = true;
	resetThread.join();
}

QSize RenderArea::sizeHint() const
{
	return QSize(400, 200);
}


QSize RenderArea::minimumSizeHint() const
{
	return QSize(100, 100);
}


void RenderArea::resetFcn() {
	constexpr int ms = 2000;
	while (!stopSignal) {
		update();
		paintMapNow = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
}

void RenderArea::paintMap() {
	
	QPainter painter(this);
	std::scoped_lock lck(robot->mapMtx);
	const auto& map = robot->getMap().getPoints();
	for (const auto& kv : map) {
		const point_t& p = kv.first;
		if (kv.second > 10) {
			drawPoint(painter, p);
		}
	}

	QPen pen;
	QVector<qreal> pattern;
	pattern.push_back(3);
	pattern.push_back(3);
	pen.setDashPattern(pattern);
	pen.setColor(QColorConstants::Gray);
	pen.setWidth(2);

	painter.setPen(pen);
	painter.drawRect(offset, 0, width() - offset, height() - offset);
	
}

void RenderArea::drawPoint(QPainter& painter, const point_t& p) {

	auto p2 = applyOffsets(p);
	if (p2.x < 0 || p2.x > width() || p2.y < 0 || p2.y > height()) return;
	painter.drawRect(p2.x, p2.y, 2, 2);
}

void RenderArea::paintRobot() {
	QPainter painter(this);
	auto pos = robot->getPosition();
	Point p = applyOffsets(pos.p);
	painter.drawEllipse(QPoint(p.x, p.y), 10, 10);
}

void RenderArea::paintSolution() {
	QPainter painter(this);
	QPen pen;


	pen.setColor(QColorConstants::DarkBlue);
	painter.setPen(pen);
	const auto solFull = solver.solFull;
	for (const point_t& p : solFull) {
		drawPoint(painter, p);
	}


	pen.setColor(QColorConstants::Red);
	painter.setPen(pen);
	const auto& solSimp = solver.getSolution();
	for (const point_t& p : solSimp) {
		drawPoint(painter, p);
	}

	

}

void RenderArea::paintMaze() {
	if (paintMapNow) {
		QPainter painter(this);
		QPen pen;
		pen.setColor(QColor(106, 190, 69, 50));
		painter.setPen(pen);

		const auto& nodes = solver.getNodes();

		for (const auto& row : nodes) {
			for (const auto& node : row) {
				if (node->blocked) {
					drawPoint(painter, node->p);
				}
			}
		}
	}
}

void RenderArea::paintEvent(QPaintEvent* event) {
	paintMap();
	paintSolution();
	paintMaze();
	paintRobot();
}
