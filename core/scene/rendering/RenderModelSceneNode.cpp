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

void RenderModelSceneNode::PreRender(const FindQuery& query, RenderBlockResultSet* resultSet)
{
	const typemask filter = query.filter;
	if (BIT_ISSET(filter, FindQueryFilter::STATIC_SHADOW_CASTER) || BIT_ISSET(filter, FindQueryFilter::DYNAMIC_SHADOW_CASTER)) {
		if (BIT_ISSET(filter, FindQueryFilter::STATIC_AND_DYNAMIC_SHADOW_CASTER)) {
			if (!IsShadowCaster()) {
				return;
			}
		}
		else if (BIT_ISSET(filter, FindQueryFilter::STATIC_SHADOW_CASTER)) {
			if (!mStaticShadowCaster || mDynamicShadowCaster) {
				return;
			}
		}
		else if (BIT_ISSET(filter, FindQueryFilter::DYNAMIC_SHADOW_CASTER)) {
			if (!mDynamicShadowCaster || mStaticShadowCaster) {
				return;
			}
		}
	}
	
	const uint32 size = mModel->GetNumMeshes();
	const ModelMesh* meshes = mModel->GetMeshes();
	for (uint32 i = 0; i < size; ++i) {
		const ModelMesh& mesh = meshes[i];
		// TODO Calculate a much better ID instead of just using the vertex buffer id
		uint32 id = mesh.vertexBuffer->GetUID();
		RenderBlock* block = resultSet->Create(id);
		block->modelMatrix = mModelMatrix;
		if (BIT_ISSET(filter, FindQueryFilter::GEOMETRY)) {
			block->vertexBuffer = mesh.vertexBuffer;
			block->indexBuffer = mesh.indexBuffer;
		}
		if (BIT_ISSET(filter, FindQueryFilter::TEXTURES)) {
			block->diffuseTexture = mesh.diffuseTexture.Get();
		}
		block->diffuseColor = mesh.diffuseColor;
		block->startIndex = 0;
		block->count = mesh.vertexBuffer->GetNumVertices();
	}
}
