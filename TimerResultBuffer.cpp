#include "TimerResultBuffer.hpp"

TimerResultBuffer::TimerResultBuffer(
	const std::chrono::microseconds& maxTime,
	const int& initSize) :
	maxTime_(maxTime),
	ringBuffer_(std::vector<TimerResult>(initSize, { std::chrono::high_resolution_clock::time_point(), std::chrono::microseconds(0) }))
{
}

const std::array<const char*, 4> TimerResultBuffer::timeBufferSizeNames  = { "1 second", "10 seconds" , "1 minute"  ,"10 minutes" };

const std::array<const char*, 3> TimerResultBuffer::unitNames = { "microseconds", "milliseconds", "seconds" };

void TimerResultBuffer::setMaxTime(const std::chrono::microseconds& maxTime) { maxTime_ = maxTime; }

void TimerResultBuffer::put(const TimerResult& result)
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

bool TimerResultBuffer::isEmpty() { return (head_ == tail_); }

void TimerResultBuffer::cullExpired() {
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

void TimerResultBuffer::expandBuffer()
{
	int originalBufferSize = ringBuffer_.size();
	ringBuffer_.insert(ringBuffer_.begin() + tail_, originalBufferSize, TimerResult());
	if (head_ >= tail_) head_ += originalBufferSize;
	tail_ += originalBufferSize;
}

int TimerResultBuffer::count() {
	cullExpired();
	if (head_ >= tail_) return head_ - tail_;
	else return ringBuffer_.size() - tail_ + head_;
}

int TimerResultBuffer::last() {
	if (head_ == tail_) return 0;
	if (head_ > 0) return ringBuffer_[head_ - 1].elapsed.count();
	return ringBuffer_.back().elapsed.count();
}

float TimerResultBuffer::mean()
{
	return (float)sum_.count() / count();
}

std::chrono::nanoseconds TimerResultBuffer::sum()
{
	cullExpired();
	return sum_;
}