#include "DataStruc.h"
#include <cmath>

using std::abs;

// 将该直线转化为显示用的直线
void Line::getQLineF(QLineF &qline)
{
	qline.setLine(a.x,a.y,b.x,b.y);
}

// 将其转化为显示用的polygon
void Area::getQPolygon(QPolygonF &qpolygon)
{
	qpolygon.clear();
	int size = points.size();
	for (int i = 0; i < size; i++)
	{
		QPointF temp;
		points[i].getQPointF(temp);
		qpolygon << temp;
	}
	QPointF temp;
	points[0].getQPointF(temp);
	qpolygon << temp;
}

void Display::clearAll()
{
	points.clear();
	pointColors.clear();
	lines.clear();
	lineColors.clear();
	areas.clear();
	areaColors.clear();
}

// 将其转化为显示用的polygon
void SimplePolygon::getQPolygon(QPolygonF &qpolygon)
{
	qpolygon.clear();
	int size = points.size();
	for (int i = 0; i < size; i++)
	{
		QPointF temp;
		points[i].getQPointF(temp);
		qpolygon << temp;
	}
	QPointF temp;
	points[0].getQPointF(temp);
	qpolygon << temp;
}

// 将凸包转化为显示用的polygon
void SimplePolygon::getConvexHullPolygon(QPolygonF &qpolygon)
{
	qpolygon.clear();
	int size = convexHull.size();
	for (int i = 0; i < size; i++)
	{
		QPointF temp;
		points[convexHull[i]].getQPointF(temp);
		qpolygon << temp;
	}
	QPointF temp;
	points[convexHull[0]].getQPointF(temp);
	qpolygon << temp;
}

// 判定范围如备注中图：
//  ==o----o----  
//      (b)   (a)
// 如图，在线段ab的b侧延长线上以及直线ab左侧的点判定为true，其他为false
// 注：不考虑三个点存在任意两点重合的情况，此种情况应该在之前的处理中已经排除
bool toLeft(const Point & a, const Point & b, const Point & c)
{
	double area2 = a.x*b.y - a.y*b.x + b.x*c.y - b.y*c.x + c.x*a.y - c.y*a.x;
	// 判断共线与容差作比较
	if (area2 > tolerance) // 在左侧
		return true;
	else if (area2 < -tolerance) // 在右侧
		return false;
	else // 共线情况
	{
		// 使用x坐标计算
		double ac = c.x - a.x;
		double bc = c.x - b.x;

		if (abs(ac) < tolerance) // 垂直共线的情况，使用y坐标
		{
			ac = c.y - a.y;
			bc = c.y - b.y;
		}
		if (ac * bc > 0 && abs(ac) > abs(bc)) // 判断符号正负时，不考虑容差因素
			return true;
		else
			return false;
	}
}

// 线段ab与线段cd是否有交点
bool intersect(const Point & a, const Point & b, const Point & c, const Point & d)
{
	return !((toLeft(a, b, c) && toLeft(a, b, d)) || (toLeft(b, a, c) && toLeft(b, a, d)) ||
	         (toLeft(c, d, a) && toLeft(c, d, b)) || (toLeft(d, c, a) && toLeft(d, c, b)));
}

// 返回值为true,点的序列为逆时针
// 返回值为false,点的序列为顺时针
bool getPolygonDirection(const Points& points)
{
	// 不考虑面积为0的情况，不考虑容差
	return getDirectedArea(points) > 0;
}

// 获得由一系列点围成的多边形有向面积，逆时针形成的有向面积为正，顺时针形成的为负
extern double getDirectedArea(const Points& points)
{
	int size = points.size();
	double x1, y1, x2, y2;
	double total = 0;
	int j;
	for (int i = 0; i < size; i++) {
		j = (i + 1) % size;
		x1 = points[i].x;
		y1 = points[i].y;
		x2 = points[j].x;
		y2 = points[j].y;
		total += (x1 - x2)*(y1 + y2);
	}
	return total;
}

