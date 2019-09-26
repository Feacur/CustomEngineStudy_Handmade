static uint8 const checksum[8] = {'r', 'a', 'y', 't', 'r', 'a', 'c', 'e'};

//
//
//

struct Game_Data {
	uint8 checksum[8];
	uint32 random_state;
};
