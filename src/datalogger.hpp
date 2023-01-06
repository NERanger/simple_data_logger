#pragma once

#include <memory>

class IDataObj{
public:
    using Ptr = std::shared_ptr<IDataObj>;

    virtual ~IDataObj() = default;
    virtual void Dump() const = 0;

};

class DataManager{
public:
    static DataManager& Instance();
    
    void SetLoopSleepMs(int ms);

    void Add(IDataObj::Ptr obj);

private:
    DataManager();
    ~DataManager();

    // Uncopyable
    class Impl;
    std::unique_ptr<Impl> impl_;
};
