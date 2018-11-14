#include "defaultimpl/components/PickArrowMovement.h"

#include "EngineInstance.h"

#include <iostream>

namespace RenderLib
{
  namespace DefaultImpl
  {
    void
    PickArrowMovement::start()
    {
      lastMouseX = lastMouseY = -1;
    }

    void
    PickArrowMovement::update()
    {
      // Adjust arrow size based on distance to camera
      // to keep it at constant size independent of the point of view
      Scene * scene = engineInstance->getSceneManager().getActiveScene();
      Camera * cam  = scene->getActiveCamera();

      VECTOR3 camPos = -(cam->transform.translationV);
      FLOAT scale
          = (camPos - object->transform.translationV).norm() * (FLOAT)0.15;

      object->transform.setScale(VECTOR3(scale, scale, scale));

      SceneObject * obj = engineInstance->getPickManager().getLastPick();

      if (obj == nullptr)
      {
        return;
      }

      SceneObject * lp = engineInstance->getPickManager().getLastPickArrow();

      // We have picked the arrow to which this component is attached
      if (lp == object)
      {
        InputHandlers::InputManager & im = engineInstance->getSceneManager()
                                               .getActiveScene()
                                               ->getInputManager();

        if (im.forceMouseButtonDown(0))
        {
          if (lastMouseX == -1 || lastMouseY == -1)
          {
            lastMouseX = im.getLastMouseX();
            lastMouseY = im.getLastMouseY();
          }
          else
          {
            int deltaX = im.getLastMouseX() - lastMouseX;
            int deltaY = im.getLastMouseY() - lastMouseY;

            lastMouseX = im.getLastMouseX();
            lastMouseY = im.getLastMouseY();

            float movement = 0.0f;

            int absX = std::abs(deltaX);
            int absY = std::abs(deltaY);

            if (absX > absY)
            {
              float factor = static_cast<float>(absX) * 0.25f;
              movement     = deltaX < 0.0f ? -0.1f : 0.1f;
              movement *= factor;
            }
            else
            {
              float factor = static_cast<float>(absY) * 0.25f;
              movement     = deltaY < 0.0f ? 0.1f : -0.1f;
              movement *= factor;
            }

            VECTOR3 desplz = VECTOR3::Zero();
            switch (arrowType)
            {
              case ArrowAxis::AXIS_FORWARD:
                desplz = obj->transform.forwardVector * movement;
                break;
              case ArrowAxis::AXIS_RIGHT:
                desplz = obj->transform.rightVector * movement;
                break;
              case ArrowAxis::AXIS_UP:
                desplz = obj->transform.upVector * movement;
                break;
            }

            // Adjust movement depending on point of view
            float signFactor
                = cam->transform.forwardVector.dot(obj->transform.forwardVector)
                    < 0.0
                ? -1.0f
                : 1.0f;

            obj->transform.translate(desplz * signFactor);

            im.consumeMouseInput();
          }
        }
        else
        {
          engineInstance->getPickManager().setLastPick(nullptr);
          lastMouseX = lastMouseY = -1;
        }
      }
    }
  } // namespace DefaultImpl
} // namespace RenderLib