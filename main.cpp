#include <SFML/Graphics.hpp>
#include <time.h>

float calc_scale(const sf::Vector2f &s1,const sf::Vector2f &s2){
    return s1.x/s2.x > s1.y/s2.y ? s1.y/s2.y:s1.x/s2.x;
}

float lerp(float v0, float v1, float t) {
  return (1 - t) * v0 + t * v1;
}

sf::Vector2f lerp_vec(sf::Vector2f v0, sf::Vector2f v1, float t) {
  return sf::Vector2f(lerp(v0.x,v1.x,t),lerp(v0.y,v1.y,t));
}

sf::Vector2f calc_img_size(const sf::Vector2f&v,const float &s){
    return sf::Vector2f(v.x * s, v.y * s);
}

void make_image_inside_screen(sf::Vector2f&cam_pos,const sf::Vector2f&img_s,const sf::RenderWindow&w){
    if(cam_pos.x > img_s.x/2.0){cam_pos.x = img_s.x/2.0;}
    if(cam_pos.y > img_s.y/2.0){cam_pos.y = img_s.y/2.0;}

    if(cam_pos.x < w.getSize().x - img_s.x/2.0){cam_pos.x = w.getSize().x - img_s.x/2.0;}
    if(cam_pos.y < w.getSize().y - img_s.y/2.0){cam_pos.y = w.getSize().y - img_s.y/2.0;}

    if(w.getSize().x >= img_s.x ){
        cam_pos.x = w.getSize().x/2.0; 
    }

    if(w.getSize().y >= img_s.y){
        cam_pos.y = w.getSize().y/2.0;
    }
}

int main(int argc, char **argv){

    sf::RenderWindow window;
    sf::Texture image;
    sf::Vector2f img_size;
    sf::Vector2f mouse_position,old_mouse_position,camera_pos,lerp_cam_pos;
    float scale = 1.0,lerp_scale = 1.0;
    double start,frame_time;
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
    sprite.setOrigin(sf::Vector2f(image.getSize().x/2.0,image.getSize().y/2.0));

    img_size = calc_img_size(sf::Vector2f(image.getSize()),scale);
    make_image_inside_screen(camera_pos,img_size,window);

    lerp_cam_pos = camera_pos;
    lerp_scale = scale;

    while(window.isOpen()){
        
        start = clock();
        moved = 0;

        old_mouse_position = mouse_position;
        mouse_position = sf::Vector2f(sf::Mouse::getPosition(window));

        sf::Event event;

        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){window.close();}

            if(event.type == sf::Event::Resized){
                window.setView(sf::View(sf::FloatRect(0,0,event.size.width,event.size.height)));
                scale = calc_scale(sf::Vector2f(window.getSize()),sf::Vector2f(image.getSize()));
            }

            if(event.type == sf::Event::MouseMoved){moved = 1;}

            if(event.type == sf::Event::MouseWheelScrolled){

                sf::Vector2f fac((camera_pos.x - mouse_position.x) / (image.getSize().x*scale),( camera_pos.y -  mouse_position.y) / (image.getSize().y*scale));

                scale += scale * 0.2 * event.mouseWheelScroll.delta;

                camera_pos.x = (fac.x * (image.getSize().x*scale)) + (mouse_position.x);
                camera_pos.y = (fac.y * (image.getSize().y*scale)) + (mouse_position.y);
            }
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && moved){
            camera_pos += mouse_position - old_mouse_position;
            lerp_cam_pos = camera_pos;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
               scale = calc_scale(sf::Vector2f(window.getSize()),sf::Vector2f(image.getSize()));
        }
         if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && sf::Keyboard::isKeyPressed(sf::Keyboard::O)){
               scale = 1.0;
        }

        img_size = calc_img_size(sf::Vector2f(image.getSize()),lerp_scale);
        make_image_inside_screen(camera_pos,img_size,window);

        lerp_scale = lerp(lerp_scale,scale,frame_time);
        lerp_cam_pos = lerp_vec(lerp_cam_pos,camera_pos,frame_time);

        sprite.setScale(sf::Vector2f(lerp_scale,lerp_scale));
        sprite.setPosition(lerp_cam_pos);

        window.clear();
        window.draw(sprite);
        window.display();

        frame_time = (clock() - start)/1000.0;
    }

    return 0;
}