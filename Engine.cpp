#include "Engine.h"
#include <cstdlib>
#include <ctime>

using namespace std;


Engine::Engine() {
    VideoMode vm = VideoMode::getDesktopMode();
    m_Window.create(vm, "Particle Fireworks + falling");
}
void Engine::run() {
    Clock clock;
    cout << "Starting Particle unit tests..." << endl;
    Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
    p.unitTests();
    cout << "Unit tests complete.  Starting engine..." << endl;
    while (m_Window.isOpen()) {
        float w = clock.restart().asSeconds();
        input();
        update(w);
        draw();
    }
}
void Engine::input() {
    Event event;
    while (m_Window.pollEvent(event)) {
        if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
            m_Window.close();
        }
        

        if (event.type == Event::MouseButtonPressed) {
            if (event.mouseButton.button == Mouse::Button::Left) {
                for (int i = 0; i < 5; ++i ) 
                {
                    Particle p(m_Window, 25 + rand() % 26, Vector2i(event.mouseButton.x, event.mouseButton.y) );
                    m_particles.push_back(p);
                }
            }
        }
    }
}

void Engine::update(float dtAsSeconds) {
  
    for (auto pPointer = m_particles.begin(); pPointer != m_particles.end();) {
        if (pPointer->getTTL() > 0.f){
           pPointer->update(dtAsSeconds);
           ++pPointer;
        }
        else {
            pPointer = m_particles.erase(pPointer);
            
        }
    }
}

void Engine::draw() {
    m_Window.clear();
    for (size_t i{}; i < m_particles.size(); ++i) {
        m_Window.draw(m_particles.at(i));
    }

    m_Window.display();
}
