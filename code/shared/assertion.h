#include <stdlib.h> // exit
#include <stdio.h> // printf

inline void log(const char *type, const char *message, const char *location) {
	printf("%s: %s\n---> %s\n", type, message, location);	
}

inline void assert_true(bool statement_value, const char *message, const char *location) {
	if (!statement_value) {
		log("ASSERT", message, location);
		exit(EXIT_FAILURE);
	}
}

#define DEBUG_STRINGIFY_VALUE(VALUE) #VALUE
#define DEBUG_STRINGIFY_MACRO(MACRO) DEBUG_STRINGIFY_VALUE(MACRO)
#define DEBUG_LOCATION __FILE__ ":" DEBUG_STRINGIFY_MACRO(__LINE__)

#define LOG_ERROR(MESSAGE) log("ERROR", MESSAGE, DEBUG_LOCATION)
#define LOG_WARNING(MESSAGE) log("WARNING", MESSAGE, DEBUG_LOCATION)
#define LOG_TRACE(MESSAGE) log("TRACE", MESSAGE, DEBUG_LOCATION)

#define ASSERT_TRUE(STATEMENT, MESSAGE) assert_true(STATEMENT, MESSAGE, DEBUG_LOCATION)
