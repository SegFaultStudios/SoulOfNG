#include "CollisionHandler.hpp"
#include <iostream>



sf::Vector2f CollisionHandler::resolveNoRotationWallMovement(sf::FloatRect playerBounds, sf::Vector2f potentialMovement, sf::FloatRect entityBounds) {

	//	   zalupa
	//       ||
	//       \/
	/*sf::Vector2f finalPos = playerBounds.position + potentialMovement; //Calculate the potential final position

	const float epsilon = 0.01f;//prevents sticking to a wall

	sf::FloatRect potentialMoveX(sf::Vector2f(playerBounds.position + sf::Vector2f(potentialMovement.x, 0)), playerBounds.size); //first, check for collisions when moving only on X-axis
	
	
	 
	if (potentialMoveX.findIntersection(entityBounds) && (playerBounds.position.x < entityBounds.position.x ||
		playerBounds.position.x + playerBounds.size.x > entityBounds.position.x + entityBounds.size.x)) { //collision detected on X-axis - adjust position 
		std::cout << "x-axis collision" << std::endl;

		if (potentialMovement.x > 0) {
			finalPos.x = entityBounds.position.x - playerBounds.size.x - epsilon; //move left of obstacle
		}
		else {
			finalPos.x = entityBounds.position.x + entityBounds.size.x + epsilon; //move right of obstacle
		}
	} else {

		sf::FloatRect potentialMoveY(sf::Vector2f(playerBounds.position + sf::Vector2f(0, potentialMovement.y)), playerBounds.size); //no X collision - check Y-axis movement

		if (potentialMoveY.findIntersection(entityBounds)) {
			std::cout << "y-axis collision" << std::endl;

			if (potentialMovement.y > 0) {
				finalPos.y = entityBounds.position.y - playerBounds.size.y - epsilon;
			}
			else {
				finalPos.y = entityBounds.position.y + entityBounds.size.y + epsilon;
			}
		}
	}

	sf::FloatRect potentialMoveBoth(finalPos, playerBounds.size); //if the finalPos is still colliding - angle entrance exception - prevent moving
	if (potentialMoveBoth.findIntersection(entityBounds)) {
		return playerBounds.position;
	}

	return finalPos;*/
	//     /\
	//     ||
	//   zalupa

	sf::Vector2f finalPosition = playerBounds.position + potentialMovement; 
	
	float penetrationX = std::min(entityBounds.position.x + entityBounds.size.x, playerBounds.position.x + playerBounds.size.x) -
		std::max(entityBounds.position.x, playerBounds.position.x);
	
	float penetrationY = std::min(entityBounds.position.y + entityBounds.size.y, playerBounds.position.y + playerBounds.size.y) -
		std::max(entityBounds.position.y, playerBounds.position.y);

	const float epsilon = 0.01f;

	if (penetrationX < penetrationY) {
		if (playerBounds.position.x <= entityBounds.position.x) {
			finalPosition.x = entityBounds.position.x - playerBounds.size.x - epsilon;
		} 
		else {
			finalPosition.x = entityBounds.position.x + entityBounds.size.x + epsilon;
		}
	}
	else if (penetrationY < penetrationX) {
		if (playerBounds.position.y <= entityBounds.position.y) {
			finalPosition.y = entityBounds.position.y - playerBounds.size.y - epsilon;
		}
		else {
			finalPosition.y = entityBounds.position.y + entityBounds.size.y + epsilon;
		}

	}
	else {
		finalPosition = playerBounds.position;
	}
	return finalPosition;
}

