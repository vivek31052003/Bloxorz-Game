#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "imageio.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#include <cstdio> // Include the header for fprintf
#include <cstdlib> // Include the header for NULL
#include <X11/keysym.h> // Include the header for XK_A
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

using namespace std;



Display* display ;
GLFWwindow* window;

struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projectionO, projectionP;
    glm::mat4 model;
    glm::mat4 view;
    GLuint MatrixID;
} Matrices;
GLboolean amb=true, spec=true, dif=true;
bool l_on1 = false,l_on2 = false,l_on3 = false;
GLuint programID;
int proj_type;
glm::vec3 tri_pos, rect_pos;
int last = -1;
int last_1=-1;
/* Function to load Shaders - Use it as it is */
bool topLightEnabled = true;
bool sideLightsEnabled = false;
double rotate_x = 0, rotate_y = 0, zoom = 1;

unsigned char *image;
int width, height, bpp;

int texImageWidth;
int texImageHeight;
GLubyte *makeTexImage(char *file){
	int width, height;
	GLubyte *texImage;
	texImage = loadImageRGBA((char*) file, &width, &height);	
	texImageWidth = width;
	texImageHeight = height;
	return texImage;
}


GLfloat front_left_light_position[] = {-1.0, 0.0, 1.0, 0.0};
GLfloat front_right_light_position[] = {1.0, 0.0, 1.0, 0.0};
GLfloat back_left_light_position[] = {-1.0, 0.0, -1.0, 0.0};
GLfloat back_right_light_position[] = {1.0, 0.0, -1.0, 0.0};

// Defining material properties
// ref. https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/lightlab.c
GLfloat brass_ambient[] =
{0.33, 0.22, 0.03, 1.0}, brass_diffuse[] =
{0.78, 0.57, 0.11, 1.0}, brass_specular[] =
{0.99, 0.91, 0.81, 1.0}, brass_shininess = 27.8;

GLfloat red_plastic_ambient[] =
{0.0, 0.0, 0.0}, red_plastic_diffuse[] =
{0.5, 0.0, 0.0}, red_plastic_specular[] =
{0.7, 0.6, 0.6}, red_plastic_shininess = 32.0;

GLfloat emerald_ambient[] =
{0.0215, 0.1745, 0.0215}, emerald_diffuse[] =
{0.07568, 0.61424, 0.07568}, emerald_specular[] =
{0.633, 0.727811, 0.633}, emerald_shininess = 76.8;

GLfloat slate_ambient[] =
{0.02, 0.02, 0.02}, slate_diffuse[] =
{0.02, 0.01, 0.01}, slate_specular[] =
{0.4, 0.4, 0.4}, slate_shininess = .78125;

float diffuse_intensity = 0.5;
int select_material = 0;

// Function to toggle light sources
void toggleLightSources(int key) {
    if (key == GLFW_KEY_KP_1) {
        topLightEnabled = true;
        sideLightsEnabled = false;
    } else if (key == GLFW_KEY_KP_2) {
        topLightEnabled = false;
        sideLightsEnabled = true;
    }
}
float textureRepeat = 1.0f; // Adjust as needed
GLfloat color[] = {1.0f, 0.0f, 0.0f};
void light()
{


    GLfloat l_amb[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat l_dif[] = {0.961, 0.871, 0.702};
    GLfloat l_spec[] = {0.2,0.2,0.2,1};
    GLfloat l_no[] = {0, 0, 0, 1.0};
    GLfloat l_pos1[] = {-20,20,20,1.0};
    GLfloat l_pos2[] = {44,30,-5,1.0};
    GLfloat l_pos3[] = {0,60,0,1.0};



    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    if(l_on1)
    {
        if(amb==true)
        {
            glLightfv(GL_LIGHT0,GL_AMBIENT,l_amb);
        }
        else
        {
            glLightfv(GL_LIGHT0,GL_AMBIENT,l_no);
        }

        if(dif==true)
        {
            glLightfv(GL_LIGHT0,GL_DIFFUSE,l_dif);
        }
        else
        {
            glLightfv(GL_LIGHT0,GL_DIFFUSE,l_no);
        }
        if(spec==true)
        {
            glLightfv(GL_LIGHT0,GL_SPECULAR,l_spec);
        }
        else
        {
            glLightfv(GL_LIGHT0,GL_SPECULAR,l_no);
        }
    }
    else
    {
        glLightfv(GL_LIGHT0,GL_AMBIENT,l_no);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,l_no);
        glLightfv(GL_LIGHT0,GL_SPECULAR,l_no);

    }
    glLightfv(GL_LIGHT0,GL_POSITION,l_pos1);







    if(l_on2)
    {
        if(amb==true)
        {
            glLightfv(GL_LIGHT1,GL_AMBIENT,l_amb);
        }
        else
        {
            glLightfv(GL_LIGHT1,GL_AMBIENT,l_no);
        }

        if(dif==true)
        {
            glLightfv(GL_LIGHT1,GL_DIFFUSE,l_dif);
        }
        else
        {
            glLightfv(GL_LIGHT1,GL_DIFFUSE,l_no);
        }
        if(spec==true)
        {
            glLightfv(GL_LIGHT1,GL_SPECULAR,l_spec);
        }
        else
        {
            glLightfv(GL_LIGHT1,GL_SPECULAR,l_no);
        }
    }
    else
    {
        glLightfv(GL_LIGHT1,GL_AMBIENT,l_no);
        glLightfv(GL_LIGHT1,GL_DIFFUSE,l_no);
        glLightfv(GL_LIGHT1,GL_SPECULAR,l_no);

    }
    glLightfv(GL_LIGHT1,GL_POSITION,l_pos2);



    if(l_on3)
    {
        if(amb==true)
        {
            glLightfv(GL_LIGHT2,GL_AMBIENT,l_amb);
        }
        else
        {
            glLightfv(GL_LIGHT2,GL_AMBIENT,l_no);
        }

        if(dif==true)
        {
            glLightfv(GL_LIGHT2,GL_DIFFUSE,l_dif);
        }
        else
        {
            glLightfv(GL_LIGHT2,GL_DIFFUSE,l_no);
        }
        if(spec==true)
        {
            glLightfv(GL_LIGHT2,GL_SPECULAR,l_spec);
        }
        else
        {
            glLightfv(GL_LIGHT2,GL_SPECULAR,l_no);
        }

    }
    else
    {
        glLightfv(GL_LIGHT2,GL_AMBIENT,l_no);
        glLightfv(GL_LIGHT2,GL_DIFFUSE,l_no);
        glLightfv(GL_LIGHT2,GL_SPECULAR,l_no);
    }

    glLightfv(GL_LIGHT2,GL_POSITION,l_pos3);

    GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    GLfloat spt_ct[] = {0};
    glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, spt_ct);
}
GLuint LoadShaders ( const char * vertex_file_path,const char * fragment_file_path ) {

    // Create the shaders
    GLuint VertexShaderID = glCreateShader ( GL_VERTEX_SHADER );
    GLuint FragmentShaderID = glCreateShader ( GL_FRAGMENT_SHADER );

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream( vertex_file_path, std::ios::in );
    if ( VertexShaderStream.is_open ( ) )
    {
       std::string Line = "";
       while( getline ( VertexShaderStream, Line ) )
          VertexShaderCode += "\n" + Line;
      VertexShaderStream.close ( );
  }

    // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream ( fragment_file_path, std::ios::in );
  if ( FragmentShaderStream.is_open ( ) ){
   std::string Line = "";
   while ( getline ( FragmentShaderStream, Line ) )
       FragmentShaderCode += "\n" + Line;
   FragmentShaderStream.close ( );
}

GLint Result = GL_FALSE;
int InfoLogLength;

    // Compile Vertex Shader
    //    printf("Compiling shader : %s\n", vertex_file_path);
char const * VertexSourcePointer = VertexShaderCode.c_str();
glShaderSource ( VertexShaderID, 1, &VertexSourcePointer , NULL );
glCompileShader ( VertexShaderID );

    // Check Vertex Shader
glGetShaderiv ( VertexShaderID, GL_COMPILE_STATUS, &Result );
glGetShaderiv ( VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
std::vector<char> VertexShaderErrorMessage ( InfoLogLength );
glGetShaderInfoLog ( VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0] );
    //    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

    // Compile Fragment Shader
    //    printf("Compiling shader : %s\n", fragment_file_path);
char const * FragmentSourcePointer = FragmentShaderCode.c_str ( );
glShaderSource ( FragmentShaderID, 1, &FragmentSourcePointer , NULL );
glCompileShader ( FragmentShaderID );

    // Check Fragment Shader
glGetShaderiv ( FragmentShaderID, GL_COMPILE_STATUS, &Result );
glGetShaderiv ( FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
std::vector<char> FragmentShaderErrorMessage ( InfoLogLength );
glGetShaderInfoLog ( FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0] );
    //    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

    // Link the program
    //    fprintf(stdout, "Linking program\n");
GLuint ProgramID = glCreateProgram ( );
glAttachShader ( ProgramID, VertexShaderID );
glAttachShader ( ProgramID, FragmentShaderID );
glLinkProgram ( ProgramID );

    // Check the program
glGetProgramiv ( ProgramID, GL_LINK_STATUS, &Result );
glGetProgramiv ( ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength );
std::vector<char> ProgramErrorMessage( max ( InfoLogLength, int( 1 ) ) );
glGetProgramInfoLog ( ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0] );
    //    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

