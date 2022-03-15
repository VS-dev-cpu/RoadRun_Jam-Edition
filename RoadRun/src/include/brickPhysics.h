struct brick
{
    int x;
    int y;
    int w;
    int h;
    int velocity_x;
    int velocity_y;
    int rotation;
};

class Physics
{
public:
    bool isCollide(int x, int y, int xx, int yy, int size_x, int size_y);
    void collision(int map[100], struct brick *body, int win_w, int win_h);
    void pathFinding(struct brick *body, int collision_map[150], int x, int y);
    void update(struct brick *body, bool stop);
};

bool Physics::isCollide(int x, int y, int xx, int yy, int size_x, int size_y)
{

    if (yy + size_y > y and y > yy)
    {
        if (xx + size_x > x and x > xx)
        {
            return true;
        }
    }
    return false;
}

void Physics::collision(int map[150], struct brick *body, int win_w, int win_h)
{
    int step_x = win_w / 15;
    int step_y = win_h / 10;

    int col_y = 0;

    int i = 0;

    for (int a = 0; a < 10; a++)
    {
        int col_x = 0;
        for (int b = 0; b < 15; b++)
        {
            if (map[i] == 1)
            {

                if ((col_y <= body->y && body->y <= col_y + step_y) || (col_y <= body->y + body->h && body->y + body->h <= col_y + step_y)) //Check, if y and col_y is collide
                {
                    if (body->velocity_x > 0 && (body->x + body->w + body->velocity_x >= col_x && col_x >= body->x + body->w)) //Check, if x + velocity_x and col_x is collide
                    {
                        body->velocity_x = 0;
                    }
                    if (body->velocity_x < 0 && (body->x + body->velocity_x <= col_x + step_x && col_x + step_x < body->x)) //Check, if x - velocity_x and col_x is collide
                    {
                        body->velocity_x = 0;
                    }
                }

                if ((col_x <= body->x && body->x <= col_x + step_x) || (col_x <= body->x + body->w && body->x + body->w <= col_x + step_x)) //Check, if y and col_y is collide
                {
                    if (body->velocity_y > 0 && (body->y + body->h + body->velocity_y >= col_y && col_y >= body->y + body->h)) //Check, if x + velocity_x and col_x is collide
                    {
                        body->velocity_y = 0;
                    }
                    if (body->velocity_y < 0 && (body->y + body->velocity_y <= col_y + step_y && col_y + step_y <= body->y)) //Check, if x - velocity_x and col_x is collide
                    {
                        body->velocity_y = 0;
                    }
                }
            }

            i += 1;
            col_x += step_x;
        }
        col_y += step_y;
    }
}

void Physics::update(struct brick *body, bool stop)
{
    body->x += body->velocity_x;
    body->y += body->velocity_y;
    if (stop)
    {
        body->velocity_x = 0;
        body->velocity_y = 0;
    }
}