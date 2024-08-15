#ifndef IMGUI_SCOPE_HPP
#define IMGUI_SCOPE_HPP

#include "TimerResult.hpp"
#include "TimerResultBuffer.hpp"

//I need the timer results buffer, as well as an intermediate object that creates the buffer for any timer that is created.
//TimerResultManager will be a static object that will be created when the first timer is created.
//It will create buffers for any named timer, and provide the results. 
//It will keep track of if a timer is on or off, based primarily on if the header is open or closed.
//I can keep track of that in a TimerInfo struct that has a bool for if it is on or off, as well as the circular buffer of results.

#include <map>
#include <memory>
#include <string>

namespace ImGuiScope
{
	namespace Internal
	{
		static std::map<std::string, std::unique_ptr<TimerResultBuffer>> timerMap;
	}

	void drawResultsHeader(std::string headerTitle);

	void putResult(const std::string& timerName, const TimerResult& result);


	//Right now, it allocates things when not needed if the timer is not active.
	// It is very small overhead, but
	//At some point, I should move to a wrapper class that checks if active to avoid this
	class TimeScope {
	public:
		TimeScope(
			const std::string& timerName = "Timer",
			const bool& consoleOutput = true
		);
		~TimeScope();
	private:
		const std::chrono::high_resolution_clock::time_point startTime_ = std::chrono::high_resolution_clock::now();
		const std::string timerName_;
		const bool consoleOutput_;
	};
}

#endif //IMGUI_SCOPE_HPP