glDeleteShader ( VertexShaderID );
glDeleteShader ( FragmentShaderID );
return ProgramID;
}

static void error_callback ( int error, const char* description )
{
    fprintf ( stderr, "Error: %s\n", description );
}

void quit ( GLFWwindow *window )
{
    glfwDestroyWindow ( window );
    glfwTerminate ( );
    exit ( EXIT_SUCCESS );
}

void initGLEW ( void ) 
{
    glewExperimental = GL_TRUE;
    if ( glewInit ( ) != GLEW_OK ) {
        fprintf ( stderr,"Glew failed to initialize : %s\n", glewGetErrorString ( glewInit ( ) ) );
   }
   if ( ! GLEW_VERSION_3_3 )
       fprintf ( stderr, "3.3 version not available\n" );
}

/* Generate VAO, VBOs and return VAO handle */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
{
    struct VAO* vao = new struct VAO;
    vao->PrimitiveMode = primitive_mode;
    vao->NumVertices = numVertices;
    vao->FillMode = fill_mode;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays (1, &(vao->VertexArrayID ) ); // VAO
    glGenBuffers (1, &(vao->VertexBuffer ) ); // VBO - vertices
    glGenBuffers (1, &(vao->ColorBuffer ) );  // VBO - colors

    glBindVertexArray ( vao->VertexArrayID ); // Bind the VAO 
    glBindBuffer ( GL_ARRAY_BUFFER, vao->VertexBuffer ); // Bind the VBO vertices 
    glBufferData ( GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW ); // Copy the vertices into VBO
    glVertexAttribPointer (
                          0,                  // attribute 0. Vertices
                          3,                  // size (x,y,z)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    glBindBuffer ( GL_ARRAY_BUFFER, vao->ColorBuffer ); // Bind the VBO colors 
    glBufferData ( GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW );  // Copy the vertex colors
    glVertexAttribPointer(
                          1,                  // attribute 1. Color
                          3,                  // size (r,g,b)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    return vao;
}

/* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
struct VAO* create3DObject ( GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL )
{
    GLfloat* color_buffer_data = new GLfloat [3*numVertices];
    for ( int i = 0; i < numVertices; i++) {
        color_buffer_data [3*i] = red;
        color_buffer_data [3*i + 1] = green;
        color_buffer_data [3*i + 2] = blue;
    }

    return create3DObject ( primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode );
}

/* Render the VBOs handled by VAO */
void draw3DObject ( struct VAO* vao )
{
    // Change the Fill Mode for this object
    glPolygonMode ( GL_FRONT_AND_BACK, vao->FillMode );

    // Bind the VAO to use
    glBindVertexArray ( vao->VertexArrayID );

    // Enable Vertex Attribute 0 - 3d Vertices
    glEnableVertexAttribArray ( 0 );
    // Bind the VBO to use
    glBindBuffer ( GL_ARRAY_BUFFER, vao->VertexBuffer );

    // Enable Vertex Attribute 1 - Color
    glEnableVertexAttribArray ( 1 );
    // Bind the VBO to use
    glBindBuffer ( GL_ARRAY_BUFFER, vao->ColorBuffer );

    // Draw the geometry !
    glDrawArrays ( vao->PrimitiveMode, 0, vao->NumVertices ); // Starting from vertex 0; 3 vertices total -> 1 triangle
}

int perspective = 0;
/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow ( GLFWwindow* window, int width, int height )
{
    int fbwidth=width, fbheight=height;
    glfwGetFramebufferSize( window, &fbwidth, &fbheight );

    GLfloat fov = M_PI/2;

    // sets the viewport of openGL renderer
    glViewport ( 0, 0, (GLsizei) fbwidth, (GLsizei) fbheight );

    // Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    Matrices.projectionP = glm::perspective ( fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f );

    // Ortho projection for 2D views
    Matrices.projectionO = glm::ortho ( -4.0f, 4.0f, -4.0f, 4.0f, 0.1f, 500.0f );
}
float yellow_r = 1.0f;
    float yellow_g = 1.0f;
    float yellow_b = 0.0f;

GLfloat *createColor ( float r1, float g1, float b1, 
                                    float r2, float g2, float b2,
                                    float r3, float g3, float b3) {
    
        GLfloat *color_buffer_data = ( GLfloat * ) malloc ( sizeof ( float ) * 108 );
    
        GLfloat Color [ ] = {
        r1, g1, b1, r1, g1, b1, r1, g1, b1, r1, g1, b1, r1, g1, b1, r1, g1, b1,  //1
        r2, g2, b2, r2, g2, b2, r2, g2, b2, r2, g2, b2, r2, g2, b2, r2, g2, b2,  //2
        r3, g3, b3, r3, g3, b3, r3, g3, b3, r3, g3, b3, r3, g3, b3, r3, g3, b3,  //3 

        r1, g1, b1, r1, g1, b1,r1, g1, b1,r1, g1, b1,r1, g1, b1, r1, g1, b1,   //4
        r2, g2, b2, r2, g2, b2, r2, g2, b2, r2, g2, b2, r2, g2, b2, r2, g2, b2, //5
        r3, g3, b3, r3, g3, b3, r3, g3, b3, r3, g3, b3, r3, g3, b3, r3, g3, b3, //6
    };
    
    for ( int i = 0; i < 108; i++ )
        color_buffer_data[ i ] = Color[ i ];

    return color_buffer_data;
}

VAO *createCell ( float l, float b, float h, GLfloat Color [ ] ,float textureRepeat)
{
    GLfloat vertex_buffer_data [ ] = {
        0, 0, 0, b, 0, 0, b, h, 0, b, h, 0, 0, h, 0, 0, 0 , 0,        //1
        0, 0, 0, 0, h, 0, 0, h, l, 0, h, l, 0, 0, l, 0, 0, 0,    //2
        0, 0, 0, 0, 0, l, b, 0, l, b, 0, l, b, 0, 0, 0, 0, 0,        //3 

        0, 0, l, b, 0, l, b, h, l, b, h, l, 0, h, l, 0, 0, l,        //4
        b, 0, l, b, 0, 0, b, h, 0, b, h, 0, b, h, l, b, 0, l,            //5
        0, h, l, b, h, l, b, h, 0, b, h, 0, 0, h, 0, 0, h, l         //6
    };
     GLfloat texture_coords[] = {
        // Front face
        0, 0, // 0
        textureRepeat, 0, // 1
        textureRepeat, textureRepeat, // 2
        textureRepeat, textureRepeat, // 2
        0, textureRepeat, // 3
        0, 0, // 0

        // Back face
        0, 0, // 4
        textureRepeat, 0, // 5
        textureRepeat, textureRepeat, // 6
        textureRepeat, textureRepeat, // 6
        0, textureRepeat, // 7
        0, 0, // 4

        0, 0, // 4
        textureRepeat, 0, // 5
        textureRepeat, textureRepeat, // 6
        textureRepeat, textureRepeat, // 6
        0, textureRepeat, // 7
        0, 0, // 4

        0, 0, // 4
        textureRepeat, 0, // 5
        textureRepeat, textureRepeat, // 6
        textureRepeat, textureRepeat, // 6
        0, textureRepeat, // 7
        0, 0, // 4

        0, 0, // 4
        textureRepeat, 0, // 5
        textureRepeat, textureRepeat, // 6
        textureRepeat, textureRepeat, // 6
        0, textureRepeat, // 7
        0, 0, // 4

        0, 0, // 4
        textureRepeat, 0, // 5
        textureRepeat, textureRepeat, // 6
        textureRepeat, textureRepeat, // 6
        0, textureRepeat, // 7
        0, 0 // 4

        // Repeat for other faces...
    };
GLfloat combined_buffer_data[6 * 6 * 2]; // 6 vertices per face, 6 faces, 2 values (x, y) per vertex
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            combined_buffer_data[i * 6 + j] = vertex_buffer_data[i * 3 + j % 3];
            combined_buffer_data[i * 6 + j + 6] = texture_coords[i * 2 + j % 2];
        }
    }

    // Create VAO using the combined buffer data
    VAO *cellObject = create3DObject(GL_TRIANGLES, 36, combined_buffer_data, Color, GL_FILL);

    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);

    // Bind the texture
    unsigned int texture;
	glGenTextures(1, &texture);
	GLubyte *texImage = makeTexImage((char*)"color.png");
	// if ( !texImage ) {
	// 	printf("\nError reading image \n");
	// 	return;
	// }
    glBindTexture(GL_TEXTURE_2D, texture); // Replace textureID with your texture ID

    return cellObject;
}

