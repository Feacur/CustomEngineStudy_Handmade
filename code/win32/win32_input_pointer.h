GLOBAL_VARIABLE Input_Pointer input_pointer;

Vector2i get_pointer_window(HWND window) {
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(window, &point);
	
	RECT client_rect;
	GetClientRect(window, &client_rect);
	auto client_height = client_rect.bottom;

	return {point.x, client_height - (point.y + 1)};
}

inline void pointer_update_previous_state() {
	memcpy(input_pointer.was_pressed, input_pointer.is_pressed, POINTER_KEYS_BYTES);
	// or as a plain code
	// for	(int32 i = 0; i < POINTER_KEYS_BYTES; ++i) {
	// 	input_pointer.was_pressed[i] = input_pointer.is_pressed[i];
	// }
}

inline void pointer_set_key(Pointer_Keys key, bool is_pressed) {
	input_pointer.is_pressed[(int32)key] = is_pressed;
}

void process_pointer(HWND window) {
	pointer_update_previous_state();

	input_pointer.previous_position = input_pointer.current_position;
	input_pointer.current_position = window_to_buffer(
		get_pointer_window(window)
	);
	
	pointer_set_key(Pointer_Keys::Left,   get_bit_at_index(GetKeyState(VK_LBUTTON), 15));
	pointer_set_key(Pointer_Keys::Middle, get_bit_at_index(GetKeyState(VK_MBUTTON), 15));
	pointer_set_key(Pointer_Keys::Right,  get_bit_at_index(GetKeyState(VK_RBUTTON), 15));
	
	// ClipCursor(&RECT {ltx, lty, rbx, rby});
	// SetCursor(HCURSOR);
	// SetCursorPos(x, y);
}
