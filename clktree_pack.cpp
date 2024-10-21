
#ifndef __CLKTREE_PACK_CPP__
#define __CLKTREE_PACK_CPP__

#include <string>
#include <cstring>
#include <list>
#include <iostream>
#include <fstream>
#include <regex>

#include "clktree_pack.cpp"

using namespace std;

class GLOBAL_PARAM;
class FLIPFLOP;

extern GLOBAL_PARAM global_param;

class PHYINFO
{
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
        int biasx;
        int biasy;

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
                         ffnumber(0),
                         biasx(0),
                         biasy(0)
        {
        }

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
                std::regex_search(problemline, matches, dieareapattern);
                problemline = matches.suffix();
                std::regex_search(problemline, matches, dieareapattern);
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

                // bias设置
                this->biasx = (this->ffsizex - this->buffersizex) >> 1;
                this->biasy = (this->ffsizey - this->buffersizey) >> 1;
        }
};

class BUFFER
{
public:
        string buffername;
        std::vector<int> position;
        double lasttothistime;
        double roottothistime;
        BUFFER *lastbuffer;
        bool nxtisbuffer;
        list<BUFFER *> nxtbuffer;
        bool nxtisflipflop;
        list<FLIPFLOP *> nxtflipflop;

        BUFFER(int locatex, int locatey, std::string buffername = "DEFAULT") : buffername(buffername)
        {
                this->position.push_back(locatex);
                this->position.push_back(locatey);
                this->lastbuffer = nullptr;
        }

        std::vector<int> GET_POSITION()
        {
                return this->position;
        }

        int MaHatanToBUFFER(BUFFER distbuffer)
        {
                vector<int> distposition = distbuffer.GET_POSITION();
                return abs(this->position[0] - distposition[0]) + abs(this->position[1] - distposition[1]);
        }
};

class FLIPFLOP
{
public:
        string ffname;
        std::vector<int> position;
        double lasttothistime;
        double roottothistime;
        BUFFER *lastbuffer;

        FLIPFLOP(int locatex, int locatey, std::string ffname = "DEFAULT") : ffname(ffname)
        {
                this->position.push_back(locatex);
                this->position.push_back(locatey);
                this->lastbuffer = nullptr;
        }

        std::vector<int> GET_POSITION()
        {
                return this->position;
        }

        int MaHatanToBUFFER(BUFFER distbuffer)
        {
                vector<int> distposition = distbuffer.GET_POSITION();
                return abs(this->position[0] - distposition[0] + global_param.biasx) + abs(this->position[1] - distposition[1] + global_param.biasy);
        }
};

class CLKROOT
{
public:
        std::vector<int> position;
        list<BUFFER> nxtbuffer;

        CLKROOT(int locatex, int locatey)
        {
                this->position.push_back(locatex);
                this->position.push_back(locatey);
        }

        void CLK_SET_LOCATEX(int locatex)
        {
                this->position[0] = locatex;
        }

        void CLK_SET_LOCATEY(int locatey)
        {
                this->position[1] = locatey;
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
                this->position[0] = std::stoi(matchedStr);
                matchedStr = matches[2].str();
#ifdef FILEINPUTDEBUG
                std::cout << matchedStr << std::endl;
#endif
                this->position[1] = std::stoi(matchedStr);
        }

        std::vector<int> GET_POSITION()
        {
                return this->position;
        }
};

class CONSTRAIN
{
public:
        double net_unit_r;
        double net_unit_c;
        double max_net_rc;
        int max_fanout;
        double buffer_delay;

        CONSTRAIN() : net_unit_r(0),
                      net_unit_c(0),
                      max_net_rc(0),
                      max_fanout(0),
                      buffer_delay(0) {}

        void CONSTRAIN_FILEINIT(ifstream *constrainfile)
        {
                // net unit r
                std::string constrainline;
                std::getline(*constrainfile, constrainline);
                std::string matchStr;
                std::string equal = "=";
                size_t equalpos = constrainline.find(equal);
                matchStr = constrainline.substr(equalpos + equal.length() + 1);
                this->net_unit_r = std::stod(matchStr);

                // net unit c
                std::getline(*constrainfile, constrainline);
                equalpos = constrainline.find(equal);
                matchStr = constrainline.substr(equalpos + equal.length() + 1);
                this->net_unit_c = std::stod(matchStr);

                // max net rc
                std::getline(*constrainfile, constrainline);
                equalpos = constrainline.find(equal);
                matchStr = constrainline.substr(equalpos + equal.length() + 1);
                this->max_net_rc = std::stod(matchStr);

                // max fanout
                std::getline(*constrainfile, constrainline);
                equalpos = constrainline.find(equal);
                matchStr = constrainline.substr(equalpos + equal.length() + 1);
                this->max_fanout = std::stoi(matchStr);

                // buffer_delay
                std::getline(*constrainfile, constrainline);
                equalpos = constrainline.find(equal);
                matchStr = constrainline.substr(equalpos + equal.length() + 1);
                this->buffer_delay = std::stod(matchStr);

#ifdef FILEINPUTDEBUG
                std::cout << "net_unit_r is " << this->net_unit_r << std::endl;
                std::cout << "net_unit_c is " << this->net_unit_c << std::endl;
                std::cout << "max_net_rc is " << this->max_net_rc << std::endl;
                std::cout << "max_fanout is " << this->max_fanout << std::endl;
                std::cout << "buffer_delay is " << this->buffer_delay << std::endl;
#endif
        }
};

class NAMECOUNTER
{
private:
        int instanceCount; // 每个实例独有的计数器
        std::string prefix;

public:
        // 构造函数
        NAMECOUNTER(int initnumber, std::string prefix = "DEFAULT") : instanceCount(0), prefix(prefix)
        {
        }

        // 析构函数
        ~NAMECOUNTER()
        {
        }

        // 每次访问时调用的函数，增加实例计数器
        std::string GET_NAME()
        {
                std::string namestring = prefix + std::to_string(instanceCount);
                ++instanceCount;
                return namestring;
        }
};

#endif