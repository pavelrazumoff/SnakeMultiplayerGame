#pragma once

#include "Engine/Level/GameLevel.h"

class SnakePawn;
class SnakePlayerState;

class PlayerHUDWidget;
class GameMenuWidget;
class PlayerLostMenuWidget;

class TimerHandler;

enum class ProfilerEngineFeature;

REGISTER_CLASS(PlayLevel)
class PlayLevel : public GameLevel
{
	GAMEOBJECT_BODY(PlayLevel, GameLevel)

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

	// Menu events.
	void HandleGameMenuOpenClicked();

	void HandleResumeGameClicked();

	void HandlePlayAgainClicked();
	void HandleExitGameClicked();

	// Timer events.
	void UpdateFPSCounter();

	// Profiler events.
	void HandleProfilerEngineFeatureChanged(ProfilerEngineFeature feature, bool bEnabled);

protected:
	TObjectPtr<PlayerHUDWidget> PlayerHUD;
	TObjectPtr<GameMenuWidget> GameMenu;
	TObjectPtr<PlayerLostMenuWidget> PlayerLostMenu;

	float SpawnNewFoodTimeValue = 1.5f;
	LV_RECT playAreaRect;

private:
	float SpawnNewFoodTimer = 0.0f;

	TObjectPtr<TimerHandler> FPSCounterTimer;
};
