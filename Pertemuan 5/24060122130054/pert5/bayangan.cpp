#include <math.h>
#include <stdio.h>
#include <GL/glut.h>

double rx = 0.0;
double ry = 0.0;

float l[] = { 0.0, 80.0, 0.0 }; // Koordinat sumber cahaya
float n[] = { 0.0, -40.0, 0.0 };
float e[] = { 0.0, -60.0, 0.0 };

// Deklarasi fungsi
void help();
void draw();
void glShadowProjection(float* l, float* e, float* n);
void render();
void keypress(unsigned char c, int a, int b);
void idle();
void resize(int w, int h);

// Fungsi untuk menampilkan bantuan
void help() {
    printf("Proyeksi contoh bayangan sebuah obyek teapot\n");
}

// Fungsi untuk menggambar objek
void draw() {
    glutSolidSphere(40, 50, 50);
}

// Fungsi untuk membuat proyeksi bayangan
void glShadowProjection(float* l, float* e, float* n) {
    float d, c;
    float mat[16];
    d = n[0] * l[0] + n[1] * l[1] + n[2] * l[2];
    c = e[0] * n[0] + e[1] * n[1] + e[2] * n[2] - d;
    mat[0] = l[0] * n[0] + c;
    mat[4] = n[1] * l[0];
    mat[8] = n[2] * l[0];
    mat[12] = -l[0] * c - l[0] * d;
    mat[1] = n[0] * l[1];
    mat[5] = l[1] * n[1] + c;
    mat[9] = n[2] * l[1];
    mat[13] = -l[1] * c - l[1] * d;
    mat[2] = n[0] * l[2];
    mat[6] = n[1] * l[2];
    mat[10] = l[2] * n[2] + c;
    mat[14] = -l[2] * c - l[2] * d;
    mat[3] = n[0];
    mat[7] = n[1];
    mat[11] = n[2];
    mat[15] = -d;
    glMultMatrixf(mat);
}

// Fungsi untuk merender
void render() {
    glClearColor(1.0, 0.2, 0.9, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLightfv(GL_LIGHT0, GL_POSITION, l);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    glVertex3f(l[0], l[1], l[2]);
    glEnd();
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-1300.0, e[1] - 0.1, 1300.0);
    glVertex3f(1300.0, e[1] - 0.1, 1300.0);
    glVertex3f(1300.0, e[1] - 0.1, -1300.0);
    glVertex3f(-1300.0, e[1] - 0.1, -1300.0);
    glEnd();

    // Gambar bayangan
    glPushMatrix();
    glRotatef(ry, 0, 1, 0);
    glRotatef(rx, 1, 0, 0);
    glEnable(GL_LIGHTING);
    glColor3f(0.0, 0.0, 0.8);
    draw();
    glPopMatrix();

    // Gambar bayangan yang muncul
    glPushMatrix();
    glShadowProjection(l, e, n);
    glRotatef(ry, 0, 1, 0);
    glRotatef(rx, 1, 0, 0);
    glDisable(GL_LIGHTING);
    glColor3f(0.4, 0.4, 0.4);
    draw();
    glPopMatrix();

    glutSwapBuffers();
}

// Fungsi untuk menangani input keyboard
void keypress(unsigned char c, int a, int b) {
    if (c == 27) exit(0);
    else if (c == 's') l[1] -= 5.0;
    else if (c == 'w') l[1] += 5.0;
    else if (c == 'a') l[0] -= 5.0;
    else if (c == 'd') l[0] += 5.0;
    else if (c == 'q') l[2] -= 5.0;
    else if (c == 'e') l[2] += 5.0;
    else if (c == 'h') help();
}

// Fungsi untuk melakukan animasi
void idle() {
    rx += 0.1;
    ry += 0.1;
    render();
}

// Fungsi untuk merubah ukuran jendela
void resize(int w, int h) {
    glViewport(0, 0, w, h);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(300, 30);
    glutCreateWindow("Proyeksi Bayangan");
    glutReshapeFunc(resize);
    glutReshapeWindow(400, 400);
    glutKeyboardFunc(keypress);
    glutDisplayFunc(render);
    glutIdleFunc(idle);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, 1.0, 1.0, 400.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -150.0);
    glutMainLoop();
    return 0;
}


/*
1. Pembuatan Bayangan
Bayangan dibuat dengan menggambar objek dua kali. Yang pertama adalah bayangan yang sebenarnya dan yang kedua adalah bayangan yang muncul. Objek kedua diberikan proyeksi bayangan berdasarkan posisi sumber cahaya, posisi mata, dan vektor normal bidang yang menjadi alas bayangan.

2. Fungsi glShadowProjection()
Fungsi glShadowProjection() memiliki peran penting dalam proses pembuatan bayangan. Ini digunakan untuk mengatur matriks proyeksi OpenGL sehingga objek yang digambar akan memiliki bayangan yang tampak sesuai dengan posisi sumber cahaya, posisi mata, dan orientasi bayangan yang diinginkan.

3. Alur glShadowProjection()
Fungsi glShadowProjection() melakukan beberapa operasi matematis untuk menghasilkan matriks proyeksi yang tepat untuk bayangan. Berikut adalah langkah-langkahnya:

Menghitung parameter d dan c: Diperoleh dari dot product antara vektor normal bidang alas bayangan (n) dengan vektor dari sumber cahaya ke titik pada bidang (d), serta dari posisi mata dengan vektor normal bidang (c).

Membuat matriks proyeksi: Dari parameter d dan c yang dihitung, matriks proyeksi (mat) dibuat dengan menggunakan formula yang sesuai dengan proyeksi perspektif OpenGL.

Mengalikan matriks proyeksi: Matriks proyeksi yang telah dibuat kemudian dikalikan dengan matriks model-view saat ini menggunakan glMultMatrixf(mat). Hal ini dilakukan untuk mempengaruhi proyeksi objek yang digambar selanjutnya.
*/