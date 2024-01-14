#pragma once

#include "Engine/Level/GameLevel.h"

class SnakePawn;
class PlayerHUDWidget;
class PlayerLostMenuWidget;
class SnakePlayerState;

class PlayLevel : public GameLevel
{
	typedef GameLevel Inherited;

public:
	PlayLevel();
	virtual ~PlayLevel();

	virtual void OpenLevel() override;
	virtual void CloseLevel() override;

	virtual void Update(float DeltaTime) override;

	virtual void ReconstructLevel() override;
	void ReconstructDynamicMenu();

protected:
	void SpawnNewFood();
	void CheckForBoundaries();

	void HandleScoreChanged(SnakePlayerState* Instigator, uint32_t newScore);
	void HandlePlayerLost(SnakePlayerState* Instigator);

	void HandlePlayAgainClicked();
	void HandleExitGameClicked();

protected:
	TObjectPtr<SnakePawn> pSnakePawn;

	TObjectPtr<PlayerHUDWidget> PlayerHUD;
	TObjectPtr<PlayerLostMenuWidget> PlayerLostMenu;

	float SpawnNewFoodTimeValue = 1.5f;
	LV_RECT playAreaRect;

private:
	float SpawnNewFoodTimer = 0.0f;
};
