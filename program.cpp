#include <GL/glut.h>
#include "mesh/Mesh.h"
#include <vector>
#include "input/Mouse.h"

#define WIDTH 800
#define HEIGHT 800

const float BackgroundColor[]={0.2,0.2,0};
void display();
void draw();
void init();
void reshape(int w, int h);
void idle();
void initStudentVariables();

std::vector<Mesh> meshes;

Vec3Df CamPos = Vec3Df(0.0f,0.0f,-4.0f);

std::vector<Vec3Df> LightPos;
std::vector<Vec3Df> LightColor;
int SelectedLight=0;

bool DiffuseLighting=true;
bool PhongSpecularLighting=false;
bool BlinnPhongSpecularLighting=false;
bool ToonLightingDiffuse=false;
bool ToonLightingSpecular=false;

float ToonSpecularThreshold=0.49;//threshold for specularity

void computeLightingForMeshes();

int main(int argc, char** argv){

    glutInit(&argc, argv);

    // TODO
    init();

    // Framebuffer options/layers used by the application
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

    // Position and size of the window
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutCreateWindow(argv[1]);	

    // Transform matrix initialization
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-4);

    tbInitTransform();     // Viewpoint initialization

    glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    // Callback assignment
    // TODO
    glutReshapeFunc(reshape);
    // glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMouseFunc(tbMouseFunc);    // Set mouse click callback
    glutMotionFunc(tbMotionFunc);  // Set mouse move callback
    glutIdleFunc(idle);

    // Drawing mode options
    glEnable( GL_DEPTH_TEST );            // Enable depth test
    glShadeModel(GL_SMOOTH); 

    // Clear everything
    glClearColor (BackgroundColor[0],BackgroundColor[1], BackgroundColor[2], 0.0);
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT); 
    
    glutMainLoop();

    return 0;
}

void display(void){

    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT); 

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();  // Set identity matrix as initial transformation matrix
    tbVisuTransform(); // Set transform according to camera position (as set by mouse movement)

    draw();    

    glutSwapBuffers();
}

void draw(void){

    glPointSize(10);
	glBegin(GL_POINTS);
	//LightColor
	glColor3f(1,0,0);	
	
	for (unsigned int i=0; i<LightPos.size();++i)	
	{	
		glVertex3f(LightPos[i][0],LightPos[i][1],LightPos[i][2]);
	}
	glEnd();
	
	glPointSize(40);
	glColor3f(1,1,0);	
	glBegin(GL_POINTS);
	glVertex3f(LightPos[SelectedLight][0],LightPos[SelectedLight][1],LightPos[SelectedLight][2]);
	glEnd();

    for(int i = 0; i < meshes.size(); i++){
        meshes[i].drawWithLight();
    }
}

void init(){
    
    Cube c = Cube(Vec3Df(0,0,0), 1);
    meshes.push_back(c);
	
    LightPos.push_back(Vec3Df(0,1,1));
	LightColor.push_back(Vec3Df(1,1,1));
	
	computeLightingForMeshes();
}

void idle()
{
	CamPos=getCameraPosition();

	computeLightingForMeshes();

	glutPostRedisplay();
}

void reshape(int w, int h){
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (40, (float) w / h, 1, 10);
    glMatrixMode(GL_MODELVIEW);
}

void computeLightingForMeshes(){
	for(int i = 0; i < meshes.size(); i++){
		meshes[i].computeLighting(LightPos, CamPos);
	}
}