class GraphicalObject
{
public:
    VAO* object;
    float x_ordinate;
    float y_ordinate;
    float z_ordinate;
    float height;
    float length;
    char color;
    glm::mat4 Itranslate_matrix;
    glm::mat4 Irotate_matrix;
    glm::mat4 translate_matrix;
    glm::mat4 rotate_matrix;

public:
    GraphicalObject ( float X=0, float Y=0, float Z=0, float H=0, float L=0, char colour='D' )
    {
      x_ordinate = X;
      y_ordinate = Y;
      z_ordinate = Z;
      height = H;
      length = L;
      color = colour;
  }

  void Irotator ( float rotation = 0, glm::vec3 rotating_vector = glm::vec3 ( 0, 0, 1 ) ) 
  {
    Irotate_matrix = glm::rotate ( (float)(rotation*M_PI/180.0f), rotating_vector );
  }
 
  void rotator ( float rotation = 0, glm::vec3 rotating_vector = glm::vec3 ( 0, 0, 1 ) )
  {
      rotate_matrix = glm::rotate ( (float)(rotation*M_PI/180.0f), rotating_vector );
  }
 
  void translator (float x = 0, float y = 0, float z = 0 )
  {
        translate_matrix = glm::translate ( glm::vec3 ( x, y, z ) );
  }

  void Itranslator (float x = 0, float y = 0, float z = 0 )
  {
        Itranslate_matrix = glm::translate ( glm::vec3 ( x, y, z ) );
  }

  void render ( )
  {
      glm::mat4 VP = ( perspective ? Matrices.projectionP : Matrices.projectionO ) * Matrices.view;
      glm::mat4 MVP;
      Matrices.model = glm::mat4 ( 1.0f );
      Matrices.model *= translate_matrix*rotate_matrix*Itranslate_matrix*Irotate_matrix;
      MVP = VP * Matrices.model;
      glUniformMatrix4fv ( Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0] );
      draw3DObject ( object );
  }

};

// CONSTANTS //
GLfloat digitopbar[] = {
                  0.1,0.35,0,
                  -0.1,0.35,0,
                  -0.1,0.30,0,

                  -0.1,0.30,0,
                  0.1,0.30,0,
                  0.1,0.35,0
                };

GLfloat digitmidbar [] = {
                  0.1,0.05,0,
                  -0.1,0.05,0,
                  -0.1,-0.05,0,

                  -0.1,-0.05,0,
                  0.1,-0.05,0,
                  0.1,0.05,0
                };

GLfloat digitbotbar [] = {
                0.1,-0.30,0,
                -0.1,-0.30,0,
                -0.1,-0.35,0,

                -0.1,-0.35,0,
                0.1,-0.35,0,
                0.1,-0.30,0
                };

GLfloat digitlefttopbar [] = {
                              -0.05,0.30,0,
                              -0.1,0.30,0,
                              -0.1,0.05,0,

                              -0.1,0.05,0,
                              -0.05,0.05,0,
                              -0.05,0.30,0
                            };

GLfloat digitleftbotbar [] = {
                              -0.05,-0.05,0,
                              -0.1,-0.05,0,
                              -0.1,-0.30,0,

                              -0.1,-0.30,0,
                              -0.05,-0.30,0,
                              -0.05,-0.05,0
                              };

GLfloat digitrighttopbar [] = {
                              0.1,0.30,0,
                              0.05,0.30,0,
                              0.05,0.05,0,

                              0.05,0.05,0,
                              0.1,0.05,0,
                              0.1,0.30,0
                              };
GLfloat digitrightbotbar [] = {
                              0.1,-0.05,0,
                              0.05,-0.05,0,
                              0.05,-0.30,0,

                              0.05,-0.30,0,
                              0.1,-0.30,0,
                              0.1,-0.05,0
                              };
GLfloat darkyellow[] = {
                                        1,1,0,
                                        1,1,0,
                                        1,1,0,
        
                                        1,1,0,
                                        1,1,0,
                                        1,1,0
                                    };

GLfloat *Grey = createColor ( 0.30196, 0.30196, 0.30196, 
                                                0.50196, 0.50196, 0.50196,
                                                0.85098, 0.85098, 0.85098 ); 

GLfloat *White = createColor ( 0.54902, 0.54902, 0.54902,
                                                    0.8, 0.8, 0.8,
                                                    0.94902, 0.94902, 0.94902 );

GLfloat *Blue = createColor ( 0, 0.52549, 0.70196,
                                                    0, 0.74902, 1,
                                                    0.50196, 0.87451, 1 );

GLfloat *Green = createColor ( 0.45098, 0.6, 0,
                                                        0.6, 0.8, 0,
                                                        0.82353, 1,  0.30196);

GLfloat *Orange = createColor ( 0.70196, 0.56078, 0,
                                                        0.8, 0.63922, 0,
                                                        1, 0.83922,  0.2 );

GLfloat *Dorange = createColor ( 0.8, 0.63922, 0,
                                                            0.90196, 0.72157, 0,
                                                            1, 0.87843, 0.4 );

