#include "GraphicsScene.h"

// 开始插入点，更改状态为正在插入点。
// 因为开始插入将消除过去的内容，因此所有数据会消除。
// 会调用clear()，并清除displays和sp中所有数据
void GraphicsScene::beginInsert()
{
	if (!isInsertingPoint)
	{
		isInsertingPoint = true;
		sp.clearAll();
		displays.clear();
		clear();
	}
}

// 中止插入点，更改状态位已经结束插入
// 只会消除用于显示的数据，不会影响sp中的数据
// 会调用clear()
void GraphicsScene::endInsert()
{
	if (isInsertingPoint)
	{
		isInsertingPoint = false;
		clear();
	}
}

// 用于获取当前插入点是否结束
bool GraphicsScene::isEndInserting()
{
	return !isInsertingPoint;
}

// 用于显示displays
void GraphicsScene::display()
{
	clear();
	displayPolygon();
	displayConvexHull();
}

// 鼠标按键事件处理
void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsScene::mousePressEvent(event);
	if (!event->isAccepted())
	{
		if (event->button() == Qt::LeftButton) {
			if (isInsertingPoint)
			{
				QPointF point = event->scenePos();
				sp.points.push_back(Point(point));

				QGraphicsRectItem *item = new QGraphicsRectItem(point.x() - 2, point.y() - 2, 4, 4);
				item->setBrush(QColor(0, 0, 0));
				addItem(item);

				if (sp.points.size() > 1)
				{
					int size = sp.points.size();
					QGraphicsLineItem *item = new QGraphicsLineItem(sp.points[size-2].x, sp.points[size-2].y, sp.points[size-1].x, sp.points[size-1].y);

					QPen pen;
					pen.setColor(QColor(0, 0, 0));
					pen.setWidth(2);
					item->setPen(pen);

					addItem(item);
				}
			}
		}
		else if (event->button() == Qt::RightButton)
		{
			if (isInsertingPoint)
			{
				endInsert();
				displayPolygon();
			}
		}
	}
}

// 显示多边形上所有的点与线
// 不调用clear()方法
void GraphicsScene::displayPolygon()
{
	for each (Point p in sp.points)
	{
		QGraphicsRectItem *item = new QGraphicsRectItem(p.x - 2, p.y - 2, 4, 4);
		item->setBrush(Qt::black);
		addItem(item);
	}
	
	if (sp.points.size() >= 3)
	{
		QPolygonF polygon;
		sp.getQPolygon(polygon);
		QGraphicsPolygonItem *item = new QGraphicsPolygonItem(polygon);

		QPen pen;
		pen.setColor(Qt::black);
		pen.setWidth(2);
		item->setPen(pen);

		addItem(item);
	}
	else if (sp.points.size() == 2)
	{
		QGraphicsLineItem *item = new QGraphicsLineItem(sp.points[0].x, sp.points[0].y, sp.points[1].x, sp.points[1].y);

		QPen pen;
		pen.setColor(Qt::black);
		pen.setWidth(2);
		item->setPen(pen);

		addItem(item);
	}
}

// 显示凸包上的点与线
// 不调用clear()方法
void GraphicsScene::displayConvexHull()
{
	if (sp.convexHull.size() <= 3)
		return;
	else
	{
		QPolygonF polygon;
		sp.getConvexHullPolygon(polygon);
		QGraphicsPolygonItem *item = new QGraphicsPolygonItem(polygon);

		QPen pen;
		pen.setColor(QColor(Qt::green));
		pen.setWidth(2);
		item->setPen(pen);

		addItem(item);

		for each (int id in sp.convexHull)
		{
			Point p = sp.points[id];
			QGraphicsRectItem *item = new QGraphicsRectItem(p.x - 2, p.y - 2, 4, 4);
			item->setBrush(QColor(Qt::green));
			addItem(item);
		}
	}
}
