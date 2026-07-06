// ◦ Xyz ◦

#include "SelectBody.h"
#include <Screen.h>
#include <Callback/Callback.h>
#include <Draw/Camera.h>
#include <Logs.h>

namespace {
    struct Ray {
        glm::vec3 origin;
        glm::vec3 direction;
    };

    Ray GetMouseRaySimple()
    {
        float ndcX = (2.0f * Engine::Callback::GetMousePos()[0]) / Engine::ScreenParams::Width() - 1.0f;
        float ndcY = 1.0f - (2.0f * Engine::Callback::GetMousePos()[1]) / Engine::ScreenParams::Height();

        // Точка на дальней плоскости в NDC
        glm::vec4 farPoint_NDC(ndcX, ndcY, 1.0f, 1.0f);

        // Обратная матрица
        auto& camera = Engine::Camera::GetLink();
        glm::mat4 inverseProjView = glm::inverse(camera.ProjectView());

        // Переводим только дальнюю точку в мир
        glm::vec4 farPoint_World = inverseProjView * farPoint_NDC;
        farPoint_World /= farPoint_World.w;

        Ray ray;
        ray.origin = camera.Pos();
        ray.direction = glm::normalize(glm::vec3(farPoint_World) - camera.Pos());

        return ray;
    }

    bool RaySphereIntersectionFast(const Ray& ray, const GravityRender::Body& body, float& t)
    {
        glm::vec3 oc = ray.origin - body.pos;

        // a = 1, так как direction нормализован
        float b = 2.0f * glm::dot(oc, ray.direction);
        float c = glm::dot(oc, oc) - body.diameter * body.diameter;

        float discriminant = b * b - 4.0f * c;

        if (discriminant < 0.0f) return false;

        float sqrtD = sqrt(discriminant);
        float t1 = (-b - sqrtD) * 0.5f; // деление на 2a, где a=1
        float t2 = (-b + sqrtD) * 0.5f;

        if (t1 > 0.0f) {
            t = t1;
        }
        else if (t2 > 0.0f) {
            t = t2;
        }
        else {
            return false;
        }

        return true;
    }
}

size_t SelectBodyIndex(const std::vector<GravityRender::Body>& bodies)
{
	if (bodies.empty()) {
		return std::numeric_limits<size_t>::max();
	}

    const auto ray = GetMouseRaySimple();
    float closestT = std::numeric_limits<float>::max();
    size_t hitBodyIndex = std::numeric_limits<size_t>::max();

    for (size_t index = 0; index < bodies.size(); ++index) {
        float t;
        if (RaySphereIntersectionFast(ray, bodies[index], t)) {
            if (t < closestT) {
                closestT = t;
                hitBodyIndex = index;
            }
        }
    }

    PRINT("hitBodyIndex: {}", hitBodyIndex);
    return hitBodyIndex;
}
