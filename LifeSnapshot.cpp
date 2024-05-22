#include "LifeSnapshot.h"
#include <SDL.h>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <random>
#include <iostream>
using std::cout;
using std::endl;

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

Particle::Particle(double _x, double _y, int R, int G, int B, double m, unsigned int t)
	: x(_x), y(_y), colorR(R), colorG(G), colorB(B), mass(m), type(t) {}

LifeSnapshot::LifeSnapshot(std::vector<int> amt, double mass, int width, int height, int colorR=255, int colorG=255, int colorB=255)
{
	WIDTH = width;
	HEIGHT = height;
	int id = 0;
	for (int number : amt) {
		for (int i = 0; i < number; ++i) {
			particles.push_back(Particle(getTillRange(width), getTillRange(height), colorR, colorG, colorB, mass, id));
		}
		id++;
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
	for (int i = 0; i < x.size(); ++i) {
		if (x[i].size() != x.size()) {
			throw std::invalid_argument("The matrix is not a square one. Please try a square matrix.");
		}
	}
	matrix = x;
}

void LifeSnapshot::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set drawing color to black
	SDL_RenderClear(renderer); // clear screen with given color
	for (Particle& i : particles) {
		SDL_SetRenderDrawColor(renderer, i.colorR, i.colorG, i.colorB, 255); // set new color
		SDL_RenderDrawPoint(renderer, (int)i.x, (int)i.y); // draw point here
	}
	SDL_RenderPresent(renderer);
}


constexpr double FORCE_TRANSFER = 0.5;
void LifeSnapshot::update()
{
	const size_t& size = particles.size();

	double gX = 0;
	double gY = 0;
	unsigned int j = 0;

	for (unsigned int i = 0; i < size; ++i) {
		bool collision = false;
		size_t colide_with = -1;

		gX = 0;
		gY = 0;
		j = 0;

		const double pi_x = particles[i].x;
		const double pi_y = particles[i].y;
		const double pi_mass = particles[i].mass;
		const unsigned int pi_type = particles[i].type;

		// get the force of gravity on each object:
		for (; j < particles.size(); ++j) {
			if (j != i) {
				const double ATTRACTION_CONSTANT = matrix[pi_type][particles[j].type];
				const double dx = particles[j].x - pi_x;
				const double dy = particles[j].y - pi_y;
				const double r2 = dx * dx + dy * dy;
				const double r = sqrt(r2);
				
				// check if particles are too close:
				if (r > SPACE_BETWEEN_PARTICLES) {

					const double angle = atan2(dy, dx);
					const double force = ATTRACTION_CONSTANT * (pi_mass * particles[j].mass) / r2;
					
					// TODO: look over variety selection
					gX += cos(angle) * force;
					gY += sin(angle) * force;
				}
				else {
					collision = true;
					colide_with = j;
				}
			}
		}
		// integrate for velocity:
		if (!collision) {
			particles[i].vX += gX;
			particles[i].vY += gY;
		}
		else {
			// transfer power to other particle:
			particles[colide_with].vX += FORCE_TRANSFER * particles[i].vX;
			particles[colide_with].vY += FORCE_TRANSFER * particles[i].vX;
			particles[i].vX = (1-FORCE_TRANSFER) * particles[i].vX;
			particles[i].vY = (1-FORCE_TRANSFER) * particles[i].vX;
		}
		// add distance:
		particles[i].x += particles[i].vX;
		particles[i].y += particles[i].vY;
		// boundary checks:
		if (particles[i].x >= WIDTH || particles[i].x <= 0) {
			particles[i].vX *= -0.5;
		} 
		if (particles[i].y >= HEIGHT || particles[i].y <= 0) {
			particles[i].vY *= -0.5;
		}
	}
}

void LifeSnapshot::addParticle(Particle p)
{
	particles.push_back(p);
}

void LifeSnapshot::addParticle(Particle* p)
{
	particles.push_back(*p);
}
