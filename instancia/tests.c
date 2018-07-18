#include <cspecs/cspec.h>

context(un_ejemplo) {
    describe("El numero 1") {
        const int UNO = 1;

        it("es igual a si mismo") {
            should_int(UNO) be equal to(1);
        } end
    } end
}