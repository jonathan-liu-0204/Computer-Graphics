#include <algorithm>
#include <memory>
#include <vector>

#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION
#include <glm/glm.hpp>

#include "camera.h"
#include "opengl_context.h"
#include "utils.h"

#define ANGEL_TO_RADIAN(x) (float)((x)*M_PI / 180.0f) 
#define RADIAN_TO_ANGEL(x) (float)((x)*180.0f / M_PI) 

#define CIRCLE_SEGMENT 64

/* Components size definition */
#define ARM_LEN 1.0f
#define ARM_RADIUS 0.05f
#define ARM_DIAMETER (ARM_RADIUS * 2)
#define JOINT_RADIUS 0.05f
#define JOINT_DIAMETER (JOINT_RADIUS * 2)
#define JOINT_WIDTH 0.1f
#define BASE_RADIUS 0.5f
#define BASE_DIAMETER (BASE_RADIUS * 2)
#define BASE_HEIGHT 0.1f
#define TARGET_RADIUS 0.05f
#define TARGET_DIAMETER (TARGET_RADIUS * 2)
#define TARGET_HEIGHT 0.1f
#define ROTATE_SPEED 1.5f 
#define CATCH_POSITION_OFFSET 0.05f
#define TOLERANCE 0.1f

#define RED 0.905f, 0.298f, 0.235f
#define BLUE 0.203f, 0.596f, 0.858f
#define GREEN 0.18f, 0.8f, 0.443f

float joint0_degree = 0;
float joint1_degree = 0;
float joint2_degree = 0;

int space_pressed = 0;
float distance = 100.0;
float robot_x = 0;
float robot_y = 0;
float robot_z = 0;
int pick = 0;

glm::vec3 target_pos(1.0f, 0.05f, 1.0f);
float target_x = target_pos.x;
float target_y = target_pos.y;
float target_z = target_pos.z;

void resizeCallback(GLFWwindow* window, int width, int height) {
  OpenGLContext::framebufferResizeCallback(window, width, height);
  auto ptr = static_cast<Camera*>(glfwGetWindowUserPointer(window));
  if (ptr) {
    ptr->updateProjectionMatrix(OpenGLContext::getAspectRatio());
  }
}

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
  // There are three actions: press, release, hold(repeat)
  if (action == GLFW_REPEAT) return;
  // Press ESC to close the window.
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    return;
  }

  /* TODO#4-1: Detect key-events, perform rotation or catch target object
   *       1. Use switch/case to find the key you want.
   *       2. Define and modify some global variable to trigger update in rendering loop
   * Hint:
   *       glfw3's key list (https://www.glfw.org/docs/3.3/group__keys.html)
   *       glfw3's action codes (https://www.glfw.org/docs/3.3/group__input.html#gada11d965c4da13090ad336e030e4d11f)
   * Note:
   *       You should finish your robotic arm first.
   *       Otherwise you will spend a lot of time debugging this with a black screen.
   */

  if (key == GLFW_KEY_SPACE) {
     space_pressed = 1;
  }
  else {
      space_pressed = 0;
    }  

  switch (key) { 
    //BASE ROTATE CLOCKWISE
    case GLFW_KEY_J:
      joint0_degree += ROTATE_SPEED;
      break;

    // BASE ROTATE COUNTERCLOCKWISE
    case GLFW_KEY_U:
      joint0_degree -= ROTATE_SPEED;
      break;

    // JOINT1 ROTATE CLOCKWISE
    case GLFW_KEY_K:
      joint1_degree += ROTATE_SPEED;
      break;

    // JOINT1 ROTATE COUNTERCLOCKWISE
    case GLFW_KEY_I:
      joint1_degree -= ROTATE_SPEED;
      break;

    // JOINT2 ROTATE CLOCKWISE
    case GLFW_KEY_L:
      joint2_degree += ROTATE_SPEED;
      break;

    // JOINT2 ROTATE COUNTERCLOCKWISE
    case GLFW_KEY_O:
      joint2_degree -= ROTATE_SPEED;
      break;
  }
}

void initOpenGL() {
  // Initialize OpenGL context, details are wrapped in class.
#ifdef __APPLE__
  // MacOS need explicit request legacy support
  OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
#else
  OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
//  OpenGLContext::createContext(43, GLFW_OPENGL_COMPAT_PROFILE);
#endif
  GLFWwindow* window = OpenGLContext::getWindow();
  /* TODO#0: Change window title to "HW1 - `your student id`"
   *         Ex. HW1 - 311550000 
   */
  glfwSetWindowTitle(window, "HW1 - 311605004");
  glfwSetKeyCallback(window, keyCallback);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#ifndef NDEBUG
  OpenGLContext::printSystemInfo();
  // This is useful if you want to debug your OpenGL API calls.
  OpenGLContext::enableDebugCallback();
#endif
}

