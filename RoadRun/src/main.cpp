#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

#include "2DEngine.h"
#else
#include "include/2DEngine.h"
#endif

#include <thread>

using namespace std;

Mixer mix(4);
Engine2D engine2d("Road Run", 720, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_FOREIGN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_INPUT_GRABBED);
gameClock clk;

const int WINDOW_CENTER_X = 720 / 2;
const int WINDOW_CENTER_Y = 480 / 2;

const int CAR_W = 50;
const int CAR_H = 100;

typedef struct car
{
    double x, y;
    int w, h;
    double speed, top_speed, low_speed, speedup;
    bool alive;
    int state;
    double nitro_state;

    double fuel;

    int coffein;
    int coins;

    SDL_Texture *tex[9];
    SDL_Texture *nitro[2];
} car;

enum gamestate
{
    MENU,
    SETTINGS,
    INFO,
    GAME,
    CUTSCENE_PETROL_STATION,
    CUTSCENE_OUT_OF_GAS,
    CUTSCENE_KILLED_BY_CHAOS,
    CUTSCENE_KILLED_BY_TREE,
    CUTSCENE_KILLED_BY_LOG,
};

car player;

SDL_Texture *road_texture[3];
SDL_Texture *line_texture;
SDL_Texture *gas_station;
SDL_Texture *tree_texture[4];
SDL_Texture *blockad_texture;
SDL_Texture *log_texture;

SDL_Texture *gui_coffee;
SDL_Texture *gui_coin;
SDL_Texture *gui_nitro[6];
SDL_Texture *gui_gas_meter[5];
SDL_Texture *gui_speed_meter[5];
SDL_Texture *gui_menu[5];

double coin_pos[2];

SDL_Texture *button[7][2];
int button_state[7];

SDL_Texture *chaos[2];

double tree[2][10][3];
int tree_num = 5;
int tree_wh[4] = {130, 100, 70, 30};

double logs[5][2];
int log_num = 5;
int log_w = 60;
int log_h = 30;

bool running = true;

bool render_button(SDL_Texture *gfx[], int x, int y, int w, int h, int var[], int index)
{
    bool focused = isCollide(engine2d.cursor_x, engine2d.cursor_y, x, y, w, h);
    engine2d.draw_texture_wh(gfx[var[index]], x, y, w, h);
    if (focused && engine2d.mousebutton_l && var[index] == 0)
    {
        var[index] = 1;
    }
    else if (focused && not engine2d.mousebutton_l && var[index] == 1)
    {
        var[index] = 0;
        return true;
    }
    else if (!focused)
    {
        var[index] = 0;
    }

    return false;
}

double nitro_fuel_loss = 2;
int nitro_speed = 400;

