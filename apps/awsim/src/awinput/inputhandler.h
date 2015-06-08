#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <map>

#include <OgreSceneNode.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "../awmechanics/action.h"

#include "../awutility/logger.h"


namespace AWInput
{
    enum MouseCodes : int {
        LMB_CODE = 777775, // random numbers chosen because OIS does not unify inputs under a single system
        RMB_CODE = 777776, // Hope that there is no collision with the keyboard keys
        MMB_CODE = 777777
    };

    /*Inventory       = ,
    MainMenu        = ,
    Character       = ,
    Map             = ,

    QuickAction1    = ,
    QuickAction2    = ,
    QuickAction3    = ,
    QuickAction4    = ,
    QuickAction5    = ,
    QuickAction6    = ,
    QuickAction7    = ,
    QuickAction8    = ,
    QuickAction9    = ,*/

    enum ActionKey : int { // assume these go from 0 to ActionsCount-1
        Forward,
        Backward,
        Right,
        Left,

        Run,
        ToggleRun,
        Crouch,
        ToggleCrouch,
        Jump,

        Attack,
        Block,
        Dodge,

        Inventory,
        MainMenu,
        Character,
        Map,

        QuickAction1,
        QuickAction2,
        QuickAction3,
        QuickAction4,
        QuickAction5,
        QuickAction6,
        QuickAction7,
        QuickAction8,
        QuickAction9,

        KeyCount // Number of available actions keys
    };


    class InputHandler
    {
    public:
        InputHandler();
        virtual ~InputHandler();

        bool attachToPlayerNode(Ogre::SceneNode *playerNode);
        bool attachToCameraNode(Ogre::SceneNode *camerNode);

        void handleBufferedInput(const float &timestep);

        // Handle key input
        void keyPressed( const OIS::KeyEvent &arg );
        void keyReleased( const OIS::KeyEvent &arg );

        // Handle mouse input
        void mouseMoved( const OIS::MouseEvent &arg );
        void mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
        void mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    protected:
        void useDefaultKeyBindings();

    private:
        Ogre::SceneNode *mPlayerNode;
        Ogre::SceneNode *mCameraNode;

        std::map<int, int> mKeyBindings; // Key: Key Code, Value: Action
        bool mIsActionKeyDown[ActionKey::KeyCount];

        // move this to creature or something:
        static constexpr float speed = 1000.f; // 100 something per second

        struct IsMouseButtonDown
        {
            bool lmb;
            bool rmb;
            bool mmb;
        } mIsMouseButtonDown;

        static constexpr float MOUSE_CAMERA_SPEED = 0.0062831853; // In radians per pixel. The number corresponds to 2pi rad / 1000 px

        AWUtil::Logger mLogger;
    };
}

#endif // INPUTHANDLER_H
