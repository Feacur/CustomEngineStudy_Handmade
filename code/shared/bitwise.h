#define SHARED_BITWISE

#if !defined(SHARED_CODE)
	#error include "shared/code.h"
#endif

#if defined(__cplusplus)
	#define BIT(T, index) static_cast<T>(static_cast<T>(1) << index)
#else
	#define BIT(T, index) (T)((T)1 << index)
#endif

//
//
//

#define BITS_ARE_SET(container, bits)\
bits_are_set_impl(container, (decltype(container))bits)

#define BITS_ARE_SET_IMPL(T)\
constexpr inline bool bits_are_set_impl(T container, T bits) {\
	return (container & bits) == bits;\
}

BITS_ARE_SET_IMPL(int8)
BITS_ARE_SET_IMPL(int16)
BITS_ARE_SET_IMPL(int32)
BITS_ARE_SET_IMPL(int64)
BITS_ARE_SET_IMPL(uint8)
BITS_ARE_SET_IMPL(uint16)
BITS_ARE_SET_IMPL(uint32)
BITS_ARE_SET_IMPL(uint64)

BITS_ARE_SET_IMPL(int48)
BITS_ARE_SET_IMPL(uint48)

#undef BITS_ARE_SET_IMPL

//
//
//

#define GET_BIT_AT_INDEX(T)\
constexpr inline bool get_bit_at_index(T container, uint8 index) {\
	ASSERT_TRUE(index < 8 * sizeof(T), "Bit index overflow");\
	return bits_are_set_impl(container, BIT(T, index));\
}

GET_BIT_AT_INDEX(int8)
GET_BIT_AT_INDEX(int16)
GET_BIT_AT_INDEX(int32)
GET_BIT_AT_INDEX(int64)
GET_BIT_AT_INDEX(uint8)
GET_BIT_AT_INDEX(uint16)
GET_BIT_AT_INDEX(uint32)
GET_BIT_AT_INDEX(uint64)

GET_BIT_AT_INDEX(int48)
GET_BIT_AT_INDEX(uint48)

#undef GET_BIT_AT_INDEX

//
//
//

#if defined(__cplusplus) // enum struct
template<typename T>
constexpr inline T flag_set(T container, T bits) {
	if constexpr (!meta::is_enum<T>::value) { return T; }
	using U = meta::underlying_type<T>::type;
	return static_cast<T>(static_cast<U>(container) | static_cast<U>(bits));
}

template<typename T>
constexpr inline T flag_intersect(T container, T bits) {
	if constexpr (!meta::is_enum<T>::value) { return T; }
	using U = meta::underlying_type<T>::type;
	return static_cast<T>(static_cast<U>(container) & static_cast<U>(bits));
}

template<typename T>
constexpr inline T flag_negate(T value) {
	if constexpr (!meta::is_enum<T>::value) { return T; }
	using U = meta::underlying_type<T>::type;
	return static_cast<T>(~static_cast<U>(value));
}

template<typename T>
constexpr inline T flag_remove(T container, T bits) {
	if constexpr (!meta::is_enum<T>::value) { return T; }
	using U = meta::underlying_type<T>::type;
	return static_cast<T>(static_cast<U>(container) & ~static_cast<U>(bits));
}

template<typename T>
constexpr inline bool flag_has(T container, T bits) {
	if constexpr (!meta::is_enum<T>::value) { return T; }
	using U = meta::underlying_type<T>::type;
	return BITS_ARE_SET(static_cast<U>(container), static_cast<U>(bits));
}

template<typename T>
constexpr inline T operator|(T container, T bits) {
	return flag_set(container, bits);
}

template<typename T>
constexpr inline T operator&(T container, T bits) {
	return flag_intersect(container, bits);
}

template<typename T>
constexpr inline T operator~(T value) {
	return flag_negate(value);
}
#endif // defined(__cplusplus) // enum struct