void drawUnitCylinder() {
  /* TODO#2-1: Render a unit cylinder
   * Hint:
   *       glBegin/glEnd (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBegin.xml)
   *       glColor3f (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBegin.xml)
   *       glVertex3f (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glVertex.xml)
   *       glNormal (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glNormal.xml)
   *       glScalef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glScale.xml)
   * Note:
   *       You can refer to ppt "Draw Cylinder" page and `CIRCLE_SEGMENT`
   *       You should set normal for lighting
   */
    
    glBegin(GL_QUAD_STRIP);

    GLfloat x;
    GLfloat y;
    GLfloat angle = 2 * M_PI;
    GLfloat angle_stepsize = angle / 64;

    while (angle > 0.0) {
        x = 1.0 * cos(angle);
        y = 1.0 * sin(angle);
        glNormal3f(cos(angle), 0, sin(angle));
        glVertex3f(x, 1.0, y);
        glVertex3f(x, 0.0, y);
        angle = angle - angle_stepsize;
    }
    glVertex3f(1.0, 1.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    glEnd();


    glBegin(GL_POLYGON);
    angle = 2 * M_PI;
    while (angle > 0.0) {
      x = 1.0 * cos(angle);
      y = 1.0 * sin(angle);
      glNormal3f(0, 1, 0);
      glVertex3f(x, 1.0, y);
      angle = angle - angle_stepsize;
    }
    glVertex3f(1.0, 1.0, 0.0);
    glEnd();


    glBegin(GL_POLYGON);
    angle = 2 * M_PI;
    while (angle > 0.0) {
      x = 1.0 * cos(angle);
      y = 1.0 * sin(angle);
      glNormal3f(0, -1, 0);
      glVertex3f(y, 0, x);
      angle = angle - angle_stepsize;
    }
    glVertex3f(1.0, 1.0, 0.0);
    glEnd();
}

void light() {
  GLfloat light_specular[] = {0.6, 0.6, 0.6, 1};
  GLfloat light_diffuse[] = {0.6, 0.6, 0.6, 1};
  GLfloat light_ambient[] = {0.4, 0.4, 0.4, 1};
  GLfloat light_position[] = {50.0, 75.0, 80.0, 1.0};
  // z buffer enable
  glEnable(GL_DEPTH_TEST);
  // enable lighting
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_NORMALIZE);
  // set light property
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}

