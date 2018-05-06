GLOBAL_VAR thread_local uint32 RANDOM_SEED;

inline void random_set_seed(uint32 value) {
	RANDOM_SEED = value;
}

inline uint32 random_uint32() {
	auto x = (RANDOM_SEED + 1u);
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 15;
	RANDOM_SEED = x;
	return x;
}

inline float random_01() {
	return (float)random_uint32() / UINT32_MAX;
}

inline float random_in_range(float min, float max) {
	return interpolate(min, max, random_01());
}

inline int32 random_in_range(int32 min, int32 max) {
	return min + (int32)(random_uint32() % (uint32)(max - min));
}
