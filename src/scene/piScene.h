#include <algorithm>
#include <iostream>
#include <vector>

#include <piObject.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//======================================================================================================

class piSpriteBatch
{

    void unpack(const std::vector<float>& flatData);
    void pack();

private:
    struct VertexData {
        glm::vec3 vertex;
        glm::vec4 color;
        glm::vec2 textureCoord;
    };

    std::vector<VertexData> vertices;
    std::vector<float> flatData;

}; // class


//======================================================================================================

class piScene
{
private:
    std::vector<piObjectPtr_t> objects;

public:
    piScene();
    ~piScene();

    void addObject(    piObject &obj);
    void addObject(piObjectPtr_t obj);

    void removeObject( piObject &obj);
    void removeObject(piObjectPtr_t obj);


    int getSize() { return objects.size() ; };

    // void unpack(const std::vector<float>& flatData);
    // void pack();

    void update(glm::mat4 &projection, float time_secs = 0.0f);
    void draw();

}; // class

//======================================================================================================
//======================================================================================================
