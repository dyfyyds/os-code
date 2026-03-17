/**
 * @file BoundedBuffer.cpp
 * @author dyf
 * @brief 使用c++标准的条件变量实现的生产者消费者模型
 * @details 该文件实现了典型的生产者-消费者模型，通过 std::condition_variable 和 std::unique_lock 确保线程安全。
 * @version 1.0
 * @date 2026-03-17
 * 
 */

#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>

/**
 * @brief 使用条件变量实现生产者消费者模型
 * 
 */
class ConcBuffer{
private:
    //存储任务队列
    std::queue<int> task;
    //最大缓存区数量
    int max_capacity;
    //互斥锁
    std::mutex mt;
    //empty 消费者取出数据通知生产者 满空间时候等待消费者取出数据
    std::condition_variable empty;
    //full  生产者放入数据通知   空缓存等待
    std::condition_variable full;



public:
    /**
     * @brief 构造函数
     * 
     * @param capacity 最大缓存区数量
     */
    ConcBuffer(int capacity):max_capacity(capacity){}

    /**
     * @brief 生产者
     * 
     * @param item  数据项
     * @param p_name  名字
     * 
     */
    void produce(int item,const std::string& p_name)
    {
        std::unique_lock<std::mutex> lock(this->mt);

        // 等待  如果条件不成立，则阻塞  这里缓冲区不满执行
        empty.wait(lock,[this](){
            return this->task.size() < this->max_capacity;
        });

        //放入数据
        task.push(item);

        std::cout << "[+productor]" << p_name << "----------------生成数据->" << item << "--缓冲空间大小->" << this->task.size() << std::endl;
        
        //通知一个消费者
        full.notify_one();
    }

    /**
     * @brief 消费者
     * 
     * @param c_name 消费者名字
     * @return int 
     */
    int consume(const std::string& c_name)
    {
        std::unique_lock<std::mutex> lock(this->mt);

        full.wait(lock,[this](){
            return !this->task.empty();
        });

        int item = this->task.front();
        this->task.pop();

        std::cout << "[-consumer]" << c_name << "-------------消费数据->" << item << "--缓冲空间大小->" << this->task.size() << std::endl;

        empty.notify_one();
        return item;
    }
 };


int main()
{
    ConcBuffer buffer(10);
    std::vector<std::thread> threads;

    for(int i = 1;i <= 2;i++)
    {
        threads.emplace_back([&buffer,i](){
            for(int j = 0;j < 10;j++)
            {
                int val = i * 100 + j;

                std::stringstream ss;
                ss << std::this_thread::get_id();  
                         
                buffer.produce(val,ss.str());

                //延时
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
    }

    for(int i = 1;i <= 3;i++)
    {
        for (int j = 0; j < 5; j++) {
            std::stringstream ss;
            ss << std::this_thread::get_id();
            
            buffer.consume(ss.str());
            
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    }


    for(auto& th : threads)
    {
        if(th.joinable())
        {
            th.join();
        }
    }


    return 0;
}