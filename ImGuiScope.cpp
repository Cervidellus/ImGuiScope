#include "ImGuiScope.hpp"

#include <imgui.h>

#include <iostream>

void ImGuiScope::putResult(const std::string& timerName, const TimerResult& result)
{
	if (!ImGuiScope::Internal::timerMap.contains(timerName))
	{
		ImGuiScope::Internal::timerMap.emplace(timerName, std::make_unique<TimerResultBuffer>());
	}
	ImGuiScope::Internal::timerMap[timerName]->put(result);
}

void ImGuiScope::drawResultsHeader(std::string headerTitle)
{
	if (ImGui::CollapsingHeader(headerTitle.c_str())) 
	{
		ImGui::Indent();
		for (const auto& [timerName, timerBuffer] : ImGuiScope::Internal::timerMap)
		{
			if (ImGui::CollapsingHeader(timerName.c_str()))
			{
				ImGui::Value("Count", timerBuffer->count());
				if (!timerBuffer->isActive) timerBuffer->isActive = true;
			}
			else if (timerBuffer->isActive) timerBuffer->isActive = false;
		}
		ImGui::Unindent();
	}
}

ImGuiScope::TimeScope::TimeScope(const std::string& timerName, const bool& consoleOutput) :
	timerName_(timerName),
	consoleOutput_(consoleOutput)
{
	if (!ImGuiScope::Internal::timerMap.contains(timerName_))
	{
		ImGuiScope::Internal::timerMap.emplace(timerName, std::make_unique<TimerResultBuffer>());
		//ImGuiScope::Internal::timerMap.emplace(timerName_, std::make_unique<TimerResultBuffer()>);
	}
}

ImGuiScope::TimeScope::~TimeScope()
{
	if (!ImGuiScope::Internal::timerMap[timerName_]->isActive) return;

	TimerResult result
	{
		startTime_,
		std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime_)
	};

	ImGuiScope::putResult(timerName_, result);

	if (consoleOutput_) std::cout << std::format("Timer {} timed out with {} microseconds.\n", timerName_, result.elapsed);
}

	