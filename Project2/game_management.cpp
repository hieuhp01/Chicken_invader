#include "game_management.h"

game_management::game_management()
{
    kill = 0;
    isRunning = true;
    scrolling = -(BACKGROUND_HEIGHT - SCREEN_HEIGHT);
}
game_management::~game_management()
{

}
void game_management::init(string title) 
{
    //create window, render screen, creat renderer for render image and font
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        isRunning = false;
    }
    else 
    {
        gWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) 
        {
            isRunning = false;
        }
        else 
        {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                isRunning = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) 
                {
                    isRunning = false;
                }
                if (TTF_Init() < 0) 
                {
                    isRunning = false;
                }
                g_font_text = TTF_OpenFont("PNG//font1.ttf", 24);
                g_font_menu = TTF_OpenFont("PNG//font1.ttf", 50);
                g_font_end_game = TTF_OpenFont("PNG//font1.ttf", 100);
                if (g_font_text == NULL || g_font_menu == NULL || g_font_end_game == NULL)
                {
                    isRunning = false;
                }
            }
        }
    }

    // init background and spaceship
    background.loadImg("PNG//background.png", gRenderer);
    spaceship.loadImg("PNG//playerShip1_red.png", gRenderer);

    //init chicken
    int t = 0;
    int y_row = 0;
    for (int c = 0; c < NUMBER_OF_CHICKEN; c++) 
    {
        Chicken* p_chicken = new Chicken();
        if (p_chicken) 
        {
            //get chicken_image
            p_chicken->loadImg("PNG//chicken_red.png", gRenderer);
            p_chicken->set_clips();
            //make row of chicken 
            //36 chickens, 12 chickens per row
            if (t % NUMBER_OF_CHICKENS_PER_ROW == 0) 
            {
                y_row -= DISTANCE_BETWEEN_CHICKENS;
                t = 0;
            }
            //set chicken_properties
            p_chicken->SetRect(5 + t * DISTANCE_BETWEEN_CHICKENS, y_row);
            p_chicken->set_y_val(CHICKEN_SPEED);
            p_chicken->set_x_val(CHICKEN_SPEED);
            p_chicken->set_heart(CHICKEN_HEART);
            int random = rand() % 10;
            if (random < 3) 
            {
                bullet* p_bullet = new bullet();
                p_chicken->InitBullet(p_bullet, gRenderer);
            }
            //put the chicken into chicken_list to handle
            p_chicken_list.push_back(p_chicken);
            t++;
        }
    }

    //init explosion
    exp.loadImg("PNG//exp.png", gRenderer);
    exp.set_clip();

    //init kill, hp image
    support.loadImg("PNG//support.png", gRenderer);
    support.SetRect(-20, 10);

    //init text
    kill_text.SetColor(Text::WHITE);
    heart_text.SetColor(Text::WHITE);
    hint.SetColor(Text::WHITE);
    end_game.SetColor(Text::WHITE);

    isRunning = true;
}

bool game_management::check_collision(const SDL_Rect& object1, const SDL_Rect& object2)
{
    int left_a = object1.x;
    int right_a = object1.x + object1.w;
    int top_a = object1.y;
    int bottom_a = object1.y + object1.h;

    int left_b = object2.x;
    int right_b = object2.x + object2.w;
    int top_b = object2.y;
    int bottom_b = object2.y + object2.h;

    if (left_a > right_b || right_a < left_b || top_a > bottom_b || bottom_a < top_b) 
    {
        return false;
    }
    return true;
}

void game_management::handle_event()
{
    while (SDL_PollEvent(&gEvent))
    {
        if (gEvent.type == SDL_QUIT) // make you quit the game using button x in the upright corner
        {
            isRunning = false;
        }
        //handle the spaceship_movement using keyboard
        spaceship.Control(gEvent, gRenderer);
    }
}

