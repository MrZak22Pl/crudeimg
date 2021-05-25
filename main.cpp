#include <SFML/Graphics.hpp>

float calc_scale(const sf::Vector2f &s1,const sf::Vector2f &s2){
    return s1.x/s2.x > s1.y/s2.y ? s1.y/s2.y:s1.x/s2.x;
}

int main(int argc, char **argv){

    sf::RenderWindow window;
    sf::Texture image;
    sf::Vector2f img_size;
    sf::Vector2f mouse_position,old_mouse_position,camera_pos;
    float scale = 1.0;
    bool moved = 0;

    if(argc > 1){
        image.loadFromFile(argv[1]);
        scale = calc_scale(sf::Vector2f(1280,720),sf::Vector2f(image.getSize()));
        window.create(sf::VideoMode(image.getSize().x*scale,720),argv[1]);
    }else{
        window.create(sf::VideoMode(300,300),"No image!");
    }

    window.setFramerateLimit(60);

    sf::Sprite sprite(image);

    while(window.isOpen()){
        
        old_mouse_position = mouse_position;
        mouse_position = sf::Vector2f(sf::Mouse::getPosition(window));

        sf::Event event;

        moved = 0;

        while(window.pollEvent(event)){

            if(event.type == sf::Event::Resized){
                window.setView(sf::View(sf::FloatRect(0,0,event.size.width,event.size.height)));
                scale = calc_scale(sf::Vector2f(window.getSize()),sf::Vector2f(image.getSize()));
            }

            if(event.type == sf::Event::Closed){window.close();}
            if(event.type == sf::Event::MouseMoved){moved = 1;}

            if(event.type == sf::Event::MouseWheelScrolled){

                sf::Vector2f fac((camera_pos.x - window.getSize().x/2.0) / (image.getSize().x*scale),( camera_pos.y - window.getSize().y/2.0) / (image.getSize().y*scale));

                scale += scale*0.2*event.mouseWheelScroll.delta;

                camera_pos.x = (fac.x * (image.getSize().x*scale))+window.getSize().x/2.0;
                camera_pos.y = (fac.y * (image.getSize().y*scale))+window.getSize().y/2.0;
            }
        }

        img_size = sf::Vector2f(image.getSize().x * scale,image.getSize().y * scale);

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && moved){
            camera_pos += mouse_position - old_mouse_position;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
               scale = calc_scale(sf::Vector2f(window.getSize()),sf::Vector2f(image.getSize()));
            }
        }
        
        if(camera_pos.x > 0){camera_pos.x = 0;}
        if(camera_pos.y > 0){camera_pos.y = 0;}

        if(camera_pos.x < window.getSize().x - img_size.x){camera_pos.x = window.getSize().x - img_size.x;}
        if(camera_pos.y < window.getSize().y - img_size.y){camera_pos.y = window.getSize().y - img_size.y;}

        if(window.getSize().x >= img_size.x ){
            camera_pos.x = window.getSize().x/2.0 - img_size.x/2.0;
        }

        if(window.getSize().y >= img_size.y){
            camera_pos.y = window.getSize().y/2.0 - img_size.y/2.0;
        }

        sprite.setScale(sf::Vector2f(scale,scale));
        sprite.setPosition(camera_pos);

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}