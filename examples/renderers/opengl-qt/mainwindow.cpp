#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

void MainWindow::on_actionLoad_triggered() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open OBJ"), ".", tr("*.obj"));
	ui->renderer->load_mesh(filename);
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}
