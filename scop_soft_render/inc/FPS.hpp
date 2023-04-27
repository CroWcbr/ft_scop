#pragma once

#include <chrono>


class FPS
{
private:
	static const int NUM_FRAMES_TO_AVERAGE = 100;
	static std::chrono::time_point<std::chrono::system_clock> start_time;
	static std::chrono::time_point<std::chrono::system_clock> end_time;
	static double frame_time;
	static int frame_count;
	static double total_frame_time;

public:
	static void	start();
	static void	end();
	static void calculate_fps();

};
