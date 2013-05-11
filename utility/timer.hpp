#ifndef MFL_CLOCK_INCLUDED
#define MFL_CLOCK_INCLUDED

#include <chrono>

#include "../common.hpp"

MFL_BEGIN
/// \class Timer
/// \brief A simple stopwatch class.
/// \details A wrapper for chrono that allows the get timedifference without
/// the ridiculous syntax of chrono. Has pausing functionality.
/// \author mojo
class Timer{
 public:
  Timer() : time_saved_(),pause_(false) {
    Start();
  }
  /// Saves the current time.
  /// \details
  void Start() {
    time_saved_ = std::chrono::system_clock::now();
  }
  /// Returns the current state.
  /// \details An instance may either be unpaused(default) or paused.
  /// \return Returns true if unpaused and false otherwise.
  bool IsActive() const {
    return !pause_;
  }
   /// Switches the instance into an paused state.
  /// \details If a instance is paused any call to a Delta function return 0.
  void Pause() {
    pause_ = true;
  }
  /// Switches the instance back into an unpaused state.
  /// \details This function also calls Start, effectivly resetting the timer.
  void Unpause() {
    pause_ = false;
    Start();
  }
  /// Returns the timedifference in seconds since
  /// the last call to Start or Unpause.
  /// \details This function returns 0 if the instance is paused.
  /// The function will reset the timer!
  /// \return Returns the timedifference. (The default return type is long.)
  template <typename Time_t = long>
  Time_t DeltaSeconds() {
    using std::chrono::seconds;
    return static_cast<Time_t>(_InternalDelta<seconds>());
  }
  /// Returns the timedifference in milliseconds since
  /// the last call to Start or Unpause.
  /// \details This function returns 0 if the instance is paused.
  /// The function will reset the timer!
  /// \return Returns the timedifference. (The default return type is long.)
  template <typename Time_t = long>
  Time_t DeltaMilliseconds() {
    using std::chrono::milliseconds;
    return static_cast<Time_t>(_InternalDelta<milliseconds>());
  }
  /// Returns the timedifference in microseconds since
  /// the last call to Start or Unpause.
  /// \details This function returns 0 if the instance is paused.
  /// The function will reset the timer!
  /// \return Returns the timedifference. (The default return type is long.)
  template <typename Time_t = long>
  Time_t DeltaMicroseconds() {
    using std::chrono::microseconds;
    return static_cast<Time_t>(_InternalDelta<microseconds>());
  }

 private:
  // Depends on the default type of duration cast!
  // Right now it is long
  template <typename Duration_t>
  long _InternalDelta() {
    using namespace std::chrono;
    if (pause_) return 0;
    auto time_end = system_clock::now();
    auto delta = duration_cast<Duration_t>(time_end - time_saved_).count();
    time_saved_ = time_end;
    return delta;
  }

  ///< TimePoint variable for storage of timedifference.
  std::chrono::time_point<std::chrono::system_clock> time_saved_;
  bool pause_; ///< A boolean switch for simple pause functionality.
};

MFL_END

#endif//MFL_CLOCK_INCLUDED
