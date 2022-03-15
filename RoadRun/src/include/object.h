#pragma once

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

typedef struct obj
{
    string name;

    int *tex;
    int x, y, w, h, speed_x, speed_y;
    int depth;
} obj;

class Object
{
public:
    vector<obj> objs;

    void add(string name, int *tex, int x, int y, int w, int h, int depth);
    void add(string name, int *tex, int x, int y, int w, int h);
    void add(string name, int *tex);
    obj get(int id);
    obj get(string name);
};

void Object::add(string name, int *tex, int x, int y, int w, int h, int depth)
{
    obj o;
    o.name = name;
    o.tex = tex;
    o.x = x;
    o.y = y;
    o.w = w;
    o.h = h;
    o.depth = depth;

    objs.push_back(o);
}

void Object::add(string name, int *tex, int x, int y, int w, int h)
{
    add(name, tex, x, y, w, h, 0);
}

void Object::add(string name, int *tex)
{
    add(name, tex, 0, 0, 20, 20, 0);
}

obj Object::get(int id)
{
    return objs[id];
}

obj Object::get(string name)
{
    for (int i = 0; i < objs.size(); i++)
    {
        if (name == objs[i].name)
            return objs[i];
    }
}