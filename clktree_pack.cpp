
#ifndef __CLKTREE_PACK_CPP__
#define __CLKTREE_PACK_CPP__

#include <string>
#include <cstring>
#include <list>
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

class PHYINFO
{
};

class FLIPFLOP
{
public:
        string ffname;
        int locatex;
        int locatey;
        double lasttothistime;
        double roottothistime;

        FLIPFLOP(std::string ffname, int locatex, int locatey) : ffname(ffname),
                                                                 locatex(locatex),
                                                                 locatey(locatey)
        {
        }
};

class BUFFER
{
public:
        string buffername;
        int locatex;
        int locatey;
        double lasttothistime;
        double roottothistime;
        bool nxtisbuffer;
        list<BUFFER> nxtbuffer;
        bool nxtisflipflop;
        list<FLIPFLOP> nxtflipflop;

        BUFFER(std::string buffername, int locatex, int locatey) : buffername(buffername),
                                                                   locatex(locatex),
                                                                   locatey(locatey) {}
};

class CLKROOT
{
public:
        int locatex;
        int locatey;
        list<BUFFER> nxtbuffer;

        CLKROOT(int locatex, int locatey) : locatex(locatex),
                                            locatey(locatey) {}

        void CLK_SET_LOCATEX(int locatex)
        {
                this->locatex = locatex;
        }

        void CLK_SET_LOCATEY(int locatey)
        {
                this->locatey = locatey;
        }

        void CLK_SET_LOCATE(int locatex, int locatey)
        {
                this->CLK_SET_LOCATEX(locatex);
                this->CLK_SET_LOCATEY(locatey);
        }

        void CLK_FILEINIT(std::ifstream *problemfile)
        {
                std::string problemline;
                std::getline(*problemfile, problemline);
                std::regex clkrootpattern("\\( (\\d+) (\\d+) \\)");
                std::smatch matches;
                std::string matchedStr;
#ifdef FILEINPUTDEBUG
                std::cout << std::regex_search(problemline, matches, clkrootpattern) << std::endl;
#else
                std::regex_search(problemline, matches, clkrootpattern);
#endif
                matchedStr = matches[1].str();
#ifdef FILEINPUTDEBUG
                std::cout << matchedStr << std::endl;
#endif
                this->locatex = std::stoi(matchedStr);
                matchedStr = matches[2].str();
#ifdef FILEINPUTDEBUG
                std::cout << matchedStr << std::endl;
#endif
                this->locatey = std::stoi(matchedStr);
        }
};

class GLOBAL_PARAM
{
public:
        int units;
        int globalareaminx;
        int globalareamaxx;
        int globalareaminy;
        int globalareamaxy;
        int buffersizex;
        int buffersizey;
        int buffernumber;
        int ffsizex;
        int ffsizey;
        int ffnumber;

        GLOBAL_PARAM() : units(0),
                         globalareaminx(0),
                         globalareamaxx(0),
                         globalareaminy(0),
                         globalareamaxy(0),
                         buffersizex(0),
                         buffersizey(0),
                         buffernumber(0),
                         ffsizex(0),
                         ffsizey(0),
                         ffnumber(0) {}

        void GLOBAL_PARAM_FILEINIT(ifstream *problemfile)
        {
                // 单位处理
                std::string problemline;
                std::getline(*problemfile, problemline);
                std::regex unitspattern(R"(\s(\d+)\s)");
                std::smatch matches;
                std::regex_search(problemline, matches, unitspattern);
                std::string matchedStr = matches.str(1); // 使用1来获取捕获组的内容
                this->units = std::stoi(matchedStr);

                // 区域处理
                std::getline(*problemfile, problemline);
                std::regex dieareapattern("\\( (\\d+) (\\d+) \\)");
#ifdef FILEINPUTDEBUG
                std::cout << std::regex_search(problemline, matches, dieareapattern) << std::endl;
#else
                std::regex_search(problemline, matches, dieareapattern);
#endif
                matchedStr = matches[1].str();
#ifdef FILEINPUTDEBUG
                std::cout << matchedStr << std::endl;
#endif
                this->globalareaminx = std::stoi(matchedStr);
                matchedStr = matches[2].str();
#ifdef FILEINPUTDEBUG
                std::cout << matchedStr << std::endl;
#endif
                this->globalareaminy = std::stoi(matchedStr);
                problemline = matches.suffix();
                std::cout << std::regex_search(problemline, matches, dieareapattern) << std::endl;
                problemline = matches.suffix();
                std::cout << std::regex_search(problemline, matches, dieareapattern) << std::endl;
                matchedStr = matches[1].str();
#ifdef FILEINPUTDEBUG
                std::cout << matchedStr << std::endl;
#endif
                this->globalareamaxx = std::stoi(matchedStr);
                matchedStr = matches[2].str();
#ifdef FILEINPUTDEBUG
                std::cout << matchedStr << std::endl;
#endif
                this->globalareamaxy = std::stoi(matchedStr);

                // FF大小处理
                std::getline(*problemfile, problemline);
                std::regex ffsizepattern("\\( (\\d+) (\\d+) \\)");
#ifdef FILEINPUTDEBUG
                std::cout << std::regex_search(problemline, matches, ffsizepattern) << std::endl;
#else
                std::regex_search(problemline, matches, ffsizepattern);
#endif
                matchedStr = matches[1].str();
#ifdef FILEINPUTDEBUG
                std::cout << matchedStr << std::endl;
#endif
                this->ffsizex = std::stoi(matchedStr);
                matchedStr = matches[2].str();
#ifdef FILEINPUTDEBUG
                std::cout << matchedStr << std::endl;
#endif
                this->ffsizey = std::stoi(matchedStr);

                // BUFFER大小处理
                std::getline(*problemfile, problemline);
                std::regex buffersizepattern("\\( (\\d+) (\\d+) \\)");
#ifdef FILEINPUTDEBUG
                std::cout << std::regex_search(problemline, matches, buffersizepattern) << std::endl;
#else
                std::regex_search(problemline, matches, buffersizepattern);
#endif
                matchedStr = matches[1].str();
#ifdef FILEINPUTDEBUG
                std::cout << matchedStr << std::endl;
#endif
                this->buffersizex = std::stoi(matchedStr);
                matchedStr = matches[2].str();
#ifdef FILEINPUTDEBUG
                std::cout << matchedStr << std::endl;
#endif
                this->buffersizey = std::stoi(matchedStr);
        }
};

class CONSTRAIN
{
public:
        double net_uint_r;
        double net_uint_c;
        double max_net_rc;
        int max_fanout;
        double buffer_delay;

        CONSTRAIN() : net_uint_r(0),
                      net_uint_c(0),
                      max_net_rc(0),
                      max_fanout(0),
                      buffer_delay(0) {}

        void CONSTRAIN_FILEINIT(ifstream *constrainfile)
        {
                // net unit r
                std::string constrainline;
                std::getline(*constrainfile, constrainline);
                std::regex netunitrpattern(R"(\s(\d+)\s)");
                std::smatch matches;
        }
};

#endif