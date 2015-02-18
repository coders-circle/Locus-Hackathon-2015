#pragma once
void CreateHouseSprite(int x, int y, int w, int h)
{
    Sprite* spr;
    spr = g_resources.AddSprite();
    spr->Init("sprites/houses1.png", x, y, w, h);
}
void CreateTreeSprite(int x, int y, int w, int h)
{
    Sprite* spr;
    spr = g_resources.AddSprite();
    spr->Init("sprites/allinone.png", x, y, w, h);
}
void CreateHouseSprs()
{
    CreateHouseSprite(0, 0, 72, 72);
    g_resources.housesStart = --g_resources.sprites.end();
    CreateHouseSprite(70, 0, 72, 72);
    CreateHouseSprite(0, 75, 72, 72);
    CreateTreeSprite(10, 0, 48, 48);
    CreateTreeSprite(10+48, 0, 48, 48);
    g_resources.houseLen = 5;
}
extern std::default_random_engine e1;

void CreateHouse()
{
    std::uniform_int_distribution<int> rnd(0, (int)g_resources.houseLen-1);
    std::uniform_int_distribution<int> rnd1(0, 3);
    int m_sprId = rnd(e1);
    auto it = g_resources.housesStart;
    for (int i=0; i<m_sprId; ++i)
        ++it;
    Sprite* spr = &*it;
    
    float x, y=0;
    do
    {
        GetFreeRandom72x72(x, y);
    } while (x > WIDTH - 72 - 16 || y >= HEIGHT-4*16 - 72);
    Object obj;
    obj.Init(spr, x, y);
    g_resources.walls.push_back(obj);
    Object* objp = &(*(--g_resources.walls.end()));
    objp->SetDir((Direction)rnd1(e1));
    g_world.AddObject(objp);

}
void CreateHouses()
{
    Sprite* spr;
    spr = g_resources.AddSprite();
    spr->Init("sprites/forest1.png");
    Object obj;
    obj.Init(spr, 0, 0);
    g_resources.walls.push_back(obj);
    Object* objp = &(*(--g_resources.walls.end()));
    g_world.AddObject(objp);

    CreateHouseSprs();
    for (int i=0; i<10; ++i)
        CreateHouse();
}

