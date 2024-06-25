#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

using namespace tinyxml2;
using namespace std;

Color round_color(const Color& a){

    Color result;

    result.r = (int)(a.r + 0.5);
    result.g = (int)(a.g + 0.5);
    result.b = (int)(a.b + 0.5);

    return result;
}

Color multiply_color(const Color& a,double number){

    Color result;

    result.r = a.r * number;
    result.g = a.g * number;
    result.b = a.b * number;

    return result;
}

Color divide_color(const Color& a,double number){

    Color result;

    result.r = a.r / number;
    result.g = a.g / number;
    result.b = a.b / number;

    return result;
}

Color color_subtraction(const Color& a,const Color& b){

    Color result;

    result.r = a.r - b.r;
    result.g = a.g - b.g;
    result.b = a.b - b.b;

    return result;
}

Color color_addition(const Color& a,const Color& b){

    Color result;

    result.r = a.r + b.r;
    result.g = a.g + b.g;
    result.b = a.b + b.b;

    return result;
}

Vec4 vect4div(const Vec4& vector, double number){

    Vec4 result;

    result.x=vector.x / number;
    result.y=vector.y / number;
    result.z=vector.z / number;
    result.t=vector.t / number;

    return result;
}

bool visible(double & t_E, double & t_l, double den, double num,int x){
    // 46.slayt

    double t;

    if(den > 0){
        t = num/den;
        if(t > t_l) return false;
        if(t > t_E) t_E = t;
    } else if( den < 0 ){
        t = num/den;
        if (t < t_E) return false;
        if(t < t_l) t_l = t;
    } else if(num > 0){
        return false;
    }

    return true;

}

bool line_clipping(Vec4 &v0, Color &c0, Vec4 &v1, Color &c1){
    // 47.slayt

    double t_E = 0, t_L = 1;
    double dx = v1.x - v0.x;
    double dy = v1.y - v0.y;
    double dz = v1.z - v0.z;

    Color dc = color_subtraction(c1 , c0);

    bool result = false;

    if (visible(t_E, t_L,dx, (-1)-v0.x,0)){
        if(visible(t_E, t_L,-dx, v0.x-1,0)){
            if(visible(t_E, t_L,dy, (-1)-v0.y,0)){
                if(visible(t_E, t_L,-dy, v0.y-1,0)){
                    if(visible(t_E, t_L,dz, (-1)-v0.z,0)){
                        if(visible(t_E, t_L,-dz, v0.z-1,0)){

                            result = true;

                            if (t_L < 1) {
                                v1.x = v0.x + (dx * t_L);
                                v1.y = v0.y + (dy * t_L);
                                v1.z = v0.z + (dz * t_L);
                                c1 = color_addition(c0 , multiply_color(dc , t_L));
                            }
                            if (t_E > 0) {
                                v0.x = v0.x + (dx * t_E);
                                v0.y = v0.y + (dy * t_E);
                                v0.z = v0.z + (dz * t_E);
                                c0 = color_addition(c0 , multiply_color(dc , t_E));
                            }
    }}}}}}

    return result;

}

void rasterize(vector<vector<Color>> & image, Vec4 & v0, Vec4 & v1, Color & c0, Color & c1) {
    //slayt sayfa 22

    int d;
    int i = 1;
    Color dc, c;
    double dif_x = abs(v1.x - v0.x);
    double dif_y = abs(v1.y - v0.y);
    double slope = dif_y/dif_x;

    if (slope <= 1) {

        if (v1.x < v0.x) {
            swap(v0, v1);
            swap(c0, c1);
        }
        if (v1.y < v0.y) {
            i = -1;
        }

        int y = v0.y;
        d = (v0.y - v1.y) + (i * 0.5 * (v1.x - v0.x));
        c = c0;
        dc = divide_color(color_subtraction(c1 , c0) , (v1.x - v0.x));

        for (int x = v0.x; x <= v1.x; x++) {

            image[x][y] = round_color(c);

            if (d * i < 0) { // choose NE

                y += i;
                d += (v0.y - v1.y) + (i * (v1.x - v0.x));

            }
            else d += (v0.y - v1.y); // choose E

            c = color_addition(c , dc);
        }
    }
    else if (slope > 1) {

        if (v1.y < v0.y) {
            swap(v0, v1);
            swap(c0, c1);
        }
        if (v1.x < v0.x) {
            i = -1;
        }

        int x = v0.x;
        d = (v1.x - v0.x) + (i * 0.5 * (v0.y - v1.y));
        c = c0;
        dc = divide_color(color_subtraction(c1 , c0) , (v1.y - v0.y));

        for (int y = v0.y; y <= v1.y; y++) {

            image[x][y] = round_color(c);

            if (d * i > 0) {
                x += i;
                d += (v1.x - v0.x) + (i * (v0.y - v1.y));
            }
            else d += (v1.x - v0.x);

            c = color_addition(c , dc);
        }
    }
}

