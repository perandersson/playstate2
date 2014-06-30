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
		// TODO Calculate a much better ID instead of just using the vertex buffer id
		uint32 id = mesh.vertexBuffer->GetUID();
		RenderBlock* block = resultSet->Create(id);
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
