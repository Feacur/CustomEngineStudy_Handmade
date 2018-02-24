#include "shared/code_types.h"
#include "shared/code_visibility.h"
#include "shared/assertion.h"
#include "shared/memory_chunk.h"
#include "shared/collection_array.h"

#include "shared/math_scalar.h"
#include "shared/math_linear.h"
#include "shared/math_linear_i.h"

#include "shared/platform_api.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // obvious
#include "win32/win32_routines.h"
#include "win32/win32_gdi.h"
// #include "win32/win32_dsound.h"
#include "win32/win32_game_code_loader.h"
#include "win32/win32_input_xinput.h"
#include "win32/win32_input_keyboard.h"
#include "win32/win32_input_pointer.h"
#include "win32/win32_input_recording.h"
#include "win32/win32_time.h"

GLOBAL_VAR Platform_Data platform_data;
GLOBAL_VAR Game_Code     game_code;

//
// Forward declared routines
//
void             process_messages(HWND window);
LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
void             toggle_full_screen_mode(HWND window);

//
// Routines
//

int main(int argc, char* argv[]) {
	HINSTANCE hInstance = GetModuleHandle(0);
// 	// ShowWindow(GetConsoleWindow(), SW_HIDE);
// 	// ShowWindow(GetConsoleWindow(), SW_SHOW);
// int CALLBACK WinMain(
// 	HINSTANCE hInstance,     // Application instance handler.
// 	HINSTANCE hPrevInstance, // Application previous instance handler. Always 0.
// 	LPSTR     lpCmdLine,     // The command line arguments without program name.
// 	int       nCmdShow       // Window visibility options.
// ) {
	// AllocConsole();
	// FreeConsole();
	// OutputDebugStringA("Debug message.");
	
	//
	// Register the window class
	//
	
	WNDCLASS window_class = {};
	window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc   = window_procedure;
	window_class.hInstance     = hInstance;
	window_class.hIcon         = 0;
	window_class.hCursor       = LoadCursor(0, IDC_ARROW);
	window_class.lpszClassName = "Elementary window class";

	ATOM window_class_atom = RegisterClass(&window_class);
	ASSERT_TRUE(window_class_atom, "Can't register the window class");
	
	//
	// Create a window
	//
	
	HWND window = CreateWindowEx(
		0,
		window_class.lpszClassName, "Elementary",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0,
		hInstance,
		0
	);
	ASSERT_TRUE(window, "Can't create a window");
	
	HDC device_context = GetDC(window);

	//
	// Allocate memory
	//
	
	platform_data.permanent_memory.size = 100 * 1024 * 1024;
	platform_data.permanent_memory.data = (uint8 *)allocate_memory(platform_data.permanent_memory.size);
	ASSERT_TRUE(platform_data.permanent_memory.data, "Can't allocate permanent memory");
	
	platform_data.transient_memory.size = 100 * 1024 * 1024;
	platform_data.transient_memory.data = (uint8 *)allocate_memory(platform_data.transient_memory.size);
	ASSERT_TRUE(platform_data.transient_memory.data, "Can't allocate transient memory");
	
	RECT default_client_rect;
	GetClientRect(window, &default_client_rect);
	platform_data.size_target = {
		default_client_rect.right, default_client_rect.bottom
	};
	
	LOG_TRACE("Allocated general memory");
	
	//
	// Initialize performance counters
	//
	
	initialize_time();
	
	int32 monitor_hz                = (int32)GetDeviceCaps(device_context, VREFRESH);
	platform_data.time.target_delta = 1.f / ((monitor_hz > 1) ? monitor_hz : 60);
	platform_data.time.delta        = platform_data.time.target_delta;
	
	LOG_TRACE("Initialized performance counters");
	
	//
	// Process input
	//
	
	load_xinput();
	
	LOG_TRACE("Started main cycle");

	game_code = {};
	platform_data.keep_alive = true;
	while(platform_data.keep_alive) {
		// init
		reload_game_code(&game_code);
		platform_data.read_file = &read_file;

		reinit_render_buffer(window, platform_data.size_target);
		platform_data.render_buffer_image = render_buffer.image;
		
		#if DSOUND_ENABLED
		reinit_sound_buffer(window);
		play_sound_buffer();
		platform_data.sound_buffer_sound = sound_buffer.sound;
		#endif
		
		// input
		process_messages(window);
		platform_data.input_keyboard = input_keyboard;

		process_pointer(window);
		platform_data.input_pointer = input_pointer;

		process_xinput_example();
		
		// logic
		process_input_recording(&platform_data.permanent_memory, 0);
		game_code.game_update(&platform_data);
		
		// rendering
		game_code.game_render(&platform_data);
		display_render_buffer(device_context);
		
		// sound
		#if DSOUND_ENABLED
		DWORD play_cursor;
		DWORD write_cursor;
		HRESULT result_GetCurrentPosition = sound_buffer.secondary_buffer->GetCurrentPosition(&play_cursor, &write_cursor);
		if (result_GetCurrentPosition == DS_OK) {
			int32 bytes_for_samples = sound_buffer.sound.channels * sound_buffer.bytes_per_sample;
			int32 bytes_for_second  = bytes_for_samples * sound_buffer.sound.samples_per_second;
			int32 bytes_for_buffer  = (int32)(bytes_for_second * sound_buffer.buffer_length_in_seconds);
			
			float seconds_ahead = 4 * platform_data.time.target_delta;
			int32 bytes_ahead   = (int32)(bytes_for_second * seconds_ahead);
			
			DWORD end_at_byte    = (write_cursor + bytes_ahead) % bytes_for_buffer;
			DWORD bytes_to_write = (
				bytes_for_buffer - sound_buffer.running_sample_byte + end_at_byte
			) % bytes_for_buffer;
		
			game_code.game_output_sound(&platform_data, bytes_to_write / bytes_for_samples);
			fill_sound_buffer(bytes_to_write);
		}
		#endif

		// idle
		platform_data.time.delta = wait_for_end_of_frame(platform_data.time.target_delta);
	}
	
	ReleaseDC(window, device_context);
	
	LOG_TRACE("Finished running");
	return EXIT_SUCCESS;
}

