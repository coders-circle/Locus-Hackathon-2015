#pragma once

#define PHEIGHT 16   // Or making this 32
extern World g_world;
class PathFinding
{
public:
    PathFinding() : tx(-1), ty(-1), obj(NULL) {}
    int tx, ty;
    bool moving;
    Object* obj;

    void Start(Object* object, int targetX, int targetY)
    {
        obj = object;
        tx = targetX;
        ty = targetY;
        float dx = (float)tx - obj->GetX();
        float dy = (float)ty - obj->GetY();
        if (fabs(dx) > fabs(dy))
        {
            if (dx < 0)
                obj->SetDir(LEFT);
            else if (dx > 0)
                obj->SetDir(RIGHT);
        }
        else
        {
            if (dy < 0)
                obj->SetDir(UP);
            else if (dy > 0)
                obj->SetDir(DOWN);
        }

    }

    void Update()
    {
        if (!obj)
            return;
        bool snapped = (int)obj->GetX() % 16 == 0 && (int)obj->GetY() % 16 == 0;
        if ((tx != -1 && ty != -1) || !snapped)
        {
            if (tx == obj->GetX() && ty == obj->GetY() && snapped)
            {
                tx = -1;
                ty = -1;
            }
            else
            {
                moving = true;
                int trial = 0;
                if (snapped)
                {
                    float dx = 0;
                    float dy = 0;
                    if (obj->GetDir() == LEFT)
                        dx = -16;
                    else if (obj->GetDir() == RIGHT)
                        dx = 16;
                    else if (obj->GetDir() == UP)
                        dy = -16;
                    else if (obj->GetDir() == DOWN)
                        dy = PHEIGHT;

                    float nx = (float)obj->GetX() + dx;
                    float ny = (float)obj->GetY() + dy;

                    float ddx = (float) tx - obj->GetX();
                    float ddy = (float) ty - obj->GetY();
                    
                    if (((PHEIGHT==32&&dy > 0) || !g_world.HasObstacle(nx, ny)) && (PHEIGHT == 16 || dy < 0 || !g_world.HasObstacle(nx, ny+16)))
                    {
                        dx = ddx;
                        dy = ddy;
                        if (fabs(dx) > fabs(dy))
                        {
                            if (dx < 0 && !g_world.HasObstacle(obj->GetX()-16, obj->GetY()) && (PHEIGHT == 16 || !g_world.HasObstacle(obj->GetX()-16, obj->GetY()+16)))
                            {
                                dx = -16;
                                obj->SetDir(LEFT);
                                dy = 0;
                            }
                            else if (dx > 0 && !g_world.HasObstacle(obj->GetX()+16, obj->GetY()) && (PHEIGHT == 16 || !g_world.HasObstacle(obj->GetX()+16, obj->GetY()+16)))
                            {
                                obj->SetDir(RIGHT);
                                dx = 16;
                                dy = 0;
                            }
                        }
                        else
                        {
                            if (dy < 0 && !g_world.HasObstacle(obj->GetX(), obj->GetY()-16))
                            {
                                obj->SetDir(UP);
                                dy = -16;
                                dx = 0;
                            }
                            if (dy > 0 && !g_world.HasObstacle(obj->GetX(), obj->GetY()+PHEIGHT))
                            {
                                obj->SetDir(DOWN);
                                dy = 32;
                                dx = 0;
                            }
                        }
                    }
                    else
                    {
                        if (dx != 0)
                        {
                            dx = 0;
                            if (ddy < 0 && !g_world.HasObstacle(obj->GetX(), obj->GetY()-16))
                            {
                                obj->SetDir(UP);
                                dy = -16;
                            }
                            if (ddy > 0 && !g_world.HasObstacle(obj->GetX(), obj->GetY()+16) && (PHEIGHT == 16 || !g_world.HasObstacle(obj->GetX(), obj->GetY()+32)))
                            {
                                obj->SetDir(DOWN);
                                dy = 32;
                            }
                        }
                        else if (dy != 0)
                        {
                            if (ddx < 0 && !g_world.HasObstacle(obj->GetX()-16, obj->GetY()) && (PHEIGHT == 16 || !g_world.HasObstacle(obj->GetX()-16, obj->GetY()+16)))
                            {
                                dx = -16;
                                obj->SetDir(LEFT);
                            }
                            else if (ddx > 0 && !g_world.HasObstacle(obj->GetX()+16, obj->GetY()) && (PHEIGHT == 16 || !g_world.HasObstacle(obj->GetX()+16, obj->GetY()+16)))
                            {
                                obj->SetDir(RIGHT);
                                dx = 16;
                            }
                            dy = 0;
                        }
                    }
                    if (dx == 0 && dy == 0)
                        moving = false;
                }
                if (moving)
                {
                    if (obj->GetDir() == RIGHT)
                    {
                        //if (!g_world.HasObstacle(obj->GetX() + 16, obj->GetY()))
                            obj->SetX(obj->GetX() + 1);
                    }
                    else if (obj->GetDir() == LEFT)
                    {
                        //if (!g_world.HasObstacle(obj->GetX() - 16, obj->GetY()))
                            obj->SetX(obj->GetX() - 1);
                    }
                    else if (obj->GetDir() == DOWN)
                    {
                        //if (!g_world.HasObstacle(obj->GetX(), obj->GetY() + 16))
                            obj->SetY(obj->GetY() + 1);
                    }
                    else if (obj->GetDir() == UP)
                    {
                        //if (!g_world.HasObstacle(obj->GetX(), obj->GetY() - 16))
                            obj->SetY(obj->GetY() - 1);
                    }
                }
            }
        }
    }
};