double bary_c(double x, double y, double x_0, double y_0, double x_1, double y_1){
    //slayt sayfa 24

    double result;
    result = (x * (y_0 - y_1)) + (y * (x_1 - x_0)) + (x_0 * y_1) - (y_0 * x_1);
    return result;

}



void Scene::forwardRenderingPipeline(Camera * camera) {

    double cam[4][4] =
            {{camera->u.x, camera->u.y, camera->u.z, -(camera->u.x*camera->pos.x + camera->u.y*camera->pos.y + camera->u.z*camera->pos.z)},
             {camera->v.x, camera->v.y, camera->v.z, -(camera->v.x*camera->pos.x + camera->v.y*camera->pos.y + camera->v.z*camera->pos.z)},
             {camera->w.x, camera->w.y, camera->w.z, -(camera->w.x*camera->pos.x + camera->w.y*camera->pos.y + camera->w.z*camera->pos.z)},
             {0,0,0,1}};
    Matrix4 M_cam = Matrix4(cam);

    Matrix4 M_proj;

    if(camera->projectionType)
    {
        double M_per[4][4]= {{(2*camera->near)/(camera->right - camera->left), 0, (camera->right + camera->left)/(camera->right - camera->left), 0},
                             {0, (2*camera->near)/(camera->top - camera->bottom), (camera->top + camera->bottom)/(camera->top - camera->bottom), 0},
                             {0, 0, -(camera->far + camera->near)/(camera->far - camera->near), -(2*camera->far*camera->near)/(camera->far - camera->near)},
                             {0, 0, -1, 0}};
        M_proj = Matrix4(M_per);
    }else {
        double M_orth[4][4]= {{2/(camera->right - camera->left), 0, 0, -(camera->right + camera->left)/(camera->right - camera->left)},
                              {0, 2/(camera->top - camera->bottom), 0, -(camera->top + camera->bottom)/(camera->top - camera->bottom)},
                              {0 ,0, -2/(camera->far - camera->near), -(camera->far + camera->near)/(camera->far - camera->near)},
                              {0, 0, 0, 1}};
        M_proj = Matrix4(M_orth);
    }

    double view[4][4] = {{(camera->horRes)/2.0, 0, 0, (camera->horRes - 1)/2.0},
                         {0, camera->verRes/2.0, 0, (camera->verRes - 1)/2.0},
                         {0, 0, 1.0/2.0, 1.0/2.0},
                         {0, 0, 0, 1}};
    Matrix4 M_viewport = Matrix4(view);


    for (int j = 0; j < meshes.size(); j++) {

        Mesh * mesh = meshes[j];
        Matrix4 M_mesh = getIdentityMatrix();

        for (int i = 0; i < mesh->numberOfTransformations; ++i) {
            if (mesh->transformationTypes[i] == 's') {

                Scaling * s = scalings[mesh->transformationIds[i]-1];
                double scaling_Matrix[4][4] = {{s->sx,0,0,0},
                                               {0,s->sy,0,0},
                                               {0,0,s->sz,0},
                                               {0,0,0,1}};
                M_mesh = multiplyMatrixWithMatrix(scaling_Matrix, M_mesh);

            }else if (mesh->transformationTypes[i] == 't') {

                Translation * t = translations[mesh->transformationIds[i]-1];
                double translation_matrix[4][4] = {{1,0,0,t->tx},
                                                   {0,1,0,t->ty},
                                                   {0,0,1,t->tz},
                                                   {0,0,0,1}};
                M_mesh = multiplyMatrixWithMatrix(translation_matrix, M_mesh);

            }
            else if (mesh->transformationTypes[i] == 'r') {

                Rotation * r = rotations[mesh->transformationIds[i]-1]; // since transformations start from 1

                // slaytlardaki sayfa 41 de baslayan alternatif method ile:

                double rotation_matrix[4][4] = {{1,0,0,0},
                                                {0,cos(r->angle * M_PI/180), -1 * sin(r->angle * M_PI/180),0},
                                                {0, sin(r->angle * M_PI/180), cos(r->angle * M_PI/180), 0},
                                                {0,0,0,1}}; //R_x(alpha) matrixi

                double minimum_component_0 = std::min(abs(r->ux), abs(r->uy));
                double minimum_component = std::min(minimum_component_0, abs(r->uz));

                Vec3 v;

                if(minimum_component == abs(r->ux)){
                    v = Vec3(0,-1 * r->uz,r->uy,-1);
                } else if(minimum_component == abs(r->uy)){
                    v = Vec3(-1 * r->uz,0,r->ux,-1);
                } else if(minimum_component == abs(r->uz)){
                    v = Vec3(-1 * r->uy,r->ux,0,-1);
                }

                Vec3 u = Vec3(r->ux,r->uy,r->uz,-1);

                Vec3 w;

                w = crossProductVec3(u,v);
                v = normalizeVec3(v);
                w = normalizeVec3(w);

                double M_inverse[4][4] = {{u.x, v.x, w.x, 0},
                                          {u.y, v.y, w.y,0},
                                          {u.z,v.z,w.z,0},
                                          {0,0,0,1}};

                double M[4][4] = {{u.x,u.y,u.z,0},
                                  {v.x,v.y,v.z,0},
                                  {w.x,w.y,w.z,0},
                                  {0,0,0,1}};

                Matrix4 first = multiplyMatrixWithMatrix(rotation_matrix, M);
                Matrix4 second = multiplyMatrixWithMatrix(M_inverse,first);
                M_mesh = second;
            }
            else;
        }

        Matrix4 M_cam_mesh = multiplyMatrixWithMatrix(M_cam, M_mesh);
        Matrix4 M_proj_cam_mesh = multiplyMatrixWithMatrix(M_proj, M_cam_mesh);

        for (int t = 0; t < mesh->triangles.size(); t++) {

            Triangle & triangle = mesh->triangles[t];

            const Vec3 * v0 = this->vertices[triangle.getFirstVertexId()-1];
            const Vec3 * v1 = this->vertices[triangle.getSecondVertexId()-1];
            const Vec3 * v2 = this->vertices[triangle.getThirdVertexId()-1];

            const Color * c0 = this->colorsOfVertices[v0->colorId-1];
            const Color * c1 = this->colorsOfVertices[v1->colorId-1];
            const Color * c2 = this->colorsOfVertices[v2->colorId-1];

            Vec4 last_v0 = multiplyMatrixWithVec4(M_proj_cam_mesh, Vec4(v0->x, v0->y, v0->z, 1, v0->colorId));
            Vec4 last_v1 = multiplyMatrixWithVec4(M_proj_cam_mesh, Vec4(v1->x, v1->y, v1->z, 1, v1->colorId));
            Vec4 last_v2 = multiplyMatrixWithVec4(M_proj_cam_mesh, Vec4(v2->x, v2->y, v2->z, 1, v2->colorId));

            Vec3 c00 = Vec3(last_v0.x,last_v0.y,last_v0.z,last_v0.colorId);
            Vec3 c11 = Vec3(last_v1.x,last_v1.y,last_v1.z,last_v1.colorId);
            Vec3 c22 = Vec3(last_v2.x,last_v2.y,last_v2.z,last_v2.colorId);

            Vec3 edge1 = subtractVec3(c11,c00);
            Vec3 edge2 = subtractVec3(c22,c00);
            Vec3 normal = crossProductVec3(edge1,edge2);

            normal = normalizeVec3(normal);

            double result = dotProductVec3(normal,c00);
            bool backface;

            if (result < 0) backface = true;
            else backface = false;

            if (this->cullingEnabled && backface)
                continue;

            if (mesh->type == 0) { //wireframe mode

                Vec4 L01_1=vect4div(last_v0,last_v0.t);
                Color L01_1_color = *c0;
                Vec4 L01_2= vect4div(last_v1, last_v1.t);
                Color L01_2_color= *c1;

                Vec4 L12_1 = vect4div (last_v1,last_v1.t);
                Color L12_1_color = *c1;
                Vec4 L12_2 = vect4div (last_v2, last_v2.t);
                Color L12_2_color= *c2;

                Vec4 L20_1=vect4div(last_v2,last_v2.t);
                Color L20_1_color = *c2;
                Vec4 L20_2 = vect4div(last_v0,last_v0.t);
                Color L20_2_color= *c0;

                bool L01_visibility = line_clipping(L01_1,L01_1_color, L01_2,L01_2_color );
                bool L12_visibility = line_clipping(L12_1,L12_1_color, L12_2,L12_2_color);
                bool L20_visibility = line_clipping(L20_1,L20_1_color, L20_2,L20_2_color);

                L01_1 = multiplyMatrixWithVec4(M_viewport, L01_1);
                L01_2 = multiplyMatrixWithVec4(M_viewport, L01_2);

                L12_1 = multiplyMatrixWithVec4(M_viewport, L12_1);
                L12_2 = multiplyMatrixWithVec4(M_viewport, L12_2);

                L20_1 = multiplyMatrixWithVec4(M_viewport, L20_1);
                L20_2 = multiplyMatrixWithVec4(M_viewport, L20_2);

                if(L01_visibility)
                    rasterize(this->image, L01_1, L01_2, L01_1_color, L01_2_color); //L01
                if(L12_visibility)
                    rasterize(this->image, L12_1, L12_2, L12_1_color, L12_2_color); //L12
                if(L20_visibility)
                    rasterize(this->image, L20_1, L20_2, L20_1_color, L20_2_color); //L20
            }
            else {

                last_v0 = vect4div(last_v0,last_v0.t);
                last_v1 = vect4div(last_v1,last_v1.t);
                last_v2 = vect4div(last_v2, last_v2.t);

                Vec4 view0 = multiplyMatrixWithVec4(M_viewport, last_v0);
                Vec4 view1 = multiplyMatrixWithVec4(M_viewport, last_v1);
                Vec4 view2 = multiplyMatrixWithVec4(M_viewport, last_v2);

                int nx=camera->horRes;
                int ny = camera->verRes;
                int x_min = min(min(view0.x, view1.x), view2.x);

                if(x_min >= 0) x_min = min(min(view0.x, view1.x), view2.x);
                else x_min = 0;

                if(x_min > nx - 1) x_min = nx - 1;

                int y_min = min(min(view0.y, view1.y), view2.y);

                if(y_min >= 0) y_min = min(min(view0.y, view1.y), view2.y);
                else y_min = 0;

                if(y_min > ny - 1) y_min = ny - 1;

                int x_max = max(max(view0.x, view1.x), view2.x);
                if(x_max < 0) x_max = 0;
                else if((max(max(view0.x, view1.x), view2.x) > nx-1 )) {
                    x_max = nx -1;
                } else x_max = max(max(view0.x, view1.x), view2.x);

                int y_max = max(max(view0.y, view1.y), view2.y);
                if(y_max < 0) y_max = 0;
                else if((max(max(view0.y, view1.y), view2.y) > ny-1 )) {
                    y_max = ny -1;
                } else y_max = max(max(view0.y, view1.y), view2.y);

                double a;
                double b;
                double g;
                Color c;

                for(int y=y_min; y<=y_max; ++y){
                    for(int x=x_min; x<=x_max; ++x){

                        a = bary_c(x,y, view1.x, view1.y, view2.x, view2.y) / bary_c(view0.x,view0.y, view1.x,view1.y, view2.x,view2.y); // L12
                        b = bary_c(x,y, view2.x, view2.y, view0.x, view0.y) / bary_c(view1.x,view1.y, view2.x,view2.y, view0.x,view0.y); // L20
                        g = bary_c(x,y, view0.x, view0.y, view1.x, view1.y) / bary_c(view2.x,view2.y, view0.x,view0.y, view1.x,view1.y); // L01

                        if(a>=0 && b>=0 && g>=0){
                            c = color_addition(multiply_color((*c0) , a) , color_addition(multiply_color((*c1) , b) , multiply_color((*c2) , g)));
                            this->image[x][y] = round_color(c);
                        }
                    }
                }

            }
        }
    }
}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
    const char *str;
    XMLDocument xmlDoc;
    XMLElement *pElement;

    xmlDoc.LoadFile(xmlPath);

    XMLNode *pRoot = xmlDoc.FirstChild();

    // read background color
    pElement = pRoot->FirstChildElement("BackgroundColor");
    str = pElement->GetText();
    sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

    // read culling
    pElement = pRoot->FirstChildElement("Culling");
    if (pElement != NULL) {
        str = pElement->GetText();

        if (strcmp(str, "enabled") == 0) {
            cullingEnabled = true;
        }
        else {
            cullingEnabled = false;
        }
    }

    // read cameras
    pElement = pRoot->FirstChildElement("Cameras");
    XMLElement *pCamera = pElement->FirstChildElement("Camera");
    XMLElement *camElement;
    while (pCamera != NULL)
    {
        Camera *cam = new Camera();

        pCamera->QueryIntAttribute("id", &cam->cameraId);

        // read projection type
        str = pCamera->Attribute("type");

        if (strcmp(str, "orthographic") == 0) {
            cam->projectionType = 0;
        }
        else {
            cam->projectionType = 1;
        }

        camElement = pCamera->FirstChildElement("Position");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

        camElement = pCamera->FirstChildElement("Gaze");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

        camElement = pCamera->FirstChildElement("Up");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

        cam->gaze = normalizeVec3(cam->gaze);
        cam->u = crossProductVec3(cam->gaze, cam->v);
        cam->u = normalizeVec3(cam->u);

        cam->w = inverseVec3(cam->gaze);
        cam->v = crossProductVec3(cam->u, cam->gaze);
        cam->v = normalizeVec3(cam->v);

        camElement = pCamera->FirstChildElement("ImagePlane");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
               &cam->left, &cam->right, &cam->bottom, &cam->top,
               &cam->near, &cam->far, &cam->horRes, &cam->verRes);

        camElement = pCamera->FirstChildElement("OutputName");
        str = camElement->GetText();
        cam->outputFileName = string(str);

        cameras.push_back(cam);

        pCamera = pCamera->NextSiblingElement("Camera");
    }

    // read vertices
    pElement = pRoot->FirstChildElement("Vertices");
    XMLElement *pVertex = pElement->FirstChildElement("Vertex");
    int vertexId = 1;

    while (pVertex != NULL)
    {
        Vec3 *vertex = new Vec3();
        Color *color = new Color();

        vertex->colorId = vertexId;

        str = pVertex->Attribute("position");
        sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

        str = pVertex->Attribute("color");
        sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

        vertices.push_back(vertex);
        colorsOfVertices.push_back(color);

        pVertex = pVertex->NextSiblingElement("Vertex");

        vertexId++;
    }

    // read translations
    pElement = pRoot->FirstChildElement("Translations");
    XMLElement *pTranslation = pElement->FirstChildElement("Translation");
    while (pTranslation != NULL)
    {
        Translation *translation = new Translation();

        pTranslation->QueryIntAttribute("id", &translation->translationId);

        str = pTranslation->Attribute("value");
        sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

        translations.push_back(translation);

        pTranslation = pTranslation->NextSiblingElement("Translation");
    }

    // read scalings
    pElement = pRoot->FirstChildElement("Scalings");
    XMLElement *pScaling = pElement->FirstChildElement("Scaling");
    while (pScaling != NULL)
    {
        Scaling *scaling = new Scaling();

        pScaling->QueryIntAttribute("id", &scaling->scalingId);
        str = pScaling->Attribute("value");
        sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

        scalings.push_back(scaling);

        pScaling = pScaling->NextSiblingElement("Scaling");
    }

    // read rotations
    pElement = pRoot->FirstChildElement("Rotations");
    XMLElement *pRotation = pElement->FirstChildElement("Rotation");
    while (pRotation != NULL)
    {
        Rotation *rotation = new Rotation();

        pRotation->QueryIntAttribute("id", &rotation->rotationId);
        str = pRotation->Attribute("value");
        sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

        rotations.push_back(rotation);

        pRotation = pRotation->NextSiblingElement("Rotation");
    }

    // read meshes
    pElement = pRoot->FirstChildElement("Meshes");

    XMLElement *pMesh = pElement->FirstChildElement("Mesh");
    XMLElement *meshElement;
    while (pMesh != NULL)
    {
        Mesh *mesh = new Mesh();

        pMesh->QueryIntAttribute("id", &mesh->meshId);

        // read projection type
        str = pMesh->Attribute("type");

        if (strcmp(str, "wireframe") == 0) {
            mesh->type = 0;
        }
        else {
            mesh->type = 1;
        }

        // read mesh transformations
        XMLElement *pTransformations = pMesh->FirstChildElement("Transformations");
        XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

        while (pTransformation != NULL)
        {
            char transformationType;
            int transformationId;

            str = pTransformation->GetText();
            sscanf(str, "%c %d", &transformationType, &transformationId);

            mesh->transformationTypes.push_back(transformationType);
            mesh->transformationIds.push_back(transformationId);

            pTransformation = pTransformation->NextSiblingElement("Transformation");
        }

        mesh->numberOfTransformations = mesh->transformationIds.size();

        // read mesh faces
        char *row;
        char *clone_str;
        int v1, v2, v3;
        XMLElement *pFaces = pMesh->FirstChildElement("Faces");
        str = pFaces->GetText();
        clone_str = strdup(str);

        row = strtok(clone_str, "\n");
        while (row != NULL)
        {
            int result = sscanf(row, "%d %d %d", &v1, &v2, &v3);

            if (result != EOF) {
                mesh->triangles.push_back(Triangle(v1, v2, v3));
            }
            row = strtok(NULL, "\n");
        }
        mesh->numberOfTriangles = mesh->triangles.size();
        meshes.push_back(mesh);

        pMesh = pMesh->NextSiblingElement("Mesh");
    }
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
    if (this->image.empty())
    {
        for (int i = 0; i < camera->horRes; i++)
        {
            vector<Color> rowOfColors;

            for (int j = 0; j < camera->verRes; j++)
            {
                rowOfColors.push_back(this->backgroundColor);
            }

            this->image.push_back(rowOfColors);
        }
    }
    else
    {
        for (int i = 0; i < camera->horRes; i++)
        {
            for (int j = 0; j < camera->verRes; j++)
            {
                this->image[i][j].r = this->backgroundColor.r;
                this->image[i][j].g = this->backgroundColor.g;
                this->image[i][j].b = this->backgroundColor.b;
            }
        }
    }
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
    if (value >= 255.0)
        return 255;
    if (value <= 0.0)
        return 0;
    return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
    ofstream fout;

    fout.open(camera->outputFileName.c_str());

    fout << "P3" << endl;
    fout << "# " << camera->outputFileName << endl;
    fout << camera->horRes << " " << camera->verRes << endl;
    fout << "255" << endl;

    for (int j = camera->verRes - 1; j >= 0; j--)
    {
        for (int i = 0; i < camera->horRes; i++)
        {
            fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
                 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
                 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
        }
        fout << endl;
    }
    fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
    string command;

    // call command on Ubuntu
    if (osType == 1)
    {
        command = "convert " + ppmFileName + " " + ppmFileName + ".png";
        system(command.c_str());
    }

        // call command on Windows
    else if (osType == 2)
    {
        command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
        system(command.c_str());
    }

        // default action - don't do conversion
    else
    {
    }
}