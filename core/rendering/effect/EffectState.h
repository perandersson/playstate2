#pragma once
#include "../../typedefs.h"
#include "UniformProperty.h"

namespace core
{
	class DefaultUniform;
	class IUniform;
	class Effect; 
	class RenderState;
	class SamplerObject;

	/*!
		\brief Class symbolizing the state of the effect bound to the current thread
	*/
	class EffectState
	{
		typedef std::hash_map<std::string, DefaultUniform*> Uniforms;

	public:
		EffectState(const Effect* effect, RenderState* renderState);
		~EffectState();

		/*!
			\brief Apply this states uniforms
		*/
		void ApplyUniforms();

		/*!
			\brief Retrieves a uniform based on it's name in one of the shaders.

			\param name
						The name of the uniform variable
			\name The uniform if found; A pointer to an UniformNotFound instance if no component is found.
		*/
		IUniform* FindUniform(const char* name);
		
		/*!
			\brief Retrieves a uniform based on it's name in one of the shaders.

			\param name
						The name of the uniform variable
			\name The uniform if found; A pointer to an UniformNotFound instance if no component is found.
		*/
		IUniform* FindUniform(const std::string& name);

	private:
		SamplerObject* GenSamplerObject(std::shared_ptr<UniformProperty> property);

	private:
		const Effect* mEffect;
		Uniforms mUniforms;
	};
}
