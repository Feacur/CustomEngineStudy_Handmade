static uint8 const checksum[8] = {'p', 'a', 't', 'h', 't', 'r', 'a', 'c'};

//
//
//

struct Hit_Data {
	int32 index;
	float distance;
};

struct Material {
	enum struct Type : uint8 {
		Diffuse,
		Metal,
		Dielectric,
	};

	Type type;
	Vector3 albedo;
	Vector3 emission;
	float roughness;
	float refractive_index;
	
	Material & set_albedo(Vector3 value);
	Material & set_emission(Vector3 value);
	Material & set_roughness(float value);
	Material & set_refractive_index(float value);
};
UNDERLYING_TYPE_META(Material::Type, uint8)
IS_ENUM_META(Material::Type)

struct Shape {
	enum struct Type : uint8
	{
		Sphere,
		Aabb,
	};

	Type type;
	union {
		Aabb3   aabb;
		Vector4 sphere;
	};
};
UNDERLYING_TYPE_META(Shape::Type, uint8)
IS_ENUM_META(Shape::Type)

struct Game_Data {
	uint8 checksum[8];
	uint32 random_state;
	Vector3 camera_position;
	Quaternion camera_rotation;
	Array_Dynamic<Shape> shapes;
	Array_Dynamic<Material> materials;
};

//
//
//

Material mat0(Material::Type type) { return { type, 0 }; }
Material & Material::set_albedo(Vector3 value) { albedo = value; return *this; }
Material & Material::set_emission(Vector3 value) { emission = value; return *this; }
Material & Material::set_roughness(float value) { roughness = value; return *this; }
Material & Material::set_refractive_index(float value) { refractive_index = value; return *this; }

Shape aabb(Aabb3 value) { Shape s = {Shape::Type::Aabb, 0}; s.aabb = value; return s; }
Shape sphere(Vector4 value) { Shape s = {Shape::Type::Sphere, 0}; s.sphere = value; return s; }

//
// funcitons
//

namespace camera {
	void reset_exposure(Game_Data * game_data) {
		if (!input::get_current(Pointer_Keys::Key1)) {
			return;
		}

		globals::render_buffer_f.exposure = 0;
		clear_buffer(globals::render_buffer_f, {0, 0, 0, 0});
	}

	void process_input_rotation(Game_Data * game_data) {
		if (!input::get_current(Pointer_Keys::Key1)) {
			return;
		}

		Vector2i pointer_delta = input::get_pointer_delta();
		game_data->camera_rotation = quaternion_multiply(
			game_data->camera_rotation,
			quaternion_from_radians(
				vec3((float)-pointer_delta.y, (float)pointer_delta.x, 0) * deg_to_rad
			)
		);
	}

	void process_input_position(Game_Data * game_data) {
		if (!input::get_current(Pointer_Keys::Key1)) {
			return;
		}

		float delta_time = globals::get_delta_seconds();

		float speed   = input::get_current(Keyboard_Keys::Shift) * 3 + 1.0f;
		
		float left    = input::get_current(Keyboard_Keys::A) * speed;
		float right   = input::get_current(Keyboard_Keys::D) * speed;
		float forward = input::get_current(Keyboard_Keys::W) * speed;
		float back    = input::get_current(Keyboard_Keys::S) * speed;
		float down    = input::get_current(Keyboard_Keys::Q) * speed;
		float up      = input::get_current(Keyboard_Keys::E) * speed;

		Vector3 offset = {right - left, up - down, forward - back};
		offset = quaternion_rotate_vector(game_data->camera_rotation, offset);
		game_data->camera_position = game_data->camera_position + offset * delta_time;
	}
}