void game_management::handle_chicken() 
{
    if (kill < NUMBER_OF_CHICKEN * 5) 
    {
        for (int ck = 0; ck < p_chicken_list.size(); ck++) 
        {
            Chicken* p_chicken = p_chicken_list.at(ck);
            if (p_chicken)
            {
                //handle properties of each chicken that init
                p_chicken->Move();
                p_chicken->Show(gRenderer);
                p_chicken->HandleBullet(gRenderer);
            }

            //check spaceship with chicken_bullet
            bool Col1 = false;
            vector<bullet*> bullet_list = p_chicken->get_bullet_list();
            for (int b = 0; b < bullet_list.size(); b++) 
            {
                bullet* p_bullet = bullet_list.at(b);
                if (p_bullet)
                {
                    Col1 = check_collision(p_bullet->GetRect(), spaceship.GetRect());
                    if (Col1 == true) 
                    {
                        //chicken_bullet remove after collapse with spaceship
                        p_chicken->RemoveBullet(b);
                        break;
                    }
                }
            }

            //check spaceship with chicken
            bool Col2 = check_collision(spaceship.GetRect(), p_chicken->GetRectFrame());
            if (Col1 || Col2) //if spaceship collapse with chicken or chicken_bullet
            {
                
                //set exp
                int x_pos = (spaceship.GetRect().x + WIDTH_MAIN / 2) - WIDTH_FRAME_EXP / 2;
                int y_pos = (spaceship.GetRect().y + HEIGHT_MAIN / 2) - HEIGHT_FRAME_EXP / 2;
                exp.SetRect(x_pos, y_pos);
                exp.set_frame(0);

                // make spaceship stay outside of the screen when it explode
                spaceship.SetRect(SCREEN_WIDTH*2 , SCREEN_HEIGHT*2 );
                spaceship.set_status(false);
                spaceship.decrease_heart();
            }


            //check spaceship_bullet with chicken
            vector<bullet*> s_bullet_list = spaceship.get_bullet_list();
            for (int sb = 0; sb < s_bullet_list.size(); sb++) 
            {
                bullet* p_bullet = s_bullet_list.at(sb);
                if (p_bullet != NULL)
                {
                    bool Col3 = check_collision(p_bullet->GetRect(), p_chicken->GetRectFrame());
                    if (Col3)
                    {
                        //chicken_HP -= spaceship_bullet_damage
                        //spaceship_bullet remove after collapse
                        p_chicken->Decrease((spaceship.get_bullet_damage()));
                        spaceship.RemoveBullet(sb);

                        if (p_chicken->get_heart() <= 0)
                        {
                            //make the chicken comeback
                            p_chicken->set_heart(CHICKEN_HEART);
                            //+point
                            kill++;
                            //set the respawn position of the killed chicken
                            p_chicken->SetRect(p_chicken->GetRect().x, -3 * SCREEN_HEIGHT);
                            if (kill > NUMBER_OF_CHICKEN*5) 
                            {
                                p_chicken->set_come_back(false);
                            }
                        }
                    }
                }
            }
        }
    }
}

void game_management::handle_game()
{
    SDL_SetRenderDrawColor(gRenderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
    SDL_RenderClear(gRenderer);

    // handle infinite loop background
    scrolling += SCREEN_SPEED;
    if (scrolling < 0) 
    {
        background.Render(gRenderer);
        background.SetRect(0, scrolling);
    }
    else 
    {
        scrolling = -(BACKGROUND_HEIGHT - SCREEN_HEIGHT);
    }
   

    // handle spaceship
    spaceship.Move();
    spaceship.Show(gRenderer);
    spaceship.HandleBullet(gRenderer);

    // handle chicken
    handle_chicken();

    // handle explosion
    if (exp.get_frame() < NUMBER_OF_FRAME * 2)
    {
        exp.Show(gRenderer);
    }

    // game over
    if (spaceship.get_heart() == 0) 
    {
            end_game.SetText("Game Over !");
            end_game.SetRect(310, SCREEN_HEIGHT / 5);
            end_game.loadText_showText(g_font_end_game, gRenderer);
    }
    else {
        if (spaceship.get_status() == false) 
        {
            hint.SetText("Press 'ENTER' to revive !");
            hint.SetRect(280, SCREEN_HEIGHT / 4);
            hint.loadText_showText(g_font_menu, gRenderer);
        }
    }

    //show kill,HP
    support.Render(gRenderer);
    heart_text.SetText(to_string(spaceship.get_heart()));
    heart_text.SetRect(195, 15);
    heart_text.loadText_showText(g_font_text, gRenderer);
    kill_text.SetText(to_string(kill));
    kill_text.SetRect(50, 15);
    kill_text.loadText_showText(g_font_text, gRenderer);

    SDL_RenderPresent(gRenderer);
}


void game_management::clean()
{
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    gWindow = NULL;
    gRenderer = NULL;

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}
