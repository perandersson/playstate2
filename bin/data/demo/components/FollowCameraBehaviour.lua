local class = require "engine.class"
local config = require "config"
local Vector3 = require "engine.core.math.vector3"

-- A C++ variant of this class can be found here: demo/FollowCameraComponent.h
FollowCameraBehaviour = class("FollowCameraBehaviour", Component)
function FollowCameraBehaviour:__init()
	Component.__init(self)
end

function FollowCameraBehaviour:Update()
	
	-- Get the current active camera
	local camera = Scene.GetActiveCamera()
	
	-- Get the node's absolute position
	local sceneNode = self:GetSceneNode()
	local position = sceneNode:GetAbsolutePosition()
	
	-- Set the camera's new positions
	local newEyePos = Vector3.Add(position, config.camera.eye)
	camera:LookAt(newEyePos, position, config.camera.up)
	
end

return FollowCameraBehaviour
