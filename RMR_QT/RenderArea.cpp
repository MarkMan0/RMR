#include "RenderArea.h"

#include <QPainter>

RenderArea::RenderArea(QWidget* parent, const std::shared_ptr<RobotManager>& _robot)
	: QWidget(parent),
	  robot(_robot)
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
	if (!paintMapNow)	return;
	paintMapNow = false;

	QPainter painter(this);

	const auto& lidarData = robot->getMap();
	const auto& map = lidarData.getMap();

	const double scale = 1.0 / 15.0;
	const int offset = 50;

	const int w = width(), h = height();

	for (const auto& kv : map) {
		const Point& p = kv.first;
		if (kv.second > 10) {
			int x = (scale * (p.x)) + offset;
			int y = h - (scale * (p.y)) - offset;
			painter.drawRect(x, y, 2, 2);
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

void RenderArea::drawRobot() {
	QPainter painter(this);

	int centX = width() / 2, centY = height() / 2;
	QPoint center(centX, centY);
	const int r = 15;
	painter.drawEllipse(center, r, r);
	const int w = r/2;
	painter.drawRect(centX - w / 2, centY + r - w / 2, w, w);

}

void RenderArea::paintEvent(QPaintEvent* event) {
	if (paintMapNow) {
		//drawRobot();
		paintMap();
	}
	
}
