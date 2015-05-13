#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <map>

#include <OgreSceneNode.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "../awmechanics/action.h"

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

    //static Actions KeyBindings;

    //static ActionStruct KeyIsDown;

    typedef int ActionKeybindMap[AWMech::Action::ActionsCount];
    //using InputAction = AWMech::Action::ActionType;

    class InputHandler
    {
    public:
        InputHandler();

        bool attachToNode(Ogre::SceneNode *playerNode);

        // Handle key input
        void keyPressed( const OIS::KeyEvent &arg );
        void keyReleased( const OIS::KeyEvent &arg );

        // Handle mouse input
        void mouseMoved( const OIS::MouseEvent &arg );
        void mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
        void mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    private:
        Ogre::SceneNode *mPlayerNode;

        ActionKeybindMap mKeyBindings;


        struct KeyIsDown
        {
            bool KeyForward;
            bool KeyBackward;
            bool KeyRight;
            bool KeyLeft;
        } mKeyIsDown;
    };
}

#endif // INPUTHANDLER_H
