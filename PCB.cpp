#include <iostream>
#include <vector>
#include <string>

/**
 * @brief 进程的行为
 * 
 */
class Process{
public:
    std::string m_name;    //进程名
    char        m_state;   //状态 W  R  F (就绪，运行，完成)
    int         priority;  //优先级
    int         nrime;     //需要的总时间
    int         rtime;     //已经运行的时间

};