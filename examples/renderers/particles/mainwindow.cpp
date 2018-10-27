#include "mainwindow.hpp"
#include "ui_mainwindow.h"

// PRIVATE

SimulationRenderer *MainWindow::get_SimRend(int t) {
	SimulationRenderer *sr = nullptr;
	switch (t) {
		case 0:	sr = ui->GL_sim0; break;
		case 1:	sr = ui->GL_sim1; break;
		case 2:	sr = ui->GL_sim2; break;
		case 3:	sr = ui->GL_sim3; break;
		case 4:	sr = ui->GL_sim4; break;
		case 5:	sr = ui->GL_sim5; break;
		default:
			cerr << "        MainWindow::get_SimRend: Error: Unhandled value '"
				 << t << "'" << endl;
	}
	return sr;
}

SimulationRenderer *MainWindow::get_SimRend() {
	return get_SimRend(current_tab);
}

QProgressBar *MainWindow::get_sim_bar(int t) {
	QProgressBar *pb = nullptr;
	switch (t) {
		case 0:	pb = ui->PBar_sim0; break;
		case 1:	pb = ui->PBar_sim1; break;
		case 2:	pb = ui->PBar_sim2; break;
		case 3:	pb = ui->PBar_sim3; break;
		case 4:	pb = ui->PBar_sim4; break;
		case 5:	pb = ui->PBar_sim5; break;
		default:
			cerr << "        MainWindow::get_scene_bar: Error: Unhandled value '"
				 << t << "'" << endl;
			break;
	}
	return pb;
}

QProgressBar *MainWindow::get_sim_bar() {
	return get_sim_bar(current_tab);
}

void MainWindow::get_init_bounce(partinit& bounce) {
	const QString& text = ui->lEBounce->text();
	if (text == "r()") {
		bounce = [&](particle *p) { p->set_bouncing( this->U01(this->eng) ); };
		cout << "        Use random bouncing coefficient" << endl;
		return;
	}

	bool ok;
	float b = text.toFloat(&ok);
	if (ok) {
		bounce = [b](particle *p) { p->set_bouncing(b); };
		cout << "        Set bouncing coefficient to " << b << endl;
		return;
	}

	cerr << "        Error: invalid text in 'friction' text box." << endl;
}

void MainWindow::get_init_friction(partinit& fric) {
	const QString& text = ui->lEFriction->text();
	if (text == "r()") {
		fric = [&](particle *p) { p->set_friction( this->U01(this->eng) ); };
		cout << "        Use random friction coefficient" << endl;
		return;
	}

	bool ok;
	float f = text.toFloat(&ok);
	if (ok) {
		fric = [f](particle *p) { p->set_friction(f); };
		cout << "        Set friction coefficient to " << f << endl;
		return;
	}

	cerr << "        Error: invalid text in 'friction' text box." << endl;
}

void MainWindow::get_init_lifetime(partinit& lifetime) {
	const QString& text = ui->lELifeTime->text();
	if (text == "r()") {
		lifetime = [&](particle *p) { p->set_lifetime( this->U010(this->eng) ); };
		cout << "        Use random particle's lifetime" << endl;
		return;
	}

	bool ok;
	float l = text.toFloat(&ok);
	if (ok) {
		lifetime = [l](particle *p) { p->set_lifetime(l); };
		cout << "        Set particle's lifetime to " << l << endl;
		return;
	}

	cerr << "        Error: invalid text in 'lifetime' text box." << endl;
}

