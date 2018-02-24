enum struct Input_Recording_State : int32 {
	Stop   = 0,
	Record = 1,
	Play   = 2,
};

GLOBAL_VAR Input_Recording_State input_recording_state = Input_Recording_State::Stop;
GLOBAL_CONST    char                  *state_file_name      = "StateSnapshot.blob";
GLOBAL_CONST    char                  *input_file_name      = "InputRecord.blob";

void save_state(Memory_Chunk *permanent_memory) {
	HANDLE file = CreateFileA(state_file_name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if (file != INVALID_HANDLE_VALUE) {
		DWORD bytes_written;
		WriteFile(file, permanent_memory->data, (DWORD)permanent_memory->size, &bytes_written, 0);
		CloseHandle(file);
	}
}

void load_state(Memory_Chunk *permanent_memory) {
	HANDLE file = CreateFileA(state_file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (file != INVALID_HANDLE_VALUE) {
		DWORD bytes_read;
		ReadFile(file, permanent_memory->data, (DWORD)permanent_memory->size, &bytes_read, 0);
		CloseHandle(file);
	}
}

void process_input_recording(Memory_Chunk *permanent_memory, int32 input) {
	PERSISTENT_LOCAL_VAR Input_Recording_State previous_state    = Input_Recording_State::Stop;
	PERSISTENT_LOCAL_VAR HANDLE                input_file        = {};
	PERSISTENT_LOCAL_CONST    DWORD                 input_memory_size = sizeof(int32);
	
	// MARK: stop
	if ((previous_state != input_recording_state) && (previous_state != Input_Recording_State::Stop)) {
		CloseHandle(input_file);
	}
	
	// MARK: record
	if (input_recording_state == Input_Recording_State::Record) {
		if (previous_state != Input_Recording_State::Record) {
			save_state(permanent_memory);
			input_file = CreateFileA(input_file_name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		}
		
		DWORD bytes_written;
		WriteFile(input_file, &input, input_memory_size, &bytes_written, 0);
	}

	// MARK: play
	if (input_recording_state == Input_Recording_State::Play) {
		if (previous_state != Input_Recording_State::Play) {
			load_state(permanent_memory);
			input_file = CreateFileA(input_file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		}
		
		DWORD bytes_read;
		BOOL read_result = ReadFile(input_file, &input, input_memory_size, &bytes_read, 0);
	
		if (read_result && (input_memory_size != bytes_read)) {
			CloseHandle(input_file);
			load_state(permanent_memory);
			input_file = CreateFileA(input_file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		}
	}
	
	previous_state = input_recording_state;
}
