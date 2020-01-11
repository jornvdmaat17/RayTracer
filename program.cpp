#include <GL/glut.h>
#include "mesh/Mesh.h"
#include <vector>
#include "input/Mouse.h"
#include "render/Renderer.h"
#include "render/Image.h"
#include "math/Vec3D.h"
#include "render/RayTracer.h"
#include "input/SceneBuilder.h"

int width = 200;
int height = 200;

Renderer renderer;
DebugDraw debugDraw;

const float BackgroundColor[]={0.2,0.3,0};
void display();
void init();
void reshape(int w, int h);
void idle();

void keyboard(unsigned char key, int x, int y);

/*Launches the program*/
int main(int argc, char** argv){

    // Init glut
    glutInit(&argc, argv);

    // Set custom witdth and height
    if(argc == 4){
       width =  atoi(argv[2]);
       height = atoi(argv[3]); 
    }

    init();

    // Framebuffer options/layers used by the application
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

    // Position and size of the window
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(width,height);
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
    glutKeyboardFunc(keyboard);
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

void init(){

    renderer = Renderer();
    debugDraw = DebugDraw();
    
    std::vector<Mesh> meshes = getMeshInformation("data/scene.txt");
    for(int i = 0; i < meshes.size(); i++){
        renderer.addMesh(meshes[i]);
    }


    renderer.addString(ASCIIString(-1.4, 1.4, 1, 1, 1, "Raytracing Program"));

    renderer.computeLighting();
}

void display(void){

    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT); 

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();  // Set identity matrix as initial transformation matrix
    tbVisuTransform(); // Set transform according to camera position (as set by mouse movement)

    renderer.render();   
    debugDraw.drawAll();

    glutSwapBuffers();
}


void idle(){
	renderer.idle();    
}

void reshape(int w, int h){
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (40, (float) w / h, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 'l': {
            renderer.replaceLight(Vec3Df(1,1,1));
            return;
        }
        case 'L': {
            renderer.addLightAtCamPos(Vec3Df(1,1,1));            
            return;
        }
        case 'r': {
            renderer.resetLights();
			return;
        }
        case '=': {
            changeZoom(0.5f);
            return;
        }
        case '-': {
            changeZoom(-0.5f);
            return;
        }
        case 's': 
        case 'S': {
            debugDraw.clear();
            Image result(width, height);
            std::vector<Mesh> meshes = renderer.meshes;
            std::vector<Vec3Df> lights = renderer.lights;
            RayTracer rayTracer = RayTracer(width, height, meshes, lights, debugDraw);
            rayTracer.startRayTracing();
            rayTracer.writeToImage(result);
            result.writeImage("result.ppm");
            return;
        }
    }
}