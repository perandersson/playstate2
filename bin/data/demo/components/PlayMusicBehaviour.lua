local class = require "engine.class"

PlayMusicBehaviour = class("PlayMusicBehaviour", Component)
function PlayMusicBehaviour:__init()
	Component.__init(self)
	
	-- The music resource
	self.music = nil
end

function PlayMusicBehaviour:SetMusic(music)
	self.music = music
end

function PlayMusicBehaviour:OnAttachedToScene()
	print("OnAttachedToScene")
	Music.Play(self.music)
end

function PlayMusicBehaviour:OnDetachedFromScene()
	print("OnDetachedFromScene")
	Music.Stop(self.music)
end

--???

return PlayMusicBehaviour