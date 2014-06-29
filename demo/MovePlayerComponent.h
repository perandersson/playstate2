#pragma once
#include <core/playstate.h>
#include <vector>
using namespace core;

class MovePlayerComponent : public Component, public Updatable
{
	DECLARE_SCRIPT_OBJECT(MovePlayerComponent);

public:
	MovePlayerComponent();
	virtual ~MovePlayerComponent();

	//
	// Set the movement speed
	//
	// @param speed
	//			The movement speed for the player node
	void SetSpeed(float32 speed);

	//
	// @return The movement speed
	inline float32 GetSpeed() const {
		return mSpeed;
	}

	//
	// Add sound effect to play when moving
	void AddSoundEffect(Resource<SoundEffect> soundEffect);

// Component
public:	
	virtual Updatable* GetUpdatable() { return this; }
	virtual void OnEventReceived(const Event* evt);

// Updatable
public:
	virtual void Update();

private:
	void RotateThis(const Point& mousePos, const Size& windowSize);
	void TranslateThis(const Point& mousePos, const Size& windowSize);
	Vector2 GetArcballVector(const Point& mousePos, const Size& windowSize);

private:
	float32 mSpeed;
	Point mPrevMousePos;

	const IMouseDevice* mMouseDevice;
	ISoundReceiver* mSoundReceiver;

	float32 mMouseDownTime;
	float32 mTimeSinceLastPlay;
	std::vector<Resource<SoundEffect>> mSoundEffects;
	uint32 mSoundEffectIndex;
};

//
// Script integration
//

int MovePlayerComponent_Factory(struct lua_State* L);
int MovePlayerComponent_Init(struct lua_State* L);
int MovePlayerComponent_SetSpeed(struct lua_State* L);
int MovePlayerComponent_SetSoundEffects(struct lua_State* L);

static luaL_Reg MovePlayerComponent_Methods[] = {
	{ SCRIPT_CONSTRUCTOR, MovePlayerComponent_Factory },
	{ SCRIPT_INHERIT_CONSTRUCTOR, MovePlayerComponent_Init },
	{ "SetSpeed", MovePlayerComponent_SetSpeed },
	{ "SetSoundEffects", MovePlayerComponent_SetSoundEffects },
	{ nullptr, nullptr }
};
