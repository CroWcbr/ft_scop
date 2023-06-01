#pragma once

#include <chrono>

class FPS
{
private:
	static const int NUM_FRAMES_TO_AVERAGE = 10;
	static std::chrono::time_point<std::chrono::system_clock>	start_time;
	static std::chrono::time_point<std::chrono::system_clock>	end_time;
	static double												frame_time;
	static double												total_frame_time;
	static int													frame_count;

public:
	static void	start();
	static void	end();
	static void calculate_fps();

};
