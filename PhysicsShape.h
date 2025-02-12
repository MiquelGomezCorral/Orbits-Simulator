// PhysicsShape.cpp

#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <list>

using namespace sf;
using namespace std;

Color BASECOLOR(72, 144, 217);
float G = 1.0e2f; //Normal
//float G = 1.0e-2f; //Normaliced
float PI = (float) M_PI;
float AirFriction = 0.000f;
float Deceleration = 0.0f;
float zero = 1e-10;

// Función para normalizar un vector 2D
Vector2f normalize(const Vector2f& v) {
	float length = std::sqrt(v.x * v.x + v.y * v.y);
	if (length != 0) {
		return { v.x / length, v.y / length };
	}
	return { 0.f, 0.f };
}
// Función para calcular el producto punto entre dos vectores 2D
float dot(const Vector2f& v1, const Vector2f& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}


class PhysicsShape {
private:
	Shape& PhShape;
	Color color, auxCol;
	float Mass;
	float Area;
	Vector2f DeltaDir;
	bool Stationary;
	//list<Vertex> Trail = {};
	list<CircleShape> Trail = {};
	float TrailSize = 3.5f;
	float maxTrailLength = 400;

public:
	PhysicsShape(Shape& shape, float or1, float or2, Color color = BASECOLOR, float mass = 1.f, float area = 1, float posX = 0.f, float posY = 0.f, bool stationary = false, float dirX = 0.f, float dirY = 0.f) :
		PhShape(shape), color(color), Mass(mass), Stationary(stationary), Area(area) {
		DeltaDir = Vector2f(dirX, dirY);
		shape.setFillColor(color);
		auxCol = Color(color.r, color.g, color.b, 128);
		shape.setOrigin(or1,or2);
		setPosition(posX, posY);
		if (Mass == 0.f) Mass = zero;
	}
	virtual void printType() {
		std::cout << "PhysicsShape" << std::endl;
	}
	
	Shape* getShape() {
		return &PhShape;
	}
	float getMass() {
		return Mass;
	}
	Vector2f getDirection() {
		return DeltaDir;
	}
	float getSpeed(){
		return sqrt(DeltaDir.x * DeltaDir.x + DeltaDir.y * DeltaDir.y);
	}
	void setTrailSize(float size) {
		TrailSize = size;
	}

