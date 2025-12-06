#include "Engine.h"
#include <cstdlib>
#include <ctime>
using namepsace std


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
        floar w = clock.restart().asSeconds();
        input();
        update(w);
        draw();
    }
}
void Engine::input() {
    Event event;
    while (m_Window.pollEvent(event)) {
        if (event.type == Event::Closed || event.type++)
    }
}