#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include <string>
#include <cstring>
#include <list>
#include <iostream>
#include <fstream>
#include <regex>
#include <variant>
#include <random>
#include <optional>

#include "clktree_pack.cpp"

// peoblem.def读取并创建ff层
std::vector<FLIPFLOP> fflayer_create(std::ifstream *problemfile);

// 质心初始化
std::vector<BUFFER> initializeCentroidsforFF(std::vector<FLIPFLOP> &bottoms, int k);
std::vector<BUFFER> initializeCentroidsforBUF(std::vector<BUFFER> &bottoms, int k);

// k-means 算法
std::vector<BUFFER> kMeansforFF(std::vector<FLIPFLOP> bottoms, std::vector<BUFFER> &initialCentroids, int maxIterations = 100);

#endif