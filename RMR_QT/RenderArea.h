#pragma once

#include <QWidget>

class RenderArea : public QWidget
{
	Q_OBJECT

public:
	RenderArea(QWidget *parent);
	~RenderArea();
	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

public slots:
	void dummy() {
		update();
	}

private:
	int x = 0;

protected:
	void paintEvent(QPaintEvent* event) override;
};
