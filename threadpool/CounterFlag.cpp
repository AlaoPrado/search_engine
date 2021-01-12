#include "CounterFlag.hpp"

namespace search_engine {

CounterFlag::CounterFlag(int taskLimit) {
  pthread_mutex_init(&this->mutex, 0);
  pthread_cond_init(&this->cond, 0);
  tasksDone = 0;
  this->taskLimit = taskLimit;
}

CounterFlag::~CounterFlag() {
  pthread_mutex_destroy(&this->mutex);
  pthread_cond_destroy(&this->cond);
}

void CounterFlag::reset(int taskLimit) {
  this->tasksDone = 0;
  this->taskLimit = taskLimit;
}

void CounterFlag::increaseTaskNum(int increaseAmount) {
  pthread_mutex_lock(&this->mutex);
  taskLimit += increaseAmount;
  pthread_mutex_unlock(&this->mutex);
}

void CounterFlag::wait() {
  pthread_mutex_lock(&this->mutex);
  if (this->tasksDone < this->taskLimit){
    pthread_cond_wait(&cond, &mutex);
  }
  pthread_mutex_unlock(&this->mutex);
}

void CounterFlag::signal() {
  pthread_mutex_lock(&this->mutex);
  this->tasksDone++;
  if (this->tasksDone == this->taskLimit) {
    pthread_cond_signal(&this->cond);
  }
  pthread_mutex_unlock(&this->mutex);
}

} // namespace search_engine
