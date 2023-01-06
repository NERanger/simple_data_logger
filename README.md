# Simple Data Logger

A simple data logger framwork

## Prerequisites

* A compiler with c++11 support

## Build

```shell
mkdir build
cd build
cmake ..
make
```

## Usage

Create custom data type inheriting `IDataObj` and override the `Dump` method

```cpp
class CustomeDataType : public IDataObj{
public:
    explicit CustomeDataType(int data) : some_data_(data) {
        if(!file_stream.is_open()){
            file_stream.open("./some_file.txt");
        }
    }

    inline void Dump() const override{
        if(file_stream.is_open()){
            file_stream << some_data_ << std::endl;
        }
    }

    static void CloseStream(){
        if(file_stream.is_open()){
            file_stream.close();
        }
    }
private:
    int some_data_;
    static std::ofstream file_stream;
};
```

Add shared pointer of the data instance to `DataManager` (Singleton)

```cpp
DataManager::Instance().Add(std::make_shared<CustomeDataType>(1));
```

Then the data will be automatically dumped by calling the `Dump` method.

`DataManger` will use another thread to perform the dumping loop, control the loop rate by the following interface

```cpp
DataManager::Instance().SetLoopSleepMs(50);
```

A sample app is provided in [sample.cc](./app/sample.cc)