# GameEngine
 
I created a small 2D game engine using C++ to learn more about how games work under the hood. This project was developed as part of a online [course](https://pikuma.com/courses/cpp-2d-game-engine-development).

Here is an image of what the final product looked like:
![image (1)](https://github.com/Marck-V/GameEngine/assets/42965997/a7338032-9f58-4c4e-89d6-e48f01460b7a)

- This engine uses SDL2, ImGui, Sol, Glm, and spdlog as part of its foundation.
- I added spdlog to the project on my own instead of using my own solution for a logger since I wanted to use something that focused on the best performance possible.
- I used the sol library to integrate lua as the scripting language.
- The game engine follows the [ECS](https://medium.com/source-true/why-is-entity-component-system-ecs-so-awesome-for-game-development-f554e1367c17) software pattern.
- For me, this was a nice oppurtunity to use modern C++ and become more familiar with the standard library.
