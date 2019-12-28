#ifndef QBuffer_h
#define QBuffer_h

/* 
 * Templated class to act as a buffered queue 
 */
template<typename T>
class QBuffer {
  /* Variables */
  private:
  static const unsigned int QUEUE_SIZE = 10;
  T queue[QUEUE_SIZE];
  int queueIndex = 0;
  bool error;

  /* Methods */
  public:
  QBuffer() {
    queueIndex = 0;
  }

  // Pushes element to queue and sets error flag if full
  void Push(T element) {
    if (!Full()) {
      queueIndex++;
      queue[queueIndex] = element;
    } else {
      error = true;
    }
  }

  // Returns the top most element without removing it
  T Peek() {
    return queue[queueIndex];
  }

  // Removes element from the queue, if empty returns constructor and sets error flag
  T Pop() {
    if (!Empty()) {
      queueIndex--;
      return queue[queueIndex + 1];
    } else {
      error = true;
      return T();
    }
  }

  // Returns queue size
  int Size() {
    return queueIndex + 1;
  }

  // Checks if queue is empty
  bool Empty() {
    return (queueIndex < 0);
  }

  // Checks if queue is full
  bool Full() {
    return (queueIndex >= QUEUE_SIZE - 1);
  }

  // Returns error status
  bool ErrorStatus() {
    return error;
  }

  // Clears queue error status
  bool ClearError() {
    error = false;
  }
};

#endif
