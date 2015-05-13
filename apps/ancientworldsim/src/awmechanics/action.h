#ifndef ACTION_H
#define ACTION_H


namespace AWMech
{
    namespace Action
    {
        enum ActionType : int { // assume these go from 0 to ActionsCount-1
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

            ActionsCount // Number of available actions
        };
    }
}
#endif // ACTION_H
