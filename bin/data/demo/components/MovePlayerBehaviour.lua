local class = require "engine.class"
local Vector3 = require "engine.core.math.Vector3"
local Size = require "engine.core.math.Size"

-- A C++ variant of this class can be found here: demo/MovePlayerComponent.h
MovePlayerBehaviour = class("MovePlayerBehaviour", Component)
function MovePlayerBehaviour:__init()
	Component.__init(self)
	
	-- Movement properties
	self.speed = 10.0
	self.prevMousePos = { 0, 0 }
	self.mouseDownTime = 0.0
	
	-- Sound properties
	self.soundEffects = {}
	self.soundEffectCount = 0
	self.timeSinceLastPlay = 0.0
	self.soundEffectIndex = 1
	
	-- Get the used input devices
	self.mouse = InputDevices.GetMouseDevice(0)
	self.keyboard = InputDevices.GetKeyboardDevice(0)
	
	-- Get sound receiver
	self.soundReceiver = SoundEngine.GetSoundReceiver(0)
end

function MovePlayerBehaviour:SetSpeed(speed)
	self.speed = speed
end

function MovePlayerBehaviour:SetSoundEffects(effects)
	self.soundEffectCount = #effects
	self.soundEffects = effects
end

function MovePlayerBehaviour:Update()
	local mouse = self.mouse
	if mouse:IsDown(MouseButton.LEFT) then
		self.mouseDownTime = self.mouseDownTime + GameDeltaTime
		if self.mouseDownTime > 0.01 then
			local mousePos = mouse:GetPosition()
			local windowSize = ActiveWindow.GetSize()
			self:RotateThis(mousePos, windowSize)
			self:TranslateThis(mousePos, windowSize)
		end
	else
		self.mouseDownTime = 0.0
	end
	
	self.timeSinceLastPlay = self.timeSinceLastPlay - GameDeltaTime
end

-- Rotate the node so that it faces the mouse cursor
function MovePlayerBehaviour:RotateThis(mousePos, windowSize)
	if mousePos[1] == self.prevMousePos[1] and mousePos[2] == self.prevMousePos[2] then
		return
	end
	
	local x, y = self:GetArcballVector(mousePos)
	local angle = math.atan2(x, y)
		
	local node = self:GetSceneNode()
	node:SetRotation({0, angle, 0})
	
	self.prevMousePos = mousePos
end

function MovePlayerBehaviour:GetArcballVector(mousePos)
	local x = mousePos[1]
	local y = mousePos[2]
	
	if x < 0 then x = 0 end
	if y < 0 then y = 0 end

	px = (x / ActiveWindow.GetWidth() * 2) - 1.0
	py = (y / ActiveWindow.GetHeight() * 2) - 1.0
	
	-- Normalize px,py,0
	local length = math.sqrt(px * px + py * py + 0)
	return px / length, py / length
end

-- Move the node where this component is attached to based on which mouse buttons are pressed
function MovePlayerBehaviour:TranslateThis(mousePos, windowSize)

	-- Half- window size. 
	local halfWindowSize = Size.DivSingle(windowSize, 2.0)
	
	-- The direction from the scene node
	local direction = Vector3.Normalize({ mousePos[1] - halfWindowSize[1], 0.0, mousePos[2] - halfWindowSize[2] })
	
	-- New position for the scene node
	local sceneNode = self:GetSceneNode()
	local position = sceneNode:GetPosition()
	position = Vector3.Add(position, Vector3.MultSingle(direction, self.speed * GameDeltaTime))
	
	-- Update node- and sound receiver position
	sceneNode:SetPosition(position)
	self.soundReceiver:SetPosition(position)
	
	-- Play walking- soundeffect
	if self.timeSinceLastPlay < 0.0 then
		local effect = self.soundEffects[self.soundEffectIndex]
		self.soundEffectIndex = self.soundEffectIndex + 1
		if self.soundEffectIndex > self.soundEffectCount then
			self.soundEffectIndex = 1
		end
		
		SoundEffect.Play(effect)
		self.timeSinceLastPlay = SoundEffect.GetDuration(effect)
	end
end

return MovePlayerBehaviour