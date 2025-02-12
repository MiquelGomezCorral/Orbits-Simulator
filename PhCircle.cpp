// PhCircle.cpp

#include <SFML/Graphics.hpp>
#include <PhysicsShape.h>

using namespace sf;
using namespace std;

class PhCircle : public PhysicsShape{
private:
	Color color;
	float X, Y;
	float dirX, dirY;

public:
	PhCircle(Color c, float X, float Y, float dirX, float dirY) :
		color(c), X(X), Y(Y), dirX(dirX), dirY(dirY) {}
	PhCircle(Color c, float X, float Y) :
		color(c), X(X), Y(Y), dirX(0.f), dirY(0.f) {}

};