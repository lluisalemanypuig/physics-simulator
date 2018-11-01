#pragma once

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

	private:
		Ui::MainWindow *ui;

	private slots:
		/* Menu buttons */
		void on_actionLoad_triggered();

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
};

