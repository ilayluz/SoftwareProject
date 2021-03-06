#include "ChooseSkill.h"
#include "../ViewInfrastructure/Widget.h"
#include "../ViewInfrastructure/Window.h"
#include "../ViewInfrastructure/Label.h"
#include "../ViewInfrastructure/Button.h"
#include "../ViewInfrastructure/Panel.h"
#include "ChooseMenu.h"
#include "../ModelInfrastructure/Model.h"
#include "../ModelInfrastructure/gui.h"
#include <stdlib.h>

Animal currAnimal;
Player currPlayer;
static int currSkill;
Bitmapfont* bitmapfont1;
GUI* guis[STATES_COUNT];

void createGUIChooseSkill(GUI* gui) {
	gui->start = chooseSkillStart;
	gui->viewTranslateEvent = chooseSkillTranslateEvent;
	gui->presenterHandleEvent = chooseSkillHandleEvent;
	gui->stop = chooseSkillStop;
	gui->model = NULL;
	gui->viewState = NULL;
}


Widget* initializeChooseSkillWindow(SDL_Surface* windowSurface) {
	Widget* window = createWindow(windowSurface);
	char buffer[3];

	SDL_Rect panelRect = { 300, 150, 400, 600 };
	Widget* panel = createPanel(windowSurface, panelRect, SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B));
	addChild(window, panel);

	SDL_Rect labelRect = { 35, 20, 20, 20 };

	Widget* label;
	if (currAnimal == CAT) {
		label = createLabel(labelRect, CHOOSE_CAT_SKILL_TITLE, window->image, 0, 0, bitmapfont1, NULL);
	}
	else {
		label = createLabel(labelRect, CHOOSE_MOUSE_SKILL_TITLE, window->image, 0, 0, bitmapfont1, NULL);
	}
	addChild(panel, label);

	SDL_Rect buttonSelectRect = { 20, 60, 20, 20 };
	SDL_Rect buttonDoneRect = { 20, 130, 20, 20 };
	SDL_Rect buttonBackRect = { 20, 200, 20, 20 };
	SDL_Rect buttonUpRect = { 186, 60, 20, 20 };
	SDL_Rect buttonDownRect = { 186, 84, 20, 20 };

	sprintf(buffer, "%d", currSkill);

	Widget* buttonSelect = createButton(buttonSelectRect, buffer, window->image, MARKED, "select_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 25, 10, BUTTON_SKILL_LEVEL, bitmapfont1);
	addChild(panel, buttonSelect);

	Widget* buttonDone = createButton(buttonDoneRect, SKILL_DONE_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 25, 10, BUTTON_SKILL_DONE, bitmapfont1);
	addChild(panel, buttonDone);

	Widget* buttonBack = createButton(buttonBackRect, SKILL_BACK_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 20, 10, BUTTON_SKILL_BACK, bitmapfont1);
	addChild(panel, buttonBack);

	Widget* buttonUp = createButton(buttonUpRect, NULL, window->image, REGULAR, "up_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 20, 10, BUTTON_SKILL_INCREASE, bitmapfont1);
	addChild(panel, buttonUp);

	Widget* buttonDown = createButton(buttonDownRect, NULL, window->image, REGULAR, "down_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 20, 10, BUTTON_SKILL_DECREASE, bitmapfont1);
	addChild(panel, buttonDown);

	return window;
}

void changeSkillButton(Widget* viewState) {
	char buffer[3]; // holds the skill level string
	sprintf(buffer, "%d", currSkill);
	Widget* skillButton = getWidgetFromId(BUTTON_SKILL_LEVEL, viewState);
	setText(skillButton, buffer);
}

void chooseSkillStart(GUI* gui, Model* initData, SDL_Surface* windowSurface) {
	if (gui->stateId == CAT_CHOOSE_SKILL) {
		currAnimal = CAT;
	}
	else {
		currAnimal = MOUSE;
	}


	char buffer[3];
	gui->viewState = initializeChooseSkillWindow(windowSurface);
	gui->model = guis[gui->stateId]->model;


	if (initData != NULL && initData->world != NULL && initData->stateIdModel != CHOOSE_MOUSE && initData->stateIdModel != CHOOSE_CAT) { // coming from "reconfigure cat/mouse"
		gui->model->gameConfig = initData->gameConfig;
		gui->model->world = initData->world;

		Widget* skillButton = getWidgetFromId(BUTTON_SKILL_LEVEL, gui->viewState);

		if (gui->model->stateIdModel == CAT_CHOOSE_SKILL) {
			currSkill = gui->model->gameConfig->catSkill;
		}
		else {
			currSkill = gui->model->gameConfig->mouseSkill;
		}
		sprintf(buffer, "%d", currSkill);

		markButtonStart(gui->model, BUTTON_SKILL_LEVEL, BUTTON_SKILL_LEVEL, gui->viewState);
		setText(skillButton, buffer);
	}
	else if (initData != NULL && initData->prevModel != NULL && gui->stateId == initData->prevModel->stateIdModel) { // coming from "back" button
		Widget* skillButton = getWidgetFromId(BUTTON_SKILL_LEVEL, gui->viewState);
		
		
		gui->model = initData->prevModel;

		if (gui->model->stateIdModel == CAT_CHOOSE_SKILL) {
			currSkill = gui->model->gameConfig->catSkill;
		}
		else {
			currSkill = gui->model->gameConfig->mouseSkill;
		}
		sprintf(buffer, "%d", currSkill);
		
		markButtonStart(gui->model, gui->model->markedButton, BUTTON_SKILL_LEVEL, gui->viewState);
		setText(skillButton, buffer);
	}
	

	else { // coming from "choose type"
		if (gui->model == NULL) { // if we come to this screen for the first time, we need to allocate the model
			gui->model = createModel(gui->stateId, initData, BUTTON_SKILL_LEVEL);
		}
		else { // if this is not the first time, we don't allocate, just update the relevant fields
			gui->model->stateIdModel = gui->stateId;
			gui->model->prevModel = initData;
			gui->model->markedButton = BUTTON_SKILL_LEVEL;
		}


		gui->model->gameConfig = initData->gameConfig;

		

		gui->model->world = initData->world;
		Widget* skillButton = getWidgetFromId(BUTTON_SKILL_LEVEL, gui->viewState);
		if (gui->model->stateIdModel == CAT_CHOOSE_SKILL && gui->model->gameConfig->catSkill != 0) {
			currSkill = gui->model->gameConfig->catSkill;
		}
		else if (gui->model->stateIdModel == MOUSE_CHOOSE_SKILL && gui->model->gameConfig->mouseSkill != 0) {
			currSkill = gui->model->gameConfig->mouseSkill;
		}
		else {
			currSkill = DEFAULT_SKILL;
		}
		sprintf(buffer, "%d", currSkill);
		setText(skillButton, buffer);
		
	}

	drawWidget(gui->viewState);
}

LogicEvent* chooseSkillTranslateUpOrDownEvent(Widget* viewState, SDL_Event* event, Model* model) {

	switch (event->key.keysym.sym) {
	case (SDLK_UP) :
		return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_SKILL_INCREASE, 0, 0, 0);
		break;
	case (SDLK_DOWN) :
		return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_SKILL_DECREASE, 0, 0, 0);
		break;

	default:
		return createLogicEvent(OTHER_EVENT, 0, 0, 0, 0, 0);
		break;
	}
}

