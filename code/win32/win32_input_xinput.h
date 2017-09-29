#include <xinput.h> // obvious

#define XINPUT_GET_STATE(ROUTINE_NAME) DWORD WINAPI ROUTINE_NAME(DWORD dwUserIndex, XINPUT_STATE *pState)
#define XINPUT_SET_STATE(ROUTINE_NAME) DWORD WINAPI ROUTINE_NAME(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)

typedef XINPUT_GET_STATE(xinput_get_state_type);
typedef XINPUT_SET_STATE(xinput_set_state_type);

XINPUT_GET_STATE(xinput_get_state_stub) { return ERROR_DEVICE_NOT_CONNECTED; }
XINPUT_SET_STATE(xinput_set_state_stub) { return ERROR_DEVICE_NOT_CONNECTED; }

GLOBAL_VARIABLE xinput_get_state_type *xinput_get_state;
GLOBAL_VARIABLE xinput_set_state_type *xinput_set_state;

#define LOAD_PROCEDURE(ROUTINE_NAME)\
if (library) {\
	ROUTINE_NAME = (ROUTINE_NAME##_type *)GetProcAddress(library, #ROUTINE_NAME);\
}\
if (!ROUTINE_NAME) {\
	ROUTINE_NAME = ROUTINE_NAME##_stub;\
}

void load_xinput()
{
	HMODULE library = 0;
	const char *libraries[] = {"xinput1_4.dll", "xinput9_1_0.dll", "xinput1_3.dll"};
	const int32 elements_in_libraries = sizeof(libraries);
	for (int32 i = 0; i < elements_in_libraries; ++i) {
		library = LoadLibraryA(libraries[i]);
		if (library) { break; }
	}
	
	LOAD_PROCEDURE(xinput_get_state)
	LOAD_PROCEDURE(xinput_set_state)
}

#undef LOAD_PROCEDURE

void process_xinput_example() {
	for (DWORD controller_index = 0; controller_index < XUSER_MAX_COUNT; ++controller_index) {
		XINPUT_STATE controller_state;
		bool is_connected = (xinput_get_state(controller_index, &controller_state) == ERROR_SUCCESS);
		if (!is_connected) { continue; }
		
		XINPUT_GAMEPAD gamepad = controller_state.Gamepad;
		
		bool dpad_up        = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_DPAD_UP);
		bool dpad_down      = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_DPAD_DOWN);
		bool dpad_left      = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_DPAD_LEFT);
		bool dpad_right     = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_DPAD_RIGHT);
		bool start          = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_START);
		bool back           = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_BACK);
		bool left_thumb     = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_LEFT_THUMB);
		bool right_thumb    = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_RIGHT_THUMB);
		bool left_shoulder  = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER);
		bool right_shoulder = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER);
		bool button_1       = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_A);
		bool button_2       = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_B);
		bool button_3       = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_X);
		bool button_4       = bits_are_set(gamepad.wButtons, XINPUT_GAMEPAD_Y);
		
		// XINPUT_GAMEPAD_TRIGGER_THRESHOLD
		float trigger0 = uint8_to_fraction(gamepad.bLeftTrigger);
		float trigger1 = uint8_to_fraction(gamepad.bRightTrigger);
		
		// XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		// XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
		float thumb0x = int16_to_fraction(gamepad.sThumbLX);
		float thumb0y = int16_to_fraction(gamepad.sThumbLY);
		float thumb1x = int16_to_fraction(gamepad.sThumbRX);
		float thumb1y = int16_to_fraction(gamepad.sThumbRY);
	}
}
