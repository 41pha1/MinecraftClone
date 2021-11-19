#include <glad/glad.h>
#include <SDL.h>
#include <SDL_main.h>
#include <SDL_mouse.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <array>
#include <iostream>
#include <map>

#include "Block.h"
#include "Chunk.h"
#include "Game.h"
#include "ImageLoader.h"
#include "Input.h"
#include "MasterRenderer.h"
#include "Mouse.h"


SDL_Window *window;
MasterRenderer *renderer;
Game *game;
Input *input;
Mouse * mouse;
std::map<int, bool> keyboard;

const int width = 1500, height = 900;
const int FPS = 60;
const int TPS = 20;

void next_frame (double dt)
{
	input->update(keyboard, mouse, game);
    game->update(dt);
    renderer->render(game);
	SDL_GL_SwapWindow(window);
}
//TODO chunk remeshing, batch chunk rendering, light levels, threaded chunk loading, reduce buffer data, dont load region file multiple times,
//smart chunk meshing, sort chunk load queue und update it, unload chunks, water, shading, combine vertical chunks
int main(int argc, char* argv[])
{
//	  SDL_Init(SDL_INIT_VIDEO);
//	    window = SDL_CreateWindow("Minecraft Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
//
//	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
//	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//	    SDL_GL_CreateContext(window);
//	    SDL_WarpMouseInWindow(window, width / 2, height / 2);
//
//	    SDL_SetWindowGrab(window, SDL_TRUE);
//	    SDL_SetRelativeMouseMode(SDL_TRUE);
//
//	    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
//
//	    ImageLoader::loadTextures();
//	    Block::init();
//	    input = new Input();
//	    mouse = new Mouse();
//	    renderer = new MasterRenderer(width, height);
//
//	    game = new Game(renderer->camera, "res/maps/Celestial Castle"); //res/maps/Celestial Castle
//
//
//
//		std::array<Chunk*, Chunk::SIZE> * chunk = game->loadChunk(0, 0);
//
//		for(int i = 0; i < 1000; i++)
//		{
//			for(int j = 0; j < Chunk::HEIGHT; j++)
//			{
//				(*chunk)[j]->generateMesh();
//				(*chunk)[j]->loadToVao();
//			}
//			std::cout << i << std::endl;
//		}
//
//		for(int j = 0; j < Chunk::HEIGHT; j++)
//			delete (*chunk)[j];
//		delete chunk;


    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Minecraft Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_CreateContext(window);
    SDL_WarpMouseInWindow(window, width / 2, height / 2);

    SDL_SetWindowGrab(window, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    ImageLoader::loadTextures();
    Block::init();
    input = new Input();
    mouse = new Mouse();
    renderer = new MasterRenderer(width, height);

    int start = SDL_GetTicks();
    game = new Game(renderer->camera, ""); //res/maps/Celestial Castle
    std::cout << SDL_GetTicks()-start << std::endl;
    SDL_Event event;
    int running = 1;
    int frame = 0;
    double mspf = 1000./FPS;
    double mspt = 1000./TPS;
    int t_prev = SDL_GetTicks();
    int t_last = t_prev;
    int last_tick = t_prev;
    bool Mousefirst = true;

    while(running)
    {
    	int t = SDL_GetTicks();
    	//next_frame(mspf);

		if((t-last_tick) > mspt)
		{
			last_tick = t;
			game->tick();
		}

		if ((t - t_prev) > mspf)
		{
			while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
				 case SDL_KEYDOWN:
					keyboard[event.key.keysym.sym] = true;
					break;
				case SDL_KEYUP:
					keyboard[event.key.keysym.sym] = false;
					break;
				case SDL_MOUSEMOTION:
					if (!Mousefirst)
					{
						 mouse->dx =event.motion.xrel;
						 mouse->dy =event.motion.yrel;
					}else {
						Mousefirst = false;
						mouse->dx = 0;
						mouse->dy = 0;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					mouse->left =(event.button.button ==SDL_BUTTON_LEFT);
					mouse->right= (event.button.button ==SDL_BUTTON_RIGHT);
				break;
				case SDL_QUIT:
					running = 0;
					break;
				}
			}
			next_frame((t - t_prev) / (float) mspf);

			t_prev = t;

			if(++frame > FPS)
			{
				frame = 0;
				float cFps = (1000. / (t - t_last)) * FPS;
				t_last = SDL_GetTicks();
				std::cout << "FPS: " << cFps << std::endl;
			}
		}
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    delete renderer;
    delete game;
    delete input;
    delete mouse;

    return 0;
}
