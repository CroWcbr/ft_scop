# scop
Basic GPU rendering with OpenGL

## Use external lib
- GLFW
	- git submodule add https://github.com/glfw/glfw.git external/glfw

## Scop_soft_render
- full project for 42 task with bonus
- using only CPU
- using only glfw (for output image as texture)
```
Commands:
- active rot:	space
- mode:			1-6
	- point
	- line
	- random triangle
	- texture
	- light
	- light + texture
- change proj:	tab
- rotate:		w-s, a-d, q-w
- move:			t-g, f-h, r-y
- scale:		+ -
- exit:			esc
```

## Scop_opengl
- only draft for myself (studing glad), much more easy then Scop_soft_render
- project on GPU
- using glad + glfw