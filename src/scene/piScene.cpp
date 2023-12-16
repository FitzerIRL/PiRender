#include <piScene.h>

#include <memory> 

piScene::piScene() {}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

piScene::~piScene()
{
    // Clean up objects in the destructor
    for (piObjectPtr_t obj : objects)
    {
        // delete obj;
    }

    objects.clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void piScene::addObject(piObject &obj)
{
    objects.push_back( std::make_shared<piObject>(obj)) ;
}

void piScene::addObject(piObjectPtr_t obj)
{
    objects.push_back( obj ) ;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// void piScene::removeObject(piObject &obj)
// {
//     // Remove the object from the vector
//     auto it = std::find(objects.begin(), objects.end(), obj);

//     if (it != objects.end())
//     {
//         objects.erase(it);
//         //delete obj; // Don't forget to delete the removed object
//     }
// }


void piScene::removeObject(piObject& obj)
{
    // auto it = std::find(objects.begin(), objects.end(),
    //                     [&obj](const std::shared_ptr<piObject>& ptr)
    //                     {
    //                         return *ptr == obj; // Assuming piObject has an equality operator defined
    //                     });

    // if (it != objects.end())
    // {
    //     objects.erase(it);
    // }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void piScene::update(glm::mat4 &projection, float time_secs /*= 0.0f*/)
{
    for (piObjectPtr_t obj : objects)
    {
        obj->update(projection, time_secs);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void piScene::draw()
{
    for (piObjectPtr_t obj : objects)
    {
        obj->draw();
    }
}


//======================================================================================================
//======================================================================================================

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Unpack the flat vector into structured vertex and texture coordinate data
void piSpriteBatch::unpack(const std::vector<float>& flatData)
{
    vertices.clear();

    // Iterate through the flat data and create vertices
    for (size_t i = 0; i < flatData.size(); i += 5) {
        // Extract vertex data
        glm::vec3 vertex(flatData[i], flatData[i + 1], flatData[i + 2]);

        // Extract color data
        glm::vec4 color(flatData[i], flatData[i + 1], flatData[i + 2], flatData[i + 3]);

        // Extract texture coordinate data
        glm::vec2 textureCoord(flatData[i + 4], flatData[i + 5]);

        // Add the vertex and texture coordinate to the structured data
        vertices.push_back({ vertex, color, textureCoord });
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Pack the structured data into a flat vector
void piSpriteBatch::pack()
{
    flatData.clear();

    // Iterate through the structured data and flatten it
    for (const auto& vertexData : vertices) {
        // Add vertex data
        flatData.push_back(vertexData.vertex.x);
        flatData.push_back(vertexData.vertex.y);
        flatData.push_back(vertexData.vertex.z);

        // Add texture coordinate data
        flatData.push_back(vertexData.textureCoord.x);
        flatData.push_back(vertexData.textureCoord.y);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

