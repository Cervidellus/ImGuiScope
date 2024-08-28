**About:**

ImGuiScope is a simple tool to quickly provide benchmarking in an application that already makes use of Dear ImGui. 

It provides:  
-Named scoped timers.  
-Internal ring buffers for each timer that hold 10 seconds of results.  
-Dear ImGui collapsing header widgets to display results.   

**Dependencies**
-Requires Dear ImGui: https://github.com/ocornut/imgui

**Usage:**
To time a scope, create a named ImGuiScope::TimeScope object at the top of the scope.  
It must be named to ensure that it's lifetime spans the entire scope.

```
{
auto timer = ImGuiScope::TimeScope("process events");
processEvents();
}
```
If you don't want to put something inside a scope you can also use it with c++17 std::optional:
```
auto timer = std::make_optional<ImGuiScope::TimeScope>("process events");
processEvents();
timer.reset();
```

To see the results, call ImGuiScope::drawResultsHeader somewhere that an ImGui frame is being drawn.
Timers are only active when the individual collapsing header for that timer is open. 
```
ImGuiScope::drawResultsHeader("Timer Results");
```

![image](https://github.com/user-attachments/assets/e0351a7a-8372-49f8-bbc8-90198d1a19b0)



