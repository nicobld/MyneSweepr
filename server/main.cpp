#include <pthread.h>
#include <signal.h>

#include <memory>
#include <cstdlib>
#include <ctime>

#include "engine/engine.h"
#include "state/state.h"
#include "common.h"
#include <httplib/httplib.h>

class Engine;
class State;

int listenfd;

State state;
Engine engine;

void* launch_engine(__attribute__((unused)) void* arg){
	engine.update();
	pthread_exit(NULL);
}

int main(){
	pthread_t thread_engine;
	std::srand(std::time(nullptr));

	engine.state = &state;
	
	if(signal(SIGINT, ctrlc) == SIG_ERR){
		ctrlc(0);
	}

	pthread_create(&thread_engine, NULL, launch_engine, NULL);

	httplib::Server srv;

	srv.Get("/state", [](const httplib::Request& req, httplib::Response& res) {
		res.set_content(state.serializeJSON().dump(), "application/json");
	});

	srv.Post("/", [](const httplib::Request& req, httplib::Response& res) {
		engine.pushCommand(req.body);
	});

	srv.listen("localhost", 8080);

	return 0;
}