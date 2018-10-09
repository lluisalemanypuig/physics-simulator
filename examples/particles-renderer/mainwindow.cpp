#include "mainwindow.hpp"
#include "ui_mainwindow.h"

// PRIVATE

SimulationRenderer *MainWindow::get_SimRend(int t) {
	SimulationRenderer *sr = nullptr;

	switch (t) {
		case 0:
			sr = ui->GL_scene0;
			break;

		case 1:
			sr = ui->GL_scene1;
			break;

		default:
			sr = ui->GL_labScene;
			break;
	}

	return sr;
}

SimulationRenderer *MainWindow::get_SimRend() {
	return get_SimRend(current_tab);
}

QProgressBar *MainWindow::get_scene_bar(int t) {
	QProgressBar *pb = nullptr;

	switch (t) {
		case 0:
			pb = ui->PBar_scene0;
			break;

		case 1:
			pb = ui->PBar_scene1;
			break;

		default:
			pb = ui->PBar_labScene;
			break;
	}

	return pb;
}

QProgressBar *MainWindow::get_scene_bar() {
	return get_scene_bar(current_tab);
}

void MainWindow::get_init_bounce(partinit& bounce) {
	const QString& text = ui->lEBounce->text();
	if (text == "r()") {
		bounce = [&](particle *p) { p->set_bouncing( this->U01(this->eng) ); };
		return;
	}

	bool ok;
	float b = text.toFloat(&ok);
	if (ok) {
		bounce = [b](particle *p) { p->set_bouncing(b); };
		return;
	}

	cerr << "Error: invalid text in 'friction' text box." << endl;
	return;
}

void MainWindow::get_init_friction(partinit& fric) {
	const QString& text = ui->lEFriction->text();
	if (text == "r()") {
		fric = [&](particle *p) { p->set_friction( this->U01(this->eng) ); };
		return;
	}

	bool ok;
	float f = text.toFloat(&ok);
	if (ok) {
		fric = [f](particle *p) { p->set_friction(f); };
		return;
	}

	cerr << "Error: invalid text in 'friction' text box." << endl;
	return;
}

void MainWindow::get_init_lifetime(partinit& lifetime) {
	const QString& text = ui->lELifeTime->text();
	if (text == "r()") {
		lifetime = [&](particle *p) { p->set_lifetime( this->U010(this->eng) ); };
		return;
	}

	bool ok;
	float l = text.toFloat(&ok);
	if (ok) {
		lifetime = [l](particle *p) { p->set_lifetime(l); };
		return;
	}

	cerr << "Error: invalid text in 'lifetime' text box." << endl;
	return;
}

void MainWindow::make_scene(SimulationRenderer *sr) {
	if (not sr->is_scene_cleared()) {
		return;
	}

	bool ok;
	float tt = ui->lETotalTime->text().toFloat(&ok);
	if (not ok) {
		cerr << "Error: invalid text in 'total time' text box." << endl;
		return;
	}

	sr->set_total_time(tt);

	switch (current_tab) {
		case 0:
			make_scene_0(sr);
			break;

		case 1:
			make_scene_1(sr);
			break;

		default:
			make_scene_lab(sr);
			break;
	}

	sr->set_scene_made();
}

void MainWindow::make_init_with_params(initialiser& i) {
	partinit bounce,fric,lifetime;

	// parse text boxes
	get_init_bounce(bounce);
	get_init_friction(fric);
	get_init_lifetime(lifetime);

	// set values to initialiser
	i.set_bounce_initialiser(bounce);
	i.set_friction_initialiser(fric);
	i.set_lifetime_initialiser(lifetime);
}

// PRIVATE SLOTS

void MainWindow::on_PBrun_clicked() {
	ui->PBreset->setEnabled(false);
	ui->PBclear->setEnabled(false);

	SimulationRenderer *sr = get_SimRend();

	// make scene if necessary
	if (sr->is_scene_cleared()) {
		make_scene(sr);

		QProgressBar *s_bar = get_scene_bar();
		s_bar->setMinimum(0);
		s_bar->setMaximum(0);
		s_bar->setValue(0);
	}

	// set all attributes (initialiser, fps,
	// total time, time step, show fps, ...)

	on_lEBounce_returnPressed();
	on_lEFriction_returnPressed();
	on_lELifeTime_returnPressed();

	on_lEfps_returnPressed();
	on_lETimeStep_returnPressed();
	on_lETotalTime_returnPressed();

	on_CBfpsCount_toggled( ui->CBfpsCount->isChecked() );

	// run the simulation
	sr->run_simulation();

	ui->PBreset->setEnabled(true);
	ui->PBclear->setEnabled(true);
}