GLfloat* Yellow = createColor(yellow_r, yellow_g, yellow_b,
                                  yellow_r, yellow_g, yellow_b,
                                  yellow_r, yellow_g, yellow_b);

static const int board_size = 20;


int board[ board_size ][ board_size ];

int stage1 [board_size][board_size] = {
        {1,1,1,6,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };

 int stage2 [board_size][board_size] = {
        {1,1,1,3,3,3,3,3,3,3,1,1,0,0,0,0,0,0,0,0},
        {1,1,1,3,3,3,3,3,3,3,1,1,0,0,0,0,0,0,0,0},
        {1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
        {1,1,1,0,0,1,1,1,1,3,3,3,3,3,0,0,0,0,0,0},
        {1,1,1,0,0,1,1,1,1,3,3,3,3,3,0,0,0,0,0,0},
        {0,0,0,0,0,1,2,1,0,0,3,3,1,3,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,1,0,0,3,3,3,3,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };    

int stage3 [board_size][board_size]  = {
    {1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,0,0,0},
    {1,1,4,1,0,0,1,1,5,1,0,0,1,2,1,0,0,0,0,0},
    {1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,0,0,0},
    {1,1,1,1,7,7,1,1,1,1,7,7,1,1,1,0,0,0,0,0},
    {1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

int stage4 [board_size][board_size] = {
    {1,1,1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

glm::vec3 eye; 
glm::vec3 target;

map < int , vector< int > > bridgeMap;    

float theta = 0.0f, 
        z_ordinate = 0.0f, 
        y_ordinate = 0.0f, 
        x_ordinate = 0.0f,
        camera_rotation_angle = 70.0f;

int level = 1, stageStart = 1, 
    prev_Bridge = 4, prevBridge[10], bridge[10],
    presentState = 0, futureState = 0, direction = 5,
    views = 4, moves = 0,
    left_button = 0, right_button = 0;
    
double oldMousex, oldMousey;

VAO *axes, 
        *cell, 
        *background;

GraphicalObject Block, 
                            Board[20][20];

void Background ( ) 
{   
    GLfloat vertex_buffer_data [ ] = {
        6,6,-6,
        -6,6,-6,
        -6,-6,-6,

        -6,-6,-6,
        6,-6,-6,
        6,6,-6,

        6,-6,-6,
        -6,-6,-6,
        -6,-6,6,

        -6,-6,6,
        6,-6,6,
        6,-6,-6,

        -6,-6,-6,
        -6,6,-6,
        -6,6,6,

        -6,6,6,
        -6,-6,6,
        -6,-6,-6,

        6,-6,-6,
        6,6,-6,
        6,6,6,

        6,6,6,
        6,-6,6,
        6,-6,-6,

        6,6,6,
        -6,6,6,
        -6,-6,6,

        -6,-6,6,
        6,-6,6,
        6,6,6
    };
    GLfloat color_buffer_data [ ] = {
        0,0,0,
        0,0,0,
        0.6,0.6,1,

        0.6,0.6,1,
        1, 0.6, 0.8,
        0,0,0,

        1, 0.6, 0.8,
        0.6,0.6,1,
        1,1,1,

        1,1,1,
        1,1,1,
        1, 0.6, 0.8,

        0.6,0.6,1,
        0,0,0,
        1,1,1,

        1,1,1,
        1,1,1,
        0.6,0.6,1,

        1, 0.6, 0.8,
        0,0,0,
        1,1,1,

        1,1,1,
        1,1,1,
        1, 0.6, 0.8,

        1,1,1,
        1,1,1,
        1,1,1,

        1,1,1,
        1,1,1,
        1,1,1

    };
    background = create3DObject ( GL_TRIANGLES, 30, vertex_buffer_data, color_buffer_data, GL_FILL );
}

void renderscore(double x, double y, double z, int score)
{
  double x_ordinate = x, y_ordinate = y, z_ordinate = z;
  glm::mat4 VP = ( perspective? Matrices.projectionP:Matrices.projectionO ) * Matrices.view;
  glm::mat4 MVP;
  glm::mat4 Irotator = glm::rotate(70.0f, glm::vec3(0,1,0));
  glm::mat4 Itranslator = glm::translate(glm::vec3(0.1f,0,0));
  glm::mat4 translator = glm::translate(glm::vec3(x_ordinate, y_ordinate, z_ordinate));
  if(score == 0){
    Matrices.model = glm::mat4(1.0f);
    Matrices.model *= translator*Irotator*Itranslator;
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(create3DObject(GL_TRIANGLES, 6, digitopbar, darkyellow, GL_FILL));
    draw3DObject(create3DObject(GL_TRIANGLES, 6, digitlefttopbar, darkyellow, GL_FILL));
    draw3DObject(create3DObject(GL_TRIANGLES, 6, digitleftbotbar, darkyellow, GL_FILL));
    draw3DObject(create3DObject(GL_TRIANGLES, 6, digitbotbar, darkyellow, GL_FILL));
    draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrightbotbar, darkyellow, GL_FILL));
    draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrighttopbar, darkyellow, GL_FILL));
    return;
  }
  int tmp = score;
  while(tmp != 0){
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translator = glm::translate(glm::vec3(x_ordinate, y_ordinate, 0));
    switch(tmp%10){
      case 0:
        Matrices.model *= translator*Irotator*Itranslator;
        MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitlefttopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitleftbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrightbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrighttopbar, darkyellow, GL_FILL));
        break;
      case 1:
        Matrices.model *= translator*Irotator*Itranslator;
        MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrightbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrighttopbar, darkyellow, GL_FILL));
          break;
      case 2:
        Matrices.model *= translator*Irotator*Itranslator;
        MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrighttopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitmidbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitleftbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitbotbar, darkyellow, GL_FILL));
          break;
      case 3:
        Matrices.model *= translator*Irotator*Itranslator;
        MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrightbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrighttopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitmidbar, darkyellow, GL_FILL));
        break;
      case 4:
        Matrices.model *= translator*Irotator*Itranslator;
        MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitlefttopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrightbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrighttopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitmidbar, darkyellow, GL_FILL));
        break;
      case 5:
        Matrices.model *= translator*Irotator*Itranslator;
        MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitlefttopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitmidbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrightbotbar, darkyellow, GL_FILL));
        break;
      case 6:
        Matrices.model *= translator*Irotator*Itranslator;
        MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitlefttopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitmidbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitleftbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrightbotbar, darkyellow, GL_FILL));
        break;
      case 7:
        Matrices.model *= translator*Irotator*Itranslator;
        MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrightbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrighttopbar, darkyellow, GL_FILL));
        break;
      case 8:
        Matrices.model *= translator*Irotator*Itranslator;
        MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitlefttopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitleftbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrightbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrighttopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitmidbar, darkyellow, GL_FILL));
        break;
      case 9:
        Matrices.model *= translator*Irotator*Itranslator;
        MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitlefttopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrightbotbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitrighttopbar, darkyellow, GL_FILL));
        draw3DObject(create3DObject(GL_TRIANGLES, 6, digitmidbar, darkyellow, GL_FILL));
        break;
    }
    tmp = tmp/10;
    x_ordinate -= 0.3;
  }
}

