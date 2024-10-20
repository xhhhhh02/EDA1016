
#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <string>
#include <cstring>
#include <list>
#include <iostream>
#include <fstream>
#include <regex>

class FLIPFLOP
{
public:
    std::string ffname;
    std::vector<int> position;
    double lasttothistime;
    double roottothistime;

    FLIPFLOP(int locatex, int locatey, std::string ffname = "DEFAULT");

    std::vector<int> GET_POSITION();
    int MaHatanToBUFFER(BUFFER distbuffer);
};

class BUFFER
{
public:
    std::string buffername;
    std::vector<int> position;
    double lasttothistime;
    double roottothistime;
    bool nxtisbuffer;
    std::list<BUFFER> nxtbuffer;
    bool nxtisflipflop;
    std::list<FLIPFLOP> nxtflipflop;

    BUFFER(int locatex, int locatey, std::string buffername = "DEFAULT");

    std::vector<int> GET_POSITION() const;
    int MaHatanToBUFFER(const BUFFER &distbuffer) const;
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

    GLOBAL_PARAM();

    void GLOBAL_PARAM_FILEINIT(std::ifstream *problemfile);
};

class CLKROOT
{
public:
    std::vector<int> position;
    std::list<BUFFER> nxtbuffer;

    CLKROOT(int locatex, int locatey);

    void CLK_SET_LOCATEX(int locatex);
    void CLK_SET_LOCATEY(int locatey);
    void CLK_SET_LOCATE(int locatex, int locatey);
    void CLK_FILEINIT(std::ifstream *problemfile);
    std::vector<int> GET_POSITION();
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
                  buffer_delay(0) {};

    void CONSTRAIN_FILEINIT(std::ifstream *constrainfile);
};

class NAMECOUNTER
{
private:
    int instanceCount; // 每个实例独有的计数器
    std::string prefix;

public:
    NAMECOUNTER(int initnumber, std::string prefix = "DEFAULT");
    ~NAMECOUNTER();

    // 每次访问时调用的函数，增加实例计数器
    std::string GET_NAME();
};

#endif