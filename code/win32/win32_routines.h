inline LPVOID allocate_memory(mem_i size_in_bytes) {
	// LPVOID lpAddress        //
	// SIZE_T dwSize           //
	// DWORD  flAllocationType //
	// DWORD  flProtect        //
	return VirtualAlloc(0, (SIZE_T)size_in_bytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

inline void free_memory(LPVOID memory) {
	// LPVOID lpAddress, //
	// SIZE_T dwSize,    //
	// DWORD  dwFreeType //
	VirtualFree(memory, 0, MEM_RELEASE);
}

FILETIME get_file_time(const char *file_name) {
	FILETIME result = {};
	
	WIN32_FIND_DATA find_data;
	HANDLE file = FindFirstFileA(file_name, &find_data);
	if (file != INVALID_HANDLE_VALUE) {
		result = find_data.ftLastWriteTime;
		FindClose(file);
	}
	
	return result;
}

PLATFORM_READ_FILE(read_file) {
	Memory_Chunk result = {};
	
	HANDLE file = CreateFileA(file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(file == INVALID_HANDLE_VALUE) {
		return result;
	}
	
	LARGE_INTEGER file_size;
	if(!GetFileSizeEx(file, &file_size)) {
		CloseHandle(file);
		return result;
	}
	
	DWORD dword_file_size = (DWORD)file_size.QuadPart;
	result = allocate_memory_chunk(memory, dword_file_size);
	if (result.data == 0) {
		CloseHandle(file);
		return result;
	}
	
	DWORD bytes_read;
	if(ReadFile(file, result.data, dword_file_size, &bytes_read, 0)) {
		result.used = bytes_read;
	}
	
	CloseHandle(file);
	return result;
}
