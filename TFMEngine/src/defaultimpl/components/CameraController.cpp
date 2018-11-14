#include "defaultimpl/components/CameraController.h"

#include "EngineInstance.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    CameraController::CameraController()
      : lastMouseX(-1), lastMouseY(-1), moveSpeed(10.0f)
    {
    }

    void
    CameraController::update()
    {
      Scene * scene = engineInstance->getSceneManager().getActiveScene();
      if (scene)
      {
        InputHandlers::InputManager & input = scene->getInputManager();
        Time & time                         = engineInstance->getTime();
        Camera * cam                        = static_cast<Camera *>(object);

        VECTOR3 movement(0.0, 0.0, 0.0);

        // Forward
        if (input.keyPressed('w') || input.keyPressed('W'))
        {
          movement
              += object->transform.forwardVector * moveSpeed * time.deltaTime();
        }
        // Backward
        else if (input.keyPressed('s') || input.keyPressed('S'))
        {
          movement
              -= object->transform.forwardVector * moveSpeed * time.deltaTime();
        }

        // Left
        if (input.keyPressed('a') || input.keyPressed('A'))
        {
          movement
              += object->transform.rightVector * moveSpeed * time.deltaTime();
        }
        // Right
        else if (input.keyPressed('d') || input.keyPressed('D'))
        {
          movement
              -= object->transform.rightVector * moveSpeed * time.deltaTime();
        }

        cam->translateView(movement);

        // Rotation
        VECTOR3 rot(0.0, 0.0, 0.0);
        if (input.mouseButtonDown(2))
        {
          if (lastMouseX == -1 || lastMouseY == -1)
          {
            lastMouseX = input.getLastMouseX();
            lastMouseY = input.getLastMouseY();
          }
          else
          {
            int deltaX = input.getLastMouseX() - lastMouseX;
            int deltaY = input.getLastMouseY() - lastMouseY;

            lastMouseX = input.getLastMouseX();
            lastMouseY = input.getLastMouseY();

            rot = VECTOR3(static_cast<FLOAT>(deltaY),
                          static_cast<FLOAT>(deltaX), 0.0);
            cam->rotateView(rot * time.deltaTime() * 0.1f);
          }
        }
        else
        {
          lastMouseX = lastMouseY = -1;
        }
      }
    }
  } // namespace DefaultImpl
} // namespace RenderLib