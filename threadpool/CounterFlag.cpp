/* Copyright 2021 Alan Prado Araújo and Igor Henrique Firmo Santos

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
Authors: alanpradoaraujo@gmail.com (Alan Prado Araújo) and
         igorfirmo5@gmail.com (Igor Henrique Firmo Santos) */

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
