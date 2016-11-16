/*! \file */
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//! Used to Define the Direction of movement of the camera
enum Camera_Movement{
    FORWARD, //!<Move the Camera towards teh target
    BACKWARD,//!< Move the camera away from the target
    LEFT, //!< Move Camera to the left
    RIGHT //!< Move Camera to the right
};

// Default Values for Camera Attributes and Angles
const GLfloat YAW        = -90.0f; //!< Default yaw angle
const GLfloat PITCH      =  0.0f; //!< Default Pitch Angle
const GLfloat SPEED      =  50.0f; //!< Movement Speed
const GLfloat SENSITIVITY =  0.10f; //!< Sensitivity of Mouse Movement
const GLfloat ZOOM       =  45.0f; //!< Zoom default Value;


/*! \brief Camera Class for Scene's single Camera.
 *
 * Define the view matrix and their individual vectors. Class also provides update functions to move camera
 * based on user interaction with keyboard, mouse movement and mouse scroll.
 */
class Camera{
private:
    
    void UpdateCameraVectors();
    
public:
    //Camera Attributes;
    glm::vec3 _position; //!< The Camera Position
    glm::vec3 _front; //!< The Camera Target ( where the camera is pointing to )
    glm::vec3 _up; //!< The up vector
    glm::vec3 _right; //!< The right hand vector to define co-ordinate space
    glm::vec3 _worldUp; //!< The up vector of the worl (_right x _up)
    
    //Angles for Panning and Zooming
    GLfloat _yaw; //!<Current Yaw Angle of camera
    GLfloat _pitch;//!< Current Pitch Angle of camera
    GLfloat _movementSpeed; //!< Movement speed for keyboard input
    GLfloat _mouseSensitivity; //!< Mouse Sensititvity for looking around
    GLfloat _zoom;

    
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
   
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

    ~Camera();
    
    glm::mat4 GetViewMatrix();
    
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
    
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    
    void ProcessMouseScroll(GLfloat yoffset);
    
    
};

#endif