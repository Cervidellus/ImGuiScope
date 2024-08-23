#ifndef IMGUI_SCOPE_HPP
#define IMGUI_SCOPE_HPP

#include "TimerResult.hpp"
#include "TimerResultBuffer.hpp"

#include <map>
#include <memory>
#include <string>

namespace ImGuiScope
{
	void drawResultsHeader(std::string headerTitle);

	void putResult(const std::string& timerName, const TimerResult& result);

	//Right now, Timescope allocates things when not needed if the timer is not active.
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

	namespace Internal
	{
		static std::map<std::string, std::unique_ptr<TimerResultBuffer>> timerMap;

		//for setting time scale from ImGui combo box
		constexpr const char* timeBufferSizeNames[4] =
		{ "1 second", "10 seconds" , "1 minute"  ,"10 minutes" };

		//for setting units from ImGui combo box
		constexpr  const char* unitNames[3] =
		{ "microseconds", "milliseconds", "seconds" };
	}
}

#endif //IMGUI_SCOPE_HPP
