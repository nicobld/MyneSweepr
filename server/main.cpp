#include <pthread.h>
#include <signal.h>

#include <memory>
#include <cstdlib>
#include <ctime>

#include "server/server.h"
#include "engine/engine.h"
#include "state/state.h"
#include "common.h"

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

	srand(time(NULL));

	pthread_create(&thread_engine, NULL, launch_engine, NULL);
	
	server_main();

	return 0;
}