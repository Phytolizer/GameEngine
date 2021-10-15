lib = library(
	'game_engine',
	'Engine.cpp',
	'Error.cpp',
	'MatrixStack.cpp',
	'Timer.cpp',
	dependencies : [sdl_dep, sdl_gfx_dep, glm_dep, fmt_dep],
	include_directories : incdir
)

game_engine_dep = declare_dependency(link_with : lib, include_directories : incdir)