	void setColor(Color c) {
		color = c;
		PhShape.setFillColor(color);
	}
	void setPosition(float X, float Y) {
		PhShape.setPosition(X, Y);
	}
	list<CircleShape> getTrail() {
		return Trail;
	}
	void move(float X, float Y){
		DeltaDir.x += X; DeltaDir.y += Y;
		//DeltaDir = normalize(DeltaDir);
	}
	void update(float deltaTime, list<PhysicsShape*> Shapes, float SimulationSpeed, bool collisions) {
		if (Stationary) return;
		for (PhysicsShape* s : Shapes) {
			if (s == this) continue; //Himself
			Shape* aux = s->getShape();
			float r2 = sqrtf(powf(aux->getPosition().x - PhShape.getPosition().x, 2) +
							 powf(aux->getPosition().y - PhShape.getPosition().y, 2));
			if (r2 == 0.f) r2 = zero; //In case there are in the same spot
			float force = G * s->Mass * this->Mass * SimulationSpeed / r2; // Force that the other shape aplies to this shape

			float angle = atan2(aux->getPosition().y - PhShape.getPosition().y, aux->getPosition().x - PhShape.getPosition().x);
			float forceX = force * cos(angle);
			float forceY = force * sin(angle);
			// Normalice forces
			/*float forceMagnitude = sqrtf(forceX * forceX + forceY * forceY);
			if (forceMagnitude != 0) {
				forceX /= forceMagnitude;
				forceY /= forceMagnitude;
			}*/
			move(forceX, forceY);
		}
		for (PhysicsShape* s : Shapes) {
			if (!collisions) break;

			if (s == this) continue; //Himself
			CircleShape* externalShape = dynamic_cast<CircleShape*> (s->getShape());
			CircleShape* thisShape = dynamic_cast<CircleShape*> (this->getShape());
			if (!externalShape) continue; //Not a Circle
			float r2 = sqrtf(powf(externalShape->getPosition().x - PhShape.getPosition().x, 2) +
							 powf(externalShape->getPosition().y - PhShape.getPosition().y, 2));
			// If distance more radius skip, dont colide
			if (r2 > thisShape->getRadius() + externalShape->getRadius()) continue;

			// Compute colision -----------------------------------------
			// Vector bewtween centers normalized
			Vector2f collisionVectorNormalized = normalize(PhShape.getPosition() - externalShape->getPosition());
			// compute proyection of original velocity over noraml vector
			float dotProduct = dot(DeltaDir, collisionVectorNormalized);
			Vector2f projection = dotProduct * collisionVectorNormalized;
			DeltaDir -= 2.f * projection;
			// Push bewteen circles
			//float pushFactor = (this->getMass() * this->getSpeed() + s->getMass() * s->getSpeed()) / (this->getMass() + s->getMass());
			// Actualiza la velocidad con el empuje
			DeltaDir += collisionVectorNormalized; //* pushFactor;
		}

		// New position
		Vector2f newPos = PhShape.getPosition() + DeltaDir * deltaTime / Mass;
		setPosition(newPos.x, newPos.y);
		// Cuadratic deceleration
		DeltaDir.x = (abs(DeltaDir.x) > Deceleration) ? DeltaDir.x - (Deceleration * DeltaDir.x + sqrtf(Area) * AirFriction * DeltaDir.x) / abs(DeltaDir.x) : 0;
		DeltaDir.y = (abs(DeltaDir.y) > Deceleration) ? DeltaDir.y - (Deceleration * DeltaDir.y + sqrtf(Area) * AirFriction * DeltaDir.y) / abs(DeltaDir.y) : 0;
		// Update trail
		updateTrail();
	}
	void updateTrail() {
		//Vertex point(Vector2f(PhShape.getPosition().x, PhShape.getPosition().y), auxCol);
		//auxCol = Color(color.r, color.g, color.b, 128);
		CircleShape point(TrailSize);
		point.setFillColor(auxCol);
		point.setPosition(Vector2f(PhShape.getPosition().x, PhShape.getPosition().y));
		Trail.push_front(point);
		if (Trail.size() >= maxTrailLength) {
			Trail.pop_back();
		}
	}
	
};

class PhCircle : public PhysicsShape {
private:
	float radius;
public:
	PhCircle(float radius, Color color = BASECOLOR, float mass = 1.f, float posX = 0.f, float posY = 0.f, bool stationary = false, float dirX = 0.f, float dirY = 0.f) :
		PhysicsShape(*new CircleShape(radius), radius, radius, color, mass, PI*radius*radius, posX, posY, stationary,
					dirX, dirY), radius(radius) {
		//setTrailSize(radius/15);
	}
	PhCircle(float radius, float mass, float posX, float posY) : PhCircle(radius, BASECOLOR, mass, posX, posY) {}
	PhCircle(float radius, float mass) : PhCircle(radius, BASECOLOR, mass) {}
	PhCircle(float radius, float mass, bool stationary) : PhCircle(radius, BASECOLOR, mass, 0.f, 0.f, stationary) {}
	float getRadius() {
		return radius;
	}
};

class PhCube : public PhysicsShape {
private:
	float large;
public:
	PhCube(float large, Color color = BASECOLOR, float mass = 1.f, float posX=0.f, float posY=0.f, bool stationary = false, float dirX = 0.f, float dirY = 0.f) :
		PhysicsShape(*new RectangleShape(Vector2f(large,large)), large / 2, large / 2, color, mass, large*large, posX, posY, stationary,
					 dirX, dirY), large(large) {}
	PhCube(float large, float mass, float posX, float posY) : PhCube(large, BASECOLOR, mass, false, posX, posY) {}
	PhCube(float large, float mass) : PhCube(large, BASECOLOR, mass, false) {}
	PhCube(float large, float mass, bool stationary) : PhCube(large, BASECOLOR, mass, 0.f, 0.f, stationary) {}
	float getLarge() {
		return large;
	}
};