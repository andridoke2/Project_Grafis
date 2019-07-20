#include <GL/glut.h>
#include <math.h>
#define PI 3.14159265

int oldMouseX, oldMouseY;
float view_rotasi_x = 20.0f, view_rotasi_y = 30.0f;

GLfloat sudut = 1;

float Cx = 0.0f, Cy = 2.5f, Cz = 0.0f;
float Lx = 0.0f, Ly = 2.5f, Lz = -20.0f;

float angle_depanBelakang = 0.0f;
float angle_depanBelakang2 = 0.0f;
float angle_samping = 0.0f;
float angle_samping2 = 0.0f;
float angle_vertikal = 0.0f;
float angle_vertikal2 = 0.0f;
float silinderAngle = 90.0f;

bool ori = true, silinder = false, kamera = false;

float toRadians(float angle){
  return angle * M_PI / 180;
}

class Vector {
  
  public:
  float x, y, z;

  void set_values (float startX, float startY, float startZ){
    x = startX;
    y = startY;
    z = startZ;
  }

  void vectorRotation(Vector refs, float angle){
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

float degreeToRadian(float degree){
  return (degree * PI) / 180.0;
}

void segiBeraturan(float r, int segi, float sudutAwal) {
  float besarSudutRad = degreeToRadian(360.0 / segi);
  float sudutAwalRad = degreeToRadian(sudutAwal);
  glBegin(GL_LINES);
  for (int titikKe = 0; titikKe < segi; titikKe ++) {
    float sudut1 = besarSudutRad * titikKe + sudutAwalRad;
    float sudut2 = besarSudutRad * (titikKe + 1) + sudutAwalRad;
    float x1 = cos(sudut1) * r;
    float x2 = cos(sudut2) * r;
    float y1 = sin(sudut1) * r;
    float y2 = sin(sudut2) * r;
    glVertex3f(x1, y1, 0);
    glVertex3f(x2, y2, 0);
  }
  glEnd();
}

void Matahari(){
  float amb[] = {1.34f, 1.34f, 0.34f, 0.0f};
  float diff[] = {1.41f, 1.41f, 0.41f, 0.0f};
  float spec[] = {0.11f, 0.11f, 0.11f, 0.0f};
  float shine = 50.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  float BODY_RADIUS = 0.6f; // ukuran gambar atau besar gambar
  int SLICES = 50;
  int STACKS = 50;
  GLUquadric *q = gluNewQuadric();
  gluSphere(q, BODY_RADIUS, SLICES, STACKS);
}

void Merkurius(){
  float amb[] = {0.40, 0.40, 0.40f, 0.0f};
  float diff[] = {1.41f, 0.41f, 0.41f, 0.0f};
  float spec[] = {0.0f, 0.0f, 0.0f, 0.0f};
  float shine = 50.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  float BODY_RADIUS = 0.3f; // ukuran gambar atau besar gambar
  int SLICES = 50;
  int STACKS = 50;
  GLUquadric *q = gluNewQuadric();
  gluSphere(q, BODY_RADIUS, SLICES, STACKS);
}

void Venus(){
  float amb[] = {0.34f, 0.34f, 0.34f, 0.0f};
  float diff[] = {1.41f, 0.41f, 0.41f, 0.0f};
  float spec[] = {0.11f, 0.11f, 0.11f, 0.0f};
  float shine = 50.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  float BODY_RADIUS = 0.4f; // ukuran gambar atau besar gambar
  int SLICES = 50;
  int STACKS = 50;
  GLUquadric *q = gluNewQuadric();
  gluSphere(q, BODY_RADIUS, SLICES, STACKS);
}

void Bumi(){
  float amb[] = {0.0f, 0.45f, 0.45f, 1.40f};
  float diff[] = {0.30f, 0.30f, 0.30f, 0.30f};
  float spec[] = {0.0f, 0.0f, 0.0f, 0.0f};
  float shine = 50.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  float BODY_RADIUS = 0.5; // ukuran gambar atau besar gambar
  int SLICES = 50;
  int STACKS = 50;
  GLUquadric *q = gluNewQuadric();
  gluSphere(q, BODY_RADIUS, SLICES, STACKS);
}

void Mars(){
  float amb[] = {1.34f, 1.34f, 0.34f, 0.0f};
  float diff[] = {1.41f, 1.41f, 0.41f, 0.0f};
  float spec[] = {0.11f, 0.11f, 0.11f, 0.0f};
  float shine = 50.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  float BODY_RADIUS = 0.3f; // ukuran gambar atau besar gambar
  int SLICES = 50;
  int STACKS = 50;
  GLUquadric *q = gluNewQuadric();
  gluSphere(q, BODY_RADIUS, SLICES, STACKS);
}

void Yupiter(){
  float amb[] = {0.45f, 0.34f, 0.34f, 0.40f};
  float diff[] = {0.41f, 0.41f, 0.41f, 0.0f};
  float spec[] = {0.11f, 0.11f, 0.11f, 0.0f};
  float shine = 50.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  float BODY_RADIUS = 0.8; // ukuran gambar atau besar gambar
  int SLICES = 50;
  int STACKS = 50;
  GLUquadric *q = gluNewQuadric();
  gluSphere(q, BODY_RADIUS, SLICES, STACKS);
}

void Saturnus(){
  float amb[] = {0.34f, 0.34f, 0.34f, 0.0f};
  float diff[] = {0.41f, 1.41f, 1.41f, 1.0f};
  float spec[] = {0.11f, 0.11f, 0.11f, 0.0f};
  float shine = 50.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  float BODY_RADIUS = 0.6; // ukuran gambar atau besar gambar
  int SLICES = 50;
  int STACKS = 50;
  GLUquadric *q = gluNewQuadric();
  gluSphere(q, BODY_RADIUS, SLICES, STACKS);
}

void Uranus(){
  float amb[] = {0.34f, 0.34f, 0.34f, 0.0f};
  float diff[] = {1.0f, 1.0f, 1.0f, 1.0f};
  float spec[] = {0.11f, 0.11f, 0.11f, 0.0f};
  float shine = 50.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  float BODY_RADIUS = 1.0; // ukuran gambar atau besar gambar
  int SLICES = 50;
  int STACKS = 50;
  GLUquadric *q = gluNewQuadric();
  gluSphere(q, BODY_RADIUS, SLICES, STACKS);
}

void Neptunus(){
  float amb[] = {0.34f, 0.34f, 0.34f, 0.0f};
  float diff[] = {0.61f, 1.61f, 1.61f, 1.60f};
  float spec[] = {0.11f, 0.11f, 0.11f, 0.0f};
  float shine = 50.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  float BODY_RADIUS = 1.0; // ukuran gambar atau besar gambar
  int SLICES = 50;
  int STACKS = 50;
  GLUquadric *q = gluNewQuadric();
  gluSphere(q, BODY_RADIUS, SLICES, STACKS);
}

void Pluto(){
  float amb[] = {0.0f, 0.0f, 0.0f, 0.0f};
  float diff[] = {0.57f, 0.80f, 0.24f, 0.0f};
  float spec[] = {0.20f, 0.30f, 0.40f, 0.50f};
  float shine = 50.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  float BODY_RADIUS = 0.3; // ukuran gambar atau besar gambar
  int SLICES = 50;
  int STACKS = 50;
  GLUquadric *q = gluNewQuadric();
  gluSphere(q, BODY_RADIUS, SLICES, STACKS);
}

void Bulan(){
  float amb[] = {0.20f, 0.20f, 0.35f, 0.0f};
  float diff[] = {0.45f, 0.45f, 0.45f, 0.0f};
  float spec[] = {0.0f, 0.0f, 0.0f, 0.0f};
  float shine = 50.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  float BODY_RADIUS = 0.2; // ukuran gambar atau besar gambar
  int SLICES = 50;
  int STACKS = 50;
  GLUquadric *q = gluNewQuadric();
  gluSphere(q, BODY_RADIUS, SLICES, STACKS);
}

void BintangKecil(){
  float amb[] = {1.0f, 1.0f, 1.0f, 1.0f};
  float diff[] = {1.0f, 1.0f, 1.0f, 1.0f};
  float spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
  float shine = 50.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  float BODY_RADIUS = 0.03; // ukuran gambar atau besar gambar
  int SLICES = 50;
  int STACKS = 50;
  GLUquadric *q = gluNewQuadric();
  gluSphere(q, BODY_RADIUS, SLICES, STACKS);
}

/* 
  ini adalah function untuk melakukan pergerakan.
  magnitude adalah besarnya gerakan sedangkan direction digunakan untuk menentukan arah.
  gunakan -1 untuk arah berlawanan dengan vektor awal.
*/

void vectorMovement(Vector toMove, float magnitude, float direction){
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

  Lx = Lx1 + Cx;
  Ly = Ly1 + Cy;
  Lz = Lz1 + Cz; 
} 

void initGL(){
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
  glClearColor(0.45f, 0.45f, 0.45f, 1.0f); // Warna Barckground / latar belakang
  glShadeModel(GL_SMOOTH);
}

void reshape(GLsizei width, GLsizei height){
  if (height == 0) height = 1;
  GLfloat aspect = (GLfloat)width / (GLfloat)height;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, aspect, 1.0f, 20.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void keyControl(int k, int x, int y){
  switch(k) {
    case GLUT_KEY_UP: // PANAH ATAS
      vectorMovement(vertikal, 2.0f, 1.0f);
      break;
    case GLUT_KEY_DOWN: // PANAH BAWAH
      vectorMovement(vertikal, 2.0f, -1.0f);
      break;
    case GLUT_KEY_LEFT: // PANAH KIRI
      angle_depanBelakang += 2.0f;
      samping.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
      vertikal.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
      angle_depanBelakang2 = angle_depanBelakang;
      break;
    case GLUT_KEY_RIGHT: // PANAH KANAN
      angle_depanBelakang -= 2.0f;
      samping.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
      vertikal.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
      angle_depanBelakang2 = angle_depanBelakang;
      break;
  }
}

void keyFunction(unsigned char key, int x, int y){

  /*
    Agar fungsi ini bekerja, pastikan CapsLock menyala,
    kecuali tombol-tombol khusus seperti Spasi, dll.
  */

  switch(key){
    case 65: // W
      vectorMovement(samping, 2.0f, 1.0f);
      break;
    case 68: // S
      vectorMovement(samping, 2.0f, -1.0f);
      break;
    case 83: // D
      vectorMovement(depanBelakang, 2.0f, 1.0f);
      break;
    case 87: // A
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
      depanBelakang.vectorRotation(vertikal, angle_vertikal - angle_vertikal2);
      depanBelakang.vectorRotation(samping, angle_samping - angle_samping2);
      cameraRotation(samping, angle_samping - angle_samping2);
      angle_samping2 = angle_samping;
      break;
    case 75: // K
      angle_samping += 2.0f;
      depanBelakang.vectorRotation(samping, angle_samping - angle_samping2);
      cameraRotation(samping, angle_samping - angle_samping2);
      angle_samping2 = angle_samping;
      break;
  }
}

void mouseControl(int button, int state, int x, int y){
  switch (button) {
    case 27: // "esc" on keyboard
      exit(0);
      break;
    case 'a': // "a" on keyboard
      state = state + 2.0f;
      glutPostRedisplay();
      break;
    case 'z':
      state = state - 2.0f;
      glutPostRedisplay();
      break;
  }
}

void mouseMotion(int x, int y){
	int getX = x;
	int getY = y;
	float thetaY = 360.0f * (getX - oldMouseX) / 640;
	float thetaX = 360.0f * (getY - oldMouseY) / 480;
	oldMouseX = getX;
	oldMouseY = getY;
	silinderAngle += thetaX;
	silinderAngle += thetaY;
  view_rotasi_x += thetaX;
  view_rotasi_y += thetaY;
}

float setSudut(float sudut){
  return sudut * (22 / 7) / 180;
}

void displayPlanet(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset the current matrix to the "identity"
  glLoadIdentity();

  float newSudut = setSudut(sudut * 10);

  // Move the "drawing cursor" around
  gluLookAt(
    Cx, Cy, Cz,
    Lx, Ly, Lz,
    vertikal.x, vertikal.y, vertikal.z
  );

  // Matahari
  glPushMatrix();
  // glRotatef(-sudut, 0, 0, -1);
  glTranslatef(0.0f, 2.5f, -10.0f);
  glRotatef(view_rotasi_x, 1, 0, 0);
  glRotatef(view_rotasi_y, 0, 1, 0);
  glRotatef(silinderAngle, 1.0f, 0.0f, 0.0f);
  Matahari();
  glPopMatrix();

  // Merkurius
  glPushMatrix();
  // glRotatef(-sudut, 0, 0, 1);
  glTranslatef(-1.0f, 0.05f, -20.0f);
  glRotatef(view_rotasi_x, 1, 0, 0);
  glRotatef(view_rotasi_y, 0, 1, 0);
  glRotatef(silinderAngle, 1.0f, 0.0f, 0.0f);
  Merkurius();
  glPopMatrix();

  // Venus
  glPushMatrix();
  // glRotatef(sudut, 0, 0, 1);
  glTranslatef(-4.0f, 0.02f, -15.0f);
  glRotatef(view_rotasi_x, 1, 0, 0);
  glRotatef(view_rotasi_y, 0, 1, 0);
  glRotatef(silinderAngle, 1.0f, 0.0f, 0.0f);
  Venus();
  glPopMatrix();

  // Bumi
  glPushMatrix();
  // glRotatef(-sudut, 0, 0, 1);
  glTranslatef(-0.10f, 0.10f, -11.0f);
  // glRotatef(view_rotasi_x, 1, 0, 0);
  // glRotatef(view_rotasi_y, 0, 1, 0);
  // glRotatef(silinderAngle, 0.0f, 0.0f, 1.0f);
  segiBeraturan(0.9, 150, 90);
  Bumi();
  glPopMatrix();

  // Bulan
  glPushMatrix();
  glRotatef(sudut, 0, 0, 1);
  glTranslatef(-0.7f, -0.5f, -11.0f);
  // glRotatef(view_rotasi_x, 1, 0, 0);
  // glRotatef(view_rotasi_y, 0, 1, 0);
  // glRotatef(silinderAngle, 0.0f, 0.0f, -1.0f);
  Bulan();
  glPopMatrix();

  // Mars
  glPushMatrix();
  // glRotatef(sudut, 0, 0, 1);
  glTranslatef(3.0f, 3.0f, -11.0f);
  glRotatef(view_rotasi_x, 1, 0, 0);
  glRotatef(view_rotasi_y, 0, 1, 0);
  glRotatef(silinderAngle, 1.0f, 0.0f, 0.0f);
  Mars();
  segiBeraturan(0.5, 150, 90);
  glPopMatrix();

  // Yupiter
  glPushMatrix();
  // glRotatef(-sudut, 0, 0, 1);
  glTranslatef(-2.0f, 6.0f, -14.0f);
  glRotatef(view_rotasi_x, 1, 0, 0);
  glRotatef(view_rotasi_y, 0, 1, 0);
  glRotatef(silinderAngle, 1.0f, 0.0f, 0.0f);
  Yupiter();
  segiBeraturan(1.4, 150, 90);
  glPopMatrix();

  // Saturnus
  glPushMatrix();
  // glRotatef(-sudut, 0, 0, 1);
  glTranslatef(-4.0f, -2.0f, -14.0f);
  glRotatef(view_rotasi_x, 1, 0, 0);
  glRotatef(view_rotasi_y, 0, 1, 0);
  glRotatef(silinderAngle, 0.0f, 0.0f, -1.0f);
  Saturnus();
  glPopMatrix();

  // Uranus
  glPushMatrix();
  // glRotatef(-sudut, 0, 0, 1);
  glTranslatef(-6.0f, 2.0f, -14.0f);
  glRotatef(view_rotasi_x, 1, 0, 0);
  glRotatef(view_rotasi_y, 0, 1, 0);
  glRotatef(silinderAngle, 0.0f, 0.0f, -1.0f);
  Uranus();
  glPopMatrix();

  // Neptunus
  glPushMatrix();
  // glRotatef(-sudut, 0, 0, 1);
  glTranslatef(7.0f, -2.0f, -13.0f);
  glRotatef(view_rotasi_x, 1, 0, 0);
  glRotatef(view_rotasi_y, 0, 1, 0);
  glRotatef(silinderAngle, 0.0f, 0.0f, -1.0f);
  Neptunus();
  glPopMatrix();

  // Pluto
  glPushMatrix();
  // glRotatef(-sudut, 0, 0, 1);
  glTranslatef(-8.0f, -2.0f, -12.0f);
  glRotatef(view_rotasi_x, 1, 0, 0);
  glRotatef(view_rotasi_y, 0, 1, 0);
  glRotatef(silinderAngle, 0.0f, 0.0f, -1.0f);
  Pluto();
  glPopMatrix();

  if (silinder){
    silinderAngle += 2.0f;
    angle_depanBelakang -= 0.7f;
    samping.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
    vertikal.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
    angle_depanBelakang2 = angle_depanBelakang;
  }

  if (kamera){
    keyFunction('J', 0, 0);
  }

  sudut++;

  glFlush();
  glutSwapBuffers();
}

void timer(int value){
  glutPostRedisplay();
  glutTimerFunc(15, timer, 0);
}

int main(int argc, char **argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(2000, 700);
  glutInitWindowPosition(50, 50);
  glutCreateWindow("Project Grafis");
  glutDisplayFunc(displayPlanet);
  glutReshapeFunc(reshape);
  initGL();
  glutSpecialFunc(keyControl);
  glutKeyboardFunc(keyFunction);
  glutMouseFunc(mouseControl);
	glutMotionFunc(mouseMotion);
  glutTimerFunc(0, timer, 0);
  glutMainLoop();
  return EXIT_SUCCESS;
}