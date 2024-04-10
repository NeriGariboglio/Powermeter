#ifndef firebaseP_h
#define firebaseP_h
#include <Arduino.h>
class FirebaseP
{
    public:
        FirebaseP(String branchName);
        void sendNode(String nodeName, float dato, int timestamp);
        void configuration();
    private:
        String _branchName;
        String _nodeName;
        String _timePath = "/timestamp";
        String _parentPath; // Parent Node (to be updated in every loop)
        String _uid;// Variable to save USER UID
        String _databasePath; // Database main path (to be updated in setup with the user UID)
        int _timestamp;
        float _dato;
};
#endif