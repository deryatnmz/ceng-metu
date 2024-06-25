#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>   // The GL Header File
#include <GL/gl.h>   // The GL Header File
#include <GLFW/glfw3.h> // The GLFW header
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string.h>
#include <ctime>
#include<unordered_map>

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

using namespace std;

GLint gIntensityLoc;
float gIntensity = 1000;
int gWidth = 640, gHeight = 600;
float row, col;
string object;
vector<vector<GLuint>> bunnyProgram;
vector<vector<int>> bunnyColors;
vector<glm::vec3> randomColors = {{255, 0, 0}, {0,0,255},{0,255,0},{255,255,255},{0,255,255}, {0,0,0} };
GLuint textProgram;
int moves = 0, scores = 0;
bool isCurrentlyExploding = false;
pair<int, int> explodingObject(-1, -1);
float currentSize = 1.0;
bool isCheckNeeded = true;
bool isCurrentlyMoving = false;
float currentYTranslation = 0;
vector<vector<int>> explodingObjects;
vector<vector<float>> yTranslationLengths;
float incrx, incry, offsetx, offsety;


struct Vertex
{
    Vertex(GLfloat inX, GLfloat inY, GLfloat inZ) : x(inX), y(inY), z(inZ) { }
    GLfloat x, y, z;
};

struct Texture
{
    Texture(GLfloat inU, GLfloat inV) : u(inU), v(inV) { }
    GLfloat u, v;
};

struct Normal
{
    Normal(GLfloat inX, GLfloat inY, GLfloat inZ) : x(inX), y(inY), z(inZ) { }
    GLfloat x, y, z;
};

struct Face
{
	Face(int v[], int t[], int n[]) {
		vIndex[0] = v[0];
		vIndex[1] = v[1];
		vIndex[2] = v[2];
		tIndex[0] = t[0];
		tIndex[1] = t[1];
		tIndex[2] = t[2];
		nIndex[0] = n[0];
		nIndex[1] = n[1];
		nIndex[2] = n[2];
	}
    GLuint vIndex[3], tIndex[3], nIndex[3];
};

vector<Vertex> gVertices;
vector<Texture> gTextures;
vector<Normal> gNormals;
vector<Face> gFaces;

GLuint gVertexAttribBuffer, gTextVBO, gIndexBuffer;
GLint gInVertexLoc, gInNormalLoc;
int gVertexDataSizeInBytes, gNormalDataSizeInBytes;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;


bool ParseObj(const string& fileName)
{
    fstream myfile;

    // Open the input 
    myfile.open(fileName.c_str(), std::ios::in);

    if (myfile.is_open())
    {
        string curLine;

        while (getline(myfile, curLine))
        {
            stringstream str(curLine);
            GLfloat c1, c2, c3;
            GLuint index[9];
            string tmp;

            if (curLine.length() >= 2)
            {
                if (curLine[0] == '#') // comment
                {
                    continue;
                }
                else if (curLine[0] == 'v')
                {
                    if (curLine[1] == 't') // texture
                    {
                        str >> tmp; // consume "vt"
                        str >> c1 >> c2;
                        gTextures.push_back(Texture(c1, c2));
                    }
                    else if (curLine[1] == 'n') // normal
                    {
                        str >> tmp; // consume "vn"
                        str >> c1 >> c2 >> c3;
                        gNormals.push_back(Normal(c1, c2, c3));
                    }
                    else // vertex
                    {
                        str >> tmp; // consume "v"
                        str >> c1 >> c2 >> c3;
                        gVertices.push_back(Vertex(c1, c2, c3));
                    }
                }
                else if (curLine[0] == 'f') // face
                {
                    str >> tmp; // consume "f"
					char c;
					int vIndex[3],  nIndex[3], tIndex[3];
					str >> vIndex[0]; str >> c >> c; // consume "//"
					str >> nIndex[0]; 
					str >> vIndex[1]; str >> c >> c; // consume "//"
					str >> nIndex[1]; 
					str >> vIndex[2]; str >> c >> c; // consume "//"
					str >> nIndex[2]; 

					assert(vIndex[0] == nIndex[0] &&
						   vIndex[1] == nIndex[1] &&
						   vIndex[2] == nIndex[2]); // a limitation for now

					// make indices start from 0
					for (int c = 0; c < 3; ++c)
					{
						vIndex[c] -= 1;
						nIndex[c] -= 1;
						tIndex[c] -= 1;
					}

                    gFaces.push_back(Face(vIndex, tIndex, nIndex));
                }
                else
                {
                    cout << "Ignoring unidentified line in obj file: " << curLine << endl;
                }
            }

            //data += curLine;
            if (!myfile.eof())
            {
                //data += "\n";
            }
        }

        myfile.close();
    }
    else
    {
        return false;
    }

	assert(gVertices.size() == gNormals.size());

    return true;
}

