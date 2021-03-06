#include "GuiFactory.h"
#include "../Screens/MainMenu.h"
#include "../Screens/ChooseType.h"
#include "../Screens/ChooseSkill.h"
#include "../Screens/SelectWorld.h"
#include "../Screens/PlayGame.h"
#include "../Screens/WorldBuilder.h"
#include "../Screens/InvalidWorld.h"
#include <stdio.h>



GUI* createGUIForState(StateId stateId) {
	GUI* gui = (GUI*)malloc(sizeof(GUI));
	if (gui == NULL) {
		perror("ERROR: malloc failed\n");
		exit(1);
	}
	gui->stateId = stateId;

	switch (stateId) {
	case MAIN_MENU:
		createGUIMain(gui);
		break;
	case CHOOSE_CAT:
		createGUIChooseType(gui);
		break;
	case CHOOSE_MOUSE:
		createGUIChooseType(gui);
		break;
	case LOAD_GAME:
		createGuiSelectWorld(gui);
		break;
	case EDIT_GAME:
		createGuiSelectWorld(gui);
		break;
	case SAVE_GAME:
		createGuiSelectWorld(gui);
		break;
	case CAT_CHOOSE_SKILL:
		createGUIChooseSkill(gui);
		break;
	case MOUSE_CHOOSE_SKILL:
		createGUIChooseSkill(gui);
		break;
	case PLAY_GAME:
		createGUIPlayGame(gui);
		break;
	case NEW_WORLD_BUILDER:
		createGUIWorldBuilder(gui);
		break;
	case EDIT_WORLD_BUILDER:
		createGUIWorldBuilder(gui);
		break;
	case INVALID_WORLD:
		createGUIInvalidWorld(gui);
		break;
	default:
		break;
	}
	return gui;
}