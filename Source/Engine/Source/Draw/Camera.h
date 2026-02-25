// ◦ Xyz ◦
#pragma once

#include <memory>
#include <string>
#include  <exception>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <Log.h>

namespace Engine
{
	class Camera
	{
	public:
		using Ptr = std::shared_ptr<Camera>;

		enum class Type {
			PERSPECTIVE,
			ORTHO
		};

	public:
		Camera();
		Camera(const Type type);
		virtual ~Camera() = default;

		void SetPerspective(float zFar = 1000000.f, float zNear = 1.f, float fov = 45.f);
		void SetOrtho(float size = 1.f, float zNear = -1000000.f, float zFar = 1000000.f);
		void Resize();
		void MakeProjectView();
		const glm::mat4x4& GetMatProjectView() const;

	public:
		static Camera& GetCurrentCameraRef();
		static const float* CurrentMatProjectView();
		static Ptr& GetCamera(const std::string& name);
		static Camera& GetCameraRef(const std::string& name);
		static void SetCurrentCamera(const std::string& name);
		static void SetCurrentCamera(const Ptr& camera);

		template<typename T, typename ...Args>
		static Ptr& MakeCamera(const std::string& name, Args&&... args)
		{
			const auto itPair = cameras.try_emplace(name, std::make_shared<T>(std::forward<Args>(args)...));
			if (!itPair.second) {
				throw std::invalid_argument(TO_STRING("[Camera::MakeCamera] fail by name: {}", name));
			}
			return itPair.first->second;
		}

	protected:
		Type _type = Type::PERSPECTIVE;
		float _fov = 45.f;
		float _zFar;
		float _zNear;

		glm::mat4x4 _matProject = glm::mat4x4(1.f);
		glm::mat4x4 _matView = glm::mat4x4(1.f);
		glm::mat4x4 _matProjectView = glm::mat4x4(1.f);

		glm::vec3 _pos { 0.f, 0.f, 1.f };
		glm::vec3 _direct { 0.f, 0.f, -1.f };
		glm::vec3 _up { 0.f, 1.f, 0.f };
	
	private:
		inline static Camera::Ptr currentCamera;
		inline static std::unordered_map<std::string, Camera::Ptr> cameras;
	};
}
