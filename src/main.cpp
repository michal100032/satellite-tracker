#include "Application.hpp"

int main() {
	Application app;
	while (app.running()) {
		app.handleEvents();
		app.update();
		app.render();
	}
}