bool init()
{
    // Setup
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    // Loading
    player.tex[0] = engine2d.load_texture("res/gfx/car_1.png");
    player.tex[1] = engine2d.load_texture("res/gfx/car_2.png");
    player.tex[2] = engine2d.load_texture("res/gfx/car_3.png");
    player.tex[3] = engine2d.load_texture("res/gfx/car_4.png");
    player.tex[4] = engine2d.load_texture("res/gfx/car_5.png");
    player.tex[5] = engine2d.load_texture("res/gfx/car_6.png");
    player.tex[6] = engine2d.load_texture("res/gfx/car_7.png");
    player.tex[7] = engine2d.load_texture("res/gfx/car_8.png");
    player.tex[8] = engine2d.load_texture("res/gfx/car_9.png");

    player.nitro[0] = engine2d.load_texture("res/gfx/nitro_1.png");
    player.nitro[1] = engine2d.load_texture("res/gfx/nitro_2.png");

    road_texture[0] = engine2d.load_texture("res/gfx/road.png");
    road_texture[1] = engine2d.load_texture("res/gfx/road2.png");
    road_texture[2] = engine2d.load_texture("res/gfx/road3.png");
    gas_station = engine2d.load_texture("res/gfx/gas.png");
    line_texture = engine2d.load_texture("res/gfx/line.png");

    tree_texture[0] = engine2d.load_texture("res/gfx/tree_1.png");
    tree_texture[1] = engine2d.load_texture("res/gfx/tree_2.png");
    tree_texture[2] = engine2d.load_texture("res/gfx/tree_3.png");
    tree_texture[3] = engine2d.load_texture("res/gfx/tree_4.png");

    blockad_texture = engine2d.load_texture("res/gfx/blockad.png");
    log_texture = engine2d.load_texture("res/gfx/log.png");

    gui_coffee = engine2d.load_texture("res/gfx/coffee.png");
    gui_coin = engine2d.load_texture("res/gfx/coin.png");

    gui_nitro[0] = engine2d.load_texture("res/gfx/nitro_img_0.png");
    gui_nitro[1] = engine2d.load_texture("res/gfx/nitro_img_1.png");
    gui_nitro[2] = engine2d.load_texture("res/gfx/nitro_img_2.png");
    gui_nitro[3] = engine2d.load_texture("res/gfx/nitro_img_3.png");
    gui_nitro[4] = engine2d.load_texture("res/gfx/nitro_img_4.png");
    gui_nitro[5] = engine2d.load_texture("res/gfx/nitro_img_5.png");

    gui_gas_meter[0] = engine2d.load_texture("res/gfx/gas_meter_0.png");
    gui_gas_meter[1] = engine2d.load_texture("res/gfx/gas_meter_1.png");
    gui_gas_meter[2] = engine2d.load_texture("res/gfx/gas_meter_2.png");
    gui_gas_meter[3] = engine2d.load_texture("res/gfx/gas_meter_3.png");
    gui_gas_meter[4] = engine2d.load_texture("res/gfx/gas_meter_4.png");

    gui_speed_meter[0] = engine2d.load_texture("res/gfx/speed_meter_0.png");
    gui_speed_meter[1] = engine2d.load_texture("res/gfx/speed_meter_1.png");
    gui_speed_meter[2] = engine2d.load_texture("res/gfx/speed_meter_2.png");
    gui_speed_meter[3] = engine2d.load_texture("res/gfx/speed_meter_3.png");
    gui_speed_meter[4] = engine2d.load_texture("res/gfx/speed_meter_4.png");

    gui_menu[0] = engine2d.load_texture("res/gfx/menu.png");
    gui_menu[1] = engine2d.load_texture("res/gfx/settings.png");
    gui_menu[2] = engine2d.load_texture("res/gfx/info.png");
    gui_menu[3] = engine2d.load_texture("res/gfx/petrol.png");
    gui_menu[4] = engine2d.load_texture("res/gfx/petrol_station.png");

    chaos[0] = engine2d.load_texture("res/gfx/c1.png");
    chaos[1] = engine2d.load_texture("res/gfx/c2.png");

    button[0][0] = engine2d.load_texture("res/gfx/button_play_a.png");
    button[0][1] = engine2d.load_texture("res/gfx/button_play_b.png");
    button[1][0] = engine2d.load_texture("res/gfx/button_settings_a.png");
    button[1][1] = engine2d.load_texture("res/gfx/button_settings_b.png");
    button[2][0] = engine2d.load_texture("res/gfx/button_exit_a.png");
    button[2][1] = engine2d.load_texture("res/gfx/button_exit_b.png");
    button[3][0] = engine2d.load_texture("res/gfx/button_info_a.png");
    button[3][1] = engine2d.load_texture("res/gfx/button_info_b.png");
    button[4][0] = engine2d.load_texture("res/gfx/button_update_a.png");
    button[4][1] = engine2d.load_texture("res/gfx/button_update_b.png");
    button[5][0] = engine2d.load_texture("res/gfx/button_back_a.png");
    button[5][1] = engine2d.load_texture("res/gfx/button_back_b.png");
    button[6][0] = engine2d.load_texture("res/gfx/button_coffee_a.png");
    button[6][1] = engine2d.load_texture("res/gfx/button_coffee_b.png");

    for (int i = 0; i < log_num; i++)
    {
        logs[i][0] = rand() % (720 - log_h);
        logs[i][1] = rand() % (480 - log_w);
    }

    for (int i = 0; i < tree_num; i++)
    {
        tree[0][i][2] = rand() % 4;
        tree[0][i][0] = rand() % (160 - tree_wh[(int)tree[0][i][2]]);
        tree[0][i][1] = rand() % (480 - tree_wh[(int)tree[0][i][2]]);

        tree[1][i][2] = rand() % 4;
        tree[1][i][0] = 560 + rand() % (160 - tree_wh[(int)tree[1][i][2]]);
        tree[1][i][1] = rand() % (480 - tree_wh[(int)tree[1][i][2]]);
    }

    mix.load("res/sfx/music1.mp3", "asdf");

    // Preparing
    // mix.play_n(0, -1);
    engine2d.update();
    // Mix_PlayChannel(1, Mix_LoadWAV("res/sfx/music1.mp3"), 0);

    return true;
}

