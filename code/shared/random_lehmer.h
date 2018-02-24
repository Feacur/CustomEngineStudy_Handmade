GLOBAL_VAR uint32 RANDOM_SEED;
GLOBAL_CONST uint32 RANDOM_SEED_MULTIPLIER = 48271u;
GLOBAL_CONST uint32 RANDOM_SEED_MODULO = UINT32_MAX;

inline void random_set_seed(uint32 value) {
	RANDOM_SEED = value;
}

inline uint32 random_uint32() {
	RANDOM_SEED = ((RANDOM_SEED + 1u) * RANDOM_SEED_MULTIPLIER) % RANDOM_SEED_MODULO;
	return RANDOM_SEED;
}

inline float random_01() {
	return (float)random_uint32() / RANDOM_SEED_MODULO;
}

inline float random_in_range(float min, float max) {
	return interpolate(min, max, random_01());
}

inline int32 random_in_range(int32 min, int32 max) {
	return min + (int32)(random_uint32() % (uint32)(max - min));
}
