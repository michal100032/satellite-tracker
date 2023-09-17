#include <cmath>
#include <glm/vec3.hpp>

extern const float PI = 4.0f * atanf(1.0f);
extern const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

extern const int WINDOW_WIDTH = 1000;
extern const int WINDOW_HEIGHT = 800;
extern const char* WINDOW_TITLE = "Satellite tracker";

extern const float CAM_MIN_DIST = 1.5f;
extern const float CAM_INIT_DIST = 5.0f;

extern const float CAM_ZOOM_SPEED = 0.5f;
extern const float CAM_DRAG_SPEED = 0.01f;

extern const float CAM_FOV = PI / 4;
extern const float CAM_NEAR_PLANE = 0.01f;
extern const float CAM_FAR_PLANE = 100.0f;