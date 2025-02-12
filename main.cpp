// main.cpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "PhysicsShape.h"
#include <direct.h>
#include <windows.h>

using namespace sf;
using namespace std;

static float VIEW_WIDTH = 200.f, VIEW_HEIGHT = 1024.f;
void ResizeView(const RenderWindow& window, View& view) {
    float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
    view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT);
}
void moveCamera(View& view, float* X, float* Y, float deltaX, float deltaY) {
    *X += deltaX; *Y += deltaY;
    view.setCenter(*X, *Y);
}

void resetSimulation(float l, float mass, list<PhysicsShape*>& SHAPES) {
    // Free memory before clearing the list
    for (PhysicsShape* shape : SHAPES) {
        delete shape;
    }
    SHAPES.clear();

    //PhCube cube(l, Color(124,0,120), mass, -100.f,100.f);
    float force_factor = 26000;
    PhCircle* circleRed = new PhCircle(l * 1.25, Color(227, 34, 76), mass * 5, -500, 0.f, false, 0.f, -force_factor);
    PhCircle* circleGreen = new PhCircle(l, Color(124, 255, 12), mass * 5, 500.f, 0.f, false, 0.f, force_factor);
    PhCircle* circlePurple = new PhCircle(l * 1.5, Color(91, 14, 235), mass * 5, 0.f, 500.f, false, -force_factor, 0.f);
    PhCircle* circleBlue = new PhCircle(l, Color(33, 70, 255), mass * 5, 0.f, -500.f, false, force_factor, 0.f); //91, 14, 235
    PhCircle* circleYellow = new PhCircle(l * 5, Color(255, 239, 69), mass * 50, 0.f, 0.f, true, 0.f, 0.f);
    //PhCircle circleStatic(5, Color(225, 33, 255), mass*50, 0.f, 0.f, true);
    //SHAPES.push_back(&cube);
    SHAPES.push_back(circleRed);
    SHAPES.push_back(circleGreen);
    SHAPES.push_back(circleBlue);
    SHAPES.push_back(circleYellow);
    SHAPES.push_back(circlePurple);
    //SHAPES.push_back(&circleStatic);
    /*PhCircle* circleRed1 = new PhCircle(l, Color(227, 34, 76), mass, 1000.f, 1000.f, false, 0.f, 3.e3f); //2.5
    PhCircle* circleRed2 = new PhCircle(l, Color(227, 34, 76), mass, -1000.f, 0.f, false, 0.f, -3.e3f); //2.5
    PhCircle* circleRed3 = new PhCircle(l, Color(227, 34, 76), mass, -1000.f, -1000.f, false, 3.e3f, -3.e3f); //2.5
    PhCircle* circleRed = new PhCircle(l, Color(227, 34, 76), mass, -1000.f, 0.f, false, 0.f, -3.e3f); //2.5
    PhCircle* circleBlue = new PhCircle(l, Color(33, 70, 255), mass, 1000.f, 0.f, false, 0.f, 3.e3f); //2.5
    PhCircle* circlePurple = new PhCircle(l * 3, Color(91, 14, 235), mass * 5, -450.f, -520.f, false, -3.e4f, 0.f);
    PhCircle* circleYellow = new PhCircle(l*6, Color(255, 239, 69), mass * 500, 0.f, 0.f, true, 0.f, 0.f);
    SHAPES.push_back(circleRed1);
    SHAPES.push_back(circleRed2);
    SHAPES.push_back(circleRed3);
    SHAPES.push_back(circleRed);
    SHAPES.push_back(circleBlue);
    SHAPES.push_back(circlePurple);
    SHAPES.push_back(circleYellow);*/
}

void addCircle(float l, float mass, list<PhysicsShape*>& SHAPES) {

}

