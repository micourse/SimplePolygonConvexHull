#include "Sklansky72.h"

const string Sklansky72::counterExample = "(-269.000000,-101.000000);(-54.000000,-93.000000);(-181.000000,-29.000000);(20.000000,38.000000);(-88.000000,-29.000000);(159.000000,-104.000000);(-91.000000,188.000000);";

// 该算法假设传入的SimplePolygon中的points序列的第一个点，为最左最下点
void Sklansky72::getConvexHull(SimplePolygon & sp)
{
	int size = sp.points.size();
	int start = sp.getLeftMostThenLowestPoint();

	Points & points = sp.points;
	vector<int> & result = sp.convexHull;

	int next = 0, rsize;
	while (next < size)
	{
		rsize = result.size();
		if (rsize <= 1 || toLeft(points[result[rsize - 2]], points[result[rsize - 1]], points[(start + next) % size]))
		{
			result.push_back((next + start) % size);
			next++;
		}
		else
			result.pop_back();
	}
	rsize = result.size();
	while (rsize > 3 && !toLeft(points[result[rsize - 2]], points[result[rsize - 1]], points[start]))
	{
		result.pop_back();
		rsize = result.size();
	}
}

void Sklansky72::getConvexHullForDisplay(SimplePolygon & sp, Displays & displays)
{
	int size = sp.points.size();
	int start = sp.getLeftMostThenLowestPoint();
	Points & points = sp.points;
	vector<int> & result = sp.convexHull;

	int next = 0, rsize;
	while (next < size)
	{
		rsize = result.size();

		// 添加演示所需显示内容
		Display temp;
		for (int i = 0; i < rsize; i++)
		{
			temp.points.push_back(points[result[i]]);
			temp.pointColors.push_back(Qt::green);
		}
		temp.points.push_back(points[(next + start) % size]);
		temp.pointColors.push_back(Qt::red);
		displays.push_back(temp);

		if (rsize <= 1)
		{
			result.push_back((next + start) % size);
			next++;

			// 添加演示所需内容
			temp.pointColors[temp.pointColors.size() - 1] = Qt::green;
			displays.push_back(temp);
		}
		else if (toLeft(points[result[rsize - 2]], points[result[rsize - 1]], points[(start + next) % size]))
		{
			// 添加演示所需内容
			Line line(points[result[rsize - 2]], points[result[rsize - 1]], LINE);
			temp.lines.push_back(line);
			temp.lineColors.push_back(Qt::blue);
			temp.pointColors[temp.pointColors.size() - 3] = Qt::yellow;
			temp.pointColors[temp.pointColors.size() - 2] = Qt::yellow;
			displays.push_back(temp);

			result.push_back((next + start) % size);
			next++;

			// 添加演示所需内容
			temp.lineColors.pop_back();
			temp.lines.pop_back();
			temp.pointColors[temp.pointColors.size() - 3] = Qt::green;
			temp.pointColors[temp.pointColors.size() - 2] = Qt::green;
			temp.pointColors[temp.pointColors.size() - 1] = Qt::green;
			displays.push_back(temp);
		}
		else
		{
			// 添加演示所需内容
			Line line(points[result[rsize - 2]], points[result[rsize - 1]], LINE);
			temp.lines.push_back(line);
			temp.lineColors.push_back(Qt::blue);
			temp.pointColors[temp.pointColors.size() - 3] = Qt::yellow;
			temp.pointColors[temp.pointColors.size() - 2] = Qt::yellow;
			displays.push_back(temp);

			result.pop_back();

			// 添加演示所需内容
			temp.lineColors.pop_back();
			temp.lines.pop_back();
			temp.pointColors[temp.pointColors.size() - 2] = Qt::gray;
			displays.push_back(temp);
		}
	}

	rsize = result.size();

	while (rsize > 3 && !toLeft(points[result[rsize - 2]], points[result[rsize - 1]], points[start]))
	{
		// 添加演示所需显示内容
		Display temp;
		for (int i = 1; i < rsize; i++)
		{
			temp.points.push_back(points[result[i]]);
			temp.pointColors.push_back(Qt::green);
		}
		temp.points.push_back(points[start]);
		temp.pointColors.push_back(Qt::red);
		displays.push_back(temp);

		// 添加演示所需显示内容
		Line line(points[result[rsize - 2]], points[result[rsize - 1]], LINE);
		temp.lines.push_back(line);
		temp.lineColors.push_back(Qt::blue);
		temp.pointColors[temp.pointColors.size() - 3] = Qt::yellow;
		temp.pointColors[temp.pointColors.size() - 2] = Qt::yellow;
		displays.push_back(temp);

		result.pop_back();
		rsize = result.size();

		// 添加演示所需显示内容
		temp.lineColors.pop_back();
		temp.lines.pop_back();
		temp.pointColors[temp.pointColors.size() - 2] = Qt::gray;
		displays.push_back(temp);
	}

	// 添加演示所需显示内容
	Display temp;
	for (int i = 1; i < rsize; i++)
	{
		temp.points.push_back(points[result[i]]);
		temp.pointColors.push_back(Qt::green);
	}
	temp.points.push_back(points[start]);
	temp.pointColors.push_back(Qt::red);
	displays.push_back(temp);

	// 添加演示所需显示内容
	Line line(points[result[rsize - 2]], points[result[rsize - 1]], LINE);
	temp.lines.push_back(line);
	temp.lineColors.push_back(Qt::blue);
	temp.pointColors[temp.pointColors.size() - 3] = Qt::yellow;
	temp.pointColors[temp.pointColors.size() - 2] = Qt::yellow;
	displays.push_back(temp);

	// 添加演示所需显示内容
	temp.lineColors.pop_back();
	temp.lines.pop_back();
	temp.pointColors[temp.pointColors.size() - 3] = Qt::green;
	temp.pointColors[temp.pointColors.size() - 2] = Qt::green;
	temp.pointColors[temp.pointColors.size() - 1] = Qt::green;
	displays.push_back(temp);
}
