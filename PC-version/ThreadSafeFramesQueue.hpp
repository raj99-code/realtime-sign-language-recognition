/** @file ThreadSafeFramesQueue.hpp
 *  @author Joe Dinius, Ph.D
 *  @date 28 Nov. 2020
 *
 *  @brief generic thread-safe queue (FIFO) implementation (with timer)
 *
 *  @see https://docs.opencv.org/3.4/d4/db9/samples_2dnn_2object_detection_8cpp-example.html#_a20
 */
#pragma once
//! c/c++ headers
#include <limits>
#include <queue>
#include <mutex>
#include <thread>
//! system headers
#include <opencv2/core/utility.hpp>
//! project headers

/**
 * @class ThreadSafeFramesQueue
 *
 * @brief generic thread-safe queue (FIFO) implementation (with timer)
 *
 * @tparam T, container type
 */
template <typename T>
class ThreadSafeFramesQueue : public std::queue<T> {
public:
  /**
   * Default constructor
   */
  ThreadSafeFramesQueue()
  : maxDepth(5) {
    tm.reset();
    tm.start();
  }
  
  /**
   * Constructor taking max depth as argument
   */
  explicit ThreadSafeFramesQueue(size_t const& _maxDepth)
  : maxDepth(_maxDepth) {
    tm.reset();
    tm.start();
  }

  /**
   * push element onto the end of the queue
   */
  void Push(const T& entry) noexcept {
    std::lock_guard<std::mutex> lock(mtx);
    //! remove elements that are too old
    while (this->size() >= maxDepth) {
      T elem;
      static_cast<void>(Pop(elem));
    }
    this->push(entry);
  }

  /**
   * pop element from the front of the queue
   */
  bool Pop(T& out) noexcept {
    std::lock_guard<std::mutex> lock(mtx);
    //! if the queue is empty, just exit
    if (this->empty()) {
      return false;
    }
    //! get the first element, and remove it
    out = this->front();
    this->pop();
    //! stop the current timer
    tm.stop();
    //! start the next timer
    tm.start();
    //! return true, indicating successful pop
    return true;
  }

  /**
   * get average frames per second
   * @note measured as frequency of interval between inserts (i.e. Push-es)
   */
  double GetFPS() noexcept {
    std::lock_guard<std::mutex> lock(mtx);
    return tm.getFPS();
  }

  /**
   * empty the queue
   */
  void Clear() noexcept {
    std::lock_guard<std::mutex> lock(mtx);
    while (!this->empty()) {
      this->pop();
    }
  }

private:
  cv::TickMeter tm;  //! OpenCV timer class
  std::mutex mtx;  //! mutex for safe read/write access of/to queue internals
  size_t const maxDepth;  //! max depth (size) of queue
};