int main() {
  initOpenGL();
  GLFWwindow* window = OpenGLContext::getWindow();

  // Init Camera helper
  Camera camera(glm::vec3(0, 2, 5));
  camera.initialize(OpenGLContext::getAspectRatio());
  // Store camera as glfw global variable for callbasks use
  glfwSetWindowUserPointer(window, &camera);

  // Main rendering loop
  while (!glfwWindowShouldClose(window)) {
    // Polling events.
    glfwPollEvents();
    // Update camera position and view
    camera.move(window);
    // GL_XXX_BIT can simply "OR" together to use.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /// TO DO Enable DepthTest
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(camera.getProjectionMatrix());
    // ModelView Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(camera.getViewMatrix());

#ifndef DISABLE_LIGHT   
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearDepth(1.0f);
    light();
#endif

    /* TODO#4-2: Update joint degrees
     *       1. Finish keyCallback to detect key events
     *       2. Update jointx_degree if the correspond key is pressed
     * Note:
     *       You can use `ROTATE_SPEED` as the speed constant. 
     *       If the rotate speed is too slow or too fast, please change `ROTATE_SPEED` value
     */

    /* TODO#5: Catch the target object with robotic arm
     *       1. Calculate coordinate of the robotic arm endpoint
     *       2. Test if arm endpoint and the target object are close enough
     *       3. Update coordinate fo the target object to the arm endpoint
     *          if the space key is pressed
     * Hint: 
     *       GLM fransform API (https://glm.g-truc.net/0.9.4/api/a00206.html)
     * Note: 
     *       You might use `ANGEL_TO_RADIAN`
     *       and refer to `CATCH_POSITION_OFFSET` and `TOLERANCE`
     */

    // Render a white board
    glPushMatrix();
    glScalef(3, 1, 3);
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glEnd();
    glPopMatrix();

    if (space_pressed == 1) {
      // Calculate the distance to the target
      robot_x = cos(glm::radians(joint0_degree)) * (ARM_LEN + JOINT_RADIUS * 2 + (JOINT_RADIUS + ARM_LEN + CATCH_POSITION_OFFSET) * cos(glm::radians( joint2_degree))) * sin(glm::radians(joint1_degree));
      robot_y = BASE_HEIGHT + ARM_LEN + JOINT_RADIUS - (JOINT_RADIUS * 2 + ARM_LEN) * cos(glm::radians(180 - joint1_degree - joint2_degree));
      robot_z = sin(glm::radians(joint0_degree)) * (ARM_LEN + JOINT_RADIUS * 2 + (JOINT_RADIUS + ARM_LEN + CATCH_POSITION_OFFSET) * cos(glm::radians(joint2_degree))) * sin(glm::radians(joint1_degree));

      distance = sqrtf(powf(robot_x - target_x, 2) + powf(robot_y - (target_y + TARGET_HEIGHT), 2) + powf(robot_z - target_z, 2));

        
     if (distance < TOLERANCE) {
        pick = 1;
        target_x = robot_x;
        target_y = robot_y;
        target_z = robot_z;
      } else {
        pick = 0;
      }
    }

    /* TODO#2: Render a cylinder at target_pos
     *       1. Translate to target_pos
     *       2. Setup vertex color
     *       3. Setup cylinder scale
     *       4. Call drawUnitCylinder
     * Hint: 
     *       glTranslatef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glTranslate.xml)
     *       glColor3f (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glColor.xml)
     *       glScalef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glScale.xml)
     * Note:
     *       The coordinates of the cylinder are `target_pos`
     *       The cylinder's size can refer to `TARGET_RADIUS`, `TARGET_DIAMETER` and `TARGET_HEIGHT`
     *       The cylinder's color can refer to `RED`
     */

    if (pick == 0) {
      glPushMatrix();
      glTranslatef(target_x, target_y, target_z);
      glColor3f(RED);
      glScalef(TARGET_RADIUS, TARGET_HEIGHT, TARGET_RADIUS);
      drawUnitCylinder();
      glPopMatrix();
    }

    /* TODO#3: Render the robotic arm
     *       1. Render the base
     *       2. Translate to top of the base
     *       3. Render an arm
     *       4. Translate to top of the arm
     *       5. Render the joint
     *       6. Translate and rotate to top of the join
     *       7. Repeat step 3-6
     * Hint:
     *       glPushMatrix/glPopMatrix (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml)
     *       glRotatef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glRotate.xml)
     * Note:
     *       The size of every component can refer to `Components size definition` section
     *       Rotate degree for joints are `joint0_degree`, `joint1_degree` and `joint2_degree`
     *       You may implement drawBase, drawArm and drawJoin first
     */

    // BASE    
    glRotatef(joint0_degree, 0.0, 1.0, 0.0);
    glTranslatef(0.0f, 0.0, 0.0f);
    glPushMatrix();
    glColor3f(GREEN);
    glScalef(BASE_RADIUS, BASE_HEIGHT, BASE_RADIUS);
    drawUnitCylinder(); 
    glPopMatrix();

    // FIRST ARM
    glTranslatef(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glColor3f(BLUE);
    glScalef(ARM_RADIUS, ARM_LEN, ARM_RADIUS);
    drawUnitCylinder();
    glPopMatrix();

    // FIRST JOINT
    glTranslatef(JOINT_RADIUS, ARM_LEN + JOINT_RADIUS, 0.0f);
    glPushMatrix();
    glColor3f(GREEN);
    glRotatef(90.0f, 0.0, 0.0, 1.0);
    glScalef(JOINT_RADIUS, JOINT_WIDTH, JOINT_RADIUS);
    drawUnitCylinder();
    glPopMatrix();    

    // SECOND ARM
    glRotatef(joint1_degree, 1.0, 0.0, 0.0);
    glTranslatef(-JOINT_RADIUS, JOINT_RADIUS, 0.0f);
    glPushMatrix();
    glColor3f(BLUE);
    glScalef(ARM_RADIUS, ARM_LEN, ARM_RADIUS);
    drawUnitCylinder();
    glPopMatrix();

    //// SECOND JOINT
    glTranslatef(JOINT_RADIUS, ARM_LEN + JOINT_RADIUS, 0.0f);
    glPushMatrix();
    glColor3f(GREEN);
    glRotatef(90.0f, 0.0, 0.0, 1.0);
    glScalef(JOINT_RADIUS, JOINT_WIDTH, JOINT_RADIUS);
    drawUnitCylinder();
    glPopMatrix();    

    //// THIRD ARM
    glRotatef(joint2_degree, 1.0, 0.0, 0.0);
    glTranslatef(-JOINT_RADIUS, JOINT_RADIUS, 0.0f);
    glPushMatrix();
    glColor3f(BLUE);
    glScalef(ARM_RADIUS, ARM_LEN, ARM_RADIUS);
    drawUnitCylinder();
    glPopMatrix();

    if (pick == 1) {
      glPushMatrix();
      glTranslatef(0.0, ARM_LEN, 0.0);
      glColor3f(RED);
      glScalef(TARGET_RADIUS, TARGET_HEIGHT, TARGET_RADIUS);
      drawUnitCylinder();
      glPopMatrix();
    }

#ifdef __APPLE__
    // Some platform need explicit glFlush
    glFlush();
#endif
    glfwSwapBuffers(window);
  }
  return 0;
}
