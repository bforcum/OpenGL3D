#include <cmath>

#define PI 3.141593f

struct mat4 {
	float entries[16];
};

struct vec3 {
	float entries[3];
};

mat4 create_matrix_translation(vec3 translation);

mat4 create_z_rotation(float angle);

mat4 create_model_transform(vec3 translation, float angle);

mat4 create_view_matrix(vec3 from, vec3 to);

mat4 create_perspective_projection(float fovy, float aspect, float near, float far);

float dot(vec3 u, vec3 v);

vec3 normalize(vec3 v);

vec3 cross(vec3 u, vec3 v);