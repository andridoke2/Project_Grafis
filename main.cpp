/*
  Materi : Praktikum Pemerograman Grafis 
  Modlue : 8
 */

#include <GL/glut.h>
#include <math.h>

float Cx = 0.0f, Cy = 2.5f, Cz = 0.0f;
float Lx = 0.0f, Ly = 2.5f, Lz = -20.0f;

float angle_depanBelakang = 0.0f;
float angle_depanBelakang2 = 0.0f;
float angle_samping = 0.0f;
float angle_samping2 = 0.0f;
float angle_vertikal = 0.0f;
float angle_vertikal2 = 0.0f;
float silinderAngle = 90.0f;

// float silinderAngleZ = 90.0f;

bool ori = true, silinder = false, kamera = false;

float toRadians(float angle)
{
  return angle * M_PI / 180;
}

class Vector {
  
  public:
  float x, y, z;

  void set_values (float startX, float startY, float startZ)
  {
    x = startX;
    y = startY;
    z = startZ;
  }

  void vectorRotation(Vector refs, float angle)
  {
    Vector temp = refs;
    float magnitude = sqrt(pow(temp.x, 2) + pow(temp.y, 2) + pow(temp.z, 2));

    temp.x = temp.x / magnitude;
    temp.y = temp.y / magnitude;
    temp.z = temp.z / magnitude;

    float dot_product = (x * temp.x)+(y * temp.y)+(z * temp.z);
    float cross_product_x = (y * temp.z) - (temp.z * z);
    float cross_product_y = -((x * temp.z) - (z * temp.x));
    float cross_product_z = (x * temp.y) - (y * temp.x);
    float last_factor_rodrigues = 1.0f - cos(toRadians(fmod(angle, 360.0f)));

    x = (x * cos(toRadians(fmod(angle, 360.0f)))) + (cross_product_x * sin(toRadians(fmod(angle, 360.0f)))) + (dot_product * last_factor_rodrigues * x);
    y = (y * cos(toRadians(fmod(angle, 360.0f)))) + (cross_product_y * sin(toRadians(fmod(angle, 360.0f)))) + (dot_product * last_factor_rodrigues * y);
    z = (z * cos(toRadians(fmod(angle, 360.0f)))) + (cross_product_z * sin(toRadians(fmod(angle, 360.0f)))) + (dot_product * last_factor_rodrigues * z);
  }
};

Vector depanBelakang, samping, vertikal;

void Tabung() 
{
  float amb[] = {0.34f, 0.34f, 0.34f, 1.0f};
  float diff[] = {0.41f, 0.41f, 0.41f, 1.0f};
  float spec[] = {0.11f, 0.11f, 0.11f, 1.0f};
  float shine = 200.0f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);
  float BODY_LENGTH = 5.0f;
  float BODY_RADIUS = 2.0f;
  int SLICES = 30;
  int STACKS = 30;
  GLUquadric *q= gluNewQuadric();
  gluCylinder(q, BODY_RADIUS, BODY_RADIUS, BODY_LENGTH, SLICES, STACKS);
  gluDisk(q, 0.0f, BODY_RADIUS, SLICES, STACKS);
  glTranslatef(0.0f, 0.0f, BODY_LENGTH);
  gluDisk(q, 0.0f, BODY_RADIUS, SLICES, STACKS);
}

static void BigBox()
{
  float amb[]={0.0f, 0.5f, 0.0f, 1.0f};
  float diff[]={0.0f, 0.5f, 0.0f, 1.0f};
  float spec[]={0.0f, 0.5f, 0.0f, 1.0f};
  float shine=0.0f;
  glMaterialfv(GL_FRONT,GL_AMBIENT,amb);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,diff);
  glMaterialfv(GL_FRONT,GL_SPECULAR,spec);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
  glBegin(GL_QUADS);
  glVertex3f(-10000.0f, 0.0f, -10000.0f);
  glVertex3f(10000.0f, 0.0f, -10000.0f);
  glVertex3f(10000.0f, 0.0f, 10000.0f);
  glVertex3f(-10000.0f, 0.0f, 10000.0f);
  glEnd();
}

/* 
  ini adalah function untuk melakukan pergerakan.
  magnitude adalah besarnya gerakan sedangkan direction digunakan untuk menentukan arah.
  gunakan -1 untuk arah berlawanan dengan vektor awal.
*/

void vectorMovement(Vector toMove, float magnitude, float direction)
{
  float speedX = toMove.x * magnitude * direction;
  float speedY = toMove.y * magnitude * direction;
  float speedZ= toMove.z * magnitude * direction;

  Cx += speedX;
  Cy += speedY;
  Cz += speedZ;
  Lx += speedX;
  Ly += speedY;
  Lz += speedZ; 
} 

void cameraRotation(Vector refer, double angle){

  float M = sqrt(pow(refer.x, 2) + pow(refer.y, 2) + pow(refer.z, 2));
  float Up_x1 = refer.x / M;
  float Up_y1 = refer.y / M;
  float Up_z1 = refer.z / M;

  float VLx = Lx - Cx;
  float VLy = Ly - Cy;
  float VLz = Lz - Cz;

  float dot_product = (VLx * Up_x1) + (VLy * Up_y1) + (VLz * Up_z1);
  float cross_product_x = (Up_y1 * VLz) - (VLy * Up_z1);
  float cross_product_y = -((Up_x1 * VLz) - (Up_z1 * VLx));
  float cross_product_z = (Up_x1 * VLy) - (Up_y1 * VLx);

  float last_factor_rodrigues = 1.0f - cos(toRadians(angle));

  float Lx1 = (VLx * cos(toRadians(angle))) + (cross_product_x * sin(toRadians(angle))) + (dot_product * last_factor_rodrigues * VLx);
  float Ly1 = (VLy * cos(toRadians(angle))) + (cross_product_y * sin(toRadians(angle))) + (dot_product * last_factor_rodrigues * VLy);
  float Lz1 = (VLz * cos(toRadians(angle))) + (cross_product_z * sin(toRadians(angle))) + (dot_product * last_factor_rodrigues * VLz);

  Lx = Lx1+Cx;
  Ly = Ly1+Cy;
  Lz = Lz1+Cz; 
} 

