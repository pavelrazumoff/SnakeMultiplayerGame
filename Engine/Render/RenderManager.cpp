#include "RenderManager.h"

#include "Core/RenderTextureLibrary.h"
#include "Core/RenderConsoleLibrary.h"
#include "Draw/DrawConsoleLibrary.h"

#include "Engine/EngineUtility.h"
#include "Engine/Log/Logger.h"

//#include <chrono>

RenderManager::RenderManager()
{
}

RenderManager& RenderManager::GetInstance()
{
	static RenderManager instance;
	return instance;
}

void RenderManager::Initialize()
{
	#if USE_VIRTUAL_TERMINAL_PROCESSING()
	RenderConsoleLibrary::EnableVirtualTerminalProcessing();

	RenderConsoleLibrary::ShowCursor(false);
	#endif // USE_VIRTUAL_TERMINAL_PROCESSING()

	RenderManager::GetInstance().ReserveRenderQueueSpace(RenderPriorityGroup::SceneObject, 100);
	RenderManager::GetInstance().ReserveRenderQueueSpace(RenderPriorityGroup::UI, 10);

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

	//using namespace std::chrono;
	//auto PushStart = high_resolution_clock::now();

	for (auto& renderGroup : RenderGroupQueue)
	{
		for (auto& renderable : renderGroup.second)
		{
			renderable->Render(SecondRenderBuffer.get());
		}
	}

	RenderGroupQueue.clear();

	//auto PushEnd = high_resolution_clock::now();
	//float TimePassed = duration_cast<duration<float>>(PushEnd - PushStart).count();

	DrawRenderBuffer();
	SwapRenderBuffers();

	//std::string logMessage = "RenderManager::Render() - render time: " + std::to_string(TimePassed) + " seconds.";
	//Logger::GetInstance().Write(logMessage.c_str());
}

void RenderManager::DrawRenderBuffer()
{
	const bool bFoundDifference = DrawConsoleLibrary::DrawTextureDifference(SecondRenderBuffer.get(), CurrentRenderBuffer.get(), 0, 0);
	if (bFoundDifference) RenderConsoleLibrary::RestoreConsoleRenderMode();
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

void RenderManager::ReserveRenderQueueSpace(RenderPriorityGroup group, uint32_t size)
{
	RenderGroupQueue[group].reserve(size);
}

bool RenderManager::PushToRenderQueue(IRenderable* renderable)
{
	if (!renderable) { DebugEngineTrap(); return false; }
	if (!renderable->IsVisible()) return false;

	#if USE_RENDER_LATENCY()
	if (!bReadyToRender) return false;
	#endif // USE_RENDER_LATENCY()

	//using namespace std::chrono;
	//auto PushStart = high_resolution_clock::now();

	const uint8_t priority = renderable->GetRenderPriority();
	auto& renderGroup = RenderGroupQueue[renderable->GetRenderPriorityGroup()];

	auto it = std::find_if(renderGroup.begin(), renderGroup.end(), [&priority](const IRenderable* other) -> bool {
		return other->GetRenderPriority() > priority;
	});
	
	if (it != renderGroup.end())
	{
		renderGroup.insert(it, renderable);
		return true;
	}

	renderGroup.push_back(renderable);

	//auto PushEnd = high_resolution_clock::now();
	//float TimePassed = duration_cast<duration<float>>(PushEnd - PushStart).count();
	//
	//std::string logMessage = "RenderManager::PushToRenderQueue() - Pushed renderable to render queue in " + std::to_string(TimePassed) + " seconds.";
	//Logger::GetInstance().Write(logMessage.c_str());
	return true;
}

void RenderManager::ClearRenderQueue()
{
	RenderGroupQueue.clear();
}
