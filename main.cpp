
#ifndef __MAIN_CPP__
#define __MAIN_CPP__

#include <string>
#include <cstring>
#include <list>
#include <iostream>
#include <fstream>
#include <regex>

#include "clktree_pack.cpp"

// debug mode
// #define DEBUG

using namespace std;

int main(int argc, char *argv[])
{
    GLOBAL_PARAM global_param;
    CLKROOT clkroot(0, 0);

    // 读入命令行参数
    bool problemflag = false;
    bool constrainflag = false;

    string problemfilepath;
    string constrainfilepath;

#ifdef DEBUG
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
#ifdef DEBUG
        std::cout << "Components number is " << problemline << std::endl;
#endif

#ifdef DEBUG
        while (std::getline(problemfile, problemline))
        {
            std::cout << problemline << std::endl;
        }
#endif
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

        std::string constrainline;

#ifdef DEBUG
        while (std::getline(constrainfile, constrainline))
        {
            std::cout << constrainline << std::endl;
        }
#endif

        constrainfile.close();
    }
    else
    {
        std::cerr << "Unable to open constrain file for reading." << std::endl;
        return 0;
    }
}

#endif