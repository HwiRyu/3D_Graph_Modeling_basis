//필요한 기능
//1. 드래그로 화면 이동
//2. 현재 보고있는 수직선을 기준으로 화면 회전
//3. 현재 보고있는 (x,y)를 기준으로 화면 확대
//4. metal을 사용한 gpu 가속
//5. 시간 조정


#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include "rotation.h"
#include "scale.h"
#include "function.h"
#include "draw.h"
#include "axes.h"



const int width = 1500;
const int height = 800;


int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "Basis");

    sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/Monaco.ttf")) {
        std::cerr << "Error loading font\n";
        return 1;
    }
    sf::VertexArray one(sf::LineStrip);

    sf::View graphView(sf::FloatRect(-width / 2, -height / 2, width, height));
    sf::View initial_View(sf::FloatRect(-width / 2, -height / 2, width, height));

    window.setView(graphView);

    double size = 30;
    double x_angle = -10;
    double y_angle = 5;
    const double Pi = 3.14159265358979;
    bool domain_fixed = true;
    double current_center_x;
    double current_center_y;


    sf::Clock clock;
    sf::Clock clock_t;

    sf::Time lastClickTime = sf::Time::Zero;
    sf::Vector2i lastMousePos;
    sf::Vector2f startPoint;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Up:
                        y_angle -= 1;
                        break;
                    case sf::Keyboard::Down:
                        y_angle += 1;
                        break;
                    case sf::Keyboard::Left:
                        x_angle += 1;
                        break;
                    case sf::Keyboard::Right:
                        x_angle -= 1;
                        break;
                    case sf::Keyboard::F:
                        if (domain_fixed)
                            domain_fixed = false;
                        else
                            domain_fixed = true;
                        break;
                    case sf::Keyboard::W:
                        size += 5;
                        break;
                    case sf::Keyboard::S:
                        size -= 5;
                        break;
                    default:
                        break;
                }
                size = (size < 5) ? 5 : ((size > 200.0) ? 200.0 : size);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Time currentTime = clock.getElapsedTime();
                    lastMousePos = sf::Mouse::getPosition(window);
                    startPoint = window.mapPixelToCoords(lastMousePos); // 초기 클릭 위치 저장
                    lastClickTime = currentTime;
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f delta = window.mapPixelToCoords(newMousePos) - window.mapPixelToCoords(lastMousePos);
                    graphView.move(-delta); // 시야 이동
                    window.setView(graphView); // 뷰 설정 업데이트
                    lastMousePos = newMousePos; // 새로운 마우스 위치 저장

                }
            }


            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        // Drawing code here


        // theta, pi에 관한 파트 ////////////////////////////////////////////////////////////////////////////////
        double pi = Pi * y_angle / 120;      //라디안 값
        double theta = Pi * (x_angle / 120);
        pi = std::fmod(pi, 2 * Pi);
        theta = std::fmod(theta, 2 * Pi);

        if (pi < 0) {
            pi += 2 * Pi;
        }
        if (theta < 0) {
            theta += 2 * Pi;
        }

        double view_center_x = graphView.getCenter().x / (size);
        double view_center_y = -graphView.getCenter().y / (size);
        double center_x = -view_center_x * sin(theta) - cos(theta) * view_center_y / sin(pi);
        double center_y = view_center_x * cos(theta) - sin(theta) * view_center_y / sin(pi);

        if (0.005 * Pi < pi && pi < 1.995 * Pi) {
            current_center_x = center_x;
            current_center_y = center_y;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        function_view(window, one, size, one_variable_function, current_center_x,current_center_y, theta, pi);
        zero_plane(window, one, size, current_center_x,current_center_y, theta, pi);
        window.display();

    }

    return 0;
}