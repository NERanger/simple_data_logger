#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <iostream>

#include "datalogger.hpp"

class DataManager::Impl{
public:
    Impl();
    ~Impl();
    void DumpProcess();
    void DrainBuffer();
    int process_loop_sleep_ms_ = 30;
    std::queue<IDataObj::Ptr> data_buffer_;
    std::mutex data_buffer_mtx_;
    std::condition_variable data_buffer_cond_;
    std::thread dump_thread_;
    std::atomic_bool dump_proc_stop_;
};

DataManager::Impl::Impl(){
    dump_proc_stop_ = false;
    dump_thread_ = std::thread(&Impl::DumpProcess, this);
}

DataManager::Impl::~Impl(){
    dump_proc_stop_ = true;
    dump_thread_.join();
}

DataManager::DataManager() : impl_(new Impl){

}

DataManager::~DataManager(){

}

DataManager& DataManager::Instance(){
    static DataManager instance;
    return instance;
}

void DataManager::SetLoopSleepMs(int ms){
    impl_->process_loop_sleep_ms_ = ms;
}

void DataManager::Add(IDataObj::Ptr obj){
    Impl& impl = *impl_;
    {
        std::lock_guard<std::mutex> lk(impl.data_buffer_mtx_);
        impl.data_buffer_.push(obj);
    }
    impl.data_buffer_cond_.notify_one();
}

void DataManager::Impl::DrainBuffer(){
    // uncomment to print statistics
    // std::cout << "queue size: " << data_buffer_.size() << std::endl;
    // std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    while(!data_buffer_.empty()){
        data_buffer_.front()->Dump();
        data_buffer_.pop();
    }
    // std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    // double tdrain= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();
    // std::cout << "Buffer drain cost time: " << tdrain << std::endl;
}

void DataManager::Impl::DumpProcess(){
    while(!dump_proc_stop_){
        std::unique_lock<std::mutex> lk(data_buffer_mtx_);
        data_buffer_cond_.wait(lk,
            [this]{return !data_buffer_.empty();});
        
        DrainBuffer();
        lk.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(process_loop_sleep_ms_));
    }

    // In case buffer is not empty when stop
    {
        std::lock_guard<std::mutex> lk(data_buffer_mtx_);
        DrainBuffer();
    }
}