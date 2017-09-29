#include <stdlib.h> // srand, rand

inline void random_set_seed(uint32 value) {
	srand(value);
}

inline float random_01() {
	return (float)rand() / RAND_MAX;
}

inline float random_in_range(float min, float max) {
	return interpolate(min, max, random_01());
}

inline int32 random_in_range(int32 min, int32 max) {
	return min + rand() % (max - min);
}
