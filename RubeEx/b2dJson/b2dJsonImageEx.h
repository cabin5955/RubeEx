
#ifndef B2DJSONIMAGE_EX_H
#define B2DJSONIMAGE_EX_H

#import <OpenGLES/ES3/gl.h>
#include "b2dJsonImage.h"
#include "shader.h"
#include "utils.h"

class b2dJsonImageEx : public b2dJsonImage {

private:
    typedef struct
    {
        glm::vec2   position;
        glm::vec2   texCoords;
    } Vertex;
    
    Shader shader;
    
    GLuint m_QuadVA;
    GLuint m_QuadVB;
    GLuint m_QuadIB;
    
    std::vector<Vertex> vertexArr;
    int m_indices[128];
    
protected:
    
    Texture2D texture;

public:
    b2dJsonImageEx(Shader &shader, const b2dJsonImage* other );
    virtual bool loadImage();
    virtual void render(glm::mat4 viewMatrix);
    void onAttach();
};

#endif // B2DJSONIMAGE_EX_H
