#include "LevelGen.hpp"

extern Conductor conductor;

void LevelGen::Init(int rooms, int grid)
{
    noRooms = rooms;
    gridSize = grid;
}

void LevelGen::erate(std::string filename)
{
    std::ifstream myfile(filename);
    std::string line;
    int posX = 0;
    int posY = 0;

    glm::vec3 A;
    glm::vec3 B;
    glm::vec3 C;
    glm::vec3 D;

    // Go through the CSV line by line
    while (std::getline(myfile, line)) {

        // Create a stringstream of the current line
        std::stringstream ss(line);
        // Tokenise the line.
        std::string val;

        // Go through the tokens one by one
        while (std::getline(ss, val, ',')) {
            if (val == " " || val == "  ") {
                // A lot of these statments print a space, so that the map looks
                // cool in the console.
                continue;
            }
            // If it's none of the above, it must be a wall or floor.
            else {
                // If *, place a wall.
                Entity entity = conductor.CreateEntity();
                levelEntities.push_back(entity);

                Renderable render;
                // Colours!
                if (val.find("W") != std::string::npos) {
                    render.color = { 72, 72, 72 };
                }
                else if (val.find("G") != std::string::npos) {
                    render.color = { 179, 192, 164 };
                }
                else {
                    render.color = { 255, 0, 255 };
                }
                conductor.AddComponent(entity, Renderable{ render });

                bool collidable;
                val.find("*") ? collidable = false : collidable = true;

                Transform transform;
                transform.position = { posX, posY, 1 };
                transform.rotation = { 1, 1, 1 };
                transform.scale = { gridSize, gridSize, gridSize };
                conductor.AddComponent(entity, Transform{ transform });

                Gravity gravity;
                gravity.force = { 0.0f, 0.0f, 0.0f };
                conductor.AddComponent(entity, Gravity{ gravity });

                if (collidable) {
                    RigidBody rigidbody;
                    rigidbody.acceleration = { 0.0f, 0.0f, 0.0f };
                    rigidbody.velocity = { 0.0f, 0.0f, 0.0f };
                    conductor.AddComponent(entity, RigidBody{ rigidbody });
                }
                

                //std::cout << conductor.GetEntitySignature(entity) << std::endl;

                
            }
            posX += gridSize;
        }

        posX = 0;
        posY += gridSize;
    }

    myfile.close();
}

std::vector<Entity> LevelGen::GetLevelEntities()
{
    return levelEntities;
}