void keyboard ( GLFWwindow* window, int key, int scancode, int action, int mods )
{
    // Function is called first on GLFW_PRESS.

    if ( action == GLFW_RELEASE ) {
        switch ( key ) {
           default:
                break;
       }
   }
   else if ( action == GLFW_PRESS ) {
    switch ( key ) {
        case GLFW_KEY_V:
            views = ( views + 1 ) % 5;
            cout<<"2";
            break;

        case GLFW_KEY_P:
            if ( perspective == 1 )
                perspective = 0;
            else 
                perspective = 1;
            break;

         case GLFW_KEY_UP:  
            // cout<<last_1;
            if(last_1!=-1&&last_1!=2){
                
            }  
            else if ( ! stageStart ) {
                if ( presentState == 0 )
                    futureState = 1;
                else if ( presentState == 1 ) 
                    futureState = 0;
                else
                    futureState = 2;
                direction = 8;
                last=1;
                last_1=-1;
                moves++;
                system("mpg123 -n 30 -i -q movement.mp4 &");
            }
            break;
            
         case GLFW_KEY_DOWN:
        //  cout<<last_1;
            if(last_1!=-1&&last_1!=1){
                
            }  
            else if ( ! stageStart ) {
                if ( presentState == 0 )
                    futureState = 1;
                else if ( presentState == 1 ) 
                    futureState = 0;
                else if ( presentState == 2 )
                    futureState = 2;
                direction = 2;
                last =2;
                last_1=-1;
                moves++;
                system("mpg123 -n 30 -i -q movement.mp4 &");
            }
            break;
               
         case GLFW_KEY_LEFT:
        //  cout<<last_1;
            if(last_1!=-1&&last_1!=4){
                
            }  
            else
            if ( ! stageStart ) {
                if ( presentState == 0 )
                    futureState = 2;
                else if ( presentState == 1 ) 
                    futureState = 1;
                else if ( presentState == 2 )
                    futureState = 0;
                direction = 4;
                last = 3;
                last_1=-1;
                moves++;
                system("mpg123 -n 30 -i -q movement.mp4 &");
            }
            break;
        
         case GLFW_KEY_RIGHT:
        //  cout<<last_1;
            if(last_1!=-1&&last_1!=3){
                
            }  
            else if ( ! stageStart ) {
                if ( presentState == 0 )
                    futureState = 2;
                else if ( presentState == 1 ) 
                    futureState = 1;
                else if ( presentState == 2 )
                    futureState = 0;
                direction = 6;
                last = 4;
                last_1=-1;
                moves++;
                system("mpg123 -n 30 -i -q movement.mp4 &");
            }
            break;


        case GLFW_KEY_KP_1:
        l_on1=!l_on1;cout<<"1";
        break;
    case GLFW_KEY_KP_2:
        l_on2=!l_on2;
        break;
    case '3':
        l_on3=!l_on3;
        break;

    case '4':
        amb= !amb;
        break;
    case '5':
        spec =!spec;
        break;
    case '6':
        dif=!dif;
        break;
    
         case GLFW_KEY_ESCAPE:
            quit ( window );
             break;
       default:
            break;
        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar ( GLFWwindow* window, unsigned int key )
{
    switch (key) {
        case 'Q':
        case 'q':
            quit ( window );
            break;
        case '1':
        l_on1=!l_on1;cout<<"1";
        break;
    case '2':
        l_on2=!l_on2;
        break;
    case '3':
        l_on3=!l_on3;
        break;

    case '4':
        amb= !amb;
        break;
    case '5':
        spec =!spec;
        break;
    case '6':
        dif=!dif;
        break;
        default:
            break;
    }
}

void mouseButton ( GLFWwindow* window, int button, int action, int mods )
{
    if ( action == GLFW_PRESS ) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                left_button = 1;
                glfwGetCursorPos ( window, &oldMousex, &oldMousey );
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                right_button = 1;
                break;
            default:
                break;
        }
    }
    else {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                left_button = 0;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                right_button = 0;
                break;
            default:
                break;
        }
    }
}

void blockRotator ( )
{    
    if ( theta < 90 && direction != 5 )
        theta += 10;
    else
    {
        if ( presentState != futureState || direction != 5) {
            if ( presentState == 0 ){
                if ( direction == 8) 
                    Block.z_ordinate -= Block.height;
                else if ( direction == 2 ) 
                    Block.z_ordinate += Block.length;
                else if ( direction == 4 ) 
                    Block.x_ordinate -= Block.height;
                else 
                    Block.x_ordinate += Block.length;
            }
            else if ( presentState == 1 ) {
                if ( direction == 8) 
                    Block.z_ordinate -= Block.length;
                else if ( direction == 2 ) 
                    Block.z_ordinate += Block.height;
                else if ( direction == 4 ) 
                    Block.x_ordinate -= Block.length;
                else 
                    Block.x_ordinate += Block.length;
            }
            else {
                if ( direction == 8) 
                    Block.z_ordinate -= Block.length;
                else if ( direction == 2 ) 
                    Block.z_ordinate += Block.length;
                else if ( direction == 4 ) 
                    Block.x_ordinate -= Block.length;
                else 
                    Block.x_ordinate += Block.height;
            }
        }
        presentState = futureState;
        theta = 0;
        direction = 5;
    }

    if ( presentState == 0 ) {
        if ( direction == 6 ) {
            Block.Irotator ( );
            Block.Itranslator ( -Block.length, 0 , 0 );
            Block.rotator ( -theta, glm::vec3 ( 0, 0, 1) );
            Block.translator ( Block.x_ordinate+Block.length - 1, Block.y_ordinate, Block.z_ordinate - 1 );   
        }
        else if ( direction == 4 ) {
            Block.Irotator ( );
            Block.Itranslator ( );
            Block.rotator ( theta, glm::vec3 ( 0, 0, 1 ) ); 
            Block.translator ( Block.x_ordinate - 1, Block.y_ordinate, Block.z_ordinate - 1);
        }
        else if ( direction == 8 ) {
            Block.Irotator ( );
            Block.Itranslator ( );
            Block.rotator ( -theta, glm::vec3 ( 1, 0, 0 ) );
            Block.translator ( Block.x_ordinate - 1, Block.y_ordinate, Block.z_ordinate - 1);
        }
        else if ( direction == 2 ) {
            Block.Irotator ( );
            Block.Itranslator ( 0, 0, -Block.length );
            Block.rotator ( theta, glm::vec3 ( 1, 0, 0 ) );
            Block.translator ( Block.x_ordinate - 1, Block.y_ordinate, Block.z_ordinate + Block.length - 1);
        }
        else {
            Block.Irotator ( );
            Block.Itranslator ( );
            Block.rotator ( );
            Block.translator ( Block.x_ordinate - 1, Block.y_ordinate, Block.z_ordinate - 1 );
        }
    }
    else if ( presentState == 1 ) {
        if ( direction == 6 ) {
            Block.Irotator ( -90, glm::vec3 ( 1, 0, 0 ) );
            Block.Itranslator ( -Block.length, 0, 0 );
            Block.rotator ( -theta, glm::vec3 ( 0, 0, 1) );
            Block.translator ( Block.x_ordinate + Block.length - 1, Block.y_ordinate, Block.z_ordinate + Block.height - 1 );
        }
        else if ( direction == 4 ) {
            Block.Irotator ( -90, glm::vec3 ( 1, 0, 0 ) );
            Block.Itranslator ( );
            Block.rotator ( theta, glm::vec3 ( 0, 0, 1) );
            Block.translator ( Block.x_ordinate - 1, Block.y_ordinate, Block.z_ordinate + Block.height - 1);
        }
        else if ( direction == 8 ) {
            Block.Irotator ( -90, glm::vec3 ( 1, 0, 0 ) );
            Block.Itranslator ( 0, 0, Block.height );
            Block.rotator ( -theta, glm::vec3 (1, 0, 0 ) );
            Block.translator ( Block.x_ordinate - 1, Block.y_ordinate, Block.z_ordinate - 1);
        }
        else if ( direction == 2 ) {
            Block.Irotator ( -90, glm::vec3 ( 1, 0, 0 ) );
            Block.Itranslator ( );
            Block.rotator ( theta, glm::vec3 (1, 0, 0 ) );
            Block.translator ( Block.x_ordinate - 1, Block.y_ordinate, Block.z_ordinate + Block.height - 1);
        }
        else {
            Block.Irotator ( -90, glm::vec3 ( 1, 0, 0 ) );
            Block.Itranslator ( Block.x_ordinate - 1, Block.y_ordinate, Block.z_ordinate + Block.height - 1); 
            Block.rotator ( );
            Block.translator ( );
        }
    }
    else {
        if ( direction == 6 ) {
            Block.Irotator ( 90, glm::vec3 ( 0, 0, 1 ) );
            Block.Itranslator ( );
            Block.rotator ( -theta, glm::vec3 (0, 0, 1) );
            Block.translator ( Block.x_ordinate + Block.height - 1, Block.y_ordinate, Block.z_ordinate - 1);
        }
        else if ( direction == 4 ) {
            Block.Irotator ( 90, glm::vec3 ( 0, 0, 1 ) );
            Block.Itranslator ( Block.height, 0 , 0 );
            Block.rotator ( theta, glm::vec3 ( 0, 0, 1 ) );
            Block.translator ( Block.x_ordinate - 1, Block.y_ordinate, Block.z_ordinate - 1);
        }
        else if ( direction == 8 ) {
            Block.Irotator ( 90, glm::vec3 ( 0, 0, 1 ) );
            Block.Itranslator ( );
            Block.rotator ( -theta, glm::vec3 ( 1, 0, 0 ) );
            Block.translator ( Block.x_ordinate + Block.height - 1, Block.y_ordinate, Block.z_ordinate - 1);
        }
        else if ( direction == 2 ) {
            Block.Irotator ( 90, glm::vec3 ( 0, 0, 1 ) );
            Block.Itranslator ( 0, 0, -Block.length );
            Block.rotator ( theta, glm::vec3 ( 1, 0, 0 ) );
            Block.translator ( Block.x_ordinate + Block.height - 1, Block.y_ordinate, Block.z_ordinate + Block.length - 1);
        }
        else {
            Block.Irotator ( );
            Block.Itranslator ( );
            Block.rotator ( 90, glm::vec3 ( 0, 0, 1 ) );
            Block.translator ( Block.x_ordinate + Block.height - 1, Block.y_ordinate, Block.z_ordinate - 1 );
        }
    }
}

