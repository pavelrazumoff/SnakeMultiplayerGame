#include "RenderManager.h"
#include "Draw/DrawConsoleLibrary.h"

RenderManager* RenderManager::GetInstance()
{
	static RenderManager instance;
	return &instance;
}

void RenderManager::Render()
{
	while (!RenderQueue.empty())
	{
		RenderQueue.front()->Render();
		RenderQueue.pop();
	}
}

void RenderManager::ClearRenderBuffer()
{
	DrawConsoleLibrary::ClearScreen();
}

void RenderManager::PushToRenderQueue(IRenderable* renderable)
{
	RenderQueue.push(renderable);
}

void RenderManager::ClearRenderQueue()
{
	while (!RenderQueue.empty())
	{
		RenderQueue.pop();
	}
}
