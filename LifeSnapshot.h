#pragma once
#include <vector>
#include <SDL.h>

typedef std::vector<std::vector<double>> InteractionMatrix;

/*
* A Particle Structure that contains basic information of a particle.
*/
struct Particle {
	size_t type = 0;
	double vX = 0;
	double vY = 0;
	double x;
	double y;
	double mass;
	int colorR;
	int colorG;
	int colorB;
	Particle();
	Particle(double _x, double _y);
	Particle(double _x, double _y, int R, int G, int B);
	Particle(double _x, double _y, int R, int G, int B, double mass);
	Particle(double _x, double _y, int R, int G, int B, double mass, unsigned int type);
};

/*
* Life Snapshot class that can calculate the new positions of particles and draw them, given an interaction matrix.
*/
class LifeSnapshot
{
private:
	std::vector<Particle> particles;
	double barrierForceThreshold;
	double barrierForceRebound;
	InteractionMatrix matrix;

	static const int VARIETY = 1;
	static const int SPACE_BETWEEN_PARTICLES = 1;
	int WIDTH;
	int HEIGHT;
public:
	LifeSnapshot(std::vector<int> amt, double mass, int width, int height, int colorR, int colorG, int colorB);
	/*
	* Set the barrier threshold for the amount of force required to exert rebound force.
	* TODO.
	* 
	* @param threshold the force threshold.
	*/
	void setBarrierForceThreshold(double threshold);
	/*
	* Set the barrier threshold for the amount of force required to exert rebound force.
	* TODO.
	*
	* @param threshold the force threshold.
	*/
	void setBarrierForceRebound(double rebound);
	/*
	* Set the interaction Matrix for the particles.
	* Will throw an error if matrix is not square and does not have the same dimensions as the variety of particles.
	* 
	* @param matrix the interaction matrix.
	*/
	void setInteractionMatrix(InteractionMatrix matrix);
	/*
	* Draws all points on screen
	* 
	* @param renderer the SDL_Renderer struct
	*/
	void draw(SDL_Renderer* renderer);
	/*
	* Updates all values and modifies points based on interaction matrix
	*/
	void update();
	/*
	* Adds a particle to the canandrum.
	* 
	* @param p the particle to add.
	*/
	void addParticle(Particle p);
	/*
	* Adds a particle to the canandrum.
	*
	* @param p the particle to add.
	*/
	void addParticle(Particle* p);
};