bool ReadDataFromFile(
    const string& fileName, ///< [in]  Name of the shader file
    string&       data)     ///< [out] The contents of the file
{
    fstream myfile;

    // Open the input 
    myfile.open(fileName.c_str(), std::ios::in);

    if (myfile.is_open())
    {
        string curLine;

        while (getline(myfile, curLine))
        {
            data += curLine;
            if (!myfile.eof())
            {
                data += "\n";
            }
        }

        myfile.close();
    }
    else
    {
        return false;
    }

    return true;
}

void createVS(GLuint& program, const string& filename)
{
    string shaderSource;

    if (!ReadDataFromFile(filename, shaderSource))
    {
        cout << "Cannot find file name: " + filename << endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*) shaderSource.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &shader, &length);
    glCompileShader(vs);

    char output[1024] = {0};
    glGetShaderInfoLog(vs, 1024, &length, output);
    //printf("VS compile log: %s\n", output);

    glAttachShader(program, vs);
}

void createFS(GLuint& program, const string& filename)
{
    string shaderSource;

    if (!ReadDataFromFile(filename, shaderSource))
    {
        cout << "Cannot find file name: " + filename << endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*) shaderSource.c_str();

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &shader, &length);
    glCompileShader(fs);

    char output[1024] = {0};
    glGetShaderInfoLog(fs, 1024, &length, output);
    //printf("FS compile log: %s\n", output);

    glAttachShader(program, fs);
}

void initShaders()
{
    int attrib_index = 0;
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            bunnyProgram[i][j] = glCreateProgram();
            // GLint lightPosition = glGetUniformLocation(bunnyProgram[i][j], "lightPosition");
            // glUniform3f(lightPosition, i, j, 1);
            createVS(bunnyProgram[i][j], "vert0.glsl");
            createFS(bunnyProgram[i][j], "frag0.glsl");

            glBindAttribLocation(bunnyProgram[i][j], 0, "inVertex");
            glBindAttribLocation(bunnyProgram[i][j], 1, "inNormal");
            int randNumber = rand() % 5;
            glm::vec3 color = randomColors[randNumber];
            bunnyColors[i][j]=randNumber;
            glLinkProgram(bunnyProgram[i][j]);
            glUseProgram(bunnyProgram[i][j]);

            gIntensityLoc = glGetUniformLocation(bunnyProgram[i][j], "intensity");
            //cout << "gIntensityLoc = " << gIntensityLoc << endl;
            glUniform1f(gIntensityLoc, gIntensity);
            attrib_index += 2;
        }
    }

    textProgram = glCreateProgram();
    createVS(textProgram, "vert_text.glsl");
    createFS(textProgram, "frag_text.glsl");
    glBindAttribLocation(textProgram, 2, "vertex");
    glLinkProgram(textProgram);
}

