#ifndef INPUT_H_
#define INPUT_H_

#include <map>

class Mouse;

class Game;

class Input {
public:
	Input();
	void update(std::map<int, bool> & keyboard, Mouse * mouse, Game * game);
	virtual ~Input();
};

#endif /* INPUT_H_ */
