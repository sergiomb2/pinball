/***************************************************************************
 Joystick test
 ***************************************************************************/

#include <iostream>

#include "Private.h"
#include "Engine.h"
#include "Keyboard.h"

/** Main */
int main(int argc, char *argv[]) {
	cerr << "Joystick test" << endl;
	Engine* engine = new Engine(argc, argv);
	
	int njoystick = SDL_NumJoysticks();
	cerr << njoystick << " joysticks were found." << endl;
	
	if (njoystick != 0) {
		cerr << "The names of the joysticks are:" << endl;
		for(int a=0; a<njoystick; a++ ) {
			cerr << "  " << SDL_JoystickNameForIndex(a) << endl;
		}
	}

	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		engine->tick();
		engine->render();
		engine->swap();
	}	

	delete(engine);

	return 0;
}