void initVBO()
{
    int attrib_index = 0;
   
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
       
    // glEnableVertexAttribArray(0);
    // glEnableVertexAttribArray(1);
    assert(glGetError() == GL_NONE);

    glGenBuffers(1, &gVertexAttribBuffer);
    glGenBuffers(1, &gIndexBuffer);

    assert(gVertexAttribBuffer > 0 && gIndexBuffer > 0);

    glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);

    gVertexDataSizeInBytes = gVertices.size() * 3 * sizeof(GLfloat);
    gNormalDataSizeInBytes = gNormals.size() * 3 * sizeof(GLfloat);
    int indexDataSizeInBytes = gFaces.size() * 3 * sizeof(GLuint);
    GLfloat* vertexData = new GLfloat [gVertices.size() * 3];
    GLfloat* normalData = new GLfloat [gNormals.size() * 3];
    GLuint* indexData = new GLuint [gFaces.size() * 3];

    float minX = 1e6, maxX = -1e6;
    float minY = 1e6, maxY = -1e6;
    float minZ = 1e6, maxZ = -1e6;

    for (int i = 0; i < gVertices.size(); ++i)
    {
        vertexData[3*i] = gVertices[i].x;
        vertexData[3*i+1] = gVertices[i].y;
        vertexData[3*i+2] = gVertices[i].z;

        minX = std::min(minX, gVertices[i].x);
        maxX = std::max(maxX, gVertices[i].x);
        minY = std::min(minY, gVertices[i].y);
        maxY = std::max(maxY, gVertices[i].y);
        minZ = std::min(minZ, gVertices[i].z);
        maxZ = std::max(maxZ, gVertices[i].z);
    }



    for (int i = 0; i < gNormals.size(); ++i)
    {
        normalData[3*i] = gNormals[i].x;
        normalData[3*i+1] = gNormals[i].y;
        normalData[3*i+2] = gNormals[i].z;
    }

    for (int i = 0; i < gFaces.size(); ++i)
    {
        indexData[3*i] = gFaces[i].vIndex[0];
        indexData[3*i+1] = gFaces[i].vIndex[1];
        indexData[3*i+2] = gFaces[i].vIndex[2];
    }


    glBufferData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes + gNormalDataSizeInBytes, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gVertexDataSizeInBytes, vertexData);
    glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes, gNormalDataSizeInBytes, normalData);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indexData, GL_STATIC_DRAW);

    // done copying; can free now
    delete[] vertexData;
    delete[] normalData;
    delete[] indexData;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));

}

void initFonts(int windowWidth, int windowHeight)
{
    // Set OpenGL options
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(windowWidth), 0.0f, static_cast<GLfloat>(windowHeight));
    glUseProgram(textProgram);
    glUniformMatrix4fv(glGetUniformLocation(textProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, "/usr/share/fonts/truetype/liberation/LiberationSerif-Italic.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
                );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    //
    // Configure VBO for texture quads
    //
    glGenBuffers(1, &gTextVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gTextVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init() 
{
	//ParseObj("armadillo.obj");
	ParseObj(object);


    glEnable(GL_DEPTH_TEST);
    initShaders();
    
    initFonts(gWidth, gHeight);
    initVBO();
}

void drawModel()
{
	glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));

	glDrawElements(GL_TRIANGLES, gFaces.size() * 3, GL_UNSIGNED_INT, 0);
}

void renderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state	
    glUseProgram(textProgram);
    glUniform3f(glGetUniformLocation(textProgram, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, gTextVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)

        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}


void checkForMatches(){
    vector<vector<bool>> visitedVertical(row, vector<bool>(col, false));
    vector<vector<bool>> visitedHorizontal(row, vector<bool>(col, false));
    bool expl = false;
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            int matchColor = bunnyColors[i][j];
            int left = j - 1;
            int right = j + 1;
            bool isFinished = false;
            //  for horizontal match
            while(left >= 0 && right < col && !isFinished && !visitedHorizontal[i][j] ){
                isFinished = true;

                if(bunnyColors[i][left] == matchColor){
                    visitedHorizontal[i][left] = true;
                    left--;
                    isFinished = false;
                }
                if(bunnyColors[i][right] == matchColor){
                    visitedHorizontal[i][right] = true; 

                    right++;
                    isFinished = false;
                }
            }
            if(right - left - 1 >= 3){
                scores += (right - left - 1);
                for(int l = left + 1; l<right ; l++){
                    expl = true;
                    explodingObjects[i][l] = 1;
                }
            }
            
            isFinished = false;
            int top = i - 1, bottom = i + 1;
            // for vertical match
            while(top >= 0 && bottom < row && !isFinished && !visitedVertical[i][j]){
                isFinished = true;
                if(bunnyColors[top][j] == matchColor){
                    visitedVertical[top][j] = true;

                    isFinished = false;
                    top--;
                }
                if(bunnyColors[bottom][j] == matchColor){
                    visitedVertical[bottom][j] = true;

                    isFinished = false;
                    bottom++;
                }
            }
            if(bottom - top - 1 >= 3){
                scores += (bottom - top - 1);
                for(int v = top + 1; v<bottom; v++){
                    expl = true;
                    explodingObjects[v][j] = 1;
                }
            }
            

        }
        
    }
    if(expl){
        isCurrentlyExploding = true;
    }
}

