project(
	'Game Engine', 
	'cpp', 
	default_options : ['warning_level=3', 'cpp_std=c++20']
)

cmake = import('cmake')

sdl_dep = dependency('sdl2', required : false)
if not sdl_dep.found()
	cmake_opt = cmake.subproject_options()
	cmake_opt.add_cmake_defines({'LIBC': true})
	sdl_proj = cmake.subproject('SDL', options : cmake_opt)
	sdl_dep = sdl_proj.dependency('SDL2')
endif

sdl_gfx_dep = dependency('SDL2_gfx', required : false)
if not sdl_gfx_dep.found()
	sdl_gfx_proj = subproject('SDL2_gfx-1.0.4')
	sdl_gfx_dep = sdl_gfx_proj.get_variable('sdl_gfx_dep')
endif
glm_proj = cmake.subproject('glm')
glm_dep = glm_proj.dependency('glm')

fmt_dep = dependency('fmt', required : false)
if not fmt_dep.found()
	fmt_proj = cmake.subproject('fmt')
	fmt_dep = fmt_proj.dependency('fmt')
endif

subdir('src')

