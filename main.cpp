
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
#include <optional>

// #include "clktree_pack.cpp"
#include "main.hpp"

// FILEINPUTDEBUG mode
// #define FILEINPUTDEBUG

using namespace std;

NAMECOUNTER ffnamecounter(0, "BUF");
GLOBAL_PARAM global_param;

int main(int argc, char *argv[])
{

    CONSTRAIN constrain;
    CLKROOT clkroot(0, 0);
    std::vector<FLIPFLOP> fflayer;

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
        fflayer = fflayer_create(&problemfile);

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
    // 初始化质心
    std::vector<std::vector<BUFFER>> clktree;
    clktree.push_back(initializeCentroidsforFF(fflayer, 3));

    for (auto &layer0component : clktree[0])
    {
        std::cout << "component " << layer0component.buffername << " at x: " << layer0component.position[0] << " y: " << layer0component.position[1] << std::endl;
    }

    clktree[0] = kMeansforFF(fflayer, clktree[0], 30);

    for (auto &layer0component : clktree[0])
    {
        std::cout << "component " << layer0component.buffername << " at x: " << layer0component.position[0] << " y: " << layer0component.position[1] << std::endl;
        layer0component.PRINT_FF();
    }
    // 时钟树输出
}

// peoblem.def读取并创建ff层
std::vector<FLIPFLOP> fflayer_create(std::ifstream *problemfile)
{
    std::vector<FLIPFLOP> tempfflayer;

    std::string problemline;
    std::getline(*problemfile, problemline);
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

        std::getline(*problemfile, problemline);
        std::string ffname;
        int fflocatex;
        int fflocatey;
        if (std::regex_search(problemline, matches, componentpattern))
        {
            ffname = matches[1].str();
            fflocatex = std::stoi(matches[2].str());
            fflocatey = std::stoi(matches[3].str());
            tempfflayer.push_back(FLIPFLOP(fflocatex, fflocatey, ffname));
        }
    }
#ifdef FILEINPUTDEBUG
#ifdef FFREADINOUT
    FLIPFLOP tempffout("FF0", 0, 0);

    for (uint32_t i = 0; i < fflayer.size(); i++)
    {
        tempffout = fflayer[i];
        std::cout << "component " << tempffout.ffname << " at x: " << tempffout.position[0] << " y: " << tempffout.position[1] << std::endl;
    }
#endif
#endif

    return tempfflayer;
}

// 初始化BUFFER层的质心
// K-means++ 初始化质心
std::vector<BUFFER> initializeCentroidsforFF(std::vector<FLIPFLOP> &bottoms, int k)
{
    std::vector<BUFFER> centroids;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, bottoms.size() - 1);

    // 随机选择第一个质心
    FLIPFLOP tempvar = bottoms[distribution(generator)];
    std::vector<int> templocation = tempvar.GET_POSITION();
    centroids.push_back(BUFFER(templocation[0], templocation[1], ffnamecounter.GET_NAME()));

    for (int i = 1; i < k; ++i)
    {
        std::vector<int> minDist(bottoms.size(), std::numeric_limits<int>::max());
        std::vector<double> distanceWeights(bottoms.size(), 0.0);

        // 计算每个点到最近质心的曼哈顿距离
        for (size_t j = 0; j < bottoms.size(); ++j)
        {
            double minDistance = std::numeric_limits<double>::max();
            for (auto &centroidbuf : centroids)
            {
                int distance = bottoms[j].MaHatanToBUFFER(centroidbuf);
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
                tempvar = bottoms[distribution(generator)];
                templocation = tempvar.GET_POSITION();
                centroids.push_back(BUFFER(templocation[0], templocation[1], ffnamecounter.GET_NAME()));
                break;
            }
        }
    }

    return centroids;
}

std::vector<BUFFER> initializeCentroidsforBUF(std::vector<BUFFER> &bottoms, int k)
{
    std::vector<BUFFER> centroids;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, bottoms.size() - 1);

    // 随机选择第一个质心
    BUFFER tempvar = bottoms[distribution(generator)];
    std::vector<int> templocation = tempvar.GET_POSITION();
    centroids.push_back(BUFFER(templocation[0], templocation[1], ffnamecounter.GET_NAME()));

    for (int i = 1; i < k; ++i)
    {
        std::vector<int> minDist(bottoms.size(), std::numeric_limits<int>::max());
        std::vector<double> distanceWeights(bottoms.size(), 0.0);

        // 计算每个点到最近质心的曼哈顿距离
        for (size_t j = 0; j < bottoms.size(); ++j)
        {
            double minDistance = std::numeric_limits<double>::max();
            for (auto &centroidbuf : centroids)
            {
                int distance = bottoms[j].MaHatanToBUFFER(centroidbuf);
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
                tempvar = bottoms[distribution(generator)];
                templocation = tempvar.GET_POSITION();
                centroids.push_back(BUFFER(templocation[0], templocation[1], ffnamecounter.GET_NAME()));
                break;
            }
        }
    }

    return centroids;
}

// k-means 算法核心
std::vector<BUFFER> kMeansforFF(std::vector<FLIPFLOP> bottoms, std::vector<BUFFER> &initialCentroids, int maxIterations)
{
    std::vector<BUFFER> centroids = initialCentroids; // 使用传入的初始质心
    std::vector<BUFFER> clusters;

    for (size_t i = 0; i < centroids.size(); i++)
    {
        clusters.emplace_back(BUFFER(centroids[i].GET_POSITION()[0], centroids[i].GET_POSITION()[1], centroids[i].buffername));
    }

    for (int iter = 0; iter < maxIterations; ++iter)
    {
        // 将每个点分配给最近的质心
        for (auto &flipflop : bottoms)
        {
            int minDistance = std::numeric_limits<int>::max();
            int closestCentroidIndex = 0;

            for (size_t i = 0; i < centroids.size(); ++i)
            {
                int distance = flipflop.MaHatanToBUFFER(centroids[i]);
                if (distance < minDistance)
                {
                    minDistance = distance;
                    closestCentroidIndex = i;
                }
            }

            clusters[closestCentroidIndex].ADD_FF(&flipflop);
        }

        // 重新计算质心
        for (size_t i = 0; i < centroids.size(); ++i)
        {
            if (clusters[i].FF_ISEMPTY())
            {
                // 如果某个簇没有点，则随机选择一个点作为新的质心
                std::default_random_engine generator;
                std::uniform_int_distribution<int> distribution(0, bottoms.size() - 1);
                FLIPFLOP tempff = bottoms[distribution(generator)];
                std::vector<int> tempposition = tempff.GET_POSITION();
                centroids[i].SET_POSITION(tempposition[0], tempposition[1]);
                continue;
            }

            // 根据簇内坐标确定质心
            int sumx = 0;
            int sumy = 0;
            for (size_t j = 0; j < clusters[i].FF_NUMBER(); j++)
            {
                FLIPFLOP *tempflipflop;
                tempflipflop = clusters[i].READ_FF(j);
                if (nullptr != tempflipflop)
                {
                    sumx += tempflipflop->GET_POSITION()[0];
                    sumy += tempflipflop->GET_POSITION()[1];
                }
                else
                {
                    continue;
                }
            }

            clusters[i].SET_POSITION(int(sumx / clusters[i].FF_NUMBER()), int(sumy / clusters[i].FF_NUMBER()));
        }
    }

    return clusters;
}

#endif