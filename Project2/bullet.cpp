#include "bullet.h"

bullet::bullet() {
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;
    bullet_type_ = BLASTER;
}
bullet::~bullet() {

}

void bullet::loadImgBullet(SDL_Renderer* screen) {
    if (bullet_type_ == BLASTER) {
        string blaster_str = "PNG//Lasers//laserRed02.png";
        loadImg(blaster_str, screen);
    }
    
}

void bullet::HandleMoveSpaceShipBullet() {
    rect_.y += y_val_;
    if (rect_.y < -rect_.h) {
        is_move_ = false;
    }
}
void bullet::HandleMoveChickenBullet() {
    rect_.y += y_val_;
    if (rect_.y > SCREEN_HEIGHT) {
        is_move_ = false;
    }
}