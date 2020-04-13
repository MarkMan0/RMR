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

RenderArea::~RenderArea()
{
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
	constexpr int ms = 500;
	while (1) {
		update();
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
}

void RenderArea::paintMap() {
	QPainter painter(this);

	const auto& map = robot->getMap();

	int minVal = map.minVal;
	int maxVal = map.maxVal;
	int spacing = map.spacing;
	int w = width(), h = height();

	for (int x = minVal; x < maxVal; ++x) {
		for (int y = minVal; y < maxVal; ++y) {
			bool wall = map.checkPoint(x, y);
			double xscale = w / (1.0 * maxVal - minVal),
				yscale = h / (1.0 * maxVal - minVal);
			if (wall) {
				int xCoord = (x + minVal) * xscale;
				int yCoord = (y + minVal) * yscale;
				painter.drawRect(xCoord, yCoord, 5, 5);
			}
		}

	}
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

		int x = w - w / 2 - lidX;
		int y = h - h / 2 - lidY;

		painter.drawRect(x, y, 5, 5);
	}

}

void RenderArea::paintEvent(QPaintEvent* event) {
	paintRaw();

	
}
