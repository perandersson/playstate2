local config = {}

-- Window configuration
config.window = {}
config.window.size = { 1024, 768 }
config.window.bpp = 32
config.window.fullscreen = false
config.window.title = "Demo FPS: "

-- Camera configuration
config.camera = {}
config.camera.eye = {0, 50.0, 50.0}
config.camera.center = {0, 0, 0}
config.camera.up = {0, 1, 0}

-- Development mode (will prevent crashes on bad shader code)
config.graphics = {}
config.graphics.developmentmode = true

-- Development mode
config.resources = {}
config.resources.developmentmode = true

-- Sound 
config.sound = {}
config.sound.mastervolume = 1.0
config.sound.musicvolume = 0.5
config.sound.soundeffectvolume = 1.0

-- Script
config.script = {}
config.script.developmentmode = true

return config