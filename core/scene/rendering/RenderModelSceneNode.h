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

	class RenderModelSceneNode : public Renderable
	{
		DECLARE_SCRIPT_OBJECT(RenderModelSceneNode);

	public:
		RenderModelSceneNode();
		virtual ~RenderModelSceneNode();
		
		/*!
			\brief Set the model to be drawn by this node

			\param model
		*/
		void SetModel(Resource<Model> model);

	public:
		virtual void PreRender(const FindQuery& query, RenderBlockResultSet* resultSet);

	private:
		Resource<Model> mModel;

	};
}
