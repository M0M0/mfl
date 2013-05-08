// TODO: adding locks on multithreaded applications
// Is moving to unique_ptr a good idea?
#ifndef MFL_SINGLETON_INCLUDED
#define MFL_SINGLETON_INCLUDED

namespace MFL{

template <class T>
class Singleton{
 public:
  static T* GetInstance() {
    if (!_instance) _instance = new T();
    return _instance;
  }

 private:
  ~Singleton() { delete _instance; }

  static T* _instance;
};

template <class T>
T* Singleton<T>::_instance = nullptr;
  
}

#endif//MFL_SINGLETON_INCLUDED