bool load = init();

bool a_state = true;

gamestate state = MENU;

double world_position = 0;
int world_speed = 30;
int world_backup = 10;

double line_pos = -480;

double station_pos = -10000;

double fuel_loss = 0.000001;

bool station_triggered;

bool loop()
{
    engine2d.color(255, 140, 0);
    engine2d.fill(0, 0, 720, 480);

    clk.update();

    if (state == MENU)
    {
        engine2d.draw_texture(gui_menu[0], 0, 0, 720, 480);

        player.alive = true;
        player.state = 0;
        player.nitro_state = 5;
        player.fuel = 4;
        player.coffein = 3;
        player.coins = 0;

        player.x = WINDOW_CENTER_X - CAR_W / 2;
        player.y = WINDOW_CENTER_Y - CAR_H / 2;
        player.w = CAR_W;
        player.h = CAR_H;

        player.speed = 0;
        player.top_speed = 150;
        player.low_speed = -50;
        player.speedup = 1000;

        world_position = 0;
        world_speed = 30;
        world_backup = 10;

        line_pos = -480;

        station_pos = -10000;
        station_triggered = false;

        fuel_loss = 0.0001;

        coin_pos[0] = rand() % (720 - 2 * 160) + 160;
        coin_pos[1] = -50;

        if (render_button(button[0], 10, 420, 50, 50, button_state, 0))
            state = GAME;

        if (render_button(button[1], 70, 420, 50, 50, button_state, 1))
            state = SETTINGS;

        if (render_button(button[3], 130, 420, 50, 50, button_state, 3))
            state = INFO;

#ifndef __EMSCRIPTEN__
        if (render_button(button[2], 190, 420, 50, 50, button_state, 2))
            return 0;
#endif
    }

    else if (state == SETTINGS)
    {
        engine2d.draw_texture(gui_menu[1], 0, 0, 720, 480);
        if (render_button(button[5], 10, 420, 50, 50, button_state, 5))
            state = MENU;
    }

    else if (state == INFO)
    {
        engine2d.draw_texture(gui_menu[2], 0, 0, 720, 480);
        if (render_button(button[5], 10, 420, 50, 50, button_state, 5))
            state = MENU;
    }

    else if (state == GAME)
    {
        engine2d.draw_texture(road_texture[0], 0, 0, 720, 480);
        engine2d.draw_texture(road_texture[1], 560, station_pos);
        engine2d.draw_texture(gas_station, 560, station_pos - 480);
        engine2d.draw_texture(road_texture[2], 560, station_pos - 480 * 2);
        engine2d.draw_texture_wh(line_texture, 354, line_pos, 12, 960);

        // Gas Station
        station_pos += world_speed * 10 * clk.deltaTime;

        if (station_pos > 480 + 480 * 2) // Reset
        {
            station_pos = -10000;
            station_triggered = false;
        }

        if (isCollide(player.x + player.w / 2, player.y + player.h / 2, 560, station_pos - 480, 160, 480) && !station_triggered)
        {
            state = CUTSCENE_PETROL_STATION;
            station_triggered = true;
            player.nitro_state = 5;
            player.fuel = 4;
        }

        // Logs
        for (int i = 0; i < log_num; i++)
        {
            if (logs[i][1] > 480)
            {
                logs[i][0] = rand() % (720 - log_w);
                logs[i][1] = -log_h - (rand() % (log_h * 3));
            }

            if (isCollide(player.x + player.w / 2, player.y + player.h / 2, logs[i][0], logs[i][1], log_w, log_h))
            {
                if (player.speed < player.top_speed + 20)
                {
                    if (player.state > 0)
                    {
                        player.state--;
                    }
                    else
                    {
                        player.coffein--;
                    }
                }
                logs[i][1] = 480 + 10;
            }

            logs[i][1] += world_speed * 10 * clk.deltaTime;

            if (logs[i][0] > (560 - log_w) && logs[i][1] < station_pos + 780 && logs[i][1] > station_pos - 480 * 2)
                logs[i][0] = 720 + 10;
            engine2d.draw_texture_wh(log_texture, logs[i][0], logs[i][1], log_w, log_h);

            if (player.speed > 0 && player.y <= 0)
                logs[i][1] += abs(player.speed) * clk.deltaTime;
        }

        // Coins

        if (coin_pos[1] > 480 + 50)
        {
            coin_pos[0] = rand() % (720 - 2 * 160) + 160;
            coin_pos[1] = -50;
        }

        coin_pos[1] += world_speed * 10 * clk.deltaTime;

        if (player.speed > 0 && player.y <= 0)
            coin_pos[1] += abs(player.speed) * clk.deltaTime;

        if (isCollide(player.x + player.w / 2, player.y + player.h / 2, coin_pos[0], coin_pos[1], 50, 50))
        {
            coin_pos[0] = 720 + 10;
            player.coins++;
        }

        engine2d.draw_texture_wh(gui_coin, coin_pos[0], coin_pos[1], 50, 50);

        // Basic Animations + Movements
        if ((int)clk.fixedTick % 1000 == 0)
        {
            if (a_state)
                a_state = false;
            else
                a_state = true;
        }

        if (player.state > 3)
            player.top_speed = 250;
        else
            player.top_speed = 150;

        if (player.state > 4)
            player.speedup = 1500;
        else
            player.speedup = 1000;

        if (line_pos > 0)
            line_pos = -480;

        line_pos += world_speed * 10 * clk.deltaTime;

        if (player.alive)
        {
            engine2d.draw_texture_wh(player.tex[player.state], (int)player.x, (int)player.y, player.w, player.h);

            if (engine2d.key[SDL_SCANCODE_W] && player.fuel > 0) // WS
            {
                if (player.speed < player.top_speed)
                    player.speed += player.speedup * clk.deltaTime;
                if (player.speed > player.top_speed)
                    player.speed -= player.speedup / 5 * clk.deltaTime;
            }
            else
            {
                if (player.speed > 0)
                    player.speed -= player.speedup / 10 * clk.deltaTime;
            }

            if (engine2d.key[SDL_SCANCODE_S] && player.fuel > 0)
            {
                if (player.speed > player.low_speed)
                    player.speed -= player.speedup * clk.deltaTime;
            }
            else
            {
                if (player.speed < 0)
                    player.speed += player.speedup / 10 * clk.deltaTime;
            }

            if (player.state >= 5)
            {
                if (engine2d.key[SDL_SCANCODE_SPACE] && player.nitro_state > 0) // Nitro
                {
                    player.nitro_state -= nitro_fuel_loss * clk.deltaTime;
                    player.speed += nitro_speed * clk.deltaTime;
                    engine2d.draw_texture_wh(player.nitro[a_state], player.x, player.y, player.w, player.h);
                }
            }

            if (engine2d.key[SDL_SCANCODE_D] && player.x < 720 - player.w) // AD
                player.x += 200 * clk.deltaTime;
            if (engine2d.key[SDL_SCANCODE_A] && player.x > 0)
                player.x -= 200 * clk.deltaTime;

            if (player.speed != 0) // MOVEMENTS
            {
                player.fuel -= fuel_loss * 1000 * clk.deltaTime;
                if (player.speed > 0)
                {
                    if (player.y > 0)
                    {
                        player.y -= (player.speed - world_speed) * clk.deltaTime;
                        if (world_position > 0)
                            world_position -= world_speed * world_backup * clk.deltaTime;
                    }
                    else
                    {
                        world_position += abs(player.speed) * clk.deltaTime;
                        line_pos += abs(player.speed) * clk.deltaTime;
                        station_pos += abs(player.speed) * clk.deltaTime;
                    }
                }
                else
                {
                    if (player.y < 480 - player.h)
                    {
                        player.y -= (player.speed - world_speed) * clk.deltaTime;
                    }
                    else if (world_position > 0)
                    {
                        world_position -= world_speed * world_backup * clk.deltaTime;
                    }
                }
            }
            else
            {
                if (world_position > 0)
                {
                    world_position -= world_speed * world_backup * clk.deltaTime;
                }
            }

            if (player.y < 480 - player.h)
            {
                player.y += world_speed * clk.deltaTime;
            }
        }

        // Trees
        for (int i = 0; i < tree_num; i++)
        {
            if (tree[0][i][1] > 480)
            {
                tree[0][i][2] = rand() % 4;
                tree[0][i][0] = rand() % (160 - tree_wh[(int)tree[0][i][2]]);
                tree[0][i][1] = -tree_wh[(int)tree[0][i][2]] - (rand() % (tree_wh[(int)tree[0][i][2]] * 3));
            }

            if (isCollide(player.x + player.w / 2, player.y + player.h / 2, tree[0][i][0], tree[0][i][1], tree_wh[(int)tree[0][i][2]], tree_wh[(int)tree[0][i][2]]))
            {
                if (player.state >= 4)
                {
                    player.state = 0;
                    tree[0][i][0] = 720 + 10;
                    player.x = 720 / 2 - player.w / 2;
                }
                else
                {
                    state = CUTSCENE_KILLED_BY_TREE;
                }
            }

            if (tree[1][i][1] > 480)
            {
                tree[1][i][2] = rand() % 4;
                tree[1][i][0] = 560 + (rand() % (160 - tree_wh[(int)tree[1][i][2]]));
                tree[1][i][1] = -tree_wh[(int)tree[1][i][2]] - (rand() % (tree_wh[(int)tree[1][i][2]] * 3));
            }

            if (isCollide(player.x + player.w / 2, player.y + player.h / 2, tree[1][i][0], tree[1][i][1], tree_wh[(int)tree[1][i][2]], tree_wh[(int)tree[1][i][2]]))
            {
                if (player.state >= 4)
                {
                    player.state = 0;
                    tree[1][i][0] = 720 + 10;
                    player.x = 720 / 2 - player.w / 2;
                }
                else
                {
                    state = CUTSCENE_KILLED_BY_TREE;
                }
            }

            tree[0][i][1] += world_speed * 10 * clk.deltaTime;
            tree[1][i][1] += world_speed * 10 * clk.deltaTime;

            if (tree[0][i][0] > 560 - tree_wh[(int)tree[0][i][2]] && tree[0][i][1] < station_pos + 800 * 3 && tree[0][i][1] > station_pos - 480 * 2)
            {
                tree[0][i][0] = 720 + 10;
            }

            if (tree[1][i][0] > 560 - tree_wh[(int)tree[0][i][2]] && tree[1][i][1] < station_pos + 480 * 3 && tree[1][i][1] > station_pos - 480 * 2)
            {
                tree[1][i][0] = 720 + 10;
            }

            engine2d.draw_texture_wh(tree_texture[(int)tree[0][i][2]], tree[0][i][0], tree[0][i][1], tree_wh[(int)tree[0][i][2]], tree_wh[(int)tree[0][i][2]]);
            engine2d.draw_texture_wh(tree_texture[(int)tree[1][i][2]], tree[1][i][0], tree[1][i][1], tree_wh[(int)tree[1][i][2]], tree_wh[(int)tree[1][i][2]]);

            if (player.speed > 0 && player.y <= 0)
            {
                tree[0][i][1] += abs(player.speed) * clk.deltaTime;
                tree[1][i][1] += abs(player.speed) * clk.deltaTime;
            }
        }

        engine2d.draw_texture(chaos[a_state], 0, 416 + world_position, 720, 480);

        engine2d.draw_texture_wh(gui_gas_meter[(int)player.fuel], 20, 320, 120, 80);

        if (player.speed < player.top_speed + 10)
        {
            engine2d.draw_texture_wh(gui_speed_meter[(int)(abs(player.speed) / player.top_speed * 3)], 580, 320, 120, 80);
        }
        else
        {
            engine2d.draw_texture_wh(gui_speed_meter[4], 580, 320, 120, 80);
        }

        if (player.state >= 5)
        {
            engine2d.draw_texture_wh(gui_nitro[(int)player.nitro_state], 20, 260, 120, 40);
        }

        for (int i = 0; i < player.coffein; i++)
        {
            engine2d.draw_texture_wh(gui_coffee, 10 + i * (40 + 10), 10, 40, 30);
        }

        if (isCollide(player.x + player.w / 2, player.y + player.h / 2, 0, 416 + world_position, 720, 480))
        {
            player.coffein--;
            player.y -= player.h * 2;
        }

        // SCAN FOR QUIT
        if (player.coffein <= 0)
        {
            state = CUTSCENE_KILLED_BY_CHAOS;
        }
        if (player.fuel <= 0 && player.speed <= 0)
        {
            state = CUTSCENE_OUT_OF_GAS;
        }
    }
    else if (state == CUTSCENE_PETROL_STATION)
    {
        engine2d.draw_texture(gui_menu[4], 0, 0, 720, 480);
        for (int i = 0; i < player.coffein; i++)
        {
            engine2d.draw_texture_wh(gui_coffee, 10 + i * (40 + 10), 10, 40, 30);
        }

        for (int i = 0; i < player.coins; i++)
        {
            engine2d.draw_texture_wh(gui_coin, 10 + i * (40 + 10), 70, 40, 30);
        }

        engine2d.draw_texture_wh(player.tex[player.state], 720 / 2 - player.w / 2, 480 / 2 - player.h / 2, player.w, player.h);

        if (render_button(button[5], 10, 420, 50, 50, button_state, 5))
        {
            state = GAME;
        }

        if (render_button(button[6], 70, 420, 50, 50, button_state, 6) && player.coffein < 3 && player.coins >= 3)
        {
            player.coffein++;
            player.coins -= 3;
        }

        if (render_button(button[4], 130, 420, 50, 50, button_state, 4) && player.state < 8 && player.coins >= 3 + player.state + 1)
        {
            player.state++;
            player.coins -= 3 + player.state;
        }

        // if (render_button(button))
    }

    else if (state == CUTSCENE_OUT_OF_GAS)
    {
        state = CUTSCENE_KILLED_BY_CHAOS;
    }

    else if (state == CUTSCENE_KILLED_BY_CHAOS)
    {
        if ((int)clk.fixedTick % 100 == 0)
        {
            if (a_state)
            {
                a_state = false;
            }
            else
            {
                a_state = true;
            }
        }

        engine2d.draw_texture(road_texture[0], 0, 0, 720, 480);
        engine2d.draw_texture(road_texture[1], 560, station_pos);
        engine2d.draw_texture(gas_station, 560, station_pos - 480);
        engine2d.draw_texture_wh(line_texture, 354, line_pos, 12, 960);
        engine2d.draw_texture_wh(player.tex[player.state], (int)player.x, (int)player.y, player.w, player.h);

        engine2d.draw_texture(chaos[a_state], 0, 416 + world_position, 720, 480);
        world_position -= 1000 * clk.deltaTime;
        if (world_position > 480)
        {
            world_position = 480;
        }
        if (world_position < -416)
        {
            state = MENU;
        }
    }
    else if (state == CUTSCENE_KILLED_BY_TREE)
    {
        state = CUTSCENE_KILLED_BY_CHAOS;
    }
    else
    {
        state = MENU;
    }

    bool running = engine2d.update();
    return running;
}

#ifdef __EMSCRIPTEN__
void emLoop()
{
    loop();
}
#endif

int main()
{

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(emLoop, 0, 1);
#else
    while (running)
    {
        running = loop();
    }
#endif

    SDL_Quit();

    return 0;
}