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

//I think I might go back to a namespace, and have the scoped timer in the namespace.
//Maybe I comment out the class and do a namespace so I can change my mind if I want.



//I can have the underlying stored data in another object not exposed in teh ImGuiScope namespace, but what should I call it?

//Could it just be the timermap? Does it even need to be a class? It could be just a public static object. 

//so just static std::map<std::string, TimerResultBuffer> timerMap;

namespace ImGuiScope
{
	//forward declarations
	//class TimeScope;

	namespace Internal
	{
		static std::map<std::string, std::unique_ptr<TimerResultBuffer>> timerMap;
	}

	//static void TimeScope(std::string timerName, bool consoleOutput = false);

	void drawResultsHeader(std::string headerTitle);

	//std::vector<TimerResult> getResults(std::string timerName);
	//Maybe also have one that returns a map of timerName and the vectors of results.

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
