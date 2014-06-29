local MovePlayerBehaviour = require "demo.components.MovePlayerBehaviour"
local FollowCameraBehaviour = require "demo.components.FollowCameraBehaviour"
local PlayMusicBehaviour = require "demo.components.PlayMusicBehaviour"

local playerGroup = LinkedListSceneGroup()

-- player
local player = RenderModelSceneNode()
-- player:EnableEventDispatching(false)
local player_MovePlayerComponent = MovePlayerBehaviour()
player_MovePlayerComponent:SetSpeed(10.0)
player_MovePlayerComponent:SetSoundEffects({
	Resource.Load("/demo/sound/soundeffects/footstep0.wav"),
	Resource.Load("/demo/sound/soundeffects/footstep1.wav")
})
player:AddComponent(player_MovePlayerComponent)
player:SetModel(Resource.Load("/demo/models/player/player.obj"))
local player_FollowCameraBehaviour = FollowCameraBehaviour()
player:AddComponent(player_FollowCameraBehaviour)
local player_PointLight = PointLight()
player_PointLight:SetColor({1.0, 0.8, 0.4, 1.0})
player_PointLight:SetRadius(10.0)
player_PointLight:SetConstantAttenuation(0.0)
player_PointLight:SetLinearAttenuation(0.0)
player_PointLight:SetQuadricAttenuation(0.0)
player:AddChildNode(player_PointLight)
-- player:AfterPropertiesSet()
playerGroup:AddSceneNode(player)
-- /player

return playerGroup