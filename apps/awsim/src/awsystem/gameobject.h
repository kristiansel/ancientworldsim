#ifndef AWGAMEOBJECT_H
#define AWGAMEOBJECT_H

namespace AWSystem
{
    class GameObject
    {
    public:
        typedef unsigned int IDtype;

        GameObject() {mUID = getNewUID();}

        IDtype getUID() const {return mUID;}

        GameObject ( const GameObject &&in ) { mUID = in.mUID; } // move constructor
        GameObject ( const GameObject &in ) { mUID = getNewUID(); } // copy constructor
    private:


        IDtype mUID;

        static IDtype sUID;
        IDtype getNewUID() {return ++sUID;}

    };

}

#endif // AWGAMEOBJECT_H