void clearExplodingObjects(){
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            explodingObjects[i][j] = 0;
        }
    }
}

void print2D(vector<vector<int>> &v){
    for(auto r : v){
        for(auto c : r){
            cout << c <<" ";

        }
        cout << "\n";
    }
}

void setNewColors(){
    for(int j = 0; j< col ; j++){
        for(int i = row - 1; i>=0; i--){
            if(explodingObjects[i][j] == -1){
                int colorIndex = -1;
                int yTranslation = 1;
                for(int k = i - 1; k>=0; k--){
                    if(bunnyColors[k][j] != -1){
                        colorIndex = bunnyColors[k][j];
                        explodingObjects[k][j] = -1;
                        break;
                    }
                    yTranslation++;
                }
                if(colorIndex != -1){
                    bunnyColors[i][j] = colorIndex;
                }
                else{
                    bunnyColors[i][j] = rand() % 5;
                }
                yTranslationLengths[i][j] = yTranslation * incry;
            }
        }
    }
    clearExplodingObjects();
}



void setExplodedObjects(){
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            if(explodingObjects[i][j]){
                explodingObjects[i][j] = -1;
            }
        }
    }
}

void decreaseYTranslationLengths(){
    bool isStillMoving = false;
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            yTranslationLengths[i][j] = yTranslationLengths[i][j] - 0.05 * incry;
            if(yTranslationLengths[i][j] > 0){
                isStillMoving = true;
            }
            else{
                yTranslationLengths[i][j] = 0;
            }
        }
    }
    if(!isStillMoving){
        isCurrentlyMoving = false;
    }
}

void display()
{
    static float angle = 0;
    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    
    
    if(!isCurrentlyExploding){
        checkForMatches();
    }
    if(isCurrentlyMoving){
        decreaseYTranslationLengths();
    }

    if(!isCurrentlyMoving && isCurrentlyExploding){
        currentSize *= 1.01;
    }

    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            
            glUseProgram(bunnyProgram[i][j]);

            float scaleFactor = 1.5 / min(row, col);
            float yTranslationFactor = 0;
            
            if(isCurrentlyMoving){
                yTranslationFactor = yTranslationLengths[i][j];
            }
            if(explodingObjects[i][j] == 1){
                scaleFactor *= currentSize;
            }
            
            glm::mat4 T = glm::translate(glm::mat4(1.f), glm::vec3((j - offsety) * incry , (offsetx - i) * incrx + yTranslationFactor , -10.f));
            glm::mat4 R = glm::rotate(glm::mat4(1.f), glm::radians(angle), glm::vec3(0, 1, 0));
            glm::mat4 S = glm::scale(T * R ,glm::vec3(scaleFactor, scaleFactor, scaleFactor ));

            glm::mat4 modelMat = S;
            glm::mat4 modelMatInv = glm::transpose(glm::inverse(modelMat));
            glm::mat4 perspMat = glm::perspective(glm::radians(45.0f), 1.f, 1.0f, 100.0f);


            glm::vec3 color = randomColors[bunnyColors[i][j]];

            glUniform3f(glGetUniformLocation(bunnyProgram[i][j], "bunnyColor"), color.x / 256.0, color.y / 256.0, color.z / 256.0);

            glUniformMatrix4fv(glGetUniformLocation(bunnyProgram[i][j], "modelingMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
            glUniformMatrix4fv(glGetUniformLocation(bunnyProgram[i][j], "modelingMatInvTr"), 1, GL_FALSE, glm::value_ptr(modelMatInv));
            glUniformMatrix4fv(glGetUniformLocation(bunnyProgram[i][j], "perspectiveMat"), 1, GL_FALSE, glm::value_ptr(perspMat));
            drawModel();
        }
    }

    if(currentSize >= 1.5){
        // TODO: Explode the object
        currentSize = 1.0;
        isCurrentlyExploding = false;
        isCurrentlyMoving = true;
        setExplodedObjects();
        setNewColors();
    }
    assert(glGetError() == GL_NO_ERROR);
    string text = "Moves: " + to_string(moves) + " Scores: " + to_string(scores);

    renderText(text, 0, 0, 1, glm::vec3(1, 1, 1));

    assert(glGetError() == GL_NO_ERROR);

	angle += 0.5;
}

