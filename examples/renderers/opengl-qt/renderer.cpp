#include "renderer.hpp"

static const float rotationFactor = 0.5f;
static const float maxRotationCamera = 75.0f;
static const float minDistanceCamera = 1.0f;
static const float maxDistanceCamera = 100.0f;

float ObjRenderer::get_aspect() const {
	float w = static_cast<float>(width());
	return w/height();
}

void ObjRenderer::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslated(0, 0, -distance);
	glRotatef(angleX, 1.0, 0.0, 0.0);
	glRotatef(angleY, 0.0, 1.0, 0.0);

	glPushMatrix();
		//glTranslated(0, 0, -5);
		sphere.slow_render();
	glPopMatrix();
}

void ObjRenderer::resizeGL(int w, int h) {
	glViewport(0,0,w,h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, get_aspect(), 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}

void ObjRenderer::initializeGL() {
	initializeOpenGLFunctions();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, get_aspect(), 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float col[] = {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
	float pos[] = {0.0, 0.0, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	float amb[] = {0.2, 0.2, 0.2, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
}

void ObjRenderer::mousePressEvent(QMouseEvent *event) {
	lastMousePos = event->pos();
}

void ObjRenderer::mouseMoveEvent(QMouseEvent *event) {
	// Rotation
	if (event->buttons() & Qt::LeftButton) {
		angleX += rotationFactor*(event->y() - lastMousePos.y());
		angleX = std::max(-maxRotationCamera, std::min(angleX, maxRotationCamera));
		angleY += rotationFactor*(event->x() - lastMousePos.x());
	}

	lastMousePos = event->pos();

	update();
}

void ObjRenderer::wheelEvent(QWheelEvent *event) {
	// Zoom
	float angle = event->angleDelta().y();

	// negative values mean 'zoom in'
	// positive values mean 'zoom out'

	distance -= 0.01f*angle;
	distance = std::max(minDistanceCamera, std::min(distance, maxDistanceCamera));

	update();
}

ObjRenderer::ObjRenderer(QWidget *parent)
	:
	QOpenGLWidget(parent),
	angleX(0.0f), angleY(0.0f), distance(2.0f)
{

}

ObjRenderer::~ObjRenderer() {
}

void ObjRenderer::load_mesh(const QString& file) {
	sphere.clear();

	int i = file.lastIndexOf(QString("/")) + 1;
	string directory = file.left(i).toStdString();
	string filename = file.right( file.length() - i).toStdString();

	cout << "Load from directory: " << directory << endl;
	cout << "      The .obj file: " << filename << endl;

	OBJ_reader obj_reader;
	obj_reader.load_object(directory, filename, sphere);
	sphere.scale_to_unit();

	update();
}
