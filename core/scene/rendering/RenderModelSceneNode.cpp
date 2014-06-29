#include "../../Memory.h"
#include "RenderModelSceneNode.h"
using namespace core;

RenderModelSceneNode::RenderModelSceneNode()
: Renderable()
{
}

RenderModelSceneNode::~RenderModelSceneNode()
{
}
//
//Updatable* RenderModelComponent::GetUpdatable()
//{
//	return mModel->GetNumFrames() > 0 ? this : nullptr;
//}
//
//Renderable* RenderModelComponent::GetRenderable()
//{
//	return this;
//}
//
//void RenderModelSceneNode::Update()
//{
//}

void RenderModelSceneNode::SetModel(Resource<Model> model)
{
	mModel = model;
	SetBoundingBox(mModel->GetBoundingBox());
}

void RenderModelSceneNode::PreRender(const FindQuery& state, RenderBlockResultSet* resultSet)
{
	const uint32 size = mModel->GetNumMeshes();
	const ModelMesh* meshes = mModel->GetMeshes();
	for (uint32 i = 0; i < size; ++i) {
		const ModelMesh& mesh = meshes[i];
		RenderBlock* block = resultSet->Create(0);
		block->modelMatrix = GetModelMatrix();
		if (BIT_ISSET(state.filter, RenderableFilter::GEOMETRY)) {
			block->vertexBuffer = mesh.vertexBuffer;
			block->indexBuffer = mesh.indexBuffer;
		}
		if (BIT_ISSET(state.filter, RenderableFilter::TEXTURES)) {
			block->diffuseTexture = mesh.diffuseTexture.Get();
		}
		block->diffuseColor = mesh.diffuseColor;
		block->startIndex = 0;
		block->count = mesh.vertexBuffer->GetNumVertices();
	}

	if (BIT_ISSET(state.filter, RenderableFilter::DEBUG_INFO)) {
		/*
		auto geometryBuilder = resultSet->GetGeometryBuilder();
		geometryBuider->AddBox(aabb);
		geometryBuilder->AddText(GetSceneNode()->GetNodeID());
		*/
	}
}