// 将更改整个数组的顺序
void SimplePolygon::reverse()
{
	std::reverse(points.begin(), points.end());
}

// 清除已有的所有数据
void SimplePolygon::clearAll()
{
	points.clear();
	convexHull.clear();
}

// 判定当前的简单多边形是否合法，即是否存在自交、重合嵌套等
// 默认已经经过normalize()
// 使用暴力方法，对于大点集，后续可以使用扫描线算法进行改进
bool SimplePolygon::isLegal()
{
	int size = points.size();
	for (int i = 0; i < size; i++)
	{
		for (int j = 2; j < size - 1; j++)
		{
			if (intersect(points[i%size], points[(i + 1) % size], points[(i + j) % size], points[(i + j + 1) % size]))
				return false;
		}
	}
	return true;
}

// 用于排除输入中连续的重复点，并使得整个点集为逆时针方向
// 注： 目前版本并没有删除所有重合的点，仅仅删除了连续的重合的点。
void SimplePolygon::normalize()
{
	double x1, y1, x2, y2;
	int j;
	for (decltype(points.size()) i = 0; i < points.size();) {
		j = (i + 1) % points.size();
		x1 = points[i].x;
		y1 = points[i].y;
		x2 = points[j].x;
		y2 = points[j].y;
		if (abs(x1 - x2) < tolerance && abs(y1 - y2) < tolerance)
			points.erase(points.begin() + j);
		else
			i++;
	}
	
	if (!getPolygonDirection(points))
		reverse();
}

// 获得最左最下点的坐标
int SimplePolygon::getLeftMostThenLowestPoint()
{
	int min = 0, size = points.size();
	for (int i = 0; i < size; i++)
		if ((points[i].x < points[min].x) || (abs(points[i].x - points[min].x) < tolerance && points[i].y < points[min].y))
			min = i;
	return min;
}

// 获得最右最上点的坐标
int SimplePolygon::getRightMostThenHighestPoint()
{
	int max = 0, size = points.size();
	for (int i = 0; i < size; i++)
		if ((points[i].x > points[max].x) || (abs(points[i].x - points[max].x) < tolerance && points[i].y > points[max].y))
			max = i;
	return max;
}

// 从序列化字符串恢复数据
void SimplePolygon::setPolygon(const string& str)
{
	// 清空原本数据
	clearAll();

	vector<string> temp;
	SplitString(str, temp, ";");
	Point tp;
	int size = temp.size();
	for (int i = 0; i < size; i++)
	{
		tp.setPoint(temp[i]);
		points.push_back(tp);
	}
}

// 序列化
string SimplePolygon::toString()
{
	string temp = "";
	int size = points.size();
	for (int i = 0; i < size; i++)
	{
		temp += points[i].toString() + ";";
	}
	return temp;
}

// 从序列化字符串恢复数据
void Point::setPoint(const string& str)
{
	vector<string> temp;
	SplitString(str, temp, ",");
	if (temp.size() != 2) // 错误情况
		return;
	double td = stod(temp[0].substr(1, temp[0].size() - 1));
	x = td;
	td = stod(temp[1].substr(0, temp[0].size() - 1));
	y = td;
}

// 序列化
string Point::toString()
{
	return "(" + to_string(x) + "," + to_string(y) + ")";
}

void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

void generateSimplePolygon(SimplePolygon& sp, int num, int width, int height)
{
	sp.clearAll();

	Point2D		*gSimplePolygon;
	int			gNumSimplePolygon;

	Point2D *point = new Point2D[num];

	for (int i = 0; i < num; i++)
	{
		point[i].x = (rand() % width) - width / 2;
		point[i].y = (rand() % height) - height / 2;
	}

	GenerateSimplePolygon(point, num, gSimplePolygon, gNumSimplePolygon);

	for (int i = 0; i < gNumSimplePolygon; i++)
		sp.points.push_back(Point(gSimplePolygon[i].x, gSimplePolygon[i].y));

	delete[]point;
	delete[]gSimplePolygon;
}
