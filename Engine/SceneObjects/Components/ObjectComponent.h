#pragma once

class ObjectComponent
{
public:
	virtual ~ObjectComponent() {}

	virtual void BeginPlayComponent() {}
	virtual void UpdateComponent(float /*DeltaTime*/) {}
};
