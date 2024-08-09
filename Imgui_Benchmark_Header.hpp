#ifndef IMGUI_BENCHMARK_HEADER_HPP
#define IMGUI_BENCHMARK_HEADER_HPP

#include <functional>
#include <string>
#include <vector>

//FOR NOW THIS STUFF IS IN SCOPED_TIMER.hpp



//What I intend to do here is to have a singleton that will gather any benchmarking timers that are created.
//You will be able to call a method that will draw an ImGui header with the benchmarking information.

//There will be static methods to create scoped timers that will automatically add themselves to the singleton.

//We'll store the last result, as well as the average over 10 100 and 1000 frames.
//Or maybe by time rather than frames? so I could display last, 1 second avg and ten second average 1 minute...
// //best to have it only store what you need... so if you have 1 second selected it does not store 1 minute of data. 
//You'll be able to select which average you want to display.

//I will also have it so that you can use a macro to compile with or without the benchmarking code.

//TODO::
//First create a data structure to hold the benchmarking information.
//It needs to be able to access timers by string name.
//timers will update this data structure with their results.
//THe data structure will store an array of the last 1000 frames... so it should be a circular buffer.

//Other behavior I want to have: 
//When the header is closed, benchmarking data collection is stopped.
//I could also have subheaders that would start and stop data collection? 



class ImGuiBenchmarkHeader
{

private:
	ImGuiBenchmarkHeader() = default;
	~ImGuiBenchmarkHeader() = default;

public:

	void drawHeader();

private:
	//Do I want a map or vector? When I draw, I want to draw a sorted list of timer results.
	//So maybe I want a vector of a struct that contains a string and a ciruclar buffer of timer results.
	//The struct could have a callback to update the result array.

	

	struct TimerData{
		std::string name = "";
		bool headerOpen = false;
		std::function<void(double result)> benchmarkCallback;


	};

	std::vector<TimerData> results;


}



#endif //IMGUI_BENCHMARK_HEADER_HPP
