#include "Scheduler.h"
#include <iostream>


void Scheduler::insertByPriority(Process* p){
    auto head = this->m_process.getHead();

    if(!head || p->getPriority() >= head->data->getPriority())
        this->m_process.push_front(p);
    else{
        auto cur = head;

        while(cur->next && cur->next->data->getPriority() > p->getPriority())
            cur = cur->next;

        this->m_process.insert(cur,p);
    }
}

void Scheduler::ListInfo(Process* p){
    std::cout << "名称:" << p->getName() << std::endl << "优先级:" << p->getPriority() << std::endl << "状态:" << p->getState()
    << std::endl << "运行时间:" << p->getRTime() << std::endl << "总时间:" << p->getNTime() << std::endl;
}

Scheduler::Scheduler():m_count(0) {}

Scheduler::~Scheduler(){
    auto node = this->m_process.getHead();

    while(node)
    {
        delete node->data;
        node = node->next;
    }
}

void Scheduler::run(){
    while(!this->m_process.empty()){
        this->m_count++;
        std::cout << "第" << m_count <<"次调度:" << std::endl;
        auto font = this->m_process.front();
        this->m_process.pop_front();

        if(font->setRunning()){
            //打印
            display(font);

            font->execute();
            font->updateState();

            if(font->isFinish()){

                std::cout << "进程[" << font->getName() << "]" << "运行完成!" << std::endl;
                delete font;
            }
            else{
                //插入链表继续运行
                insertByPriority(font);
            }
        }
    }
}

void Scheduler::display(Process* p){
    std::cout << "----------------------------------------------------" << std::endl;

    std::cout << "当前进程: " << p->getName() << std::endl;
    ListInfo(p);

    std::cout << "当前就绪队列状态为：" << std::endl;
    auto curr = this->m_process.getHead();
    if (!curr)
        std::cout << "(空)" << std::endl;
    else {
        while (curr) {
            ListInfo(curr->data);
            curr = curr->next;
            }
        }
    std::cout << "----------------------------------------------------" << std::endl;
}


std::istream& operator>>(std::istream& is,Scheduler& s){
    int num;
    std::cout << "请输入调度进程数目：";
    is >> num;

    for (int i = 0; i < num; ++i) {
        std::string name;
        int prio, time;
        std::cout << "\n输入进程号 No." << i + 1 << "：" << std::endl;
        std::cout << "输入进程名："; 
        is >> name;
        std::cout << "输入进程优先数："; 
        is >> prio;
        std::cout << "输入进程运行时间："; 
        is >> time;

        s.insertByPriority(new Process(name, prio, time));
    }
    return is;

}