/*! \file */
#include "Camera.hpp"

/*! \brief Constructor using vectors
 *
 * \param position The starting Camera Position
 * \param up The up vector for the scene (Not the Co-ordinate space up vector)
 * \param yaw The starting yaw value
 * \param pitch the Starting pitch value
 */
Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) :
_front(glm::vec3(0.0f,0.0f,-1.0f)),
_movementSpeed(SPEED),
_mouseSensitivity(SENSITIVITY),
_zoom(ZOOM)
{
    _position = position;
    _worldUp = up;
    _yaw = yaw;
    _pitch=pitch;
    UpdateCameraVectors(); //Create first view of scene
}

/*! \brief Constructor using scalar values */
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch):
_front(glm::vec3(0.0f,0.0f,-1.0f)),_movementSpeed(SPEED), _mouseSensitivity(SENSITIVITY), _zoom(ZOOM)
{
    _position = glm::vec3(posX,posY,posZ);
    _worldUp = glm::vec3(upX,upY,upZ);
    _yaw = yaw;
    _pitch = pitch;
    UpdateCameraVectors();
}

Camera::~Camera(){
    
}

/*! \brief Return current view matrix
 *  \returns The lookAt as defined by current vectors
 */
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(_position,_position + _front, _up);
}


/*! \brief Update Camera according to keyboard input
 *
 * Takes the results of the keyboard scrollback function to
 * manipulate the camera vectors and lookAt matrix to provide
 * desired view of scene
 * \param direction The direction of camera movement
 * \param deltaTime To smooth movement across different frame rates
 */
void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = _movementSpeed * deltaTime;
    
    if(direction == FORWARD)
        _position += _front * velocity;
    if(direction == BACKWARD)
        _position -= _front * velocity;
    if(direction == LEFT)
        _position -= _right * velocity;
    if(direction == RIGHT)
        _position += _right * velocity;
    
   // _position.y= 0.0f;
    
    if( _position.z >= 99.0f)
        _position.z = 99.0f;
    if( _position.z <= -99.0f)
        _position.z = -99.0f;

    if( _position.x >= 99.0f)
        _position.x = 99.0f;
    if( _position.x <= -99.0f)
        _position.x = -99.0f;
    
    if( _position.y >= 55.0f)
        _position.y = 55.0f;
    if( _position.y <= 0.0f)
        _position.y = 0.0f;


    
}

/*! \brief Updates camera angle from mouse input
 *
 * Proceeds from the cursor_callback function  to
 * rotate/roll/pitch camera in desired direction based on
 * user mouse movement.
 * \param xoffset,yoffset Mouse movement
 * \param constrainPitch to Ensure view does not flip when user looks too high/low
 */
void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= _mouseSensitivity;
    yoffset *= _mouseSensitivity;
    
    _yaw += xoffset;
    _pitch += yoffset;
    
    if(constrainPitch){
        if(_pitch > 89.0f)
            _pitch = 89.0f;
        if(_pitch < -89.0f)
            _pitch = -89.0f;
    }
    
    UpdateCameraVectors();
}

/*! \brief Update camera based on scroll zoom */
void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    if(_zoom >= 1.0f && _zoom <=45.0f)
        _zoom -= yoffset;
    if(_zoom <= 1.0f)
        _zoom = 1.0f;
    if(_zoom >=45.0f)
        _zoom = 45.0f;
}

/*! \brief Get View Matrix ready for next frame */
void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);

    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up    = glm::normalize(glm::cross(_right, _front));
}
