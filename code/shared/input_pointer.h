enum struct Pointer_Keys : int32 {
	Left,
	Middle,
	Right,
};

GLOBAL_CONST int32 POINTER_KEYS_NUMBER = 3;

struct Input_Pointer {
	Vector2i previous_position;
	Vector2i current_position;
	bool was_pressed[POINTER_KEYS_NUMBER];
	bool is_pressed[POINTER_KEYS_NUMBER];
};

GLOBAL_CONST int32 POINTER_KEYS_BYTES = sizeof(Input_Pointer::is_pressed);

inline bool pointer_get_previous_state(Input_Pointer *pointer, Pointer_Keys key) {
	return pointer->was_pressed[(int32)key];
}

inline bool pointer_get_current_state(Input_Pointer *pointer, Pointer_Keys key) {
	return pointer->is_pressed[(int32)key];
}

inline bool pointer_get_transition_pressed(Input_Pointer *pointer, Pointer_Keys key) {
	bool was_pressed = pointer_get_previous_state(pointer, key);
	bool is_pressed = pointer_get_current_state(pointer, key);
	return is_pressed && (is_pressed != was_pressed);
}

inline bool pointer_get_transition_released(Input_Pointer *pointer, Pointer_Keys key) {
	bool was_pressed = pointer_get_previous_state(pointer, key);
	bool is_pressed = pointer_get_current_state(pointer, key);
	return !is_pressed && (is_pressed != was_pressed);
}
