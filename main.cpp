
#ifndef __MAIN_CPP__
#define __MAIN_CPP__

#include <string>
#include <cstring>
#include <list>
#include <iostream>
#include <fstream>
#include <regex>
#include <variant>
#include <random>

#include "clktree_pack.cpp"

// FILEINPUTDEBUG mode
// #define FILEINPUTDEBUG

using namespace std;

int main(int argc, char *argv[])
{
    GLOBAL_PARAM global_param;
    CONSTRAIN constrain;
    CLKROOT clkroot(0, 0);
    vector<FLIPFLOP> fflayer;

    // 读入命令行参数
    bool problemflag = false;
    bool constrainflag = false;

    string problemfilepath;
    string constrainfilepath;

#ifdef FILEINPUTDEBUG
    std::cout << "Number of command line arguments: " << argc << std::endl;
#endif

    if (argc == 3)
    {
        string findstring1 = "problem=";
        string findstring2 = "constrain=";
        for (int8_t i = 1; i < 3; i++)
        {
            std::string tempstring(argv[i]);
            size_t problempos = tempstring.find(findstring1);
            size_t constrainpos = tempstring.find(findstring2);
            if (problempos != std::string::npos)
            {
                problemfilepath = tempstring.substr(problempos + findstring1.length());
                problemflag = true;
                std::cout << "Problem file path: " << problemfilepath << std::endl;
            }
            if (constrainpos != std::string::npos)
            {
                constrainfilepath = tempstring.substr(constrainpos + findstring2.length());
                constrainflag = true;
                std::cout << "Constrain file path: " << constrainfilepath << std::endl;
            }
        }
        if (problemflag == false)
        {
            std::cout << "No Problem File Path" << std::endl;
        }
        if (constrainflag == false)
        {
            std::cout << "No Constrain File Path" << std::endl;
        }
    }
    else
    {
        std::cout << "Use this program as follows" << std::endl;
        std::cout << "./solution problem=problemfilepath constrain=constrainfilepath" << std::endl;
        return 0;
    }

    // 读取文件
    std::ifstream problemfile(problemfilepath, std::ios::in);
    //    std::ifstream problemfile(problemfilepath, std::ios::in);
    if (problemfile.is_open())
    {
        problemfile.seekg(0, std::ios::beg);
        global_param.GLOBAL_PARAM_FILEINIT(&problemfile);
        clkroot.CLK_FILEINIT(&problemfile);

        std::string problemline;
        std::getline(problemfile, problemline);
        std::smatch matches;
        std::string matchedStr;
        std::regex componentnumberpattern(R"((\d+))");
        std::regex_search(problemline, matches, componentnumberpattern);
        matchedStr = matches[1].str();
        uint32_t componentnumber = std::stoi(matchedStr);
        std::cout << "Components number is " << componentnumber << std::endl;
        std::regex componentpattern(R"((\w+)\s+\w+\s+\(\s*(\d+)\s+(\d+)\s*\))");
        for (uint32_t i = 0; i < componentnumber; i++)
        {

            std::getline(problemfile, problemline);
            std::string ffname;
            int fflocatex;
            int fflocatey;
            if (std::regex_search(problemline, matches, componentpattern))
            {
                ffname = matches[1].str();
                fflocatex = std::stoi(matches[2].str());
                fflocatey = std::stoi(matches[3].str());
                fflayer.push_back(FLIPFLOP(fflocatex, fflocatey, ffname));
            }
        }
#ifdef FILEINPUTDEBUG
#ifdef FFREADINOUT
        FLIPFLOP tempffout("FF0", 0, 0);

        for (uint32_t i = 0; i < fflayer.size(); i++)
        {
            tempffout = fflayer[i];
            std::cout << "component " << tempffout.ffname << " at x: " << tempffout.locatex << " y: " << tempffout.locatey << std::endl;
        }
#endif
#endif
        std::cout << "Read in FF number is " << fflayer.size() << std::endl;
        global_param.ffnumber = fflayer.size();
        problemfile.close();
    }
    else
    {
        std::cerr << "Unable to open problem file for reading." << std::endl;
        return 0;
    }
    // 读取constrain文件
    std::ifstream constrainfile(constrainfilepath, std::ios::in);
    if (constrainfile.is_open())
    {
        constrainfile.seekg(0, std::ios::beg);
        constrain.CONSTRAIN_FILEINIT(&constrainfile);
        constrainfile.close();
    }
    else
    {
        std::cerr << "Unable to open constrain file for reading." << std::endl;
        return 0;
    }

    // 算法

    // 时钟树输出
}

// 初始化BUFFER层的质心
// K-means++ 初始化质心
std::vector<BUFFER> initializeCentroids(const std::vector<std::variant<BUFFER, FLIPFLOP>> &bottoms, int k)
{
    std::vector<BUFFER> centroids;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, bottoms.size() - 1);

    // 随机选择第一个质心
    if (typeid(BUFFER) == typeid(bottoms[0]))
    {
        std::variant<BUFFER, FLIPFLOP> tempvar = bottoms[distribution(generator)];
        centroids.push_back(BUFFER(1, 1, "BUF0"));
    }

    for (int i = 1; i < k; ++i)
    {
        std::vector<double> minDist(points.size(), std::numeric_limits<double>::max());
        std::vector<double> distanceWeights(points.size(), 0.0);

        // 计算每个点到最近质心的曼哈顿距离
        for (size_t j = 0; j < points.size(); ++j)
        {
            double minDistance = std::numeric_limits<double>::max();
            for (const auto &centroid : centroids)
            {
                double distance = points[j].manhattanDistanceTo(centroid);
                if (distance < minDistance)
                {
                    minDistance = distance;
                }
            }
            minDist[j] = minDistance;
            distanceWeights[j] = 1.0 / minDistance;
        }

        // 计算下一个质心
        double totalWeight = std::accumulate(distanceWeights.begin(), distanceWeights.end(), 0.0);
        double r = (static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)) * totalWeight;
        double cumulativeWeight = 0.0;
        for (size_t j = 0; j < distanceWeights.size(); ++j)
        {
            cumulativeWeight += distanceWeights[j];
            if (cumulativeWeight >= r)
            {
                centroids.push_back(points[j]);
                break;
            }
        }
    }

    return centroids;
}
#endif