#pragma once

#include "Renderable.h"
#include "Core/RCTexture.h"

#include <vector>
#include <map>
#include <memory>

#define USE_RENDER_LATENCY() 0

class RenderManager
{
public:
	RenderManager();
	~RenderManager() {}

	static RenderManager& GetInstance();

	void Initialize();

	void Render();
	void ReconstructRenderBuffer();

	bool PushToRenderQueue(IRenderable* renderable);
	void ClearRenderQueue();

protected:
	void DrawRenderBuffer();
	void SwapRenderBuffers();

protected:
	std::map<RenderPriorityGroup, std::vector<IRenderable*>> RenderGroupQueue;

private:
	#if USE_RENDER_LATENCY()
	bool bReadyToRender = true;
	#endif // USE_RENDER_LATENCY()

	std::shared_ptr<RCTexture> CurrentRenderBuffer;
	std::shared_ptr<RCTexture> SecondRenderBuffer;
};