void process_messages(HWND window) {
	keyboard_update_previous_state();

	MSG message;
	while(PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
		switch(message.message) {
			case WM_QUIT: {
				// Indicates a request to terminate an application.
				platform_data.keep_alive = false;
			} break;
			
			case WM_SYSKEYUP:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_KEYDOWN: {
				WPARAM virtual_key_code = message.wParam;
				
				bool is_an_extended_key = get_bit_at_index(message.lParam, 24);
				bool alt_key_is_pressed = get_bit_at_index(message.lParam, 29);
				bool was_pressed        = get_bit_at_index(message.lParam, 30);
				bool is_released        = get_bit_at_index(message.lParam, 31);
				
				bool is_transition      = (was_pressed == is_released);
				
				keyboard_set_key(virtual_key_code, !is_released);				
			} break;
			
			default: {
				TranslateMessage(&message);
				DispatchMessage(&message);
			} break;
		}	
	}
}

LRESULT CALLBACK window_procedure(
	HWND   window,
	UINT   message,
	WPARAM wParam,
	LPARAM lParam
) {
	PERSISTENT_LOCAL_VAR bool window_is_minimized = false;
	PERSISTENT_LOCAL_VAR bool window_is_maximized = false;
	PERSISTENT_LOCAL_VAR bool window_has_focus    = false;
	PERSISTENT_LOCAL_VAR bool inner_area_focus    = false;
	
	switch (message) {
		case WM_SETFOCUS: {
			// Sent to a window after it has gained the keyboard focus.
			window_has_focus = true;
			// An application should return zero if it processes this message.
			return 0;
		} break;
		
		case WM_KILLFOCUS: {
			// Sent to a window immediately before it loses the keyboard focus.
			window_has_focus = false;
			keyboard_reset_state();
			// An application should return zero if it processes this message.
			return 0;
		} break;
		
		case WM_SIZE: {
			// Sent to a window after its size has changed.
			if (wParam == SIZE_MINIMIZED) {
				window_is_minimized = true;
				window_is_maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED) {
				window_is_minimized = false;
				window_is_maximized = true;
				make_render_buffer_fit(window);
			}
			else if (wParam == SIZE_RESTORED) {
				if (window_is_minimized) {
					window_is_minimized = false;
				}
				else if (window_is_maximized) {
					window_is_maximized = false;
					make_render_buffer_fit(window);
				}
			}
			// If an application processes this message, it should return zero.
			return 0;
		} break;
		
		case WM_SIZING: {
			// Sent to a window after its size has changed.
			make_render_buffer_fit(window);
			RedrawWindow(window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			// If an application processes this message, it should return zero.
			return TRUE;
		}
		
		case WM_EXITSIZEMOVE: {
			// Sent one time to a window, after it has exited the moving or sizing modal loop.
			make_render_buffer_fit(window);
			// An application should return zero if it processes this message.
			return 0;
		} break;
		
		case WM_SETCURSOR: {
			// Sent to a window if the mouse causes the cursor to move within a window and mouse input is not capture
			bool in_the_client_area = (LOWORD(lParam) == HTCLIENT);
			if (window_has_focus && in_the_client_area) {
				// If an application processes this message, it should return TRUE to halt further processing or FALSE to continue.
				// return TRUE;
			}
			// return FALSE;
		} break;
		
		case WM_PAINT: {
			// Is sent when the system or another application makes a request to paint a portion of an application's window.
			RECT client_rect;
			bool has_update_region = GetUpdateRect(window, &client_rect, false);
			
			if (has_update_region) {
				PAINTSTRUCT paint;
				HDC device_context = BeginPaint(window, &paint);
			
				display_blackness(device_context);
				display_render_buffer(device_context);

				EndPaint(window, &paint);
			}
			
			// An application returns zero if it processes this message.
			return 0;
		} break;
		
		case WM_CLOSE: {
			// Sent as a signal that a window or an application should terminate.
			platform_data.keep_alive = false;
			// Go to WM_DESTROY
			// DestroyWindow(window);
			// If an application processes this message, it should return zero.
			return 0;
		} break;
		
		case WM_DESTROY: {
			// Sent when a window is being destroyed. It is sent to the window procedure of the window being destroyed after the window is removed from the screen.
			platform_data.keep_alive = false;
			// Go to WM_QUIT
			// PostQuitMessage(0);
			// If an application processes this message, it should return zero.
			return 0;
		} break;
	}
	
	return DefWindowProc(window, message, wParam, lParam);
}

void toggle_full_screen_mode(HWND window) {
	// INITIALLY BY Raymond Chen
	// FROM http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx
	PERSISTENT_LOCAL_VAR WINDOWPLACEMENT normal_window_position;
	
	DWORD window_style = GetWindowLong(window, GWL_STYLE);
	
	bool window_is_of_normal_style = ((window_style & WS_OVERLAPPEDWINDOW) == WS_OVERLAPPEDWINDOW);
	if(window_is_of_normal_style)
	{
		// Go to windowed fullscreen mode if can remember window placement and get monitor info.
		MONITORINFO monitor_info = {};
		monitor_info.cbSize = sizeof(MONITORINFO);
		if(GetWindowPlacement(window, &normal_window_position) && GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &monitor_info))
		{
			SetWindowLong(window, GWL_STYLE, window_style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(
				window, HWND_TOP,
				monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
				monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
				monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED
			);
		}
	}
	else
	{
		// Restore windowed mode.
		SetWindowLong(window, GWL_STYLE, window_style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(window, &normal_window_position);
		SetWindowPos(
			window, 0,
			0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED
		);
	}
	
	make_render_buffer_fit(window);
}
