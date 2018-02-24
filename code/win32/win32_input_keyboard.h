GLOBAL_VAR Input_Keyboard input_keyboard;

inline bool keyboard_set_key(
	Keyboard_Keys base,
	bool is_pressed,
	WPARAM virtual_key_code,
	WPARAM min_code,
	WPARAM max_code
) {
	if (virtual_key_code < min_code) { return false; }
	if (virtual_key_code > max_code) { return false; }

	auto offset = virtual_key_code - min_code;
	int32 index = (int32)base + offset;
	input_keyboard.is_pressed[index] = is_pressed;
	return true;
}
#define KEYBOARD_SET_RANGE(VALUE_BASE, MIN, MAX)\
if (keyboard_set_key(Keyboard_Keys::VALUE_BASE, is_pressed, virtual_key_code, MIN, MAX))

#define KEYBOARD_TEST_RANGE(VALUE_BASE, MIN, MAX) KEYBOARD_SET_RANGE(VALUE_BASE, MIN, MAX) { return; }

inline bool keyboard_set_key(
	Keyboard_Keys key,
	bool is_pressed,
	WPARAM virtual_key_code,
	WPARAM expected_code
) {
	if (virtual_key_code != expected_code) { return false; }

	input_keyboard.is_pressed[(int32)key] = is_pressed;
	return true;
}
#define KEYBOARD_SET_VALUE(VALUE, EXPECTED)\
keyboard_set_key(Keyboard_Keys::VALUE, is_pressed, virtual_key_code, EXPECTED)

#define KEYBOARD_TEST_VALUE(VALUE, EXPECTED) if (KEYBOARD_SET_VALUE(VALUE, EXPECTED)) { return; }

inline void keyboard_set_key(WPARAM virtual_key_code, bool is_pressed) {
	KEYBOARD_TEST_RANGE(N1,      '0', '9')
	KEYBOARD_TEST_RANGE(F1,      VK_F1, VK_F12)
	KEYBOARD_TEST_RANGE(A,       'A', 'Z')

	KEYBOARD_TEST_VALUE(Left,    VK_LEFT)
	KEYBOARD_TEST_VALUE(Right,   VK_RIGHT)
	KEYBOARD_TEST_VALUE(Down,    VK_DOWN)
	KEYBOARD_TEST_VALUE(Up,      VK_UP)
	
	KEYBOARD_TEST_VALUE(Escape,  VK_ESCAPE)
	KEYBOARD_TEST_VALUE(Enter,   VK_RETURN)
	KEYBOARD_TEST_VALUE(Control, VK_CONTROL)
	KEYBOARD_TEST_VALUE(Shift,   VK_SHIFT)
	KEYBOARD_TEST_VALUE(Alt,     VK_MENU)
	KEYBOARD_TEST_VALUE(Space,   VK_SPACE)
	KEYBOARD_TEST_VALUE(Tab,     VK_TAB)
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