void MainWindow::make_sim(SimulationRenderer *sr) {
	if (not sr->is_scene_cleared()) {
		return;
	}

	switch (current_tab) {
		case 0: make_sim0(sr); break;
		case 1: make_sim1(sr); break;
		case 2: make_sim2(sr); break;
		case 3: make_sim3(sr); break;
		case 4: make_sim4(sr); break;
		case 5: make_sim5(sr); break;
		default:
			cerr << "        MainWindow::make_sim: Error: can't make simulation for tab '"
				 << current_tab << "'" << endl;
			break;
	}

	sr->set_scene_made();
	// used to set to 0 the renderer's progress bar
	sr->reset_simulation();
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

void MainWindow::set_params() {
	if (current_tab != 4 and current_tab != 5) {
		on_lEBounce_returnPressed();
		on_lEFriction_returnPressed();
	}
	on_lELifeTime_returnPressed();

	on_lEfps_returnPressed();
	on_lETimeStep_returnPressed();
	on_lETotalTime_returnPressed();

	on_CoBsolver_currentIndexChanged(QString());
	on_Slider_PointSize_sliderMoved(0);
}

void MainWindow::init_environment() {

	if (current_tab == 4 or current_tab == 5) {
		ui->lEBounce->setEnabled(false);
		ui->lEFriction->setEnabled(false);
	}
	else {
		ui->lEBounce->setEnabled(true);
		ui->lEFriction->setEnabled(true);
	}

	set_params();
	make_sim( get_SimRend() );
	get_SimRend()->allow_to_run();
}

// PUBLIC SLOTS

void MainWindow::simulation_done() {
	ui->PBreset->setEnabled(true);
	ui->PBclear->setEnabled(true);
}

// PRIVATE SLOTS

void MainWindow::on_PBrun_clicked() {
	cout << "------------------------" << endl;
	cout << "Run!" << endl;

	ui->PBreset->setEnabled(false);
	ui->PBclear->setEnabled(false);

	// make scene if necessary
	make_sim( get_SimRend() );

	// run the simulation
	get_SimRend()->run_simulation();
}

void MainWindow::on_PBpause_clicked() {
	cout << "------------------------" << endl;
	cout << "Pause simulation" << endl;

	ui->PBreset->setEnabled(true);
	ui->PBclear->setEnabled(true);

	SimulationRenderer *sr = get_SimRend();
	sr->pause_simulation();
}

void MainWindow::on_PBreset_clicked() {
	cout << "------------------------" << endl;
	cout << "Reset simulation" << endl;

	SimulationRenderer *sr = get_SimRend();
	sr->reset_simulation();
}

void MainWindow::on_PBclear_clicked() {
	cout << "------------------------" << endl;
	cout << "Clear simulation" << endl;

	SimulationRenderer *sr = get_SimRend();
	sr->clear_simulation();
}

void MainWindow::on_TWscenes_currentChanged(int index) {
	// pause simulation of current renderer
	on_PBpause_clicked();

	current_tab = index;

	init_environment();
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
	double fps = ui->lEfps->text().toDouble(&ok);
	if (not ok) {
		cerr << "        Error: invalid text in 'fps' text box." << endl;
		return;
	}

	SimulationRenderer *sr = get_SimRend();
	if (fps > 0) {
		cout << "        Set FPS limit to " << fps << endl;
		sr->set_limit_fps(true);
		sr->set_fps( fps );
	}
	else {
		sr->set_limit_fps(false);
		ui->lShowFPS->setText(QString("-1"));
	}
}

void MainWindow::on_lETimeStep_returnPressed() {
	bool ok;
	float dt = ui->lETimeStep->text().toFloat(&ok);
	if (not ok) {
		cerr << "        Error: invalid text in 'time step' text box." << endl;
		return;
	}

	cout << "        Set time step to " << dt << endl;

	SimulationRenderer *sr = get_SimRend();
	sr->set_time_step( dt );

	get_sim_bar()->setMaximum( static_cast<int>(sr->get_total_time()/dt) );
}

void MainWindow::on_lETotalTime_returnPressed() {
	bool ok;
	float tt = ui->lETotalTime->text().toFloat(&ok);
	if (not ok) {
		cerr << "        Error: invalid text in 'total time' text box." << endl;
		return;
	}

	cout << "        Set total time to " << tt << endl;

	SimulationRenderer *sr = get_SimRend();
	sr->set_total_time(tt);
	float dt = ui->lETimeStep->text().toFloat(&ok);

	get_sim_bar()->setMaximum( static_cast<int>(tt/dt) );
}

void MainWindow::on_CoBsolver_currentIndexChanged(const QString& ) {
	// this slot has its parameter removed (the text
	// is retrieved to 'text') so that it can be called
	// from other parts of the code.

	QString text = ui->CoBsolver->currentText();

	SimulationRenderer *sr = get_SimRend();
	if (text == "Euler Explicit") {
		cout << "        Use Euler Explicit" << endl;
		sr->get_simulator().set_solver( solver_type::EulerOrig );
	}
	else if (text == "Euler Semi-Imp") {
		cout << "        Use Semi-Implicit Euler" << endl;
		sr->get_simulator().set_solver( solver_type::EulerSemi );
	}
	else if (text == "Verlet") {
		cout << "        Use Verlet" << endl;
		sr->get_simulator().set_solver( solver_type::Verlet );
	}
	else {
		cerr << "        Invalid value for solver type" << endl;
	}
}

void MainWindow::on_Slider_PointSize_sliderMoved(int ) {
	// this slot has its parameter removed (the text
	// is retrieved to 'text') so that it can be called
	// from other parts of the code.

	int value = ui->Slider_PointSize->value();
	SimulationRenderer *sr = get_SimRend();
	sr->set_particle_size( value/2.0f );
}

// PUBLIC

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow)
{
	cout << "Initialising main window..." << endl;
	ui->setupUi(this);
	current_tab = ui->TWscenes->currentIndex();
	cout << "    Current tab: " << current_tab << endl;

	cout << "    Initialising random generators..." << endl;
	random_device r;
	eng = default_random_engine(r());
	U01 = uniform_real_distribution<float>(0.0f,1.0f);
	U010 = uniform_real_distribution<float>(0.0f,10.0f);

	cout << "    Several things:" << endl;
	cout << "        -> Assigning label and progress bars" << endl;
	cout << "           to renderers..." << endl;
	cout << "        -> Connecting signals and slots..." << endl;
	for (int i = 0; i < ui->TWscenes->count(); ++i) {
		SimulationRenderer *sr = get_SimRend(i);
		sr->set_progress_bar( get_sim_bar(i) );
		sr->set_label_show_fps( ui->lShowFPS );
		connect(
			sr,   &SimulationRenderer::simulation_completed,
			this, &MainWindow::simulation_done
		);
	}

	init_environment();

	cout << "    Loading models for renderisation..." << endl;
	sim_ball = new rendered_mesh();
	OBJ_reader obj;
	obj.load_object("../particles-renderer/models", "SPH_HalfSmooth_Mat.obj", *sim_ball);
	sim_ball->scale_to_unit();
	get_SimRend(2)->set_sphere(sim_ball);
	get_SimRend(3)->set_sphere(sim_ball);
}

MainWindow::~MainWindow() {
	delete ui;
	if (sim_ball != nullptr) {
		delete sim_ball;
	}
}
