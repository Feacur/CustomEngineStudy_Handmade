inline LPVOID allocate_memory(mem_i size_in_bytes) {
	return VirtualAlloc(
		0,                        // LPVOID lpAddress
		(SIZE_T)size_in_bytes,    // SIZE_T dwSize
		MEM_RESERVE | MEM_COMMIT, // DWORD  flAllocationType
		PAGE_READWRITE            // DWORD  flProtect
	);
}

inline void free_memory(LPVOID memory) {
	VirtualFree(
		memory,     // LPVOID lpAddress
		0,          // SIZE_T dwSize
		MEM_RELEASE // DWORD  dwFreeType
	);
}

PLATFORM_READ_FILE(read_file) {
	Memory_Chunk result = {};
	
	HANDLE file = CreateFileA(file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(file == INVALID_HANDLE_VALUE) {
		LOG_ERROR("Can't open file");
		return result;
	}
	
	LARGE_INTEGER file_size;
	if(!GetFileSizeEx(file, &file_size)) {
		LOG_ERROR("Can't get file size");
		CloseHandle(file);
		return result;
	}
	
	DWORD dword_file_size = (DWORD)file_size.QuadPart;
	result = ALLOCATE_BYTES(memory, dword_file_size);
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
