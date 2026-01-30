#include "definitions.h"
#include "app.h"

int main(int argc, const char** argv) {
    if (app_init() != 0) return 1;
    app_run();
    app_destroy();
    return OK;
}
