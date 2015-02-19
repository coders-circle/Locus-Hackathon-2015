#pragma once

#define TSZ 32
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
        moving = true;
    }

    void Update()
    {
        if (!obj || !moving)
            return;
        bool snapped = (int)obj->GetX() % TSZ == 0 && (int)obj->GetY() % TSZ == 0;
        if ((tx != -1 && ty != -1) || !snapped)
        {
            if (tx == obj->GetX() && ty == obj->GetY() && snapped)
            {
                tx = -1;
                ty = -1;
                moving = false;
            }
            else
            {
                int trial = 0;
                if (snapped)
                {
                    float dx = 0;
                    float dy = 0;
                    if (obj->GetDir() == LEFT)
                        dx = -TSZ;
                    else if (obj->GetDir() == RIGHT)
                        dx = TSZ;
                    else if (obj->GetDir() == UP)
                        dy = -TSZ;
                    else if (obj->GetDir() == DOWN)
                        dy = TSZ;

                    float nx = (float)obj->GetX() + dx;
                    float ny = (float)obj->GetY() + dy;

                    float ddx = (float) tx - obj->GetX();
                    float ddy = (float) ty - obj->GetY();
                    
                    if (!g_world.GetObstacle(nx, ny))
                    {
                        dx = ddx;
                        dy = ddy;
                        if (fabs(dx) > fabs(dy))
                        {
                            if (dx < 0 && !g_world.GetObstacle(obj->GetX()-TSZ, obj->GetY()))
                            {
                                dx = -TSZ;
                                obj->SetDir(LEFT);
                            }
                            else if (dx > 0 && !g_world.GetObstacle(obj->GetX()+TSZ, obj->GetY()))
                            {
                                obj->SetDir(RIGHT);
                                dx = TSZ;
                            }
                            dy = 0;
                        }
                        else
                        {
                            if (dy < 0 && !g_world.GetObstacle(obj->GetX(), obj->GetY()-TSZ))
                            {
                                obj->SetDir(UP);
                                dy = -TSZ;
                            }
                            if (dy > 0 && !g_world.GetObstacle(obj->GetX(), obj->GetY()+TSZ))
                            {
                                obj->SetDir(DOWN);
                                dy = TSZ;
                            }
                            dx = 0;
                        }
                    }
                    else
                    {
                        if (dx != 0)
                        {
                            dx = 0;
                            if (ddy < 0 && !g_world.GetObstacle(obj->GetX(), obj->GetY()-TSZ))
                            {
                                obj->SetDir(UP);
                                dy = -TSZ;
                            }
                            if (ddy > 0 && !g_world.GetObstacle(obj->GetX(), obj->GetY()+TSZ))
                            {
                                obj->SetDir(DOWN);
                                dy = TSZ;
                            }
                        }
                        else if (dy != 0)
                        {
                            dy = 0;
                            if (ddx < 0 && !g_world.GetObstacle(obj->GetX()-TSZ, obj->GetY()))
                            {
                                dx = -TSZ;
                                obj->SetDir(LEFT);
                            }
                            else if (ddx > 0 && !g_world.GetObstacle(obj->GetX()+TSZ, obj->GetY()))
                            {
                                obj->SetDir(RIGHT);
                                dx = TSZ;
                            }
                        }
                    }
                    if (dx == 0 && dy == 0)
                        moving = false;
                }
                if (moving)
                {
                    if (obj->GetDir() == RIGHT)
                        obj->SetX(obj->GetX() + 1);
                    else if (obj->GetDir() == LEFT)
                        obj->SetX(obj->GetX() - 1);
                    else if (obj->GetDir() == DOWN)
                        obj->SetY(obj->GetY() + 1);
                    else if (obj->GetDir() == UP)
                        obj->SetY(obj->GetY() - 1);
                }
            }
        }
        ((People*)obj)->m_moving = moving;

    }
};
