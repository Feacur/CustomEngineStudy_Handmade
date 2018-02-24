GLOBAL_VAR LARGE_INTEGER ticks_per_second;
GLOBAL_VAR LARGE_INTEGER frame_start_clock;

// TODO: look at <chrono>

void initialize_time() {
	QueryPerformanceFrequency(&ticks_per_second);
	QueryPerformanceCounter(&frame_start_clock);
}

float get_clock_span_seconds(LARGE_INTEGER start, LARGE_INTEGER end) {
	return (float)(end.QuadPart - start.QuadPart) / (float)ticks_per_second.QuadPart;
}

float wait_for_end_of_frame(float target_delta) {
	LARGE_INTEGER current_clock;
	QueryPerformanceCounter(&current_clock);
	float delta = get_clock_span_seconds(frame_start_clock, current_clock);
	if (delta < target_delta) {
		DWORD sleep_millis = (DWORD)((target_delta - delta) * 1000.f);
		Sleep(sleep_millis);
		
		while (delta < target_delta) {
			QueryPerformanceCounter(&current_clock);
			delta = get_clock_span_seconds(frame_start_clock, current_clock);
		}
	}
	
	frame_start_clock = current_clock;
	return min(delta, target_delta);
}
