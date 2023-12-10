#ifndef FILE_H
#define FILE_H

#include <iostream>
class FileHandling
{
public:
    std::string getData(const std::string fileName);
    void setData(const std::string data, const std::string fileName);
    FileHandling(/* args */);
    ~FileHandling();
};
#endif