#pragma once

#include "Algorithm.h"

// 该算法假设传入的SimplePolygon中的points序列的第一个点，为最左最下点
class GhoshShyamasundar83 : Algorithm
{
private:
	// 禁止外部初始化
	GhoshShyamasundar83() {}

public:
	// 禁止拷贝函数
	GhoshShyamasundar83(GhoshShyamasundar83 const&) = delete;
	// 禁止赋值拷贝
	GhoshShyamasundar83& operator=(GhoshShyamasundar83 const&) = delete;
	static GhoshShyamasundar83* Instance() {
		static GhoshShyamasundar83 instance;
		return &instance;
	}
	const static string counterExample;
	void getConvexHull(SimplePolygon & sp);
	void getConvexHullForDisplay(SimplePolygon & sp, Displays & displays);
};
