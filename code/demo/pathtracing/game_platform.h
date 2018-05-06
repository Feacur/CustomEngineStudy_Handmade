GLOBAL_VAR Memory_Chunk *permanent_memory;
GLOBAL_VAR Memory_Chunk *transient_memory;

GLOBAL_VAR Input_Keyboard *input_keyboard;

GLOBAL_VAR RGBA_Data image;

//
// funcitons
//

namespace input {
	inline bool get_current(Keyboard_Keys key) {
		return keyboard_get_current_state(input_keyboard, key);
	}

	inline bool get_pressed(Keyboard_Keys key) {
		return keyboard_get_transition_pressed(input_keyboard, key);
	}

	inline bool get_released(Keyboard_Keys key) {
		return keyboard_get_transition_released(input_keyboard, key);
	}
}
