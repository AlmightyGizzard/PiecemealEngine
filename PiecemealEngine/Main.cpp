#pragma once


// PiecemealEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SDL.h>
#include <iostream>
#include <chrono>
#include <random>

#include "Core/Conductor.hpp"
#include "Components/LevelGen.hpp"
#include "Components/ComponentTest.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/RenderSystem.hpp"



const int DELTA_TIME = 60;
int windowSizeX = 1200;
int windowSizeY = 800;
int centreX = windowSizeX / 2;
int centreY = windowSizeY / 2;

SDL_Window* window;
SDL_Renderer* renderer;

Conductor conductor;
LevelGen gen;


glm::vec3 createNewVec3(float x, float y, float z) {
    glm::vec3 result(x, y, z);
    return result;
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return 1;
    }

    // Create a window
    window = SDL_CreateWindow("Piecemeal Engine", 300, 300, windowSizeX, windowSizeY, SDL_WINDOW_RESIZABLE);

    //Create a Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_LogError(0, "Main renderer is null, quitting...");
    }
    SDL_RenderSetLogicalSize(renderer, windowSizeX, windowSizeY);

    conductor.Init();

    // Register all the components to be used:
    conductor.RegisterComponent<Transform>();
    conductor.RegisterComponent<Renderable>();
    conductor.RegisterComponent<Gravity>();
    conductor.RegisterComponent<RigidBody>();
    

    // And the systems:
    auto physicsSystem = conductor.RegisterSystem<PhysicsSystem>();
    auto renderSystem = conductor.RegisterSystem<RenderSystem>();
    
    // Fixed a major issue - in this section, you have to define a signature and what components are NEEDED, not
    // just all components - before the block of signature.set(s) meant that every entity had to have ALL components
    // assigned in order for it to qualify for rendering.
    Signature signature;
    signature.set(conductor.GetComponentType<Transform>());
    signature.set(conductor.GetComponentType<Renderable>());
    conductor.SetSystemSignature<RenderSystem>(signature);


    signature.set(conductor.GetComponentType<Gravity>());
    signature.set(conductor.GetComponentType<RigidBody>());
    conductor.SetSystemSignature<PhysicsSystem>(signature);
    
    
    physicsSystem->Init();
    renderSystem->Init(renderer, window);

    std::vector<Entity> entities(0);

    // Random code for the cubes:
    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
    std::uniform_real_distribution<float> randX(0.0f, windowSizeX);
    std::uniform_real_distribution<float> randY(0.0f, windowSizeY);

    std::uniform_real_distribution<float> randColour(0.0f, 255.0f);
    std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
    std::uniform_real_distribution<float> randScale(13.0f, 15.0f);
    std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);
  
    gen.Init(5);
    gen.erate("Content/TestLevel.csv");
    std::vector<Entity> le = gen.GetLevelEntities();
    entities.insert(entities.end(), le.begin(), le.end());


    //// lots of cubes
    //std::vector<Entity> entities(50);
    //std::vector<Entity> wallBlocks(25);

    //
    //for (auto& entity : entities) {
    //    entity = conductor.CreateEntity();

    //    Gravity g;
    //    g.force = { 0.0f, randGravity(generator), 0.0f };
    //    conductor.AddComponent(entity, Gravity{g});

        /*RigidBody r;
        r.acceleration = { 0.0f, 0.0f, 0.0f };
        r.velocity = { 0.0f, 0.0f, 0.0f };
        conductor.AddComponent(entity, RigidBody{ r });*/

    //    Transform t;
    //    t.position = { randX(generator), randY(generator) , 1 };
    //    t.rotation = { randRotation(generator) , randRotation(generator) , randRotation(generator) };
    //    t.scale = { scale*2, scale, scale };
    //    conductor.AddComponent(entity, Transform{ t });

    //    Renderable c;
    //    c.color = { randColour(generator), randColour(generator), randColour(generator) };
    //    conductor.AddComponent(entity, Renderable{ c });

    //}

    //for (auto& entity : wallBlocks) {
    //    entity = conductor.CreateEntity();

    //    Gravity g;
    //    g.force = { 0.0f, 0.0f, 0.0f };
    //    conductor.AddComponent(entity, Gravity{ g });

    //    RigidBody r;
    //    r.acceleration = { 0.0f, 0.0f, 0.0f };
    //    r.velocity = { 0.0f, 0.0f, 0.0f };
    //    conductor.AddComponent(entity, RigidBody{ r });

    //    Transform t;
    //    t.position = { randX(generator), randY(generator) , 1 };
    //    t.rotation = { randRotation(generator) , randRotation(generator) , randRotation(generator) };
    //    t.scale = { scale, scale*1.5f, scale };
    //    conductor.AddComponent(entity, Transform{ t });

    //    Renderable c;
    //    c.color = { 255, 255, 255 };
    //    conductor.AddComponent(entity, Renderable{ c });
    //}

    // Try and create an entitiy of our own, to be replaced with a way to read it in via text
    /*Entity newTest = conductor.CreateEntity();
    Transform transform; Renderable render;
    transform.position = { centreX, centreY, 1 };
    transform.rotation = { 1, 1, 1 };
    transform.scale = { scale * 5, scale * 5, scale * 5 };
    render.color = { 255, 255, 255 };
    conductor.AddComponent(newTest, Transform{ transform });
    conductor.AddComponent(newTest, Renderable{ render });*/

    
    // TODO : figure out a way to get the above entity to render : for some reason entities only
    //        render when they have a gravity+rigidbody, which they don't need.



    std::cout << "Created No of Entities: " << le.size() << std::endl;
    

    float dt = 0.0f;

    bool running = true;

    // Until the end screen exits out properly:
    while (running) {
        SDL_GetWindowSize(window, &windowSizeX, &windowSizeY);
        SDL_SetWindowSize(window, windowSizeX, windowSizeY);

        centreX = windowSizeX / 2, centreY = windowSizeY / 2;

        SDL_Event _event;
        while (SDL_PollEvent(&_event)) {
            if (_event.type == SDL_QUIT) {
                SDL_Quit();
                exit(0);
            }
        }
        


        auto startTime = std::chrono::high_resolution_clock::now();

        //std::cout << dt << std::endl;
        physicsSystem->Update(dt);
        renderSystem->Update();

        auto stopTime = std::chrono::high_resolution_clock::now();

        dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        SDL_RenderPresent(renderer);
    }

    printf("Goodbye Cruel World!\n");

    SDL_Quit();
    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
