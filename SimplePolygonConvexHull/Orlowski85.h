#pragma once

#include "Algorithm.h"

// 该算法假设传入的SimplePolygon中的points序列的第一个点，为最左最下点
class Orlowski85 : Algorithm
{
private:
	// 禁止外部初始化
	Orlowski85() {}
	// 禁止拷贝函数
	Orlowski85(Orlowski85 const&) {}
	// 禁止赋值拷贝
	Orlowski85& operator=(Orlowski85 const&) {}
	// 禁止析构函数
	~Orlowski85() {}
	static Orlowski85* instance;

public:
	static Orlowski85* Instance() {
		if (!instance)
			instance = new Orlowski85();
		return instance;

	}
	void getConvexHull(SimplePolygon & sp);
	void getConvexHullForDisplay(SimplePolygon & sp, Displays & displays);
};