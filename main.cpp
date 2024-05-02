#include"server.h"

int main(){
    frc::Server srv("127.0.0.1", 5050);
    srv.run();
}