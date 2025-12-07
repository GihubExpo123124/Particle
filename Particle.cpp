#include "Particle.h"
#include <SFML/System.hpp>
using namespace std;

SoundBuffer Particle::explosionBuffer;

bool bufferWorks = Particle::explosionBuffer.loadFromFile("explosion.wav");

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) : m_A(2, numPoints), 
                                                                                       m_ttl(TTL), 
                                                                                      m_numPoints(numPoints),
                                                                                       m_radiansPerSec((float)rand() / RAND_MAX * M_PI),
                                                                                       m_vx(100 + rand() % 401), m_vy(100 + rand() % 401)
                                                                                

{
    if (!bufferWorks) {
        cerr << "The audio file failed to load" << std::endl;
    }
    else {
        explosionSound.setBuffer(explosionBuffer);
    }
   
   

  m_hue1 = static_cast<float>(rand() % 360);
  m_hue2 = static_cast<float>(rand() % 360);
  m_saturation1 = 1.0f;
  m_saturation2 = 1.0f;
   m_value1 = 1.0f;
  m_value2 = 1.0f;
  m_color1 = hsvToRgb(m_hue1, m_saturation1, m_value1);
    m_color2 = hsvToRgb(m_hue2, m_saturation2, m_value2);
    
    
   
    m_cartesianPlane.setCenter(0, 0);
    m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);
    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);
    
  float theta = (float)rand() / RAND_MAX * (M_PI / 2);
  float dTheta = 2 * M_PI / (numPoints - 1);
  for (int j = 0; j < numPoints; ++j) {
      float r = 20 + rand() % 61, dx, dy;
      dx = r * cos(theta);
      dy = r * sin(theta);
      m_A(0, j) = m_centerCoordinate.x + dx;
      m_A(1, j) = m_centerCoordinate.y + dy;
      theta += dTheta;
  }
}

void Particle::draw(RenderTarget& target, RenderStates states) const {
    VertexArray lines(TriangleFan, m_numPoints + 1);
   
    Vector2f center(target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane));
    lines[0].position = center;
    lines[0].color = m_color1;

    for (int j = 1; j < m_numPoints + 1; j++ ) {
        Vector2i temp = target.mapCoordsToPixel(Vector2f(m_A(0, j - 1), m_A(1, j - 1) ), m_cartesianPlane);
        lines[j].position = Vector2f((float)temp.x, float(temp.y)); // Test comment
        lines[j].color = m_color2;
    }
   
    target.draw(lines);
}

void Particle::update(float DT) {
    

    m_ttl -= DT;
    rotate(DT * m_radiansPerSec);
    scale(SCALE);
    float dx{m_vx*DT}, dy;
    m_vy -= G * DT;
    dy = m_vy * DT;
    translate(dx, dy);
    m_hue1 += DT * 60.f;
    m_hue2 += DT * 60.f;

        if (!explosionHappened) {
            explosionSound.play();
            explosionHappened = true;
        }

if (m_hue1 > 360.f) m_hue1 -= 360.f;
if (m_hue2 > 360.f) m_hue2 -= 360.f;


m_color1 = hsvToRgb(m_hue1, m_saturation1, m_value1);
m_color2 = hsvToRgb(m_hue2, m_saturation2, m_value2);
}

void Particle::translate(double xShift, double yShift) {
    TranslationMatrix T(xShift, yShift, m_A.getCols());
    m_A = T + m_A;
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}

void Particle::rotate(double theta) {
    Vector2f temp(m_centerCoordinate);
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    RotationMatrix R(theta);
    m_A = R *m_A;
    translate(temp.x, temp.y);
}

void Particle::scale(double c) {
    Vector2f temp(m_centerCoordinate);
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    ScalingMatrix S(c);
    m_A = S * m_A;
    translate(temp.x, temp.y);


}


bool Particle::almostEqual(double a, double b, double eps)
{
    return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    std::cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        std::cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        std::cout << "Failed." << endl;
    }

    std::cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        std::cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        std::cout << "Failed." << endl;
    }

    std::cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        std::cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        std::cout << "Failed." << endl;
    }


    std::cout << "Testing Particles..." << endl;
    std::cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        std::cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        std::cout << "Passed.  +1" << endl;
        score++;
    }

    std::cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            std::cout << "Failed mapping: ";
            std::cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        std::cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        std::cout << "Failed." << endl;
    }

    std::cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0, j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            std::cout << "Failed mapping: ";
            std::cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        std::cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        std::cout << "Failed." << endl;
    }

    std::cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            std::cout << "Failed mapping: ";
            std::cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        std::cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        std::cout << "Failed." << endl;
    }

    std::cout << "Score: " << score << " / 7" << endl;
}
void rgbToHsv(const sf::Color& c, float& h, float& s, float& v) {
  float r = c.r / 255.f;
  float g = c.g / 255.f;
  float b = c.b / static_cast<float>(255);
  float maxVal = r;
  if (g > maxVal) maxVal = g;
  if (b > maxVal) maxVal = b;
  float minVal = r;
  if (g < minVal) minVal = g;
  if (b < minVal) minVal = b;
  float delta = maxVal - minVal;
  v = maxVal;
  if (delta == 0.f) {
    h = 0.f;
    s = 0.f;
    return;
  }
   s = delta / maxVal;
  if (maxVal == r) {
    h = 60.f * ((g - b) / delta);
  } else if (maxVal == g) {
    h = 60.f * (((b - r) / delta) + 2.f);
  } else {
    h = 60.f * (((r - g) / delta) + 4.f);
  }
  if (h < 0) {
    h += 360.f;
  }
}
sf::Color hsvToRgb(float h, float s, float v) {
  float c = v * s;
  float hSection = h / 60.f;
  int section = static_cast<int>(hSection);
  float fraction = hSection - static_cast<float>(section);

  float r1 = 0.f;
  float g1 = 0.f;
  float b1 = 0.f;

  float x = c * (1 - std::abs((fraction * 2.f) - 1.f));

  if (section == 0) {
    r1 = c; g1 = x; b1 = 0;
  } else if (section == 1) {
    r1 = x; g1 = c; b1 = 0;
  } else if (section == 2) {
    r1 = 0; g1 = c; b1 = x;
  } else if (section == 3) {
    r1 = 0; g1 = x; b1 = c;
  } else if (section == 4) {
    r1 = x; g1 = 0; b1 = c;
  } else {
    r1 = c; g1 = 0; b1 = x;
  }
   float m = v - c;
  float r = r1 + m;
  float g = g1 + m;
  float b = b1 + m;
   return sf::Color(
    static_cast<sf::Uint8>(r * 255),
    static_cast<sf::Uint8>(g * 255),
    static_cast<sf::Uint8>(b * 255)
  );
}
