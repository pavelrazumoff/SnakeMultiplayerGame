#include "RenderManager.h"

#include "Core/RenderTextureLibrary.h"
#include "Core/RenderConsoleLibrary.h"
#include "Draw/DrawConsoleLibrary.h"

#include <chrono>

RenderManager::RenderManager()
{
}

RenderManager* RenderManager::GetInstance()
{
	static RenderManager instance;
	return &instance;
}

void RenderManager::Initialize()
{
	#if USE_VIRTUAL_TERMINAL_PROCESSING()
	RenderConsoleLibrary::EnableVirtualTerminalProcessing();

	RenderConsoleLibrary::ShowCursor(false);
	#endif // USE_VIRTUAL_TERMINAL_PROCESSING()

	ReconstructRenderBuffer();
}

void RenderManager::Render()
{
	#if USE_RENDER_LATENCY()
	{
		using namespace std::chrono;
		static auto RenderStart = high_resolution_clock::now();
		const float RenderUpdateFreq = 0.05f;

		auto Now = high_resolution_clock::now();
		float TimePassed = duration_cast<duration<float>>(Now - RenderStart).count();

		if (TimePassed < RenderUpdateFreq) { bReadyToRender = false; return; }

		RenderStart = Now;
		bReadyToRender = true;
	}
	#endif // USE_RENDER_LATENCY()

	while (!RenderQueue.empty())
	{
		RenderQueue.front()->Render(SecondRenderBuffer.get());
		RenderQueue.pop();
	}

	DrawRenderBuffer();
	SwapRenderBuffers();
}

void RenderManager::DrawRenderBuffer()
{
	DrawConsoleLibrary::DrawTextureDifference(SecondRenderBuffer.get(), CurrentRenderBuffer.get(), 0, 0);
	RenderConsoleLibrary::RestoreConsoleRenderMode();
}

void RenderManager::SwapRenderBuffers()
{
	std::shared_ptr<RCTexture> temp = CurrentRenderBuffer;
	CurrentRenderBuffer = SecondRenderBuffer;
	SecondRenderBuffer = temp;

	RenderTextureLibrary::FillTexture(SecondRenderBuffer.get(), RenderConstants::ClearPixel);
}

void RenderManager::ReconstructRenderBuffer()
{
	ClearRenderQueue();

	RC_SIZE bufferDimension = RenderConsoleLibrary::GetConsoleDimensions();

	CurrentRenderBuffer = std::make_shared<RCTexture>(bufferDimension.cx, bufferDimension.cy);
	CurrentRenderBuffer->SetAllowAlphaPixelWrite(false);
	RenderTextureLibrary::FillTexture(CurrentRenderBuffer.get(), RenderConstants::ClearPixel);

	SecondRenderBuffer = std::make_shared<RCTexture>(bufferDimension.cx, bufferDimension.cy);
	SecondRenderBuffer->SetAllowAlphaPixelWrite(false);
	RenderTextureLibrary::FillTexture(SecondRenderBuffer.get(), RenderConstants::ClearPixel);

	DrawConsoleLibrary::DrawTexture(CurrentRenderBuffer.get(), 0, 0);
}

bool RenderManager::PushToRenderQueue(IRenderable* renderable)
{
	#if USE_RENDER_LATENCY()
	if (!bReadyToRender) return false;
	#endif // USE_RENDER_LATENCY()

	RenderQueue.push(renderable);
	return true;
}

void RenderManager::ClearRenderQueue()
{
	while (!RenderQueue.empty())
	{
		RenderQueue.pop();
	}
}
