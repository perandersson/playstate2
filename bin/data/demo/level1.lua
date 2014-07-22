local MovePlayerBehaviour = require "demo.components.MovePlayerBehaviour"
local FollowCameraBehaviour = require "demo.components.FollowCameraBehaviour"
local SinusLightBehaviour = require "demo.components.SinusLightBehaviour"
local RotateLightBehaviour = require "demo.components.RotateLightBehaviour"

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
		node:SetID("CHINALAMPMODEL")
		level1:AddSceneNode(node)
	end
end

-- Add lighting using random positions
math.randomseed( os.time() )
--[[for i=-10, 9 do
	for j=-10, 9 do
		local color = {math.random(0, 100) / 100.0, math.random(0, 100) / 100.0, math.random(0, 100) / 100.0}
		local radius = 10.0
		local constantAttenuation = 0.0
		local linearAttenuation = 0.6
		local quadricAttenuation = 0.2
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
end]]--

-- Add spotlights
for i=-10, 9 do
	for j=-10, 9 do
		local spotlightNode = SpotLight()
		local position = { (i * 20), 20.0, (j * 20) }
		spotlightNode:SetPosition(position)
		spotlightNode:SetColor({1.0, 1.0, 1.0})
		spotlightNode:SetSpotDirection({ position[1] + 35.0, -20.0, position[3] + 35.0 })
		spotlightNode:SetCutoff(20.0)
		spotlightNode:SetSpotExponent(7)
		--spotlightNode:SetTexture(Resource.Load("/demo/effects/cross_light.png"))
		--local rotateLightBehaviour = RotateLightBehaviour()
		--rotateLightBehaviour:SetRadius(40.0)
		--spotlightNode:AddComponent(rotateLightBehaviour)
		level1:AddSceneNode(spotlightNode)
	end
end
-- Add ground
local ground1 = Resource.Load("/demo/models/ground1/ground1.obj")
for i=-10, 9 do
	for j=-10, 9 do
		local node = RenderModelSceneNode()
		node:SetPosition({i * 20, 0, j * 20})
		node:SetModel(ground1)
		node:SetID("ground1")
		level1:AddSceneNode(node)
	end
end

return level1
