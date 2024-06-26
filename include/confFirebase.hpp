#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h" // Provide the token generation process info.
#include "addons/RTDBHelper.h" // Provide the RTDB payload printing info and other helper functions.
#define API_KEY "AIzaSyApTTyO6HAivzvcYs0ILDye2uQRZbQJZGM" // Insert Firebase project API Key
#define USER_EMAIL "nerigaribo@gmail.com" // Insert Authorized Email and Corresponding Password
#define USER_PASSWORD "powermeter"
#define DATABASE_URL "https://powermeteresp32-default-rtdb.firebaseio.com/" // Insert RTDB URLefine the RTDB URL

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String uid;// Variable to save USER UID
String databasePath; // Database main path (to be updated in setup with the user UID)
// Database child nodes
String currPath = "/current";
String voltPath = "/voltage";
String powPath = "/power";
String powHsPath = "/powerForHour";
String timePath = "/timestamp";
String parentPath; // Parent Node (to be updated in every loop)

void initFirebase(){
    config.api_key = API_KEY;// Assign the api key (required)
    auth.user.email = USER_EMAIL;  // Assign the user sign in credentials
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;  // Assign the RTDB URL (required)
    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);
    // Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
    config.max_token_generation_retry = 5;  // Assign the maximum retry of token generation
    Firebase.begin(&config, &auth);  // Initialize the library with the Firebase authen and config
    // Getting the user UID might take a few seconds
    Serial.println("Getting User UID");
    while ((auth.token.uid) == "") {
      Serial.print('.');
      delay(1000); 
    }
    // Print user UID
    uid = auth.token.uid.c_str();
    Serial.print("User UID: ");
    Serial.println(uid);
    databasePath = "/UsersData/" + uid + "/readings";  // Update database path
}