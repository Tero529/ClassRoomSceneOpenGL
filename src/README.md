\mainpage Computer Graphics IS F311 Assignment 2 
\section Contributors
 *  Varun Natu 2014A7PS841H
 *  Ayush Sharma 2014A7PS039H
 *  Akanksha Pandey 2014A7PS151H
\section Implementations
\subsection a1 Classroom
We have drawn a classroom scene using just one cube which has been replicated and transformed to produce all the objects seen. All objects are drawn with solid colors, we have not used gradients or textures. We have used the programmable graphics pipeline that comes as part of OpenGL 3.+. To aid all our transformations, we have used the OpenGL Mathematics Library glm.
*   The class room has four rows each at an elevated level  with 3 chairs and a table on either side with an aisle in between.
*   We have done some decoration on the side walls to give a more aesthetic look to the solid colours. There is a whiteboard in the center of the front wall, flanked by two blackboards on either side. 
*   Furthermore we have added funcitonality for a drop down projector sheet, which can be toggled to descend / retract by pressing P on the keyboard.
*   Pressing the L keys causes sound animations to come out of the speakrs situated on either side of the room
 \see main.cpp cubeVerts.h

\subsection a2 Camera
We have implemented all camera movement using a camera class which processes user input through keyboard and mouse to make necessary changes to the lookAt matrix.
*   Keyboard keys w,a,s,d can be used to move in plane containing the direction of current camera target. This keyboard input has been smoothened using a keys[1024] parameter to allow user to move in two directions at once and to remove choppy movement (Keyboard changes camera position)
* Mouse can be used to look around the scene, thereby changing the camera target . A sensitivity parameter with a deltaTime parameter has been used to provide a more uniform mouse movement irrespective of frame rate of the device.

\see Camera.cpp Camera.hpp main.cpp

\subsection a3 Shaders

As part of using the programmable graphics pipeline, all out drawing is done ultimately by the shaders. The vertex shader, which gets executed once for every vertex and the fragment shader which gets executed once for every screen pixel.
* We have used 4 uniforms in the vertext shader, one for each matrix (view,projection and model) and 1 vec3 for the color. The operation of our vertex shader it simple, it performs matrix multiplication of all the matrices to the vertex and forwards the color to the fragment shader.
* The fragment shader simply outputs the color it recieved without performing any further operations. 

\see Shader.hpp vertex.vsh