local class = require "engine.class"

SinusLightBehaviour = class("SinusLightBehaviour", Component)
function SinusLightBehaviour:__init()
	Component.__init(self)
	
	-- The speed of the light behaviour
	self.speed = 10
end

function SinusLightBehaviour:SetSpeed(speed)
	self.speed = speed
end

function SinusLightBehaviour:Tick()
	local node = self:GetSceneNode()
	local position = node:GetPosition()
	position[2] = math.abs(math.sin(GameTotalTime * self.speed) * 5.0);
	node:SetPosition(position)
end

return SinusLightBehaviour