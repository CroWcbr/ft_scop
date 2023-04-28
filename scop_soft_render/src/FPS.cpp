#include "FPS.hpp"
#include <iostream>

std::chrono::time_point<std::chrono::system_clock>	FPS::start_time;
std::chrono::time_point<std::chrono::system_clock>	FPS::end_time;
double												FPS::frame_time;
double												FPS::total_frame_time;
int													FPS::frame_count;

void	FPS::start()
{
	start_time = std::chrono::high_resolution_clock::now();
}

void	FPS::end()
{
	end_time =  std::chrono::high_resolution_clock::now();
}

void	FPS::calculate_fps()
{
	double frame_time = std::chrono::duration<double>(end_time - start_time).count();
	total_frame_time += frame_time;
	frame_count++;
	if (frame_count == NUM_FRAMES_TO_AVERAGE) 
	{
		double fps = frame_count / total_frame_time;
		std::cout << "FPS: " << fps << std::endl;
		frame_count = 0;
		total_frame_time = 0.0;
	}
}