void initGL()
{
  depanBelakang.set_values(0.0f, 0.0f, -1.0f);
  samping.set_values(1.0f, 0.0f, 0.0f);
  vertikal.set_values(0.0f, 1.0f, 0.0f);

  float ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
  float diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
  float position[] = {1.0f, 1.0f, 1.0f, 0.0f};

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
  glShadeModel(GL_SMOOTH);
}

void reshape(GLsizei width, GLsizei height)
{
  if (height == 0) height = 1;
  GLfloat aspect = (GLfloat)width / (GLfloat)height;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, aspect, 1.0f, 20.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void keyControl(int k, int x, int y) 
{
  switch(k) {
    case GLUT_KEY_UP: //arrow up
      vectorMovement(vertikal, 2.0f, 1.0f);
      break;
    case GLUT_KEY_DOWN: //arrow down
      vectorMovement(vertikal, 2.0f, -1.0f);
      break;
    case GLUT_KEY_LEFT: //arrow left
      angle_depanBelakang += 2.0f;
      samping.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
      vertikal.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
      //cameraRotation(vertikal, angle_samping-angle_samping2);
      angle_depanBelakang2 = angle_depanBelakang;
      break;
    case GLUT_KEY_RIGHT: //arrow right
      angle_depanBelakang -= 2.0f;
      samping.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
      vertikal.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
      //cameraRotation(vertikal, angle_samping-angle_samping2);
      angle_depanBelakang2 = angle_depanBelakang;
      break;
  }
}

void keyFunction(unsigned char key, int x, int y)
{

/*
  Agar fungsi ini bekerja, pastikan CapsLock menyala,
  kecuali tombol-tombol khusus seperti Spasi, dll 
*/

switch(key){
  case 87: // W
    vectorMovement(samping, 2.0f, 1.0f);
    break;
  case 83: // S
    vectorMovement(samping, 2.0f, -1.0f);
    break;
  case 68: // D
    vectorMovement(depanBelakang, 2.0f, 1.0f);
    break;
  case 65: // A
    vectorMovement(depanBelakang, 2.0f, -1.0f);
    break;
  case 32: // Spasi
    if (silinder) {
      silinder = false;
    } else {
      silinder = true;
    }
      break;
  case 13: // Enter
    if (kamera) {
      kamera = false;
    } else {
      kamera = true;
    }
    break;
  case 74: // J
    angle_vertikal += 2.0f;
    samping.vectorRotation(vertikal, angle_vertikal - angle_vertikal2);
    //memutar vector sumbu z terhadap x (target, patokan)
    depanBelakang.vectorRotation(vertikal, angle_vertikal - angle_vertikal2);
    cameraRotation(vertikal, angle_vertikal - angle_vertikal2); // look at
    angle_vertikal2 = angle_vertikal;
    break;
  case 76: // L
    angle_vertikal -= 2.0f;
    samping.vectorRotation(vertikal, angle_vertikal - angle_vertikal2);
    cameraRotation(vertikal, angle_vertikal - angle_vertikal2);
    angle_vertikal2 = angle_vertikal;
    break;
  case 73: // I
    angle_samping -= 2.0f;
    //vertikal.vectorRotation(samping, angle_samping - angle_samping2);
    depanBelakang.vectorRotation(vertikal, angle_vertikal - angle_vertikal2);
    depanBelakang.vectorRotation(samping, angle_samping - angle_samping2);
    cameraRotation(samping, angle_samping - angle_samping2);
    angle_samping2 = angle_samping;
    break;
  case 75: // K
    angle_samping += 2.0f;
    //vertikal.vectorRotation(samping, angle_samping-angle_samping2);
    depanBelakang.vectorRotation(samping, angle_samping - angle_samping2);
    cameraRotation(samping, angle_samping - angle_samping2);
    angle_samping2 = angle_samping;
    break;
  }
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset the current matrix to the "identity"
  glLoadIdentity();
  
  // Move the "drawing cursor" around
  gluLookAt(Cx, Cy, Cz,
            Lx, Ly, Lz,
            vertikal.x, vertikal.y, vertikal.z);
  
  glPushMatrix();
  glTranslatef(0.0f, 5.0f, -15.0f);
  glRotatef(silinderAngle, 1.0f, 0.0f, 0.0f);

  // glRotatef(silinderAngleZ, 0.0f, 1.0f, 0.0f);
  
  Tabung();
  glPopMatrix();
  
  glPushMatrix();
  BigBox();
  glPopMatrix();

  if (silinder) {
    silinderAngle += 2.0f;
    // silinderAngleZ += 2.0f;
  }

  if (kamera) {
    keyFunction('J', 0, 0);
  }
  
  glFlush();
  glutSwapBuffers();
}

void timer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(15, timer, 0);
}

int main(int argc, char **argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(50, 50);
  glutCreateWindow("Pertemuan 8");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  initGL();
  glutTimerFunc(0, timer, 0);
  glutSpecialFunc(keyControl);
  glutKeyboardFunc(keyFunction);
  glutMainLoop();
  return EXIT_SUCCESS;
}