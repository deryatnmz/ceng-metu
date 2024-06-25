#include <iostream>
#include "parser.h"
#include "ppm.h"
#include <limits>
#include <cmath>
#include "raytracer.h"
using namespace std;
typedef unsigned char RGB[3];
parser::Scene scene;
std::vector<Surface> surfaces;


parser::Vec3f vector_addition(parser::Vec3f a, parser::Vec3f b){
    parser::Vec3f result;
    result.x = a.x+b.x;
    result.y = a.y+b.y;
    result.z = a.z+b.z;
    return result;
}

parser::Vec3f vector_subtract(parser::Vec3f a, parser::Vec3f b){
    parser::Vec3f result;
    result.x = a.x-b.x;
    result.y = a.y-b.y;
    result.z = a.z-b.z;
    return result;
}

parser::Vec3f vector_scale(float scale, parser::Vec3f a){
    parser::Vec3f result;
    result.x = scale * a.x;
    result.y = scale * a.y;
    result.z = scale * a.z;
    return result;
}

float dot_product(parser::Vec3f a, parser::Vec3f b){
    float result = 0;
    result += (a.x * b.x);
    result += (a.y * b.y);
    result += (a.z * b.z);
    return  result;

}

parser::Vec3f cross_product(parser::Vec3f a, parser::Vec3f b){
    parser::Vec3f result;
    result.x = (a.y * b.z) - (a.z * b.y);
    result.y = (a.z * b.x) - (a.x * b.z);
    result.z = (a.x * b.y) - (a.y * b.x);
    return result;
}

float vector_len(parser::Vec3f a){
    return sqrtf(powf(a.x, 2) + powf(a.y, 2) + powf(a.z, 2));
}

parser::Vec3f normalize(parser::Vec3f a){
    return vector_scale(1.0f / vector_len(a), a);
}

parser::Vec3f combine_vectorf(parser::Vec3f a, parser::Vec3f b){
    parser::Vec3f result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    return result;
}


float determinant(parser::Vec3f a, parser::Vec3f b, parser::Vec3f c){
    float f1 = a.x * ((b.y * c.z) - (c.y * b.z));
    float f2 = a.y * ((c.x * b.z) - (b.x * c.z));
    float f3 = a.z * ((b.x * c.y) - (b.y * c.x));
    return f1 + f2 + f3;
}


float triangle_intersect(parser::Face face, parser::Vec3f ray_origin, parser::Vec3f ray_direction){
    parser::Vec3f v1 = scene.vertex_data[face.v0_id-1];
    parser::Vec3f v2 = scene.vertex_data[face.v1_id-1];
    parser::Vec3f v3 = scene.vertex_data[face.v2_id-1];

    parser::Vec3f vec1 = vector_subtract(v1, v2);
    parser::Vec3f vec2 = vector_subtract(v1,  v3);
    parser::Vec3f vec3 = vector_subtract(v1, ray_origin);

    float detA = determinant(vec1, vec2, ray_direction);
    float beta = determinant(vec3, vec2, ray_direction) / detA;
    float gamma = determinant(vec1, vec3, ray_direction) / detA;

    if( beta >= 0 && gamma >= 0 && beta + gamma <= 1)
        return determinant(vec1, vec2, vec3) / detA;
    else
        return -1;
}

float tri_camera_intersect(Plane t_camera, parser::Vec3f ray_origin, parser::Vec3f ray_direction){
    float detA = determinant(t_camera.vec1, t_camera.vec2, ray_direction);
    float beta = determinant(t_camera.vec3, t_camera.vec2, ray_direction) / detA;
    float gamma = determinant(t_camera.vec1, t_camera.vec3, ray_direction) / detA;

    if(beta >= 0 && gamma >= 0 && beta + gamma <= 1 )
        return t_camera.det / detA;
    else
        return -1;
}

float sphere_intersect(parser::Sphere sphere, parser::Vec3f ray_origin, parser::Vec3f ray_direction){
    float delta, t, d;
    parser::Vec3f c = scene.vertex_data[sphere.center_vertex_id-1];
    parser::Vec3f o_to_c = vector_subtract(ray_origin,  c);
    delta = powf(dot_product(ray_direction, o_to_c),2) - dot_product(ray_direction, ray_direction)*(dot_product(o_to_c, o_to_c) - powf(sphere.radius, 2));
    if (delta < 0)
        t = -1;
    else{
        d = dot_product(ray_direction, ray_direction);
        delta = sqrtf(delta);
        t = -dot_product(ray_direction, o_to_c);
        t = std::min((t + delta)/d , (t - delta)/d );
    }
    return t;
}

