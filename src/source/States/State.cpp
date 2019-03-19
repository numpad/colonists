#include "States/State.hpp"

State::~State() {
	
}

void State::setFinished(bool is_finished) {
	finished = is_finished;
}

void State::drawUntilFinished(Window &window) {
	while (window.isOpen() && !finished) {
		glfwPollEvents();
		
		/* debug */
		ImUtil::PrepareFrame();
		
		draw();
		
		ImUtil::RenderFrame();
		window.display();
	}
}
