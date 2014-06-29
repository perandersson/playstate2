#include <core/Memory.h>
#include "MovePlayerComponent.h"
#include <core/StringUtils.h>

MovePlayerComponent::MovePlayerComponent()
: Component(), mSpeed(0.0f), mMouseDevice(nullptr), mMouseDownTime(0), mTimeSinceLastPlay(0.0f), mSoundEffectIndex(0)
{
	mMouseDevice = InputDevices::GetMouseDevice(0);
	mSoundReceiver = SoundEngine::GetSoundReceiver(0);
}

MovePlayerComponent::~MovePlayerComponent()
{
}

void MovePlayerComponent::SetSpeed(float32 speed)
{
	mSpeed = speed;
}

void MovePlayerComponent::AddSoundEffect(Resource<SoundEffect> soundEffect)
{
	mSoundEffects.push_back(soundEffect);
}

void MovePlayerComponent::Update()
{
	if (mMouseDevice->IsDown(MouseButton::LEFT)) {
		mMouseDownTime += GameDeltaTime;
		if (mMouseDownTime > 0.01f) {
			const Point mousePos = mMouseDevice->GetPosition();
			const Size& windowSize = ActiveWindow::GetSize();
			RotateThis(mousePos, windowSize);
			TranslateThis(mousePos, windowSize);
		}
	}
	else {
		mMouseDownTime = 0.0f;
	}
	mTimeSinceLastPlay -= GameDeltaTime;
}

void MovePlayerComponent::RotateThis(const Point& mousePos, const Size& windowSize)
{
	const Vector2 arcball = GetArcballVector(mousePos, windowSize);
	const float32 angle = atan2f(arcball.x, arcball.y);
	GetSceneNode()->SetRotation(Vector3(0, angle, 0));
}

void MovePlayerComponent::TranslateThis(const Point& mousePos, const Size& windowSize)
{
	// Half- window size. 
	const Size halfWindowSize = ActiveWindow::GetSize() / 2;
	
	// The direction from the scene node
	const Vector3 direction = Vector3::GetNormalized((float32)(mousePos.x - halfWindowSize.x), 0, (float32)(mousePos.y - halfWindowSize.y));

	// New position for the scene node
	const Vector3 position = GetSceneNode()->GetPosition() + direction * mSpeed * GameDeltaTime;

	// Update node- and sound receiver position
	GetSceneNode()->SetPosition(position);
	mSoundReceiver->SetPosition(position);

	// Play walking- soundeffect
	if (mTimeSinceLastPlay < 0.0f) {
		auto effect = mSoundEffects[mSoundEffectIndex++];
		if (mSoundEffectIndex >= mSoundEffects.size())
			mSoundEffectIndex = 0;
		SoundEngine::Play(effect);
		mTimeSinceLastPlay = effect->GetDuration();
	}
}

Vector2 MovePlayerComponent::GetArcballVector(const Point& mousePos, const Size& windowSize)
{
	if (mousePos == Point::ZERO) {
		return Vector2::ZERO;
	}

	Vector2 arcball(
		(mousePos.x / (float32)windowSize.width * 2) - 1.0f,
		(mousePos.y / (float32)windowSize.height * 2) - 1.0f
	);
	arcball.Normalize();
	return arcball;
}

void MovePlayerComponent::OnEventReceived(const Event* evt)
{
	// Play sound whenver this component receives an animation changed event
	if (dynamic_cast<const AnimationChangedEvent*>(evt) != nullptr) {
		auto effect = mSoundEffects[mSoundEffectIndex++];
		if (mSoundEffectIndex >= mSoundEffects.size())
			mSoundEffectIndex = 0;
		SoundEngine::Play(effect);
	}
}

int MovePlayerComponent_Factory(struct lua_State* L)
{
	return ScriptUtils::PushObject(L, new MovePlayerComponent());
}

int MovePlayerComponent_Init(struct lua_State* L)
{
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		ScriptUtils::LogError(L, "Expected: MovePlayerComponent.__init(self)");
		return 0;
	}

	ScriptUtils::SetObject(L, new MovePlayerComponent());
	return 0;
}

int MovePlayerComponent_SetSpeed(struct lua_State* L)
{
	float32 speed = ScriptUtils::ToFloat32(L);
	MovePlayerComponent* self = ScriptUtils::ToObject<MovePlayerComponent>(L);
	if (self != nullptr)
		self->SetSpeed(speed);
	else
		ScriptUtils::LogError(L, "Expected: MovePlayerComponent.SetSpeed(self, speed)");

	return 0;
}

int MovePlayerComponent_SetSoundEffects(struct lua_State* L)
{
	size_t tableSize = lua_rawlen(L, -1);
	MovePlayerComponent* self = ScriptUtils::ToObject<MovePlayerComponent>(L, -2);
	if (tableSize > 0) {
		if (self != nullptr) {
			lua_pushnil(L);
			while (lua_next(L, -2) != 0) {
				Resource<SoundEffect> soundEffect = ScriptUtils::ToResource<SoundEffect>(L);
				self->AddSoundEffect(soundEffect);
			}
		}
		else
			ScriptUtils::LogError(L, "Expected: MovePlayerComponent.SetSoundEffects(self, { Resource<SoundEffect>, ... })");
	}
	lua_pop(L, 1);
	
	return 0;
}
