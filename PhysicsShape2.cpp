// PhysicsShape.cpp

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class PhysicsShape2{
private:
	Shape& PhShape;
	Color color;
	float dirX, dirY;
	float deceleration = 0.f;
public:
	PhysicsShape2(Shape& s, Color c, float dirX, float dirY) :
		PhShape(s), color(c), dirX(dirX), dirY(dirY){
		s.setFillColor(c);
	}
	PhysicsShape2(Shape& s, Color c) : PhysicsShape2(s,c,0.f,0.f){}
	void setColor(Color c) {
		color = c;
		PhShape.setFillColor(color);
	}
	
	void setPosition(float X, float Y) {
		PhShape.setPosition(X, Y);
	}
	void update() {
		float X = PhShape.getPosition().x + dirX;
		float Y = PhShape.getPosition().y + dirY;
		dirX - deceleration;
		dirY - deceleration;
		setPosition(X, Y);
	}
};

class PhCircle : public PhysicsShape2 {
private:
	float r;
public:
	PhCircle(CircleShape s, Color c, float r, float dirX, float dirY) :
		PhysicsShape2(s, c, dirX, dirY), r(r) {
		s.setOrigin(r,r);
	}
	PhCircle(CircleShape s, Color c, float r) : PhCircle(s, c, r, 0.f, 0.f) {}
};

class PhCube : public PhysicsShape2 {
private:
	float l;
public:
	PhCube(RectangleShape s, Color c, float l, float dirX, float dirY) :
		PhysicsShape2(s, c, dirX, dirY), l(l) {
		s.setOrigin(l / 2, l / 2);
	}
	PhCube(RectangleShape s, Color c, float l) : PhCube(s, c, l, 0.f, 0.f) {}

};