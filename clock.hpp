#ifndef MFL_CLOCK_INCLUDED
#define MFL_CLOCK_INCLUDED

#include <chrono>

#include "common.hpp"

MFL_BEGIN
/// \class Clock
/// \brief A simple stopwatch class.
/// \details A wrapper for chrono that allows the get timedifference without
/// the ridiculous syntax of chrono. Has pausing functionality.
/// \author mojo
class Clock{
  typedef std::chrono::system_clock	   Clock_t;
  typedef std::chrono::time_point<Clock_t> TimePoint;
public:
  Clock() : time_saved_(),pause_(false) {
    Start();
  }
  /// Saves the current time.
  /// \details
  void Start() {
    time_saved_ = Clock_t::now();
  }
  /// Returns the current state.
  /// \details An instance may either be unpaused(default) or paused.
  /// \return Returns true if unpaused and false otherwise.
  bool IsActive() const {
    return !pause_;
  }
  /// Switches the instance into an paused state.
  /// \details If a instance is paused any call to a Delta function return 0.
  /// \sa DeltaMS and DeltaS
  void Pause() {
    pause_ = true;
  }
  /// Switches the instance back into an unpaused state.
  /// \details This function also calls Start, effectivly resetting the timer.
  void Unpause() {
    pause_ = false;
    Start();
  }
  
  /// Returns the timedifference in milliseconds since
  /// the last call to Start or Unpause.
  /// \details This function returns 0 if the instance is paused.
  /// The function will reset the timer!
  /// \return Returns the timedifference. (The default return type is float.)
  template <typename Time = float>
  Time DeltaMS() {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    if (pause_) return static_cast<Time>(0);
    auto time_end = Clock_t::now();
    Time delta = static_cast<Time>(
	         duration_cast<milliseconds>(time_end - time_saved_).count());
    time_saved_ = time_end;
    return delta;
  }
  /// Returns the timedifference in seconds since
  /// the last call to Start or Unpause.
  /// \details This function returns 0 if the instance is paused.
  /// The function will reset the timer!
  /// \return Returns the timedifference. (The default return type is float.)
  template <typename Time = float>
  Time DeltaS() {
    using std::chrono::duration_cast;
    using std::chrono::seconds;
    if (pause_) return static_cast<Time>(0);
    auto time_end = Clock_t::now();
    Time delta = static_cast<Time>(
	         duration_cast<seconds>(time_end - time_saved_).count());
    time_saved_ = time_end;
    return delta;
  }
  
private:
  TimePoint time_saved_; ///< TimePoint variable for storage of timedifference.
  bool      pause_; ///< A boolean switch for simple pause functionality.
};

MFL_END

#endif//MFL_CLOCK_INCLUDED
