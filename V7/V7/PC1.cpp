#include "Kommunication.cpp"

int main(int argc, char** argv) {
    // solve just the first two tasks,
    // return if user added more than 7 tasks
    // Example: ./AdaOdd.o 1 2
    if (argc > 4) return 1;

    Kommunication s;
    s.setPCnr(false);
    B15F& drv = B15F::getInstance();

    for (int i = 1; i < argc; i++) {
        switch (argv[i][0]) {
        case '1': // send and receive one Byte
            s.charKommuniktion();
            break;
        case '2': // send a message
            s.stringKommunication();
            break;
        case '3': // send package
            s.DateiKommuniktion();
            break;
        case '0':
            drv.setRegister(&DDRA, 0b00000000);
            drv.setRegister(&PORTA, 0b00000000);
            break;

        default: break;
        }
    }

    return 0;
}
