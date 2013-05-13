#ifndef MFL_MATH_STATISTICS_INCLUDED
#define MFL_MATH_STATISTICS_INCLUDED

#include <numeric>
#include <vector>

namespace math{

namespace detail {
  template <typename T, typename UIntT>
  T mean(T sum, UIntT n) {
    //TODO: make this asserts work!
    // static_assert(std::is_integral<UIntT>::value
    //		  && std::is_unsigned<UIntT>::value,"n must be integer.");
    // static_assert(std::is_arithmetic<T>::value,"sum must be arithmetic.");
    return sum / static_cast<T>(n);
  }
  template <typename IterT, typename T>
  T std(IterT first, IterT last, T mean) {
    //FIXME: just does not work (why?)
    T std = 0, temp;
    while (first != last) {
      temp = *(first++) - mean;
      std += temp * temp;
    }
    return std;
  }
}

template <typename IterT>
typename std::iterator_traits<IterT>::
value_type sum(IterT first, IterT last) {
  return std::accumulate(first,last,0);
}
template <typename IterT>
typename std::iterator_traits<IterT>::
value_type mean(IterT first, IterT last) {
  return detail::mean(sum(first,last),std::distance(first,last));
}
template <typename IterT>
typename std::iterator_traits<IterT>::
value_type std(IterT first, IterT last) {
  return detail::std(first,last,mean(first,last));
}

namespace variadic{
  namespace detail{
    template <typename T>
    std::vector<T> makeVector(T const arg1){
      return std::vector<T>(1,arg1);
    }
    template <typename T, typename... Args>
    std::vector<T> makeVector(T const arg1, Args const... args) {
      auto data = makeVector(args...);
      data.push_back(arg1);
      return data;
    }
  }
  template <typename T, typename... Args>
  T sum(Args const... args) {
    auto data = detail::makeVector(args...);
    return math::sum(data.begin(),data.end());
  }
 
  template <typename T, typename... Args>
  T mean(Args const... args) {
    auto data = detail::makeVector(args...);	
    return math::mean(data.begin(),data.end());
  }
  template <typename T, typename... Args>
  T std(Args const... args) {
    auto data = detail::makeVector(args...);
    return math::std(data.begin(),data.end());
  }
}

}



#endif//MFL_MATH_STATISTICS_INCLUDED
