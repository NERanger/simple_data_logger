#include <fstream>
#include "datalogger.hpp"

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

std::ofstream CustomeDataType::file_stream;

int main(int argc, char const *argv[]){
    DataManager::Instance().SetLoopSleepMs(50);  // Default value 30 ms
    DataManager::Instance().Add(std::make_shared<CustomeDataType>(1));
    DataManager::Instance().Add(std::make_shared<CustomeDataType>(2));
    DataManager::Instance().Add(std::make_shared<CustomeDataType>(3));
    CustomeDataType::CloseStream();
    return EXIT_SUCCESS;
}
