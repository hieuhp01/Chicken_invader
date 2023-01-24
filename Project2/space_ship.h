#pragma once
#include "common.h"
#include "base.h"
#include "bullet.h"

#define WIDTH_MAIN 99
#define HEIGHT_MAIN 75
#define MAIN_SPEED 5
#define MAIN_HEART 3

class space_ship : public base
{
public:
    space_ship();
    ~space_ship();

    enum BULLET_SPEED
    {
        BLASTER_SPEED = 6
    };

    void Control(SDL_Event event, SDL_Renderer* screen);
    void Move();
    void Show(SDL_Renderer* screen);

    void set_bullet_list(vector<bullet*> bullet_list) { bullet_list_ = bullet_list; }
    vector<bullet*> get_bullet_list() { return bullet_list_; }
    void HandleBullet(SDL_Renderer* screen);
    void RemoveBullet(const int& idx);

    void set_bul_type(const int& bul_type) { bullet_type_ = bul_type; }
    int get_bul_type()const { return bullet_type_; }
    void set_status(const bool& Status) { status = Status; }
    int get_status()const { return status; }

    float get_bullet_damage()const { return bullet_damage_; }
    void set_heart(const int& Heart) { heart_ = Heart; }
    int get_heart()const { return heart_; }
    void decrease_heart() { heart_--; }
private:
    std::vector<bullet*> bullet_list_;
    int x_val_;
    int y_val_;

    int bullet_type_;
    bool status;
    int heart_;
    float bullet_damage_;
};