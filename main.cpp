#include "SFML/Graphics.hpp"
#include <random>

typedef std::mt19937 Rng;

struct Particle {
    float x;
    float y;
    float vx;
    float vy;
};


void line(sf::VertexArray &vertices, Particle &p1, Particle &p2, float particleSize);

int main() {
    sf::RenderWindow window(sf::VideoMode(800 * POINT_RATIO, 600 * POINT_RATIO), "Polycons: particle constellations",
                            sf::Style::Default, sf::ContextSettings(0, 0, 16, 1, 1));
    window.setFramerateLimit(60);

#ifdef MAX_PARTICLES
    const int maxParticles = MAX_PARTICLES;
#else
    const int maxParticles = 40;
#endif

#ifndef POINT_RATIO
#define POINT_RATIO 1
#endif

#ifdef PARTICLE_SIZE
    const auto particleSize = PARTICLE_SIZE * POINT_RATIO;
#else
    const auto particleSize = 4 * POINT_RATIO;
#endif

#ifdef THRESHOLD
    const auto threshold = THRESHOLD * POINT_RATIO;
#else
    const auto threshold = 100 * POINT_RATIO;
#endif

    Particle particles[maxParticles];

    std::random_device rd;
    Rng rng(rd());
    auto windowSize = window.getSize();
    std::uniform_real_distribution<float> widthDistribution(0, windowSize.x);
    std::uniform_real_distribution<float> heightDistribution(0, windowSize.y);
    std::uniform_real_distribution<float> velocityDistribution(-2.0, 2.0);
    for (auto &particle : particles) {
        particle = Particle{
                .x = widthDistribution(rng),
                .y = heightDistribution(rng),
                .vx = velocityDistribution(rng),
                .vy = velocityDistribution(rng),
        };
    }

    while (window.isOpen()) {
        sf::Event event; // NOLINT(cppcoreguidelines-pro-type-member-init)
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window to a blank white background
        window.clear(sf::Color(0, 0, 0, 0));

        // Plan drawing
        sf::VertexArray lines(sf::Lines);
        sf::RectangleShape particleRects[maxParticles];
        for (int i = 0; i < maxParticles; ++i) {
            Particle *current = &particles[i];

            // Plan the drawing of the current particle
            sf::RectangleShape rect(sf::Vector2f(particleSize, particleSize));
            rect.setPosition(current->x, current->y); // copy the position from the struct
            rect.setFillColor(sf::Color(255, 255, 255, 255)); // black
            particleRects[i] = rect;

            // Search other particles for possible lines
            for (int j = 0; j < maxParticles; ++j) {
                if (i != j) {
                    Particle *other = &particles[j];

                    // Find the distance between the particles
                    float distanceX = abs(current->x - other->x);
                    float distanceY = abs(current->y - other->y);

                    // If particles are close enough, draw a line
                    if (distanceX < threshold && distanceY < threshold) {
                        line(lines, *current, *other, static_cast<float>(particleSize));
                    }
                }
            }
        }

        // Draw to window
        window.draw(lines);
        for (const auto &particle : particleRects) {
            window.draw(particle);
        }

        // Display the current frame
        window.display();

        // Step particle world
        for (auto &particle : particles) {
            // Step the particles' positions by their velocities
            particle.x += particle.vx;
            particle.y += particle.vy;

            // Bounce off the walls
            if (particle.x > windowSize.x - particleSize || particle.x <= 0) {
                particle.vx = -particle.vx;
            }
            if (particle.y > windowSize.y - particleSize || particle.y <= 0) {
                particle.vy = -particle.vy;
            }
        }
    }

    return 0;
}

void line(sf::VertexArray &vertices, Particle &p1, Particle &p2, float particleSize) {
    vertices.append(sf::Vertex(sf::Vector2f(p1.x + particleSize / 2, p1.y + particleSize / 2))); // p1
    vertices.append(sf::Vertex(sf::Vector2f(p2.x + particleSize / 2, p2.y + particleSize / 2))); // p2
}
