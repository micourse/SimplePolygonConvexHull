#pragma once

#include "Algorithm.h"

// 该算法假设传入的SimplePolygon中的points序列的第一个点，为最左最下点
class PreparataShamos85 : Algorithm
{
private:
	// 禁止外部初始化
	PreparataShamos85() {}

public:
	// 禁止拷贝函数
	PreparataShamos85(PreparataShamos85 const&) = delete;
	// 禁止赋值拷贝
	PreparataShamos85& operator=(PreparataShamos85 const&) = delete;
	static PreparataShamos85* Instance() {
		static PreparataShamos85 instance;
		return &instance;

	}
	void getConvexHull(SimplePolygon & sp);
	void getConvexHullForDisplay(SimplePolygon & sp, Displays & displays);
private:
	Display createDisplay(SimplePolygon& sp, vector<int>& hull, Point& sopport, int leftIndex, int rightIndex, int curIndex);
};
