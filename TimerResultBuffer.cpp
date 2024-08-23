#include "TimerResultBuffer.hpp"

TimerResultBuffer::TimerResultBuffer(
	const std::chrono::microseconds& maxTime = std::chrono::microseconds(10000000),
	const int& initSize = 100) :
	maxTime_(maxTime),
	ringBuffer_(std::vector<TimerResult>(initSize, { std::chrono::high_resolution_clock::time_point(), std::chrono::microseconds(0) }))
{
}

void setMaxTime(const std::chrono::microseconds& maxTime) { maxTime_ = maxTime; }

void put(const TimerResult& result)
{
	sum_ += result.elapsed;
	ringBuffer_[head_] = result;
	int nextHead_ = (head_ + 1) % ringBuffer_.size();
	if (nextHead_ == tail_)
	{
		//First cull. If that doesn't work, resize and reset the head and tail.
		cullExpired();
		if (nextHead_ == tail_) expandBuffer();
	}
	head_ = nextHead_;
}

bool isEmpty() { return (head_ == tail_); }

void cullExpired() {
	while (
		std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::high_resolution_clock::now() - ringBuffer_[tail_].start
		) > maxTime_
		&& tail_ != head_)
	{
		sum_ -= ringBuffer_[tail_].elapsed;
		tail_ = (tail_ + 1) % ringBuffer_.size();
	}
}

void expandBuffer()
{
	int originalBufferSize = ringBuffer_.size();
	ringBuffer_.insert(ringBuffer_.begin() + tail_, originalBufferSize, TimerResult());
	if (head_ >= tail_) head_ += originalBufferSize;
	tail_ += originalBufferSize;
}

int count() {
	cullExpired();
	if (head_ >= tail_) return head_ - tail_;
	else return ringBuffer_.size() - tail_ + head_;
}

int last() {
	if (head_ == tail_) return 0;
	if (head_ > 0) return ringBuffer_[head_ - 1].elapsed.count();
	return ringBuffer_.back().elapsed.count();
}

float mean()
{
	return (float)sum_.count() / count();
}

std::chrono::nanoseconds sum()
{
	cullExpired();
	return sum_;
}