void bridgeConstruct ( )
{
    vector < int > v;
    
    for ( map < int, vector< int > >::iterator it = bridgeMap.begin ( ); it != bridgeMap.end ( ); it++ ) {
        vector < int > V = bridgeMap[it->first];
        for ( int i = 0; i < board_size; i++ ) {
            for ( int j = 0; j < board_size; j++ ) {
                if ( board[ i ][ j ] == it->first ) {  
                    for ( int i = 0; i < 4; i += 2 )
                        board[ V [ i ] ][ V [ i + 1 ] ] = 7;

                    bridge[ it->first ] = 0;
                    prevBridge[ it->first ] = 1;
                }
            }
        }
    }
    // Bridge 1
    v.push_back(3);
    v.push_back(4);
    v.push_back(3);
    v.push_back(5);
    bridgeMap.insert ( make_pair ( 4, v) );
    v.clear ( );

    // Bridge 2
    v.push_back(3);
    v.push_back(10);
    v.push_back(3);
    v.push_back(11);
    bridgeMap.insert ( make_pair ( 5, v ) );
    v.clear ( );
}

void levelup ( )
{
    level++;
    switch ( level ) {
        case 2:
            for ( int i = 0; i < board_size; i++ )
                for ( int j = 0; j < board_size; j++ )
                    board[ i ][ j ] = stage2[ i ][ j ];
            break;
        case 3:
            for ( int i = 0; i < board_size; i++ )
                for ( int j = 0; j < board_size; j++ )
                    board[ i ][ j ] = stage3[ i ][ j ];
            break;
        case 4:
            for ( int i = 0; i < board_size; i++ )
                for ( int j = 0; j < board_size; j++ )
                    board[ i ][ j ] = stage4[ i ][ j ];
            break;
        default:
            quit ( window );
            break;
    };

    z_ordinate = 0.0f;
    for ( int i = 0; i < board_size; i++ ) {
            VAO *cell;
            GraphicalObject temp;
             x_ordinate = 0.0f;
            for ( int j = 0; j < board_size; j++ ) {
                y_ordinate = rand ( ) % 2 - 6.0f;
                if ( board[ i ][ j ] == 1) {
                    if ( ( i + j ) % 2 == 0 ) {
                        cell = createCell ( 0.3f, 0.3f, -0.1f, Grey ,textureRepeat);
                        temp = GraphicalObject ( x_ordinate, y_ordinate, z_ordinate, 0.1f, 0.3f, 'r' );
                    }
                    else {
                        cell = createCell ( 0.3f, 0.3f, -0.1f, White,textureRepeat );
                        temp = GraphicalObject ( x_ordinate, y_ordinate, z_ordinate, 0.1f, 0.3f,  'g' );
                    }
                }
                else if ( board[ i ][ j ] == 3 ) {
                    if ( ( i + j ) % 2 == 0 ) {
                        cell = createCell ( 0.3f, 0.3f, -0.1f,  Orange,textureRepeat );
                        temp = GraphicalObject ( x_ordinate, y_ordinate, z_ordinate, 0.1f, 0.3f,  'b' );
                    }
                    else {
                        cell = createCell ( 0.3f, 0.3f, -0.1f,  Dorange ,textureRepeat );
                        temp = GraphicalObject ( x_ordinate, y_ordinate, z_ordinate, 0.1f, 0.3f,  'b' );   
                    }
                }
                else if(board[i][j]==6){
                    cell = createCell ( 0.3f, 0.3f, -0.1f,  Yellow,textureRepeat) ;
                    temp = GraphicalObject ( x_ordinate, y_ordinate, z_ordinate, 0.1f, 0.3f,  'y' ); 
                }
                else {
                    cell = createCell ( 0.3f, 0.3f, -0.1f,  Green,textureRepeat );
                    temp = GraphicalObject ( x_ordinate, y_ordinate, z_ordinate, 0.1f, 0.3f,  'b' );   
                }
                temp.object = cell;
                Board[ i ][ j ] = temp;
                x_ordinate += 0.3f;
            }
            z_ordinate += 0.3f;
        }
        bridgeConstruct ( );
}

void drawBoard ( )
{
    for ( int i = 0; i < board_size; i++ ) {
        for ( int j = 0; j < board_size; j++ ) {
            Board[ i ][ j ].translator ( Board[ i ][ j ].x_ordinate - 1,
                                                        Board[ i ][ j ].y_ordinate,
                                                        Board[ i ][ j ].z_ordinate - 1);   
            if ( board[ i ][ j ] != 0 && board[ i ][ j ] != 2 && board[ i ][ j ] != 7 && Board[ i ][ j ].y_ordinate > -4.0f)
                Board[ i ][ j ].render ( );
        }
    }
}

void fallBlocksBoards ( )
{
    if ( Block.y_ordinate > -6.0f ) {
            Block.y_ordinate -= 0.1f;
            Block.translator ( Block.x_ordinate - 1, Block.y_ordinate, Block.z_ordinate - 1 );   
            return;
        }
    for ( int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++ ) {
                  if ( Board[ i ][ j ].y_ordinate >= -5.0f && 
                        board[ i ][ j ] != 0 &&
                        board[ i ][ j ] != 2 ) {
                        Board[ i ][ j ].y_ordinate -= 1.0f;
                        return;
                    }
                }   
            }
    stageStart = 1;
}

void buildBlocksBoards ( )
{
    for ( int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++ ) {
            if ( Board[ i ][ j ].y_ordinate < -0.1f && 
                board[ i ][ j ] != 0 && 
                board[ i ][ j ] != 2 ) {
                Board[ i ][ j ].y_ordinate += 1.0f;
                return;
            }
        }
    }
    stageStart = 0;
}