LogicEvent* chooseSkillTranslateEvent(Widget* viewState, SDL_Event* event, Model* model) {

	if ((event->type == SDL_KEYUP) && (event->key.keysym.sym == SDLK_UP || event->key.keysym.sym == SDLK_DOWN)) {
		return chooseSkillTranslateUpOrDownEvent(viewState, event, model);
	}
	else {
		return ChooseMenuTranslateEvent(viewState, event, model, CHOOSE_SKILL_NUM_BUTTONS);
	}
	

}


StateId chooseSkillHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent) {
	Widget* exMarkedButton = NULL;
	Widget* newMarkedButton = NULL;
	Widget* selectedButton = NULL;
	StateId stateid;

	switch (logicalEvent->type) {

	case QUIT_EVENT:
		stateid = QUIT;
		free(logicalEvent);
		return stateid;
		break;

	case MARK_BUTTON:
		markButton(model, logicalEvent, exMarkedButton, viewState, newMarkedButton);
		stateid = model->stateIdModel;
		break;

	case SELECT_BUTTON:
		markButton(model, logicalEvent, exMarkedButton, viewState, newMarkedButton);
		selectedButton = getWidgetFromId(logicalEvent->buttonId, viewState);

		// return the appropriate StateId
		switch (selectedButton->id) {
		

		case BUTTON_SKILL_DONE:
			if (currAnimal == CAT) {
				model->gameConfig->catSkill = currSkill;
				if (model->world == NULL) {
					stateid = CHOOSE_MOUSE;
				}
				else {
					stateid = PLAY_GAME;
				}
			}
			else {
				model->gameConfig->mouseSkill = currSkill;
				stateid = PLAY_GAME;
			}

			break;
		case BUTTON_SKILL_INCREASE:
			markButtonStart(model, BUTTON_SKILL_LEVEL, BUTTON_SKILL_LEVEL, viewState);
			if (currSkill < MAX_SKILL) {
				currSkill++;
				changeSkillButton(viewState);
			}
			stateid = model->stateIdModel;

			break;

		case BUTTON_SKILL_DECREASE:
			markButtonStart(model, BUTTON_SKILL_LEVEL, BUTTON_SKILL_LEVEL, viewState);
			if (currSkill > MIN_SKILL) {
				currSkill--;
				changeSkillButton(viewState);
			}
			stateid = model->stateIdModel;

			break;

		case BUTTON_SKILL_BACK:
			stateid = model->prevModel->stateIdModel;
			if (model->world == NULL) { // if the game hasn't started, we want to "forget" the skill when clicking back
				if (currAnimal == CAT) {
					model->gameConfig->catSkill = DEFAULT_SKILL;
				}
				else {
					model->gameConfig->mouseSkill = DEFAULT_SKILL;
				}
			}
			break;
		default:
			stateid = model->stateIdModel;
			break;
		}
		break;

		
	default:
		stateid = model->stateIdModel;
		free(logicalEvent);
		return stateid;
		break;
	}

	drawWidget(viewState);

	free(logicalEvent);
	return stateid;
}

void* chooseSkillStop(GUI* gui) {
	freeTree(gui->viewState);
	free(gui->viewState); // frees the widget struct without freeing the video surface
	return gui->model;
}