#pragma once
#include "Renderable.h"
#include "../Updatable.h"
#include "../../model/Model.h"

namespace core
{
	struct AnimationChangedEvent : public Event
	{
		std::string animationName;

		AnimationChangedEvent() : Event() {}
		virtual ~AnimationChangedEvent() {}
	};

	class RenderModelSceneNode : public Renderable/*, public Updatable*/
	{
		DECLARE_SCRIPT_OBJECT(RenderModelSceneNode);

	public:
		RenderModelSceneNode();
		virtual ~RenderModelSceneNode();

		//virtual Updatable* GetUpdatable();
		//virtual Renderable* GetRenderable();

		/*!
			\brief Set the model to be drawn by this node

			\param model
		*/
		void SetModel(Resource<Model> model);

	public:
		//
		// Perform's a pre-render stage
		virtual void PreRender(const FindQuery& state, RenderBlockResultSet* resultSet);

	//public:
		//virtual void Update();

	private:
		Resource<Model> mModel;

	};
}
