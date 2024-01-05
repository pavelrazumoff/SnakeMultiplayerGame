#pragma once

#include "Engine/Level/GameLevel.h"

class SnakePawn;

class PlayLevel : public GameLevel
{
	typedef GameLevel Inherited;

public:
	PlayLevel();
	virtual ~PlayLevel();

	virtual void OpenLevel() override;
	virtual void Update(float DeltaTime) override;

	virtual void ReconstructLevel() override;

protected:
	TObjectPtr<SnakePawn> pSnakePawn;
};