void reshape(GLFWwindow* window, int w, int h)
{
    w = w < 1 ? 1 : w;
    h = h < 1 ? 1 : h;

    gWidth = w;
    gHeight = h;

    glViewport(0, 0, w, h);
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    float scalex = 640 / col;
    float scaley = 600 / row;
    if(isCurrentlyExploding){
        return;
    }
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
    {
       double xpos, ypos;
       //getting cursor position
       glfwGetCursorPos(window, &xpos, &ypos);
       xpos /= scalex;
       ypos /= scaley;
       int i = int(ypos), j = int(xpos);
       moves++;
       isCurrentlyExploding = true;
       explodingObjects[i][j] = 1;

    }
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        for(int i = 0; i<row; i++){
            for(int j = 0; j<col; j++){
                int randomIndex = rand() % 5;
                bunnyColors[i][j] = randomIndex;
            }
        }
    }
    // else if (key == GLFW_KEY_V && action == GLFW_PRESS)
    // {
    //     cout << "V pressed" << endl;
    //     glUseProgram(gProgram[0]);
    // }
    // else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    // {
    //     cout << "D pressed" << endl;
    //     gIntensity /= 1.5;
    //     cout << "gIntensity = " << gIntensity << endl;
    //     glUseProgram(gProgram[0]);
    //     glUniform1f(gIntensityLoc, gIntensity);
    // }
    // else if (key == GLFW_KEY_B && action == GLFW_PRESS)
    // {
    //     cout << "B pressed" << endl;
    //     gIntensity *= 1.5;
    //     cout << "gIntensity = " << gIntensity << endl;
    //     glUseProgram(gProgram[0]);
    //     glUniform1f(gIntensityLoc, gIntensity);
    // }
}

void mainLoop(GLFWwindow* window)
{
    while (!glfwWindowShouldClose(window))
    {
        display();
        glfwSwapBuffers(window);
        if(!isCurrentlyExploding && !isCurrentlyMoving){
            glfwPollEvents();
        }
    }
}

int main(int argc, char** argv)   // Create Main Function For Bringing It All Together
{
    srand(time(NULL));
    GLFWwindow* window;
    if (!glfwInit())
    {
        exit(-1);
    }
    if(!argv[1] || !argv[2] || !argv[3]){
        cout << "You should run with the row column and object argument"<<endl;
        return 0;
    }
    //string row_col_str(argv[1]);
    //int delimeter_pos = row_col_str.find(" ");
    row = atoi(argv[1]); 
    //stoi(row_col_str.substr(0, delimeter_pos));
    col = atoi(argv[2]); 
    //stoi(row_col_str.substr(delimeter_pos + 1));
    
    string tmpobject(argv[3]);
    object = tmpobject.c_str();

    incrx = 8.0 / row;
    incry = 8.0 / col;

    offsetx = (row - 1) / 2.0;
    offsety = (col -1) / 2.0;

    vector<vector<GLuint>> tmpb(row, vector<GLuint>(col, 1));
    bunnyProgram = tmpb;

    vector<vector<int>> tmpc(row, vector<int>(col, -1));
    bunnyColors = tmpc;

    vector<vector<int>> tmpd(row, vector<int>(col, 0));
    explodingObjects = tmpd;
    vector<vector<float>> tmpe(row, vector<float>(col, 0));
    yTranslationLengths = tmpe;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(gWidth, gHeight, "Simple Example", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    char rendererInfo[512] = {0};
    strcpy(rendererInfo, (const char*) glGetString(GL_RENDERER));
    strcat(rendererInfo, " - ");
    strcat(rendererInfo, (const char*) glGetString(GL_VERSION));
    glfwSetWindowTitle(window, rendererInfo);

    init();

    glfwSetKeyCallback(window, keyboard);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetWindowSizeCallback(window, reshape);

    reshape(window, gWidth, gHeight); // need to call this once ourselves
    mainLoop(window); // this does not return unless the window is closed

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

