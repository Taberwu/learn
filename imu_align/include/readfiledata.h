/****************************************************************************************
 * @FilePath: readfiledata.h
 * @Author: Taber.wu
 * @Date: 2023-10-20 14:54:09
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-10-27 15:01:02
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#pragma once

#include <deque>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iostream>

#include "message.h"

namespace jsos{

class FileDataReader{
public:
    FileDataReader(){}
    FileDataReader(const std::string filename);
    virtual ~FileDataReader() = default;
    void readimudata(std::deque<std::shared_ptr<ImuData>> &imudatas);
protected:

private:
    std::fstream datafile_;
};
}

