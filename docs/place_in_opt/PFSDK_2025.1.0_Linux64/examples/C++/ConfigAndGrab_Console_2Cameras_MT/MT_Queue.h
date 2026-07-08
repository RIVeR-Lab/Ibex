#ifndef PF_MT_QUEUE_H_
#define PF_MT_QUEUE_H_

#include <queue>
#include <thread>
#include <mutex>

///
 /// @class MT_Queue.h
 ///
 /// This class defines a thread safe lock-based on std::queue template class
 ///
template <typename T>
class MT_Queue
{
public:

  /// @brief Default constructor
  MT_Queue() {}

  /// @brief Copy constructor
  MT_Queue(const MT_Queue &queue) 
  {
    std::lock_guard<std::mutex> lock(queue.m_mutex);
    m_queue = queue.m_queue;
  }

  /// @brief Assignment operator
  MT_Queue& operator=(const MT_Queue&) = delete;
          
  /// 
  /// @brief Pop item from queue
  ///
  /// Unqueues item from front of the Queue and return it. 
  ///
  /// @param[out] item 
  /// @return True if value has popped from Queue false if queue is empty
  bool getItem(T& item)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_queue.empty())
    {
      return false;
    }

    item = std::move(m_queue.front());
    m_queue.pop();
    return true;
  }

  /// 
  /// @brief Push item 
  ///
  /// Enqueue new item
  ///
  /// @param[in] item to be queued 
  void push(const T& item)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(std::move(item));
  }

  /// 
  /// @brief Push item 
  ///
  /// Enqueue new item
  ///
  /// @param[in] item to be queued 
  void push(T&& item)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(std::move(item));
  }


  /// 
  /// @brief Release entire queue
  ///
  /// All items in queue will be freed
  ///
  void release()
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    // Free all items
    while (m_queue.size() > 0)
    {
      m_queue.pop();
    }
  }

  /// 
  /// @brief Check if queue is item
  ///
  /// Verifies if queue has items
  ///
  /// @return true if queue is empty false otherwise
  bool empty() const
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.empty();
  }

  /// 
  /// @brief Get number of items in queue
  ///
  /// @return Size of queue
  size_t getSize() const
  {
    return m_queue.size();
  }


private:
  std::queue<T> m_queue;          // generic queue 
  mutable std::mutex m_mutex;     // mutex
};

#endif