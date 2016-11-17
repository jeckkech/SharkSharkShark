#include "AbstractFish.h"

USING_NS_CC;

int AbstractFish::getScore() {
	return score;
}

int AbstractFish::getCurrentStage() {
	return currentStage;
}

void AbstractFish::increaseScore() {
	score++;
}