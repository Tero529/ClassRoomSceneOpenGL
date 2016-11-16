/*! \file */

#include <iostream>
#include<string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Other includes
#include "Shader.hpp" // For the Shader object
#include "Camera.hpp" // For Camera Definition
#include "cubeVerts.h" // Vertices of main cube used for all objects



// Function prototypes

/*! \brief Handles Keyboard Input
 *
 * Is called whenever a key is pressed/released via GLFW.\n
 * Processes User Interaction through the keyboard to manipulate the keyboard
 * \param window the window in which event was generated
 * \param key The key which caused the event
 * \param action Whether the key was pressed or released
 */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

/*! \brief Handles Mouse cursor Input
 *
 * Is called whenever the mouse is moved within the window,
 * Changes the angles and vectors of camera class to alter view.
 * \param window Window in which event was caused
 * \param xpos,ypos Mouse Position when even was caused
 */
void mouse_callback(GLFWwindow* window,double xpos,double ypos);

/*! \brief Handles Mouse Scroll Input
 *
 * Zooms on scrool movement.
 * \param window Window in which event was caused
 * \param xpos,ypos Mouse Position when even was caused
 */
void scroll_callback(GLFWwindow* windoe,double xoffset,double yoffset);

/*!
 */
void do_movement();

// Window dimensions
const GLuint WIDTH = 1280, //!<Width of the window
HEIGHT = 720; //!< Height of the window

//For Projector Functionality
bool down=false; //!< For the projector movement in the scene
GLfloat projectorLength; //!< Length upto which projector is unfurled.


//Camera Values
Camera camera(glm::vec3(0.0f,50.0f,53.0f)); //!< Camera Object Creation

bool keys[1024];//!< To Keey Track of currently pressed keys (Smooth camera movement)

GLfloat deltaTime = 0.0f;	//!< Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	//!< Time of last frame

GLfloat lastX =400, //!< Last x position of mouse
lastY = 300; //!< Last y position of mouse

bool firstMouse=true; //!< To avoid jerk when mouse first comes into window

GLfloat nearPlane=0.1f, //!<Smallest z-value to draw from
farPlane=500.0f; //!< Largest z-value to draw till

GLfloat temp=0.0;//!< For length of speaker animation lines
bool speakInc=true,//!< Whether speaker lines should be increased or decreased
    speaker=false; //!< Whether to show speaker lines