int checkBlock ( )
{
    int i = ( Block.z_ordinate * 10 ) / 3;
    int j = ( Block.x_ordinate * 10 ) / 3;
    last_1=-1;
    if ( ( i < 0 || j < 0) || 
        ( presentState == 0 && ( board[ i ][ j ] == 0 || board[ i ][ j ] == 7 || board[ i ][ j ] == 3 ) ) ||
        ( presentState == 1 && ( board[ i ][ j ] == 0 || board[ i + 1 ][ j ] == 0 || board[ i + 1 ][ j ] == 7 || board[ i ][ j + 1 ] == 7) ) ||      
        ( presentState == 2 && ( board[ i ][ j ] == 0 || board[ i ][ j + 1 ] == 0 || board[ i ][ j ] == 7 || board[ i ][ j + 1 ] == 7) ) ) 
        return 1;

    if ( presentState == 0 && board[ i ][ j ] == 2) 
        return 2;
    if ( (presentState==0&& board[i][j]==6)||(presentState==1 && (board[i][j]==6||board[i+1][j]==6))||(presentState==2&&(board[i][j]==6||board[i][j+1]==6))) return 3;
    if ( ( presentState == 0 && board[ i ][ j ] >  3 ) ||
        ( presentState == 1 && ( board[ i ][ j ] > 3 || board[ i + 1][ j ] > 3 ) ) ||
        ( presentState == 2 && ( board[ i ][ j ] > 3 || board[ i ][ j + 1 ] > 3 ) ) ) {

        int a = i, b = j;
        if ( presentState == 0 ) {
            a = i;
            b = j;
        }
        else if ( presentState == 1 ) {
            if ( board[ i ][ j ] > 3 ) {
                a = i;
                b = j;
            }
            else {
                a = i + 1;
                b = j ;
            }
        }
        else {
            if ( board[ i ][ j ] > 3 ) {
                a = i;
                b = j;
            }
            else {
                a = i ;
                b = j + 1;
            }
        }

        vector < int > V = bridgeMap[ board[ a ][ b ] ] ;

        if ( bridge[ board[ a ][ b ] ] == 0 ) {
            for ( int k = 0; k < V.size ( ); k+=2 ) 
                board[ V[ k ] ][ V[ k + 1 ] ] = 1;
        
            prevBridge[ board[ a ][ b ] ] = 0;
        }
        else {
            for ( int k = 0; k < V.size ( ); k+=2 ) 
                board[ V[ k ] ][ V[ k + 1 ] ] = 7;
            
            prevBridge[ board[ a ][ b ] ] = 1;
        }
        prev_Bridge = board[ a ][ b ];
        return 0;
    }

    if ( prevBridge[ prev_Bridge] == 0 ) {
        bridge[ prev_Bridge ] = 1;
    }
    else {
        bridge[ prev_Bridge ] = 0;
    }
    return 0;
}

void Viewer ( )
{
    switch ( views ) {
            case 0:
                //Block
                perspective = 1;
                eye = glm::vec3 ( Block.x_ordinate - 1, 1, Block.z_ordinate - 1);
                target = glm::vec3 ( 5 , 0.1, 5 ) ;
                break;
            case 1:
                //Top
                perspective = 1;
                eye = glm::vec3 (Block.x_ordinate, 3, Block.z_ordinate);
                target = glm::vec3 ( Block.x_ordinate + 0.3, 0, Block.z_ordinate);
                break;
            case 2:
                //Follow
                perspective = 1;
                eye = glm::vec3 ( Block.x_ordinate - 0.6, Block.height + 1, Block.z_ordinate );
                target = glm::vec3 ( Block.x_ordinate + 0.6, 0.1, Block.z_ordinate + 0.6); 
                break;
            case 3:
                //Helicopter
                perspective = 0;
                eye = glm::vec3 ( 4*cos((float)camera_rotation_angle*M_PI/180.0f), 4,  4*sin((float)camera_rotation_angle*M_PI/180.0f) );
                target = glm::vec3 ( 0, 0, 0 );
                break;
            case 4:
                //Tower
                perspective = 0;
                eye = glm::vec3 ( 3*cos((float)camera_rotation_angle*M_PI/180.0f), 2,  3*sin((float)camera_rotation_angle*M_PI/180.0f) );
                target =  glm::vec3 ( 0, 0, 0 );
                break;
            default:
                //Nothing
                break;
        }

}
void reset ( )
{    
    for ( map < int, vector< int > >::iterator it = bridgeMap.begin ( ); it != bridgeMap.end ( ); it++ ) {
        vector < int > V = bridgeMap[it->first];
        for ( int i = 0; i < board_size; i++ ) {
            for ( int j = 0; j < board_size; j++ ) {
                if ( board[ i ][ j ] == it->first ) {  
                    for ( int k = 0; k < 4; k += 2 )
                        board[ V [ k ] ][ V [ k + 1 ] ] = 7;
                    bridge[ it->first ] = 0;
                    prevBridge[ it->first ] = 1;
                }
            }
        }
    }
    theta = 0.0f;
    direction = 5;
    presentState = futureState = 0;
    Block.y_ordinate = 6.0f;
    Block.x_ordinate = 0.0f;
    Block.z_ordinate = 0.0f;
    Block.Irotator( );
    Block.Itranslator( );
    Block.rotator ( );
    Block.translator ( Block.x_ordinate - 1, Block.y_ordinate, Block.z_ordinate - 1);
}

// Render the scene with openGL 
// Edit this function according to your assignment 
// Selects material to be shown on the screen
void material(){
	if(select_material == 0){
		glMaterialfv(GL_FRONT, GL_AMBIENT, brass_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, brass_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, brass_specular);
  		glMaterialf(GL_FRONT, GL_SHININESS, brass_shininess);
	}
	if(select_material == 1){
		glMaterialfv(GL_FRONT, GL_AMBIENT, emerald_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, emerald_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, emerald_specular);
  		glMaterialf(GL_FRONT, GL_SHININESS, emerald_shininess);
	}
	if(select_material == 2){
		glMaterialfv(GL_FRONT, GL_AMBIENT, red_plastic_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red_plastic_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, red_plastic_specular);
  		glMaterialf(GL_FRONT, GL_SHININESS, red_plastic_shininess);
	}
	if(select_material == 3){
		glMaterialfv(GL_FRONT, GL_AMBIENT, slate_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, slate_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, slate_specular);
  		glMaterialf(GL_FRONT, GL_SHININESS, slate_shininess);
	}
}


