#include "PlayLevel.h"

PlayLevel::PlayLevel()
{
}

PlayLevel::~PlayLevel()
{
}

void PlayLevel::OpenLevel()
{
	Inherited::OpenLevel();

	ReconstructLevel();
}

void PlayLevel::Update(float DeltaTime)
{
	Inherited::Update(DeltaTime);

}

void PlayLevel::ReconstructLevel()
{
}
