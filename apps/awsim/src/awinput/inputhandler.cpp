#include "inputhandler.h"

AWInput::InputHandler::InputHandler()
    : mPlayerNode(nullptr),
      mCameraNode(nullptr),
      mIsMouseButtonDown({false, false, false}),
      mLogger("awinput.log")
{

    for (int i = 0; i<ActionKey::KeyCount; i++)
    {
        mIsActionKeyDown[i] = false;
    }

    useDefaultKeyBindings();


}

AWInput::InputHandler::~InputHandler()
{
    mLogger << "InputHandler destroyed";
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
bool AWInput::InputHandler::attachToPlayerNode(Ogre::SceneNode *playerNode)
{
    if (!playerNode)
    {
        return false;
    }
    mPlayerNode = playerNode;

    mLogger << "PlayerNode added";

    return true;
}

bool AWInput::InputHandler::attachToCameraNode(Ogre::SceneNode *cameraNode)
{
    if (!cameraNode)
    {
        return false;
    }
    mCameraNode = cameraNode;

    mLogger.reportStringStream( std::stringstream() << "mCameraNode " << (mCameraNode) );

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

    if (mIsMouseButtonDown.rmb)
    {
        //mLogger << "rotating camera";

        // Rotate the camera
        mCameraNode->yaw(Ogre::Radian(MOUSE_CAMERA_SPEED * -arg.state.X.rel), Ogre::SceneNode::TransformSpace::TS_WORLD);
        mCameraNode->pitch(Ogre::Radian(MOUSE_CAMERA_SPEED * -arg.state.Y.rel));

    }

}
//-------------------------------------------------------------------------------------
void AWInput::InputHandler::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    switch (id)
    {
    case OIS::MB_Left:
        mIsMouseButtonDown.lmb = true;
        break;
    case OIS::MB_Right:
        mIsMouseButtonDown.rmb = true;
        mLogger << "mouse pressed rmb";
        break;
    case OIS::MB_Middle:
        mIsMouseButtonDown.mmb = true;
        break;
    }


}
//-------------------------------------------------------------------------------------
void AWInput::InputHandler::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    switch (id)
    {
    case OIS::MB_Left:
        mIsMouseButtonDown.lmb = false;
        break;
    case OIS::MB_Right:
        mIsMouseButtonDown.rmb = false;
        mLogger << "mouse released rmb";
        break;
    case OIS::MB_Middle:
        mIsMouseButtonDown.mmb = false;
        break;
    }


}

void AWInput::InputHandler::handleBufferedInput(const float &timestep)
{
    // handle buffered input on action basis
    if (mIsActionKeyDown[ActionKey::Forward])
    {
        mCameraNode->translate(0.0, 0.0, -timestep*speed, Ogre::SceneNode::TransformSpace::TS_LOCAL);
    }
    if (mIsActionKeyDown[ActionKey::Backward])
    {
        mCameraNode->translate(0.0, 0.0, timestep*speed, Ogre::SceneNode::TransformSpace::TS_LOCAL);
    }
    if (mIsActionKeyDown[ActionKey::Right])
    {
        mCameraNode->translate(timestep*speed, 0.0, 0.0, Ogre::SceneNode::TransformSpace::TS_LOCAL);
    }
    if (mIsActionKeyDown[ActionKey::Left])
    {
        mCameraNode->translate(-timestep*speed, 0.0, 0.0, Ogre::SceneNode::TransformSpace::TS_LOCAL);
    }

    // handle buffered input on mouse/button basis
    //if

}