parser::Vec3f specular_diffused(parser::Vec3f x, parser::Vec3f n, parser::Vec3f w0_direction, parser::Material material, int count){
    parser::Vec3f specular;
    parser::Vec3f diffused;

    specular.x = 0;
    specular.y = 0;
    specular.z = 0;

    diffused.x = 0;
    diffused.y = 0;
    diffused.z = 0;

    for (int i = 0; i < scene.point_lights.size(); i++){
        parser::Vec3f lightRay_origin = x;
        parser::Vec3f lightRay_direction = normalize(vector_subtract(scene.point_lights[i].position, x));
        float t;
        float minT = vector_len(vector_subtract(x,scene.point_lights[i].position));
        bool visible = true;

        for(int j = 0; j < surfaces.size(); j++){
            t = triangle_intersect(surfaces[j].triangle.indices, lightRay_origin,lightRay_direction);
            if(t > scene.shadow_ray_epsilon && t < minT){
                visible = false;
                break;
            }
        }
        if(visible){
            for (int k = 0; k < scene.spheres.size(); k++) {
                t = sphere_intersect(scene.spheres[k], lightRay_origin,lightRay_direction);
                if (t > scene.shadow_ray_epsilon && t < minT){
                    visible = false;
                    break;
                }
            }
        }
        if(visible){
            parser::Vec3f h = normalize(vector_addition(w0_direction, lightRay_direction));
            float cos_theta = dot_product(lightRay_direction, n);
            if (cos_theta > 0){
                float cos_alpha = powf(std::max(0.0f, dot_product(n, h)), material.phong_exponent);
                parser::Vec3f radiance = vector_scale((1.0f / powf(vector_len(vector_subtract(x,scene.point_lights[i].position)), 2)), scene.point_lights[i].intensity);
                specular = vector_addition(specular, vector_scale(cos_alpha, radiance));
                diffused = vector_addition(diffused, vector_scale(cos_theta, radiance));
            }
        }
    }
    specular = combine_vectorf(specular, material.specular);
    diffused = combine_vectorf(diffused, material.diffuse);
    if (material.is_mirror && count <= scene.max_recursion_depth) {
        parser::Vec3f reflect_origin = x;
        parser::Vec3f reflect_direction = normalize(vector_subtract(vector_scale(2*dot_product(n, w0_direction), n),w0_direction));
        Surface nearest;
        parser::Sphere close_sphere;
        int closePoint = 0;
        float t;
        float minT = std::numeric_limits<float>::max();
        for(int m = 0; m < surfaces.size(); m++) {
            t = triangle_intersect(surfaces[m].triangle.indices, reflect_origin,reflect_direction);
            if(t > scene.shadow_ray_epsilon && t < minT){
                minT = t;
                nearest = surfaces[m];
                closePoint = 1;
            }
        }
        for (int s = 0; s < scene.spheres.size(); s++) {
            t = sphere_intersect(scene.spheres[s], reflect_origin,reflect_direction);
            if (t > scene.shadow_ray_epsilon && t < minT){
                minT = t;
                close_sphere = scene.spheres[s];
                closePoint = 2;
            }
        }
        parser::Vec3f color;
        if(closePoint==0){
            color.x = 0;
            color.y = 0;
            color.z = 0;
        }
        else if(closePoint==1){
            parser::Vec3f ambient = combine_vectorf(scene.ambient_light, scene.materials[nearest.triangle.material_id-1].ambient);
            parser::Vec3f p = vector_addition(reflect_origin, vector_scale(minT, reflect_direction));
            color = vector_addition(ambient, specular_diffused(p, nearest.normal, normalize(vector_scale(-1, reflect_direction)), scene.materials[nearest.triangle.material_id-1], count+1));
            color = combine_vectorf(color, material.mirror);
        }
        else{
            parser::Vec3f ambient = combine_vectorf(scene.ambient_light, scene.materials[close_sphere.material_id-1].ambient);
            parser::Vec3f p = vector_addition(reflect_origin, vector_scale(minT, reflect_direction));
            color = vector_addition(ambient, specular_diffused(p, normalize(vector_addition(p, vector_scale(-1, scene.vertex_data[close_sphere.center_vertex_id-1]))), vector_scale(-1, reflect_direction), scene.materials[close_sphere.material_id-1], count+1));
            color = combine_vectorf(color, material.mirror);
        }
        specular = vector_addition(specular, color);
    }
    return vector_addition(specular, diffused);
}


