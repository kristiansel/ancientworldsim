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
    : mPlayerNode(nullptr),
      mKeyIsDown({
                    false,  // KeyForward
                    false,  // KeyBackward
                    false,  // KeyRight
                    false   // KeyLeft
                 })
{
    mKeyBindings[AWMech::Action::Forward]         = OIS::KeyCode::KC_W;
    mKeyBindings[AWMech::Action::Backward]        = OIS::KeyCode::KC_S;
    mKeyBindings[AWMech::Action::Right]           = OIS::KeyCode::KC_D;
    mKeyBindings[AWMech::Action::Left]            = OIS::KeyCode::KC_A;

    mKeyBindings[AWMech::Action::Run]             = OIS::KeyCode::KC_LSHIFT;
    mKeyBindings[AWMech::Action::ToggleRun]       = OIS::KeyCode::KC_CAPITAL;
    mKeyBindings[AWMech::Action::Crouch]          = OIS::KeyCode::KC_LCONTROL;
    mKeyBindings[AWMech::Action::ToggleCrouch]    = OIS::KeyCode::KC_C;
    mKeyBindings[AWMech::Action::Jump]            = OIS::KeyCode::KC_SPACE;

    mKeyBindings[AWMech::Action::Attack]          = AWInput::LMB_CODE;
    mKeyBindings[AWMech::Action::Block]           = AWInput::RMB_CODE;
    mKeyBindings[AWMech::Action::Dodge]           = OIS::KeyCode::KC_LMENU ; // left alt
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

    switch (keycode)
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
    }
}
//-------------------------------------------------------------------------------------
void AWInput::InputHandler::keyReleased( const OIS::KeyEvent &arg )
{
    OIS::KeyCode keycode = arg.key;

    switch (keycode)
    {
    case OIS::KeyCode::KC_W:
        mKeyIsDown.KeyForward = false;
        break;
    case OIS::KeyCode::KC_S:
        mKeyIsDown.KeyBackward = false;
        break;
    case OIS::KeyCode::KC_D:
        mKeyIsDown.KeyRight = false;
        break;
    case OIS::KeyCode::KC_A:
        mKeyIsDown.KeyLeft= false;
        break;
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
