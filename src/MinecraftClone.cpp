#include <glad/glad.h>
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_main.h>
#include <SDL_mouse.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <iostream>
#include <map>

#include "Block.h"
#include "Game.h"
#include "ImageLoader.h"
#include "Input.h"
#include "MasterRenderer.h"
#include "Mouse.h"
#include "OctreeElement.h"

class Mouse;

SDL_Window *window;
MasterRenderer *renderer;
Game *game;
Input *input;
Mouse * mouse;
std::map<int, bool> keyboard;

const int width = 1000, height = 600;
const int FPS = 60;
const int TPS = 20;

void next_frame (double dt)
{
	input->update(keyboard, mouse, game);
    game->update(dt);
    renderer->render(game);
	SDL_GL_SwapWindow(window);
}
//TODO std::array, chunk remeshing
int main(int argc, char* argv[])
{
	for(int j = 0; j < 100; j++)
	{
		OctreeElement tree = OctreeElement(OctreeElement::NODE);
		for(int x = 0; x < 32; x++)
			for(int y = 0; y < 32; y++)
				for(int z = 0; z < 32; z++)
					tree.put(x, y, z, 0, 16, 17);


		for(int x = 0; x < 32; x++)
			for(int y = 0; y < 32; y++)
				for(int z = 0; z < 32; z++)
				{
					OctreeResult res = tree.get(x, y, z, 0, 16);
					//std::cout << (int)res.id << ", " << (int)res.depth << std::endl;
				}

		int size = 0;
		size+= sizeof(tree);
		for(auto elem : tree.children)
		{
			size+= sizeof(*(elem));
			for(auto elem2 : elem->children)
			{
				size+= sizeof(*(elem2));
				for(auto elem3 : elem2->children)
				{
					size+= sizeof(*(elem3));
					for(auto elem4 : elem3->children)
					{
						size+= sizeof(*(elem4));
						for(auto elem5 : elem4->children)
						{
							size+= sizeof(*(elem5));
						}
					}
				}
			}
		}
		std::cout << j << ", " << size << std::endl;
	}

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
    Block::generateMeshData();
    input = new Input();
    mouse = new Mouse();
    renderer = new MasterRenderer(width, height);

    int start = SDL_GetTicks();
    game = new Game(renderer->camera);
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
			next_frame(t - t_prev);

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

    return 0;
}