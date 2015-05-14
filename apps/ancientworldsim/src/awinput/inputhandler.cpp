#include "inputhandler.h"

/*AWInput::ActionStruct::ActionStruct()
{
    // should preferably read this from a file
    // implicit conversion here from OIS::KeyCode to int
    Forward         = OIS::KeyCode::KC_W;
    Backward        = OIS::KeyCode::KC_S;
    Right           = OIS::KeyCode::KC_D;
    Left            = OIS::KeyCode::KC_A;

    Run             = OIS::KeyCode::KC_LSHIFT;
    ToggleRun       = OIS::KeyCode::KC_CAPITAL;
    crouch          = OIS::KeyCode::KC_LCONTROL;
    ToggleCrouch    = OIS::KeyCode::KC_C;
    Jump            = OIS::KeyCode::KC_SPACE;

    Attack          = AWInput::LMB_CODE;
    Block           = AWInput::RMB_CODE;
    Dodge           = OIS::KeyCode::KC_LMENU ; // left alt

    Inventory       = OIS::KeyCode::KC_I;
    MainMenu        = OIS::KeyCode::KC_ESCAPE;
    Character       = OIS::KeyCode::KC_P;
    Map             = OIS::KeyCode::KC_M;

    QuickAction1    = OIS::KeyCode::KC_1;
    QuickAction2    = OIS::KeyCode::KC_2;
    QuickAction3    = OIS::KeyCode::KC_3;
    QuickAction4    = OIS::KeyCode::KC_4;
    QuickAction5    = OIS::KeyCode::KC_5;
    QuickAction6    = OIS::KeyCode::KC_6;
    QuickAction7    = OIS::KeyCode::KC_7;
    QuickAction8    = OIS::KeyCode::KC_8;
    QuickAction9    = OIS::KeyCode::KC_9;
}*/

AWInput::InputHandler::InputHandler()
    : mPlayerNode(nullptr)
{

    for (int i = 0; i<ActionKey::KeyCount; i++)
    {
        mIsActionKeyDown[i] = false;
    }

    useDefaultKeyBindings();
}

void AWInput::InputHandler::useDefaultKeyBindings()
{
    mKeyBindings[OIS::KeyCode::KC_W]            = ActionKey::Forward;
    mKeyBindings[OIS::KeyCode::KC_S]            = ActionKey::Backward;
    mKeyBindings[OIS::KeyCode::KC_D]            = ActionKey::Right;
    mKeyBindings[OIS::KeyCode::KC_A]            = ActionKey::Left;

    mKeyBindings[OIS::KeyCode::KC_LSHIFT]       = ActionKey::Run;
    mKeyBindings[OIS::KeyCode::KC_CAPITAL]      = ActionKey::ToggleRun;
    mKeyBindings[OIS::KeyCode::KC_LCONTROL]     = ActionKey::Crouch;
    mKeyBindings[OIS::KeyCode::KC_C]            = ActionKey::ToggleCrouch;
    mKeyBindings[OIS::KeyCode::KC_SPACE]        = ActionKey::Jump;

    mKeyBindings[AWInput::LMB_CODE]             = ActionKey::Attack;
    mKeyBindings[AWInput::RMB_CODE]             = ActionKey::Block;
    mKeyBindings[OIS::KeyCode::KC_LMENU]        = ActionKey::Dodge;

    // add additional actions here
}

//-------------------------------------------------------------------------------------
bool AWInput::InputHandler::attachToNode(Ogre::SceneNode *playerNode)
{
    if (!playerNode)
    {
        return false;
    }
    mPlayerNode = playerNode;
    return true;
}

//-------------------------------------------------------------------------------------
void AWInput::InputHandler::keyPressed( const OIS::KeyEvent &arg )
{
    OIS::KeyCode keycode = arg.key;

    auto findkey_it = mKeyBindings.find(keycode);
    if(findkey_it != mKeyBindings.end())
    {
            int action_key = findkey_it->second;
            mIsActionKeyDown[action_key] = true;
            // handle specific action, switch |here| or in a new function?
            /*switch (action_key)
            {
            }*/

    }

    /*switch (keycode)
    {
    //case mKeyBindings[AWMech::Action::Forward]:
    case OIS::KeyCode::KC_W:
        mKeyIsDown.KeyForward = true;
        break;
    case OIS::KeyCode::KC_S:
        mKeyIsDown.KeyBackward = true;
        break;
    case OIS::KeyCode::KC_D:
        mKeyIsDown.KeyRight = true;
        break;
    case OIS::KeyCode::KC_A:
        mKeyIsDown.KeyLeft= true;
        break;
    }*/
}
//-------------------------------------------------------------------------------------
void AWInput::InputHandler::keyReleased( const OIS::KeyEvent &arg )
{
    OIS::KeyCode keycode = arg.key;

    auto findkey_it = mKeyBindings.find(keycode);
    if(findkey_it != mKeyBindings.end())
    {
            int action_key = findkey_it->second;
            mIsActionKeyDown[action_key] = false;
            // handle specific action, switch |here| or in a new function?

    }
}
//-------------------------------------------------------------------------------------
void AWInput::InputHandler::mouseMoved( const OIS::MouseEvent &arg )
{

}
//-------------------------------------------------------------------------------------
void AWInput::InputHandler::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{

}
//-------------------------------------------------------------------------------------
void AWInput::InputHandler::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{

}

void AWInput::InputHandler::handleBufferedInput(const float &timestep)
{
    // handle buffered input
    if (mIsActionKeyDown[ActionKey::Forward])
    {
        mPlayerNode->translate(0.0, 0.0, -timestep*speed, Ogre::SceneNode::TransformSpace::TS_LOCAL);
    }
    if (mIsActionKeyDown[ActionKey::Backward])
    {
        mPlayerNode->translate(0.0, 0.0, timestep*speed, Ogre::SceneNode::TransformSpace::TS_LOCAL);
    }
    if (mIsActionKeyDown[ActionKey::Right])
    {
        mPlayerNode->translate(timestep*speed, 0.0, 0.0, Ogre::SceneNode::TransformSpace::TS_LOCAL);
    }
    if (mIsActionKeyDown[ActionKey::Left])
    {
        mPlayerNode->translate(-timestep*speed, 0.0, 0.0, Ogre::SceneNode::TransformSpace::TS_LOCAL);
    }
}