void draw ( GLFWwindow* window, float x, float y, float w, float h )
{


    material();	// selecting material

	GLfloat light_diffuse[] = {diffuse_intensity, diffuse_intensity, diffuse_intensity, 1.0}; 	// Varing Diffuse light intersity
	
	// Enabling Lights 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);


    // Eabling Texture Environment
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    int fbwidth, fbheight;
    glfwGetFramebufferSize ( window, &fbwidth, &fbheight );
    glViewport ( (int)(x*fbwidth), (int)(y*fbheight), (int)(w*fbwidth), (int)(h*fbheight) );
    double currentMousex;
    double currentMousey;
    if ( left_button == 1 ) {
        glfwGetCursorPos( window, &currentMousex, &currentMousey);
        camera_rotation_angle = currentMousex - oldMousex;
    }
    else {
        camera_rotation_angle = 70.0f;
    }
    // light();

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram ( programID );

    Viewer ( );

    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up ( 0, 1, 0 );
    Matrices.view = glm::lookAt ( eye, target, up ); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    //  Don't change unless you are sure!!
    glm::mat4 VP =  ( perspective ? Matrices.projectionP : Matrices.projectionO ) * Matrices.view;

    renderscore ( 0, 4, 0, level );
    renderscore ( 3, 2, 0, ( int ) glfwGetTime ( ) );
    renderscore ( -3, 1, 0, moves );

    glm::mat4 MVP;	
    
    Matrices.model = glm::mat4 (1.0f);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv (Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject (background);  

    if ( stageStart ) {
        buildBlocksBoards ( );
    }

    drawBoard ( );

    if ( ! stageStart && Block.y_ordinate > 0.1 ) {
         Block.y_ordinate -= 0.1f; 
    }

    blockRotator ( );

    Block.render ( );

    
    switch ( checkBlock ( ) ) {
        
        case 0:
            Block.render ( );
            break;
        
        case 1:
            if ( ! stageStart )
                fallBlocksBoards ( );        
            else 
                reset ( ); 
            break;
        
        case 2:
            if ( ! stageStart )
                fallBlocksBoards ( ); 
            else { 
                reset ( );
                levelup ( );
            }
            break;
        case 3:
            if(last==1){
                   last_1=1;
            }
            else if(last==2){    // Simulate pressing the 'A' key
                last_1=2;
            }
            else if(last == 3){    
                last_1=3;
            }
            else if(last == 4){    // Simulate pressing the 'A' key
                last_1=4;
            }
            
            break;
        
        default :
            break;   
    }
}

// Initialise glfw window, I/O callbacks and the renderer to use 
GLFWwindow* initGLFW ( int width, int height )
{
    GLFWwindow* window;        
    // window desciptor/handle
    const char *Game = "Bloxorz";
    glfwSetErrorCallback ( error_callback );
    if ( ! glfwInit ( ) ) {
        exit ( EXIT_FAILURE );
    }

    glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint ( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    window = glfwCreateWindow ( width, height, "Sample OpenGL 3.3 Application", NULL, NULL );

    if ( ! window ) {
       exit ( EXIT_FAILURE );
       glfwTerminate ( );
   }

   glfwMakeContextCurrent ( window );
   glfwSwapInterval ( 1 );
   glfwSetFramebufferSizeCallback ( window, reshapeWindow );
   glfwSetWindowSizeCallback ( window, reshapeWindow );
   glfwSetWindowCloseCallback (window, quit );
   glfwSetWindowTitle(window, Game);
    glfwSetKeyCallback ( window, keyboard );
    // general keyboard input
    glfwSetCharCallback(window, keyboardChar);  
    // simpler specific character handling
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(window, mouseButton);
     // mouse button clicks
    // glfwSetScrollCallback(window, mouseScroll);

    return window;
}
void load_texture(){
	// image = stbi_load("texture.jpg", &width, &height, &bpp, 3);
	unsigned int texture;
	glGenTextures(1, &texture);
	GLubyte *texImage = makeTexImage((char*)"color.png");
	if ( !texImage ) {
		printf("\nError reading image \n");
		return;
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImageWidth, texImageWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage);
	delete texImage;
}

// Initialize the OpenGL rendering properties 
// Add all the models to be created here 
void initGL(GLFWwindow* window, int width, int height) {
    // Enable lighting
    // light();
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0); // Enable light source 0

    // // Set light position
    // GLfloat light_position[] = { 0.0, 10.0, 0.0, 1.0 }; // Positional light (above the scene)
    // glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // // Set light color (Blue)
    // GLfloat light_color[] = { 0.0, 0.0, 1.0, 1.0 }; // Blue light
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);

    // // Set material properties
    // GLfloat material_ambient[] = { 0.3, 0.3, 0.3, 1.0 }; // Ambient color
    // GLfloat material_diffuse[] = { 0.8, 0.8, 0.8, 1.0 }; // Diffuse color
    // GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // Specular color
    // GLfloat material_shininess[] = { 100.0 }; // Shininess factor

    // glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    // glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    // glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    // glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    // Objects should be created before any other gl function and shaders 
    // Create the models
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glLightfv(GL_LIGHT0, GL_POSITION, front_left_light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, front_right_light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, back_left_light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, back_right_light_position);

	glEnable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);

	// gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    // 0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    // 0.0, 1.0, 0.);      /* up is in positive Y direction */

    Background();
    // drawAxes();

    // BLOCK
    x_ordinate = 0.0f;
    y_ordinate = rand() % 2 + 6.0f;
    z_ordinate = 0.0f;
    GraphicalObject temp = GraphicalObject(x_ordinate, y_ordinate, z_ordinate, 0.6f, 0.3f);
    temp.object = createCell(0.3f, 0.3f, 0.6f, Blue,textureRepeat );
    Block = temp;
    Block.translator(Block.x_ordinate - 1, Block.y_ordinate, Block.z_ordinate - 1);

    //BOARD
    for (int i = 0; i < board_size; i++)
        for (int j = 0; j < board_size; j++)
            board[i][j] = stage1[i][j];

    for (int i = 0; i < board_size; i++) {
        VAO* cell;
        GraphicalObject temp;
        x_ordinate = 0.0f;
        for (int j = 0; j < board_size; j++) {
            y_ordinate = rand() % 2 - 6.0f;
            if (board[i][j] == 1) {
                if ((i + j) % 2 == 0) {
                    cell = createCell(0.3f, 0.3f, -0.1f, Grey,textureRepeat );
                    temp = GraphicalObject(x_ordinate, y_ordinate, z_ordinate, 0.1f, 0.3f, 'r');
                }
                else {
                    cell = createCell(0.3f, 0.3f, -0.1f, White,textureRepeat );
                    temp = GraphicalObject(x_ordinate, y_ordinate, z_ordinate, 0.1f, 0.3f, 'g');
                }
            }
            else if (board[i][j] == 3) {
                if ((i + j) % 2 == 0) {
                    cell = createCell(0.3f, 0.3f, -0.1f, Orange,textureRepeat );
                    temp = GraphicalObject(x_ordinate, y_ordinate, z_ordinate, 0.1f, 0.3f, 'b');
                }
                else {
                    cell = createCell(0.3f, 0.3f, -0.1f, Dorange,textureRepeat );
                    temp = GraphicalObject(x_ordinate, y_ordinate, z_ordinate, 0.1f, 0.3f, 'b');
                }
            }
            else {
                cell = createCell(0.3f, 0.3f, -0.1f, Green,textureRepeat );
                temp = GraphicalObject(x_ordinate, y_ordinate, z_ordinate, 0.1f, 0.3f, 'b');
            }
            temp.object = cell;
            Board[i][j] = temp;
            x_ordinate += 0.3f;
        }
        z_ordinate += 0.3f;
    }
    bridgeConstruct();

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");
    reshapeWindow(window, width, height);
    // Background color of the scene
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f); // R, G, B, A
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Define the light position
GLfloat lightPos[] = { 5.0f, 0.0f, 0.0f, 1.0f }; // Light source from the side

// Function to handle resizing of the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Function to handle errors from GLFW
// void error_callback(int error, const char* description) {
//     std::cerr << "Error: " << description << std::endl;
// }


int main(int argc, char** argv) {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Error: Could not initialize GLFW.\n");
        return 1;
    }

    // Set OpenGL version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    GLFWwindow* window = initGLFW(1000, 1000);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Error: Failed to initialize GLEW.\n");
        glfwTerminate();
        return -1;
    }

    // Initialize OpenGL settings and objects
    load_texture();
    initGL(window, 1000, 1000);

    // Main rendering loop
    double last_update_time = glfwGetTime(), current_time;
    while (!glfwWindowShouldClose(window)) {
        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the scene
        draw(window, 0, 0, 1, 1);

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        // Poll for events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    // Close the display
    XCloseDisplay(display);

    return 0;
}