#include "RenderArea.h"

#include <QPainter>

RenderArea::RenderArea(QWidget* parent, const std::shared_ptr<RobotManager>& _robot, MC::MotionController& _mc)
	: QWidget(parent),
	  robot(_robot),
	  mc(_mc)
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

/*QRect RenderArea::getRect(const LidarPoint& point) {
	const int h = height(), w = width();
	constexpr double scale = 1.0/20.0;
	constexpr int sz = 2;
	QRect rect(static_cast<int>(scale*point.x + w/2), static_cast<int>(scale*point.y + h/2), sz, sz);

	return rect;
}*/

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

	const auto& lidarData = robot->getMap();
	const auto& map = lidarData.getMap();

	const double scale = 1.0 / 15.0;
	const int offset = 50;

	const int w = width(), h = height();

	for (const auto& kv : map) {
		const point_t& p = kv.first;
		if (kv.second > 10) {
			drawPoint(painter, p);
		}
	}
	auto pos = robot->getPosition();
	int x = scale * pos.x + offset;
	int y = h - (scale * pos.y) - offset;
	painter.drawEllipse(QPoint(x, y), 10, 10);


	QPen pen;
	QVector<qreal> pattern;
	pattern.push_back(3);
	pattern.push_back(3);
	pen.setDashPattern(pattern);
	pen.setColor(QColorConstants::Gray);
	pen.setWidth(2);

	painter.setPen(pen);
	painter.drawRect(offset, 0, w - offset, h - offset);
	
}

void RenderArea::paintRaw() {
	QPainter painter(this);
	const auto& map = robot->getMap();
	std::scoped_lock lck(map.mtx);

	const auto& raw = map.rawData;
	const auto pos = robot->getPosition();
	const int w = width(), h = height();
	const double scale = 1.0 / 20.0;
	for (const auto& data : raw) {
		double lidX = scale * data.dist * cos(deg2rad(90-data.angle));
		double lidY = scale * data.dist * sin(deg2rad(90-data.angle));

		int x = w / 2 + lidX;
		int y = h / 2 + lidY;

		painter.drawRect(x, y, 5, 5);
	}

}

void RenderArea::drawPoint(QPainter& painter, const point_t& p) {
	const double scale = 1.0 / 15.0;
	const int offset = 50;
	const int w = width(), h = height();

	int x = (scale * (p.x)) + offset;
	int y = h - (scale * (p.y)) - offset;

	if (x < 0 || x > w || y < 0 || y > h) return;

	painter.drawRect(x, y, 2, 2);
}

void RenderArea::drawRobot() {
	QPainter painter(this);

	int centX = width() / 2, centY = height() / 2;
	QPoint center(centX, centY);
	const int r = 15;
	painter.drawEllipse(center, r, r);
	const int w = r/2;
	painter.drawRect(centX - w / 2, centY + r - w / 2, w, w);

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
}

void RenderArea::solve() {
	solver.loadMaze(robot->getMap());

	point_t p;
	const auto pos = robot->getPosition();
	p.x = pos.x;
	p.y = pos.y;
	solver.setSource(p);

	p.x = 1000;
	p.y = 1000;
	solver.setTarger(p);
	solver.dijkstra();
}

void RenderArea::follow() {
	for (const auto& p : solver.getSolution()) {
		mc.arcToXY(p.x, p.y);
	}
}