//
// Created by Seyma on 31-Oct-22.
//

#ifndef CODE_TEMPLATE_RAYTRACER_H
#define CODE_TEMPLATE_RAYTRACER_H
#include "parser.h"


class Plane
{
public:
    parser::Vec3f vec1;
    parser::Vec3f vec2;
    parser::Vec3f vec3;
    float det;
};

class Surface
{
public:
    parser::Triangle triangle;
    parser::Vec3f normal;
    std::vector<Plane> relationship;
};

#endif //CODE_TEMPLATE_RAYTRACER_H
