#include "GameLevel.h"

#include "Engine/Render/RenderManager.h"

GameLevel::GameLevel()
{
}

GameLevel::~GameLevel()
{
}

void GameLevel::OpenLevel()
{
}

void GameLevel::Update(float DeltaTime)
{
	while (!ObjectBeginPlayQueue.empty())
	{
		ObjectBeginPlayQueue.front()->BeginPlay();
		ObjectBeginPlayQueue.pop();
	}

	for (auto& obj : ObjectsOnLevel)
	{
		if (obj->IsWaitingForDestroy()) continue;

		if (SceneObject* pSceneObj = dynamic_cast<SceneObject*>(obj.Get()))
			pSceneObj->Update(DeltaTime);
		
		if (IRenderable* renderObj = dynamic_cast<IRenderable*>(obj.Get()))
			RenderManager::GetInstance()->PushToRenderQueue(renderObj);
	}
}

bool GameLevel::PassInput(const InputState& is)
{
	for (auto& obj : ObjectsOnLevel)
	{
		if (auto inputHandler = dynamic_cast<IInputHandler*>(obj.Get()))
		{
			bool bConsumed = inputHandler->PassInput(is);
			if (bConsumed) return true;
		}
	}

	return false;
}

void GameLevel::PlaceObjectOnLevel(GameObject* obj)
{
	auto it = std::find_if(ObjectsOnLevel.begin(), ObjectsOnLevel.end(), [obj](TObjectPtr<GameObject>& Other) -> bool {
		return obj == Other.Get();
		});

	if (it == ObjectsOnLevel.end())
	{
		ObjectsOnLevel.push_back(obj);

		if (auto sceneObj = dynamic_cast<SceneObject*>(obj))
			ObjectBeginPlayQueue.push(sceneObj);
	}
}