/*! The MAIN function, from here we start the application and run the game loop, refer to code for implementation details*/
int main()
{
    

    // Initialise GLFW
    glfwInit();
    
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a GLFWwindow object to render into
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Class Room", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    
    // Set the required callback functions to process user interaction
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetScrollCallback(window,scroll_callback);
    
    //To Hide the cursor while within  the window
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    
    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();
    
    // Define viewport dimensions
    int width,height;
    glfwGetFramebufferSize(window,&width,&height); //To counteract any device specific scaling
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    

    // Build and compile the shader program
    Shader ourShader("vertex.vsh", "fragment.fsh");
    
    // Set up vertex data buffers and attribute pointers for the front and back wall
    GLfloat vertices[] = {
        // Positions
        105.0f,  105.0f, -100.0f,
        105.0f,  0.0f, -100.0f,
        -105.0f,  105.0f, -100.0f,
        -105.0f,  0.0f, -100.0f
    };
    
    GLuint indices[]={
        0, 1, 2,
        1, 2, 3
    };
    
    //Front and Back of the room
    GLuint VBO, VAOfront,EBO;
    
    glGenVertexArrays(1, &VAOfront);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffers and attribute pointers.
    glBindVertexArray(VAOfront);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    
    glBindVertexArray(0); // Unbind VAO
    
    
    //Sides of the room
    GLfloat sides[] = {
        // Positions
        105.0f,  105.0f, -100.0f,
        105.0f,  0.0f, -100.0f,
        -105.0f,  105.0f, -100.0f,
        -105.0f,  0.0f, -100.0f,
    };
    
    
    GLuint VBOsides, VAOsides,EBOsides;
    
    glGenVertexArrays(1, &VAOsides);
    glGenBuffers(1, &VBOsides);
    glGenBuffers(1,&EBOsides);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffers and attribute pointers.
    glBindVertexArray(VAOsides);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBOsides);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sides), sides, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBOsides);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    
    glBindVertexArray(0); // Unbind VAO
    
    
    
    //Ceiling and Floor of the room
    GLfloat Ceiling[] = {
        105.0f,  0.0f, 100.0f,
        105.0f,  0.0f, -100.0f,
        -105.0f, 0.0f, 100.0f,
        -105.0f, 0.0f, -100.0f,
    };
    
    
    GLuint VBOtop, VAOtop,EBOtop;
    
    glGenVertexArrays(1, &VAOtop);
    glGenBuffers(1, &VBOtop);
    glGenBuffers(1,&EBOtop);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffers and attribute pointers.
    glBindVertexArray(VAOtop);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBOtop);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Ceiling), Ceiling, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBOsides);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    
    glBindVertexArray(0);
    
    //Arbitrary Cube which will be used for all object creation henceforth
    GLuint VBOcube, VAOcube;
    
    glGenVertexArrays(1, &VAOcube);
    glGenBuffers(1, &VBOcube);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffers and attribute pointers.
    glBindVertexArray(VAOcube);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBOcube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeV), cubeV, GL_STATIC_DRAW);
    
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    
    glBindVertexArray(0);
    
    
    
    
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Process and update camera based on suer input information
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glfwPollEvents();
        do_movement();
        
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw the triangle
        ourShader.Use();
        
        
        //Define Matrices to transform world coordinated by in the shader
        glm::mat4 model,view,projection;
        GLint modelLoc,viewLoc,projectionLoc,colorLoc;
        glm::vec3 color;
        
        //Define Perspective Projection settings
        projection = glm::perspective(camera._zoom,float(width)/float(height),nearPlane,farPlane); //Field of view, Aspect Ratio, Near and Far Plane
        view=camera.GetViewMatrix(); //Get the current view matrix from the camera class
        
        //Get IDs of the uniforms in the shader which are to be set with values
        modelLoc=glGetUniformLocation(ourShader.Program,"model");
        viewLoc=glGetUniformLocation(ourShader.Program,"view");
        projectionLoc=glGetUniformLocation(ourShader.Program,"projection");
        colorLoc=glGetUniformLocation(ourShader.Program,"color");
        
        //Set Uniforms except model matrix which will differ for each object
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc,1,GL_FALSE,glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        
        
        
        
        
        //The Walls
        glBindVertexArray(VAOfront);
        
        //Front Wall
        glUniform3f(colorLoc,1.000, 0.922, 0.804);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        //Back Walls
        glUniform3f(colorLoc,0.961, 0.871, 0.702); //Change color for the back wal
        
        model=glm::rotate(model,glm::radians(180.0f),glm::vec3(0.0f,1.0f,0.0f));
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        glm::mat4 Wallsave=model;
        model=glm::mat4();
        
        //Clock Background
        model=glm::translate(model,glm::vec3(0.0,35.0,-1.0));
        model=glm::rotate(model,glm::radians(180.0f),glm::vec3(0.0f,1.0f,0.0f));
        model=glm::scale(model,glm::vec3(0.1,0.1,1.0));
        glUniform3f(colorLoc,0.0,0.0,0.0);
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        glUniform3f(colorLoc,1.0,1.0,1.0);
        
        model=glm::translate(model,glm::vec3(0.0,-10.0,-0.5));
        model=glm::scale(model,glm::vec3(1.1,1.2,1.0));
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        
        glBindVertexArray(0); //Unbind Array Holding front and back walls
        
        model=Wallsave;
        //Side Walls
        glBindVertexArray(VAOsides); //bind Vertex Array object holding side wall vertices
        glUniform3f(colorLoc,1.000, 0.922, 0.804);
        
        model=glm::rotate(model,glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f)); //Rotate to one side
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        model=glm::rotate(model,glm::radians(180.0f),glm::vec3(0.0f,1.0f,0.0f)); //rotate to other side
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        //Cieling and Floor
        glUniform3f(colorLoc,0.467, 0.533, 0.600);
        
        model=glm::mat4(); //Reset model matrix
        glBindVertexArray(VAOtop);
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);// Draw floor at 0 y-value
        
        glUniform3f(colorLoc,0.961, 0.961, 0.863);
        
        model=glm::translate(model,glm::vec3(0.0,55.1f,0.0f)); //Translate to top of the room
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        //Blackboard Center
        model=glm::mat4(); //Reset model matrix
        
        model=glm::translate(model,glm::vec3(0.0f,27.5f,-100.0f)); //Translate the scaled cube
        model=glm::scale(model,glm::vec3(30.0f,20.0f,1.0f)); //Scale object to size of blackboard.
        
        
        glUniform3f(colorLoc,1.0f,1.0f,1.0f);
        glBindVertexArray(VAOcube); //Bind array holding the vertices of our 1x1x1 cube
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);// Draw Blackboard
        
        
        //Repeat procedure for the border (A larger cube drawn behind blackboard cube)
        glUniform3f(colorLoc,0.871f, 0.722f, 0.529f);
        model=glm::translate(model,glm::vec3(0.0,0.0,-0.1f));
        model=glm::scale(model,glm::vec3(1.1f,1.1f,1.0f));
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);
        
        //Blackboard  on the Right
        model=glm::mat4();
        
        model=glm::translate(model,glm::vec3(35.0f,27.5f,-100.0f));
        model=glm::scale(model,glm::vec3(25.0f,20.0f,1.0f));
        
        
        glUniform3f(colorLoc,0.0f,0.0f,0.0f);
        glBindVertexArray(VAOcube);
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);
        
        glUniform3f(colorLoc,0.871f, 0.722f, 0.529f);
        model=glm::translate(model,glm::vec3(0.0,0.0,-0.1f));
        model=glm::scale(model,glm::vec3(1.1f,1.1f,1.0f));
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);
        
        //Blackboard on the Left
        model=glm::mat4();
        
        model=glm::translate(model,glm::vec3(-35.0f,27.5f,-100.0f));
        model=glm::scale(model,glm::vec3(25.0f,20.0f,1.0f));
        
        
        glUniform3f(colorLoc,0.0f,0.0f,0.0f);
        glBindVertexArray(VAOcube);
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);
        
        glUniform3f(colorLoc,0.871f, 0.722f, 0.529f);
        model=glm::translate(model,glm::vec3(0.0,0.0,-0.1f));
        model=glm::scale(model,glm::vec3(1.1f,1.1f,1.0f));
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);
        
        
        //Teacher Stage
        model=glm::mat4();
        
        model=glm::translate(model,glm::vec3(0.0f,0.0f,-85.0f)); //Move to front of room
        model=glm::scale(model,glm::vec3(220.0f,5.0f,60.0f)); //Scale to size of stage
        model=glm::translate(model,glm::vec3(0.0f,0.5f,0.0f));//Translate cube to rest on floor
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);
        
        //Wall Decoration
        float pos=110.0;
        float rot=0;
        glUniform3f(colorLoc,0.871, 0.722, 0.529);
        
        for(int j=0;j<2;j++,rot+=180){
            model=glm::mat4();
            glm::mat4 save=glm::rotate(model,glm::radians(rot),glm::vec3(0.0,1.0,0.0)); // Save matrix for each decoration object
            pos*=-1;
            for(int i=0;i<15;i++,pos=pos+17.0){
                model=save;
                model=glm::translate(model,glm::vec3(100.0f,55.0f,pos)); // Move to next decoration position
                model=glm::rotate(model,glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f));//Rotate to the side wall
                model=glm::scale(model,glm::vec3(10.0,30.0,1.0)); //Scale cube
                model=glm::rotate(model,glm::radians(45.0f),glm::vec3(0.0f,0.0f,1.0f)); //Rotate so corner points up
                glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES,0,16); //16 as no need to draw entire cube
            }
        }
        
        //Seating Levels
        model=glm::mat4();
        glUniform3f(colorLoc,0.871f, 0.722f, 0.529f);
        
        
        for(float i=0,a=0,b=0;i<4;i++,a+=5.0f,b+=30.0f){ //Repeat for each level (4 level, but only 3 stages)
            model=glm::mat4();
            model=glm::translate(model,glm::vec3(0.0f,a,b)); //Move up and back for next level
            
            glm::mat4 Levelsave=model; // To be used when drawing level Stage
            
            
            for(int k=0;k<2;k++){
                //Base Matrix for the Chair
                model=Levelsave;
                
                if(k==0)
                    model=glm::translate(model,glm::vec3(-90.0f,0.0f,0.0f));// Move right once for right set of chairs
                else
                    model=glm::translate(model,glm::vec3(30.0f,0.0f,0.0f)); //move left second for left set of chairs
                
                glm::mat4 Tempsave=model;
                
                
                for(float m=0, t=0.0f;m<4.0f;m++,t+=20.0){
                    model=Tempsave;
                    
                    model=glm::translate(model,glm::vec3(t,0.0f,0.0f));
                    glm::mat4 save=model;
                    
                    //Chair Drawing
                    
                    // Seat of Chair
                    model = glm::translate(model, glm::vec3(0.0f, 6.0f, 0.0f));
                    model = glm::scale(model, glm::vec3(5.0f,1.0f, 5.0f));
                    glUniform3f(colorLoc, 0.941, 1.000, 0.941);
                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    
                    //chiar legs
                    float arr[8] = {1,1,1,-1,-1,1,-1,-1};//array containing the deviation for the legs of the chair
                    
                    for (int x = 0; x < 4; x++) {
                        //chair leg main part
                        model = save;
                        model = glm::translate(model, glm::vec3(1.5f*arr[2*x], 0.5f, 1.5f*arr[2 * x+1]));
                        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
                        glUniform3f(colorLoc, 0.941, 1.000, 0.941);
                        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                        //chair rubber stopper at the bottom of the legs
                        model = save;
                        model = glm::translate(model, glm::vec3(1.5f*arr[2 * x], 3.0f, 1.5f*arr[2 * x + 1]));
                        model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
                        glUniform3f(colorLoc, 0.184, 0.310, 0.310);
                        
                        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                    
                    //chair back
                    model = save;
                    model = glm::translate(model, glm::vec3(0.0f, 9.0f, +2.5f));
                    model = glm::scale(model, glm::vec3(5.0f, 7.0f, 1.0f));
                    glUniform3f(colorLoc, 0.941, 1.000, 0.941);
                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    
                    // chair handles
                    int tr[2] = { 1,-1 };//array containing the handles deviation
                    glUniform3f(colorLoc, 0.184, 0.310, 0.310);
                    for (int x = 0; x < 2; x++) {
                        model = save;
                        model = glm::translate(model, glm::vec3(1.9f*tr[x],9.0f, 0.0f));
                        model = glm::scale(model, glm::vec3(0.5f, 1.0f, 5.0f));
                        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                        model = save;
                        model = glm::translate(model, glm::vec3(1.9f*tr[x], 7.5f, -2.25f));
                        model = glm::scale(model, glm::vec3(0.5f, 2.0f, 0.5f));
                        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                    
                    
                }
            }
            
            glUniform3f(colorLoc, 0.545, 0.271, 0.075);
            
            
            for(float i=0,pos=60.0f;i<2.0;i++,pos-=120.0f){
            //desk for th chair
                //the top of the table
                model = Levelsave;
                model = glm::translate(model, glm::vec3(0.0f+pos, 7.5f, -10.0f));
                model = glm::scale(model,glm::vec3(80.0f,1.0f,6.0f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
                float tr[3] = {-37.5,0.0,37.5};//deviation of the planks with resp to its centre
                //the legs or planks for the table
                for (int x = 0; x < 3; x++) {
                    model = Levelsave;
                    model = glm::translate(model, glm::vec3(tr[x] + pos, 3.5f, -10.0f));
                    model = glm::scale(model, glm::vec3(1.0f, 7.0f, 6.0f));
                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
                
            }
            
            
            
            glUniform3f(colorLoc,0.871f, 0.722f, 0.529f);
            
            //the code to re create for different levels
            
            model=Levelsave;
            if(i==3)
                continue;
            if(int(i)%2!=0)
                glUniform3f(colorLoc,1.000, 0.973, 0.863);
            
            model=glm::translate(model,glm::vec3(0.0f,0.0f,70.0f));
            model=glm::scale(model,glm::vec3(220.0f,5.0f,120.0f));
            model=glm::translate(model,glm::vec3(0.0f,0.5f,0.0f));
            glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES,0,36);
            
            //Stairs
            model=Levelsave;
            glUniform3f(colorLoc,1.000, 0.894, 0.710);
            
            
            model=glm::translate(model,glm::vec3(0.0f,0.0f,10.0f));
            model=glm::scale(model,glm::vec3(30.0f,2.5f,10.0f));
            model=glm::translate(model,glm::vec3(0.0f,0.5f,0.0f));
            glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES,0,36);
            
            
            
            
        }
        //Clock Time
        glUniform3f(colorLoc,1.0, 1.0, 1.0);
        
        for(float i=0.0;i<6.0f;i+=3.0f){
            model=glm::mat4();
            model=glm::translate(model,glm::vec3(7.5-i,40.0,99.0));
            model=glm::scale(model,glm::vec3(1.0,6.0,1.0));
            glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES,0,36);
        }
        
        model=glm::scale(model,glm::vec3(1.0,1.0f/6.0,1.0f));
        model=glm::translate(model,glm::vec3(-3.0f,0.75f,0.0f));
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);
        
        model=glm::translate(model,glm::vec3(0.0f,-1.5f,0.0f));
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);
        
        for(float i=0.0;i<3.0f;i+=2.0f){
            model=glm::mat4();
            model=glm::translate(model,glm::vec3(-1.5-i,40.0,99.0));
            model=glm::scale(model,glm::vec3(1.0,6.0,1.0));
            glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES,0,36);
        }
        
        for(float i=0.0;i<3.0f;i+=2.0f){
            model=glm::mat4();
            model=glm::translate(model,glm::vec3(-6.5-i,40.0,99.0));
            model=glm::scale(model,glm::vec3(1.0,6.0,1.0));
            glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES,0,36);
        }
        
        for(float k=0;k<7.0;k+=5.0){
            for(float i=0.0;i<7.0f;i+=5.0f){
                model=glm::mat4();
                model=glm::translate(model,glm::vec3(-2.5 -k,42.5-i,99.0));
                model=glm::rotate(model,glm::radians(90.0f),glm::vec3(0.0,0.0,1.0));
                model=glm::scale(model,glm::vec3(1.0,3.0,1.0));
                glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES,0,36);
            }
        }
        
        
        //Speakers
        
        for(float i=97.0;i>-100.0;i-=194.0f){
            model=glm::mat4();
            //translation matrix to translate the speakers
            model=glm::translate(model,glm::vec3(i,40.0,-35.0));
            
            if(i>0)
                model=glm::rotate(model,glm::radians(-45.0f),glm::vec3(0.0,1.0,0.0));
            else
                model=glm::rotate(model,glm::radians(45.0f),glm::vec3(0.0,1.0,0.0));
            
            glm::mat4 SpeakerSave=model;
            
            for(float t=0.0;t<1.5f;t+=1){
                model = SpeakerSave;
                if(t!=0.0f){
                    model = glm::scale(model,glm::vec3(1.3,1.2,1.2));
                    model = glm::translate(model, glm::vec3(0.0f,4.2f,-0.5f));
                    model = glm::scale(model, glm::vec3(5.0f, 10.0f, 5.0f));
                    glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
                }
                else{
                    model = glm::translate(model, glm::vec3(0.0f,5.0f,0.0f));
                    model = glm::scale(model, glm::vec3(5.0f, 10.0f, 5.0f));
                    glUniform3f(colorLoc, 0.647, 0.165, 0.165);
                }
                
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            float angles[] = {30.0f,-4.0f,0.0f,0.0f,-30.0f,+4.0f};
            //code for the sound effects for speakers
            if(speaker==true){//condition for the sound to play
            if(temp >=10.0 )
                speakInc=false;
            else if(temp <= 0.0f)
                speakInc = true;
            
            if(speakInc == true)
                temp+=0.1;
            else if(speakInc == false)
                temp=0;
            }
            else
                temp=0.0f;
            //the loop for adjusting the length of the sound animation
            for (int x = 0; x < 3; x++) {
                model = SpeakerSave;
                model = glm::translate(model, glm::vec3(0.0f, 5.0f+angles[2*x+1], (5.0f+(temp)/2)));
                
                model = glm::rotate(model, glm::radians(angles[2*x]), glm::vec3(1.0f,0.0f,0.0f));
               if(temp != 0.0f)
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, temp));
                else
                    model = glm::scale(model, glm::vec3(0.0f, 0.5f, 0.0f));

                glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            
        }
        //Pressing P releases and brings back the projector
        if(down == true)
            projectorLength+=0.5f;
        else
            projectorLength-=0.5f;
        
        //Check and set bounds
        if(projectorLength<0)
            projectorLength=0.0f;
        else if(projectorLength>35.0f)
            projectorLength=35.0f;
        
        //projectors sheet which draws as triggered
        glUniform3f(colorLoc,0.690, 0.878, 0.902);
        
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0, 50.0f-(projectorLength/2), -99.0f));
        model = glm::scale(model, glm::vec3(35.0f, projectorLength, 0.1f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glUniform3f(colorLoc,0.871, 0.722, 0.529);
        //base or top of the projector
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0, 50.0f, -102.0f));
        model = glm::scale(model, glm::vec3(35.0f, 10.0f, 10.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        //Teacher's Table
        glUniform3f(colorLoc,0.545, 0.271, 0.075);
        //the threee varying tops
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 17.5f, -63.0f));
        model = glm::scale(model, glm::vec3(60.0f, 1.0f, 15.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUniform3f(colorLoc,0.804, 0.522, 0.247);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 16.5f, -63.0f));
        model = glm::scale(model, glm::vec3(62.0f, 1.0f, 16.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUniform3f(colorLoc,0.545, 0.271, 0.075);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 15.5f, -63.0f));
        model = glm::scale(model, glm::vec3(64.0f, 1.0f, 17.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        //the code to create the compartments for the table
        glUniform3f(colorLoc,0.545, 0.271, 0.075);
        float displacement[] = { -30.0f,-15.0f,15.0f,30.0f };
        for (int x = 0; x < 4; x++) {
            model = glm::mat4();
            model = glm::translate(model, glm::vec3(displacement[x], 10.0f, -63.0f));
            model = glm::scale(model, glm::vec3(1.0f, 10.0f, 15.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //the back and the front for the covered box of the table
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 10.0f, -70.5f));
        model = glm::scale(model, glm::vec3(30.0f, 10.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 10.0f, -55.5f));
        model = glm::scale(model, glm::vec3(30.0f, 10.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        model=glm::mat4();
        glUniform3f(colorLoc,1.0f,1.0f,1.0f);
        model = glm::translate(model, glm::vec3(-15.0f, 18.0f, -63.0f));

        model=glm::scale(model,glm::vec3(0.8f,1.0f,2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    // Properly de-allocate all resources
    glDeleteVertexArrays(1, &VAOfront);
    glDeleteBuffers(1, &VBO);
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    
    return 0;
}

void do_movement(){
    
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
    
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);GLfloat cameraSpeed = 0.05f;
    
    if (action == GLFW_PRESS)
        keys[key]= true;
    else if(action == GLFW_RELEASE)
        keys[key]= false;
    
    if(key == GLFW_KEY_P && action == GLFW_PRESS) //Release / Bring back projector
        down = !down;
    if(key == GLFW_KEY_L && action == GLFW_PRESS) //Release / Bring back projector
        speaker = !speaker;
}

void mouse_callback(GLFWwindow *window, double xpos,double ypos){
    
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset,yoffset);
}

void scroll_callback(GLFWwindow* window,double xoffset,double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
