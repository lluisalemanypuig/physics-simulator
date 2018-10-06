#pragma once

// C++ includes
#include <iostream>
using namespace std;

// Qt includes
#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	private:
		Ui::MainWindow *ui;
		int current_tab;

	private slots:

		void on_PBrun_clicked();
		void on_TWscenes_currentChanged(int index);

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
};
