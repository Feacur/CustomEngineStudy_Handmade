GLOBAL_VARIABLE Input_Keyboard input_keyboard;

inline bool keyboard_set_key(
	WPARAM virtual_key_code,
	WPARAM min_code,
	WPARAM max_code,
	Keyboard_Keys base,
	bool is_pressed) {

	if ((virtual_key_code >= min_code) && (virtual_key_code <= max_code)) {
		auto offset = virtual_key_code - min_code;
		int32 index = (int32)base + offset;
		input_keyboard.is_pressed[index] = is_pressed;
		return true;
	}

	return false;
}

inline void keyboard_set_key(Keyboard_Keys key, bool is_pressed) {
	input_keyboard.is_pressed[(int32)key] = is_pressed;
}

inline void keyboard_set_key(WPARAM virtual_key_code, bool is_pressed) {
	// numbers
	if (keyboard_set_key(virtual_key_code, '0', '9', Keyboard_Keys::N1, is_pressed)) {
		return;
	}

	// functional
	if (keyboard_set_key(virtual_key_code, VK_F1, VK_F12, Keyboard_Keys::F1, is_pressed)) {
		return;
	}
	
	// letters
	if (keyboard_set_key(virtual_key_code, 'A', 'Z', Keyboard_Keys::A, is_pressed)) {
		return;
	}

	// arrows
	if (virtual_key_code == VK_LEFT) {
		keyboard_set_key(Keyboard_Keys::Left, is_pressed);
	}
	else if (virtual_key_code == VK_RIGHT) {
		keyboard_set_key(Keyboard_Keys::Right, is_pressed);
	}
	else if (virtual_key_code == VK_DOWN) {
		keyboard_set_key(Keyboard_Keys::Down, is_pressed);
	}
	else if (virtual_key_code == VK_UP) {
		keyboard_set_key(Keyboard_Keys::Up, is_pressed);
	}
	// commands
	else if (virtual_key_code == VK_ESCAPE) {
		keyboard_set_key(Keyboard_Keys::Escape, is_pressed);
	}
	else if (virtual_key_code == VK_RETURN) {
		keyboard_set_key(Keyboard_Keys::Enter, is_pressed);
	}
	else if (virtual_key_code == VK_CONTROL) {
		keyboard_set_key(Keyboard_Keys::Control, is_pressed);
	}
	else if (virtual_key_code == VK_SHIFT) {
		keyboard_set_key(Keyboard_Keys::Shift, is_pressed);
	}
	else if (virtual_key_code == VK_MENU) {
		keyboard_set_key(Keyboard_Keys::Alt, is_pressed);
	}
	else if (virtual_key_code == VK_SPACE) {
		keyboard_set_key(Keyboard_Keys::Space, is_pressed);
	}
	else if (virtual_key_code == VK_TAB) {
		keyboard_set_key(Keyboard_Keys::Tab, is_pressed);
	}
}

inline void keyboard_reset_state() {
	memset(input_keyboard.was_pressed, 0, KEYBOARD_KEYS_BYTES);
	memset(input_keyboard.is_pressed, 0, KEYBOARD_KEYS_BYTES);
	// or as a plain code
	// for	(int32 i = 0; i < KEYBOARD_KEYS_NUMBER; ++i) {
	// 	input_keyboard.was_pressed[i] = false;
	// }
	// for	(int32 i = 0; i < KEYBOARD_KEYS_NUMBER; ++i) {
	// 	input_keyboard.is_pressed[i] = false;
	// }
}

inline void keyboard_update_previous_state() {
	memcpy(input_keyboard.was_pressed, input_keyboard.is_pressed, KEYBOARD_KEYS_BYTES);
	// or as a plain code
	// for	(int32 i = 0; i < KEYBOARD_KEYS_NUMBER; ++i) {
	// 	input_keyboard.was_pressed[i] = input_keyboard.is_pressed[i];
	// }
}
