#include "LifeSnapshot.h"
#include <SDL.h>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <random>

static int getTillRange(int max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, max);

	return dist(gen);
}

Particle::Particle()
	: x(0), y(0), colorR(255), colorG(255), colorB(255), mass(1) {}

Particle::Particle(double _x, double _y)
	: x(_x), y(_y), colorR(255), colorG(255), colorB(255), mass(1) {}

Particle::Particle(double _x, double _y, int R, int G, int B)
	: x(_x), y(_y), colorR(R), colorG(G), colorB(B), mass(1) {}

Particle::Particle(double _x, double _y, int R, int G, int B, double m)
	: x(_x), y(_y), colorR(R), colorG(G), colorB(B), mass(m) {}

LifeSnapshot::LifeSnapshot(int amt, double mass, int width, int height, int colorR=255, int colorG=255, int colorB=255)
{
	WIDTH = width;
	HEIGHT = height;
	for (int i = 0; i < amt; ++i) {
		particles.push_back(Particle(getTillRange(width), getTillRange(height), colorR, colorG, colorB, mass));
	}
	setBarrierForceRebound(0); // Temporary
	setBarrierForceThreshold(0); // Temporary
}

void LifeSnapshot::setBarrierForceThreshold(double x)
{
	barrierForceThreshold = x;
}

void LifeSnapshot::setBarrierForceRebound(double x)
{
	barrierForceRebound = x;
}

void LifeSnapshot::setInteractionMatrix(InteractionMatrix x) {
	for (std::vector<double> i : x) {
		if (i.size() != x.size()) {
			throw std::invalid_argument("The matrix is incorrect. Please try a better matrix.");
		}
	}
	if (x.size() != VARIETY) {
		throw std::invalid_argument("The matrix is incorrect. Please try a better matrix.");
	}
	matrix = x;
}

void LifeSnapshot::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set drawing color to black
	SDL_RenderClear(renderer); // clear screen with given color
	for (Particle& i : particles) {
		SDL_SetRenderDrawColor(renderer, i.colorR, i.colorG, i.colorB, 255); // set new color
		SDL_RenderDrawPoint(renderer, i.x, i.y); // draw point here
	}
	SDL_RenderPresent(renderer);
}

void LifeSnapshot::update()
{
	for (int i = 0; i < particles.size(); ++i) {
		// get the force of gravity on each object:
		double gX = 0;
		double gY = 0;
		for (int j = 0; j < particles.size(); ++j) {
			if (j != i) {
				// check if particles are too close:
				if (abs(particles[j].x - particles[i].x) > SPACE_BETWEEN_PARTICLES && abs(particles[j].y - particles[i].y) > SPACE_BETWEEN_PARTICLES) {
					double r = hypot(particles[j].x - particles[i].x, particles[j].y - particles[i].y);
					double angle = atan2(particles[i].y - particles[j].y, particles[i].x - particles[j].x);
					// TODO: look over variety selection
					gX += -cos(angle) * matrix[0][0] * (particles[i].mass * particles[j].mass) / pow(r, 2);
					gY += -sin(angle) * matrix[0][0] * (particles[i].mass * particles[j].mass) / pow(r, 2);
				}
			}
		}
		// integrate for velocity:
		particles[i].vX += gX;
		particles[i].vY += gY;
		// add distance:
		particles[i].x += particles[i].vX;
		particles[i].y += particles[i].vY;
		// boundary checks:
		if (particles[i].x > WIDTH || particles[i].x <= 0) {
			particles[i].vX *= -0.5;
		}
		if (particles[i].y > HEIGHT || particles[i].y <= 0) {
			particles[i].vY *= -0.5;
		}
	}
}
