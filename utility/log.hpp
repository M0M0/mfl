#ifndef MFL_LOG_INCLUDED
#define MFL_LOG_INCLUDED

#include <fstream>
#include <string> 
#include <memory>
#include <algorithm>
#include "MFL_Exception.hpp"

#include <ctime>   // for Errorlog
#include <cstdarg> // va functions also Errorlog
#include <memory>  // Errorlog

namespace MFL{

class Log{
 public:
  Log(std::string name) : _filename(name+".txt") {}
  virtual ~Log() {
    std::ofstream file(_filename.c_str(),
		       std::ios_base::out|std::ios_base::trunc);
    std::copy(_data.begin(),_data.end(),std::ostream_iterator<char>(file));
    file.close();
  }

 protected:
   virtual std::string LogFormat() = 0; //not nessesary

  std::string		_data;

 private:
  std::string		_filename;
};

#define STRING_SIZE 512

#pragma warning(push)
#pragma warning(disable:4996)
class ErrorLog : public Log {
 public:
  ErrorLog(std::string name) : Log(name) {}

  void Error(const char* str,...) {
    va_list arg_list;
    va_start(arg_list,str);
    std::unique_ptr<char[]> tmp(new char[STRING_SIZE]);
    vsnprintf(tmp.get(),STRING_SIZE,str,arg_list);
    Error(std::string(tmp.get()));
  }
  void Error(const std::string& str) {
    _data += LogFormat() + "<-Err-> " + StringFormat(str) + '\n';
  } 

  void Message(const char* str,...) {
    va_list arg_list;
    va_start(arg_list,str);
    std::unique_ptr<char[]> tmp(new char[STRING_SIZE]);
    vsnprintf(tmp.get(),STRING_SIZE,str,arg_list);
    Message(std::string(tmp.get()));
  }
  void Message(const std::string& str) {
    _data += LogFormat() + "<-Msg-> " + StringFormat(str) + '\n';
  }

protected:
  std::string AddDate(){
    time_t rawtime;
    time(&rawtime);
    tm* timeinfo = localtime(&rawtime);
    return std::to_string(timeinfo->tm_mday)+'/'
      +std::to_string(timeinfo->tm_mon+1)+'/'
      +std::to_string(timeinfo->tm_year+1900);
  }
  std::string AddTime(){
    time_t rawtime;
    time(&rawtime);
    tm* timeinfo = localtime(&rawtime);
    return  std::to_string(timeinfo->tm_hour)+':'
      +std::to_string(timeinfo->tm_min)
      +'.'+std::to_string(timeinfo->tm_sec);
  }
  std::string LogFormat(){
    return addDate()+' '+addTime()+'\t';
  }

  std::string StrFormat(std::string str){
    //auto it = str.begin();
    //for(;it!=str.end()-1;++it){
    //	if(*it!='\n')
    //		continue;
    //	str.insert(it+1,2,'\t'); //dunoo
    //}
    return str;
  }
};
#pragma warning(pop)

#undef STRING_SIZE
}

#endif//MFL_LOG_INCLUDED
