#include "RenderArea.h"

#include <QPainter>

RenderArea::RenderArea(QWidget *parent)
	: QWidget(parent)
{
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
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

void RenderArea::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	QRect rect(0, 0, x, 60);
	++x;
	if (x > 300)
		x = 0;
	painter.drawRect(rect);

}
