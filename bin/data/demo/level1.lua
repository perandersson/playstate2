local MovePlayerBehaviour = require "demo.components.MovePlayerBehaviour"
local FollowCameraBehaviour = require "demo.components.FollowCameraBehaviour"
local PlayMusicBehaviour = require "demo.components.PlayMusicBehaviour"
local SinusLightBehaviour = require "demo.components.SinusLightBehaviour"

local level1 = QuadTreeSceneGroup()
level1:SetQuadTree({0, 0, 0}, 1100.0, 10000.0, 1100.0, 3)

local i = 0
local j = 0
local chinalampModel = Resource.Load("/demo/models/chinalamp/China_lampEX.obj")
for i=-10, 9 do
	for j=-10, 9 do
		local node = RenderModelSceneNode()
		node:SetPosition({i * 20, 0, j * 20})
		node:SetModel(chinalampModel)
		level1:AddSceneNode(node)
	end
end

-- Add lighting using random positions
math.randomseed( os.time() )
for i=-10, 9 do
	for j=-10, 9 do
		local color = {math.random(0, 100) / 100.0, math.random(0, 100) / 100.0, math.random(0, 100) / 100.0}
		local radius = 10.0
		local constantAttenuation = 0.0
		local linearAttenuation = 0.0
		local quadricAttenuation = 0.5
		local pointLight = PointLight()
		pointLight:SetColor(color)
		pointLight:SetRadius(radius)
		pointLight:SetConstantAttenuation(constantAttenuation)
		pointLight:SetLinearAttenuation(linearAttenuation)
		pointLight:SetQuadricAttenuation(quadricAttenuation)
		pointLight:SetPosition({i * 20, 0, j * 20})
		local lightBehavior = SinusLightBehaviour()
		lightBehavior:SetSpeed((math.random(0, 100) / 100.0 * 5.0))
		pointLight:AddComponent(lightBehavior)
		level1:AddSceneNode(pointLight)
	end
end

-- Add ground
local ground1 = Resource.Load("/demo/models/ground1/ground1.obj")
for i=-10, 9 do
	for j=-10, 9 do
		local node = RenderModelSceneNode()
		node:SetPosition({i * 20, 0, j * 20})
		node:SetModel(ground1)
		level1:AddSceneNode(node)
	end
end

return level1