void MainWindow::on_PBpause_clicked() {
	ui->PBreset->setEnabled(true);
	ui->PBclear->setEnabled(true);

	SimulationRenderer *sr = get_SimRend();
	sr->pause_simulation();
}

void MainWindow::on_PBreset_clicked() {
	SimulationRenderer *sr = get_SimRend();
	sr->reset_simulation();

	get_scene_bar()->setValue(0);
}

void MainWindow::on_PBclear_clicked() {
	SimulationRenderer *sr = get_SimRend();
	sr->clear_simulation();

	get_scene_bar()->setValue(0);
}

void MainWindow::on_TWscenes_currentChanged(int index) {
	// old renderer: pause simulation
	SimulationRenderer *old_sr = get_SimRend();
	old_sr->pause_simulation();

	current_tab = index;

	// make appropriate scene for the new renderer
	// only if the simulation was not cleared
	make_scene( get_SimRend() );

	// enable all buttons
	ui->PBreset->setEnabled(true);
	ui->PBclear->setEnabled(true);
}

void MainWindow::on_lEBounce_returnPressed() {
	// obtain bouncing coefficient initialiser
	// and set it to the appropriate simulator
	partinit bounce;
	get_init_bounce(bounce);

	SimulationRenderer *sr = get_SimRend();
	initialiser *I = sr->get_simulator().get_initialiser();
	I->set_bounce_initialiser(bounce);

	// change bouncing coefficient of all particles
	const vector<particle *>& ps = sr->get_simulator().get_particles();
	for (particle *p : ps) {
		bounce(p);
	}
}

void MainWindow::on_lEFriction_returnPressed() {
	// obtain friction coefficient initialiser
	// and set it to the appropriate simulator
	partinit fric;
	get_init_friction(fric);

	SimulationRenderer *sr = get_SimRend();
	initialiser *I = sr->get_simulator().get_initialiser();
	I->set_friction_initialiser(fric);

	// change friction coefficient of all particles
	const vector<particle *>& ps = sr->get_simulator().get_particles();
	for (particle *p : ps) {
		fric(p);
	}
}

void MainWindow::on_lELifeTime_returnPressed() {
	// obtain lifetime initialiser and set
	// it to the appropriate simulator
	partinit lifetime;
	get_init_lifetime(lifetime);

	SimulationRenderer *sr = get_SimRend();
	initialiser *I = sr->get_simulator().get_initialiser();
	I->set_lifetime_initialiser(lifetime);

	// change lifetime of all particles
	const vector<particle *>& ps = sr->get_simulator().get_particles();
	for (particle *p : ps) {
		lifetime(p);
	}
}

void MainWindow::on_lEfps_returnPressed() {
	bool ok;
	float fps = ui->lEfps->text().toFloat(&ok);
	if (not ok) {
		cerr << "Error: invalid text in 'fps' text box." << endl;
		return;
	}

	SimulationRenderer *sr = get_SimRend();
	if (fps > 0) {
		sr->set_limit_fps(true);
		sr->set_fps( fps );
		ui->CBfpsCount->setEnabled(true);
	}
	else {
		sr->set_limit_fps(false);
		ui->CBfpsCount->setEnabled(false);
	}
}

void MainWindow::on_lETimeStep_returnPressed() {
	bool ok;
	float dt = ui->lETimeStep->text().toFloat(&ok);
	if (not ok) {
		cerr << "Error: invalid text in 'time step' text box." << endl;
		return;
	}

	SimulationRenderer *sr = get_SimRend();
	sr->set_time_step( dt );

	get_scene_bar()->setMaximum( sr->get_total_time()/dt );
}

void MainWindow::on_lETotalTime_returnPressed() {
	bool ok;
	float tt = ui->lETotalTime->text().toFloat(&ok);
	if (not ok) {
		cerr << "Error: invalid text in 'total time' text box." << endl;
		return;
	}

	SimulationRenderer *sr = get_SimRend();
	sr->set_total_time(tt);

	get_scene_bar()->setMaximum( tt/sr->get_time_step() );
}

void MainWindow::on_CBfpsCount_toggled(bool checked) {
	SimulationRenderer *sr = get_SimRend();
	sr->set_show_fps(checked);
}

// PUBLIC

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	random_device r;
	eng = default_random_engine(r());
	U01 = uniform_real_distribution<float>(0.0f,1.0f);
	U010 = uniform_real_distribution<float>(0.0f,10.0f);

	current_tab = ui->TWscenes->currentIndex();

	for (int i = 0; i < ui->TWscenes->count(); ++i) {
		SimulationRenderer *sr = get_SimRend(i);
		sr->set_progress_bar( get_scene_bar(i) );
	}
}

MainWindow::~MainWindow() {
	delete ui;
}
