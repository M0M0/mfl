#ifndef MFL_LOG_INCLUDED
#define MFL_LOG_INCLUDED

//for Log
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
//for ErrorLog
#include <ctime>
#include <cstdarg>
#include <vector>

#include "../common.hpp"
#include "exception.hpp"

MFL_BEGIN

class Log{
 public:
  Log(std::string name) : filename_(name+".txt") {}
  virtual ~Log() {
    std::ofstream file(filename_.c_str(),
		       std::ios_base::out|std::ios_base::trunc);
    std::copy(data_.begin(),data_.end(),std::ostream_iterator<char>(file));
  }

 protected:
  std::string		data_;

 private:
  std::string		filename_;
};

#define STRING_SIZE 512

class ErrorLog : public Log {
 public:
  ErrorLog(std::string name) : Log(name) {}
  void Error(char const * str,...) {
    va_list arg_list;
    va_start(arg_list,str);
    std::vector<char> tmp(STRING_SIZE);
    vsnprintf(tmp.data(),STRING_SIZE,str,arg_list);
    Error(std::string(tmp.data()));
  }
  void Error(std::string const& str) { data_ += LogFormat() + "<-Err-> " + StringFormat(str) + '\n'; } 
  void Message(char const * str,...) {
    va_list arg_list;
    va_start(arg_list,str);
    std::vector<char> tmp(STRING_SIZE);
    vsnprintf(tmp.data(),STRING_SIZE,str,arg_list);
    Message(std::string(tmp.data()));
  }
  void Message(const std::string& str) { data_ += LogFormat() + "<-Msg-> " + StringFormat(str) + '\n'; }

 protected:
  std::string AddDate() {
    time_t rawtime;
    time(&rawtime);
    tm* timeinfo = localtime(&rawtime);
    return std::to_string(timeinfo->tm_mday) + '/' + std::to_string(timeinfo->tm_mon+1) + '/'
      + std::to_string(timeinfo->tm_year + 1900);
  }
  std::string AddTime() {
    time_t rawtime;
    time(&rawtime);
    tm* timeinfo = localtime(&rawtime);
    return std::to_string(timeinfo->tm_hour) + ':' + std::to_string(timeinfo->tm_min) + '.'
      + std::to_string(timeinfo->tm_sec);
  }
  std::string LogFormat() { return AddDate() + ' ' + AddTime() + '\t'; }
  std::string StringFormat(std::string str) {
    //auto it = str.begin();
    //for(;it!=str.end()-1;++it){
    //	if(*it!='\n')
    //		continue;
    //	str.insert(it+1,2,'\t'); //dunno
    //}
    return str;
  }
};

#undef STRING_SIZE

MFL_END

#endif//MFL_LOG_INCLUDED
