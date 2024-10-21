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

std::vector<BUFFER> initializeCentroidsforFF(std::vector<FLIPFLOP> &bottoms, int k);
std::vector<BUFFER> initializeCentroidsforBUF(std::vector<BUFFER> &bottoms, int k);

#endif