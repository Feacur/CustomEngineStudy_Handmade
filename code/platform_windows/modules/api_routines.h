PLATFORM_READ_FILE(platform_read_file) {
	HANDLE file = CreateFileA(file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(file == INVALID_HANDLE_VALUE) {
		log_last_error();
		ASSERT_TRUE(false, "Can't open file");
		return 0;
	}
	
	LARGE_INTEGER file_size;
	if(!GetFileSizeEx(file, &file_size)) {
		log_last_error();
		ASSERT_TRUE(false, "Can't get file size");
		CloseHandle(file);
		return 0;
	}
	
	DWORD dword_file_size = (DWORD)file_size.QuadPart;
	uint8 * bytes = allocate_data(memory, dword_file_size);
	if (!bytes) {
		log_last_error();
		CloseHandle(file);
		return 0;
	}
	
	DWORD bytes_read;
	if(!ReadFile(file, bytes, dword_file_size, &bytes_read, 0)) {
		log_last_error();
		ASSERT_TRUE(false, "Can't read file");
		deallocate_from_top(memory, dword_file_size);
		CloseHandle(file);
		return 0;
	}
	
	CloseHandle(file);

	return bytes;
}

PLATFORM_ALLOCATE_MEMORY(platform_allocate_memory) {
	return VirtualAlloc(
		0,                        // LPVOID lpAddress
		(SIZE_T)size_in_bytes,    // SIZE_T dwSize
		MEM_RESERVE | MEM_COMMIT, // DWORD  flAllocationType
		PAGE_READWRITE            // DWORD  flProtect
	);
}

PLATFORM_FREE_MEMORY(platform_free_memory) {
	VirtualFree(
		memory,     // LPVOID lpAddress
		0,          // SIZE_T dwSize
		MEM_RELEASE // DWORD  dwFreeType
	);
}

// HANDLE HeapCreate(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
// BOOL   HeapValidate(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem)
// LPVOID HeapAlloc(HANDLE hHeap, DWORD  dwFlags, SIZE_T dwBytes)
// LPVOID HeapReAlloc(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes)
// BOOL   HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem)
// BOOL   HeapDestroy(HANDLE hHeap)
