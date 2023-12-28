#pragma once

#include "Renderable.h"
#include <queue>

class RenderManager
{
public:
	RenderManager() {}
	~RenderManager() {}

	static RenderManager* GetInstance();

	void Render();
	void ClearRenderBuffer();

	void PushToRenderQueue(IRenderable* renderable);
	void ClearRenderQueue();

protected:
	std::queue<IRenderable*> RenderQueue;
};
