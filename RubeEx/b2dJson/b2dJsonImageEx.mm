
#include <cstring>
#include "b2dJsonImageEx.h"
#include "bitmap.h"
#include "global.hpp"

using namespace std;

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

b2dJsonImageEx::b2dJsonImageEx(Shader &shader,const b2dJsonImage *other) : b2dJsonImage(other)
{
    this->shader = shader;
    texture.ID = 0;
    loadImage();
    onAttach();
}

bool checkFileExtension(string s, string ext) 
{
	long extStartPos = s.length() - ext.length();
	if ( extStartPos <= 0 )
		return false;
	return s.substr( extStartPos ) == ext;	
}

bool b2dJsonImageEx::loadImage()
{
    //clear any existing texture
    if ( texture.ID ) {
        glDeleteTextures(1, &texture.ID);
        texture.ID = 0;
    }

    if ( checkFileExtension(file, ".png") || checkFileExtension(file, ".PNG") )
    {
        char des[1024]={0};
        vector<string> res = split(file, "/");
        string imgName = res[res.size()-1];
        Global::ResFullPath(des, imgName.c_str());
        texture = loadTextureFromFile(des, true);
        return true;
    }

    return false;
}

void b2dJsonImageEx::onAttach(){
    
    for (int i=0; i<numPoints; i++) {
        Vertex vert;
        vert.position= {points[2*i],points[2*i+1]};
        vert.texCoords = {uvCoords[2*i],uvCoords[2*i+1]};
        vertexArr.push_back(vert);
    }
    for (int i=0; i<numIndices; i++) {
        m_indices[i] = indices[i];
    }
    
    glGenVertexArrays(1,&m_QuadVA);
    glBindVertexArray(m_QuadVA);
    
    glGenBuffers(1, &m_QuadVB);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferData(GL_ARRAY_BUFFER, numPoints * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,texCoords));
    glEnableVertexAttribArray(1);
    
    glGenBuffers(1, &m_QuadIB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), m_indices, GL_STATIC_DRAW);
}

void b2dJsonImageEx::render(glm::mat4 viewMatrix)
{
    if ( body ) {
        b2Vec2 bodyPos = body->GetPosition();
        
        // Prepare transformations
        glm::mat4 model = glm::mat4(1.0f);
        float px = bodyPos.x;
        float py = bodyPos.y;
        model = glm::translate(model, glm::vec3(px, py, 0.0f));
        
        glm::vec2 size ={1.0f,1.0f};// {texture.Width,texture.Height};
        model = glm::rotate(model, body->GetAngle(), {0,0,1}); // Then rotate
        model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale
        
        this->shader.use();
        this->shader.setMat4("model", model);
        this->shader.setMat4("view", viewMatrix);
        // Render textured quad
        this->shader.setVec3("spriteColor", {1.0f,1.0f,1.0f});

        glActiveTexture(GL_TEXTURE0);
        texture.Bind();
        
        glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexArr.size() * sizeof(Vertex), vertexArr.data());
        glBindVertexArray(m_QuadVA);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
        glDisable(GL_BLEND);
        glBindVertexArray(0);
    }
}
