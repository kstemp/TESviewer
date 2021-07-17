#pragma once

#include <QThread>

class LoaderThread : public QThread {
	Q_OBJECT
public:

	LoaderThread(const QSet<QString>& files) : QThread() {
		while (true) {
			sleep(100);
		}
	}

signals:

	void onProgress();

private:
	void run() {
	}
};