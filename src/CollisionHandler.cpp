#include "CollisionHandler.hpp"



sf::Vector2f CollisionHandler::resolveNoRotationWallMovement(sf::FloatRect bounds, sf::Vector2f potentialMovement, sf::FloatRect entityBounds) {
	
	sf::Vector2f finalPos = bounds.position + potentialMovement; //Calculate the potential final position

	sf::FloatRect potentialMoveX(sf::Vector2f(bounds.position + sf::Vector2f(potentialMovement.x, 0)), bounds.size); //first, check for collisions when moving only on X-axis

	const float epsilon = 0.1f; //prevents sticking to a wall
	 
	if (potentialMoveX.findIntersection(entityBounds)) { //collision detected on X-axis - adjust position 

		if (potentialMovement.x > 0) {
			finalPos.x = entityBounds.position.x - bounds.size.x - epsilon; //move left of obstacle
		}
		else {
			finalPos.x = entityBounds.position.x + entityBounds.size.x + epsilon; //move right of obstacle
		}
	} else {

		sf::FloatRect potentialMoveY(sf::Vector2f(bounds.position + sf::Vector2f(0, potentialMovement.y)), bounds.size); //no X collision - check Y-axis movement

		if (potentialMoveY.findIntersection(entityBounds)) {

			if (potentialMovement.y > 0) {
				finalPos.y = entityBounds.position.y - bounds.size.y - epsilon;
			}
			else {
				finalPos.y = entityBounds.position.y + entityBounds.size.y + epsilon;
			}
		}
	}

	sf::FloatRect potentialMoveBoth(finalPos, bounds.size); //if the finalPos is still colliding - angle entrance exception - prevent moving
	if (potentialMoveBoth.findIntersection(entityBounds)) {
		return bounds.position;
	}

	return finalPos;

}