int main(int argc, char* argv[])
{
    scene.loadFromXml(argv[1]);
    for (int i = 0; i < scene.triangles.size(); i++) {
        Surface surface;
        surface.triangle = scene.triangles[i];
        surface.normal = normalize(cross_product(vector_subtract(scene.vertex_data[scene.triangles[i].indices.v2_id-1], scene.vertex_data[scene.triangles[i].indices.v1_id-1]), vector_subtract(scene.vertex_data[scene.triangles[i].indices.v0_id-1],  scene.vertex_data[scene.triangles[i].indices.v1_id-1])));
        surfaces.push_back(surface);
    }
    for (int m = 0; m < scene.meshes.size(); m++) {
        for(int f = 0; f < scene.meshes[m].faces.size(); f++) {
            Surface surface;
            surface.triangle.indices = scene.meshes[m].faces[f];
            surface.triangle.material_id = scene.meshes[m].material_id;
            surface.normal =normalize(cross_product(vector_subtract(scene.vertex_data[surface.triangle.indices.v2_id-1], scene.vertex_data[surface.triangle.indices.v1_id-1]), vector_subtract(scene.vertex_data[surface.triangle.indices.v0_id-1],  scene.vertex_data[surface.triangle.indices.v1_id-1])));
            surfaces.push_back(surface);
        }
    }
    for(int c  = 0; c < scene.cameras.size(); c++){
        parser::Camera camera = scene.cameras[c];
        for (int s_id = 0; s_id < surfaces.size(); s_id++){
            Plane relation;
            parser::Vec3f a = scene.vertex_data[surfaces[s_id].triangle.indices.v0_id-1];
            parser::Vec3f b = scene.vertex_data[surfaces[s_id].triangle.indices.v1_id-1];
            parser::Vec3f c = scene.vertex_data[surfaces[s_id].triangle.indices.v2_id-1];
            relation.vec1 = vector_subtract(a,  b);
            relation.vec2 = vector_subtract(a,  c);
            relation.vec3 = vector_subtract(a, camera.position);
            relation.det = determinant(relation.vec1, relation.vec2, relation.vec3);
            surfaces[s_id].relationship.push_back(relation);
        }
    }
    float pixel_width, pixel_height;
    parser::Vec3f u,v,w,q,m;
    int width, height;
    for (int c  = 0; c < scene.cameras.size(); c++){
        parser::Camera camera = scene.cameras[c];
        parser::Vec4f image_plane = camera.near_plane;
        pixel_width =  (image_plane.y - image_plane.x)/camera.image_width;
        pixel_height = (image_plane.w - image_plane.z) / camera.image_height;
        w = normalize(vector_scale(-1,camera.gaze));
        v = normalize(camera.up);
        u = normalize(cross_product(v,w));
        m = vector_subtract(camera.position, vector_scale(camera.near_distance,w));
        q = vector_addition( m, vector_addition(vector_scale(image_plane.x,u),vector_scale(image_plane.w,v)));
        height = camera.image_height;
        width = camera.image_width; 
        unsigned char * image = new unsigned char [height * width * 3];
        parser::Vec3f eye_ray_origin, eye_ray_direction, s, t_0, t_1,t_2;
        float s_u, s_v;
        int image_id=0;
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                parser::Vec3f eye_ray_origin = camera.position;
                parser::Vec3f eye_ray_direction,s;
                float  s_u = ((float)j+0.5f) * pixel_width;
                float s_v = ((float)i+0.5f) * pixel_height;
                s = vector_addition (q , vector_addition(vector_scale(s_u,u), vector_scale(-s_v,v)));
                eye_ray_direction = vector_subtract(s,eye_ray_origin);
                int closePoint = 0;
                float t;
                float minT = std::numeric_limits<float>::max();
                Surface nearest;
                parser::Sphere close_sphere;
                for(int surf_id = 0; surf_id < surfaces.size(); surf_id++) {
                    t = tri_camera_intersect(surfaces[surf_id].relationship[c], eye_ray_origin,eye_ray_direction);
                    if(t > 0 && t < minT){
                        minT = t;
                        nearest = surfaces[surf_id];
                        closePoint = 1;
                    }
                }
                for (int sph_id = 0; sph_id < scene.spheres.size(); sph_id++) {
                    t = sphere_intersect(scene.spheres[sph_id], eye_ray_origin,eye_ray_direction);
                    if (t > 0 && t < minT){
                        minT = t;
                        close_sphere = scene.spheres[sph_id];
                        closePoint = 2;
                    }
                }
                parser::Vec3f color;
                if(closePoint==0){
                    image[image_id++] = scene.background_color.x;
                    image[image_id++] = scene.background_color.y;
                    image[image_id++] = scene.background_color.z;
                }
                else if (closePoint==1){
                    parser::Vec3f ambient = combine_vectorf(scene.ambient_light, scene.materials[nearest.triangle.material_id-1].ambient);
                    parser::Vec3f p = vector_addition(eye_ray_origin, vector_scale(minT, eye_ray_direction));
                    color = vector_addition(ambient, specular_diffused(p, nearest.normal, normalize(vector_scale(-1, eye_ray_direction)), scene.materials[nearest.triangle.material_id-1], 0));
                    image[image_id++] = max(0, min((int)(color.x + 0.5f), 255));
                    image[image_id++] = max(0, min((int)(color.y + 0.5f), 255));
                    image[image_id++] = max(0, min((int)(color.z + 0.5f), 255));
                }
                else{
                    parser::Vec3f ambient = combine_vectorf(scene.ambient_light, scene.materials[close_sphere.material_id-1].ambient);;
                    parser::Vec3f p = vector_addition(eye_ray_origin, vector_scale(minT, eye_ray_direction));
                    color = vector_addition(ambient, specular_diffused(p, normalize(vector_addition(p, vector_scale(-1, scene.vertex_data[close_sphere.center_vertex_id-1]))), vector_scale(-1, eye_ray_direction), scene.materials[close_sphere.material_id-1], 0));
                    image[image_id++] = max(0, min((int)(color.x + 0.5f), 255));
                    image[image_id++] = max(0, min((int)(color.y + 0.5f), 255));
                    image[image_id++] = max(0, min((int)(color.z + 0.5f), 255));
                }
            }
        }
        write_ppm(camera.image_name.c_str(), image, camera.image_width, camera.image_height);
    }
}