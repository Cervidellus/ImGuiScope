#include "TimerResultBuffer.hpp"

TimerResultBuffer::TimerResultBuffer() 
{
}

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
		) > maxTimeMicroSeconds_()
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

float TimerResultBuffer::last() {
	if (head_ == tail_) return 0;
	if (head_ > 0) return microsecondsToSelectedUnit_(ringBuffer_[head_ - 1].elapsed.count());
	return microsecondsToSelectedUnit_(ringBuffer_.back().elapsed.count());
}

float TimerResultBuffer::mean()
{
	return microsecondsToSelectedUnit_((float)sum_.count() / count());
}

std::chrono::nanoseconds TimerResultBuffer::sum()
{
	cullExpired();
	return sum_;
}

std::chrono::microseconds TimerResultBuffer::maxTimeMicroSeconds_()
{
	//Expects value in microseconds.
	switch (timeBufferSizeIndex)
	{
	case 0:
		return std::chrono::microseconds(1000000);//1 second
	case 1:
		return std::chrono::microseconds(10000000);//10 seconds
	case 2:
		return std::chrono::microseconds(600000000);//1 minute
	case 3:
		return std::chrono::microseconds(6000000000);//10 minutes
	default:
		return std::chrono::microseconds(1000000);//1 second
	}
}

float TimerResultBuffer::microsecondsToSelectedUnit_(const float& microseconds)
{
	//{ "microseconds", "milliseconds", "seconds" };
	switch (unitNameIndex)
	{
	case 0:
		return microseconds;
	case 1:
		return microseconds * .001;//milliseconds
	case 2:
		return microseconds * .000001;//seconds
	default:
		return microseconds;
	}
}