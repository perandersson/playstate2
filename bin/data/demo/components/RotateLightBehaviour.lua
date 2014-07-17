local class = require "engine.class"

RotateLightBehaviour = class("RotateLightBehaviour", Component)
function RotateLightBehaviour:__init()
	Component.__init(self)
	
	self.speed = 20
	self.radius = 5
end

function RotateLightBehaviour:SetSpeed(speed)
	self.speed = speed
end

function RotateLightBehaviour:SetRadius(radius)
	self.radius = radius
end

function RotateLightBehaviour:Tick()
	local node = self:GetSceneNode()
	local direction = node:GetSpotDirection()
	direction[1] = math.sin(math.rad(GameTotalTime) * self.speed) * self.radius;
	direction[3] = math.cos(math.rad(GameTotalTime) * self.speed) * self.radius;
	node:SetSpotDirection(direction)
end

return RotateLightBehaviour