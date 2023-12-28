#include "GameLevel.h"

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

}

void GameLevel::PlaceObjectOnLevel(GameObject* obj)
{
	auto it = std::find_if(ObjectsOnLevel.begin(), ObjectsOnLevel.end(), [obj](TObjectPtr<GameObject>& Other) -> bool {
		return obj == Other.Get();
		});

	if (it == ObjectsOnLevel.end())
		ObjectsOnLevel.push_back(obj);
}