int main()
{
    //================ Create window ======================================================
    VideoMode windowedMode(VIEW_HEIGHT, VIEW_HEIGHT);
    //RenderWindow window(windowedMode, "Circle physics"); //, Style::Close | Style::Titlebar /* | Style::Resize */ 
    RenderWindow window(VideoMode::getDesktopMode(), "Circle physics", Style::Fullscreen);
    View view(Vector2f(0.f, 0.f), Vector2f(VIEW_HEIGHT, VIEW_HEIGHT));
    float initialZoom = 3.f;
    //================ Variables ======================================================
    float zoomRatioX = view.getSize().x / VIEW_HEIGHT;
    float zoomRatioY = view.getSize().y / VIEW_HEIGHT;
    float deltaTime = 0.f, SimulationSpeed = 0.25f;
    Clock clock;
    float posX = 0.f, posY = 0.f, cameraSpeedWASD = 2.f, cameraSpeedMOUSE = 5.f, deltaX, deltaY;
    Vector2i previousMousePosition;
    bool isFullscreen = true;
    bool collisions = false;
    //================ TextUI ======================================================
    list<Text*> TEXTS = {};
    Color TextColor(72, 144, 217);
    Font font; 
    float textSize = 0;

    if (!font.loadFromFile("Fonts/Montserrat-Bold.ttf"))
        return -1;//throw("COULD NOT LOAD FONT!");
    Text SimulationSpeedText("Simulation speed: x" + to_string(SimulationSpeed), font, textSize);
    Text PressQToText("Q speed up", font, textSize);
    Text PressEToText("E slow down", font, textSize);
    /*char buffer[256];
    _getcwd(buffer, sizeof(buffer));
    std::cout << "Directorio de trabajo actual: " << buffer << std::endl;*/
    SimulationSpeedText.setFillColor(TextColor);
    PressQToText.setFillColor(TextColor);
    PressEToText.setFillColor(TextColor);
    //SimulationSpeedText.setPosition(10.f, 10.f);
    TEXTS.push_back(&SimulationSpeedText);
    TEXTS.push_back(&PressQToText);
    TEXTS.push_back(&PressEToText);
    //================ Shapes ======================================================

    list<PhysicsShape*> SHAPES = {};
    float l = 20; float mass = 2.5f;
    resetSimulation(l, mass, SHAPES);

    //================ Full screnn ======================================================
    /*
    isFullscreen = true;
    window.close();
    */
    if (isFullscreen) {
        window.create(VideoMode::getDesktopMode(), "Circle Physics", Style::Fullscreen);
        view.setSize(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);
    }
    else {
        window.create(VideoMode(VIEW_HEIGHT, VIEW_HEIGHT), "Circle Physics", Style::Close | Style::Titlebar);
        view.setSize(VIEW_HEIGHT, VIEW_HEIGHT);
    }

    window.setView(view);
    
    view.zoom(initialZoom);

    //================ WINDOW MANAGEMENT ======================================================
    while (window.isOpen()){
        // Deltatime --------------------------------------------
        deltaTime = clock.restart().asSeconds() * SimulationSpeed;
        zoomRatioX = view.getSize().x / VIEW_HEIGHT;
        zoomRatioY = view.getSize().y / VIEW_HEIGHT;
        // EVENTS --------------------------------------------
        Event event;
        while (window.pollEvent(event)){
            switch (event.type){
                case Event::Closed: window.close();
                    break;
                case Event::Resized: printf("Size: %i, %i\n", event.size.width, event.size.height);
                    ResizeView(window, view);
                    break;

                /*case Event::TextEntered: event.text.unicode*/
                case Event::MouseWheelScrolled:
                    if ((event.mouseWheelScroll.delta > 0 && zoomRatioX >= 0.01) ||
                        (event.mouseWheelScroll.delta < 0 && zoomRatioX <= 35))
                        view.zoom(1.f - event.mouseWheelScroll.delta*0.1f);
                    break;
                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left) {
                        // Random color generation (bright)
                        Color randomColor(rand() % 256, rand() % 128 + 128, rand() % 128 + 128);

                        // Random scale for l and mass (0.75 to 1.25 times)
                        float randomScale = 0.75f + (rand() % 51) / 100.0f;
                        float newL = l * randomScale * 2;
                        float newMass = mass * randomScale * 5;
                        float r1 = (rand() % 2001 - 1000) / 1000.0f;
                        float r2 = (rand() % 2001 - 1000) / 1000.0f;
                        float force_factor = 5000;

                        // Convert mouse position to world coordinates
                        Vector2f worldPos = window.mapPixelToCoords(Mouse::getPosition(window), view);

                        // Create and add new circle to SHAPES
                        SHAPES.push_back(new PhCircle(newL, randomColor, newMass, worldPos.x, worldPos.y, false, r1*force_factor, r2*force_factor));
                    } else if (event.mouseButton.button == Mouse::Right) {
                        // Random color generation (bright)
                        Color randomColor(rand() % 256, rand() % 200 + 55, rand() % 200 + 55);

                        // Random scale for l and mass (0.75 to 1.25 times)
                        float randomScale = 0.75f + (rand() % 51) / 100.0f;
                        float newL = l * randomScale * 2;
                        float newMass = mass * randomScale * 10;
                        float r1 = (rand() % 2001 - 1000) / 1000.0f;
                        float r2 = (rand() % 2001 - 1000) / 1000.0f;
                        float force_factor = 5000;

                        // Convert mouse position to world coordinates
                        Vector2f worldPos = window.mapPixelToCoords(Mouse::getPosition(window), view);

                        // Create and add new circle to SHAPES
                        SHAPES.push_back(new PhCircle(newL, randomColor, newMass, worldPos.x, worldPos.y, true, r1 * force_factor, r2 * force_factor));
                    }
                    break;
                case Event::MouseMoved:
                    /*if (Mouse::isButtonPressed(Mouse::Left)) {
                        Vector2i currentMousePosition = Mouse::getPosition(window);
                        Vector2i deltaMouse = currentMousePosition - previousMousePosition;
                        moveCamera(view, &posX, &posY, -(float)deltaMouse.x * zoomRatioX,
                                                       -(float)deltaMouse.y * zoomRatioY);
                        previousMousePosition = currentMousePosition;
                    }*/
                    break;
                case Event::KeyPressed: 
                    switch (event.key.code){
                        case sf::Keyboard::Escape:
                            window.close();
                            break;

                        case sf::Keyboard::Q:
                            if (SimulationSpeed < 1.f)
                                SimulationSpeed = SimulationSpeed + 0.25f;
                            else if (SimulationSpeed <= 14.f)
                                SimulationSpeed = SimulationSpeed + 1.f;
                            SimulationSpeedText.setString("Simulation speed: x" + to_string(SimulationSpeed));
                            break;

                        case sf::Keyboard::E:
                            if (SimulationSpeed > 1.f)
                                SimulationSpeed = SimulationSpeed - 1.f;
                            else if(SimulationSpeed > 0.f)
                                SimulationSpeed = SimulationSpeed - 0.25f;
                            SimulationSpeedText.setString("Simulation speed: x" + to_string(SimulationSpeed));
                            break;
                        case sf::Keyboard::R:
                            resetSimulation(l, mass, SHAPES);
                            view.setCenter(Vector2f(0.f,0.f));
                            //view.setSize(VIEW_HEIGHT * zoomRatioX, VIEW_HEIGHT);
                            //view.zoom(initialZoom);
                            break;
                        case sf::Keyboard::F:
                            SHAPES.clear();
                            break;
                        case sf::Keyboard::C:
                            collisions = !collisions;
                            break;

                        case sf::Keyboard::F11:
                            isFullscreen = !isFullscreen;
                            window.close();

                            if (isFullscreen) {
                                window.create(VideoMode::getDesktopMode(), "Circle Physics", Style::Fullscreen);
                                view.setSize(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);
                            }
                            else {
                                window.create(VideoMode(VIEW_HEIGHT, VIEW_HEIGHT), "Circle Physics", Style::Close | Style::Titlebar);
                                view.setSize(VIEW_HEIGHT, VIEW_HEIGHT);
                            }

                            window.setView(view);
                            break;
                    }
                    
                default:
                    break;
            }
        }

        // Movement --------------------------------------------
        if (Keyboard::isKeyPressed(Keyboard::Key::Z)) view.zoom(0.99f);
        if (Keyboard::isKeyPressed(Keyboard::Key::X)) view.zoom(1.01f);
        deltaX = 0.f; deltaY = 0.f;
        if (Keyboard::isKeyPressed(Keyboard::Key::A)) deltaX -= cameraSpeedWASD * zoomRatioX;
        if (Keyboard::isKeyPressed(Keyboard::Key::D)) deltaX += cameraSpeedWASD * zoomRatioX;
        if (Keyboard::isKeyPressed(Keyboard::Key::W)) deltaY -= cameraSpeedWASD * zoomRatioY;
        if (Keyboard::isKeyPressed(Keyboard::Key::S)) deltaY += cameraSpeedWASD * zoomRatioY;
        moveCamera(view, &posX, &posY, deltaX, deltaY);

        // TextDisplay --------------------------------------------
        Vector2f cornerPosition = view.getCenter() - view.getSize() / 2.0f;
        SimulationSpeedText.setPosition(cornerPosition);
        SimulationSpeedText.setCharacterSize(textSize * view.getSize().x / VIEW_HEIGHT);
        cornerPosition.y += (textSize * view.getSize().x / VIEW_HEIGHT) * 1.5;
        PressQToText.setPosition(cornerPosition);
        PressQToText.setCharacterSize(textSize * view.getSize().x / VIEW_HEIGHT);
        cornerPosition.y += (textSize * view.getSize().x / VIEW_HEIGHT) * 1.3;
        PressEToText.setPosition(cornerPosition);
        PressEToText.setCharacterSize(textSize * view.getSize().x / VIEW_HEIGHT);

        // Object Rendering --------------------------------------------
        window.clear();
        //view.setCenter(cube.getShape()->getPosition());
        window.setView(view);
        for (PhysicsShape* s : SHAPES) s->update(deltaTime, SHAPES, SimulationSpeed, collisions);
        for (PhysicsShape* s : SHAPES) {
            window.draw(*(s->getShape()));
            list<CircleShape> trail = s->getTrail();
            for (const CircleShape& p : trail) {
                window.draw(p);
            }
        }
        for (Text* t : TEXTS) window.draw(*t);


        window.display();
    }

    return 0;
}
