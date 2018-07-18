#include <cspecs/cspec.h>
#include <string.h>
#include "../protocolo.h"

context(protocolo_tests) {

    describe("Un mensaje") {
        mensaje_t *mensaje = NULL;
        int rv;

        describe("Con un header valido") {
            header_t header = H_GET;            

            describe("Se crea con crear_mensaje") {

                before {
                    rv = crear_mensaje(header, &mensaje);
                } end

                after {
                    destruir_mensaje(&mensaje);
                } end

                it("se crea correctamente") {
                    should_int(rv) be equal to(EXITO);
                    should_ptr(mensaje) not be equal to(NULL);
                    should_int(mensaje->header) be equal to(H_GET);
                    should_ptr(mensaje->payload_set) be equal to(NULL);
                } end

                it("se destruye correctamente") {
                    destruir_mensaje(&mensaje);

                    should_ptr(mensaje) be equal to(NULL);
                } end

                it("se le puede setear una clave valida con set_clave") {           
                    should_ptr(mensaje->clave) be equal to(NULL);

                    rv = set_clave("asd", mensaje);

                    should_int(rv) be equal to(EXITO);
                    should_int(mensaje->tamanio_clave) be equal to(4);
                    should_ptr(mensaje->clave) not be equal to(NULL);
                    // El cleanup es importante, pero no me anda after,
                    // esto es una villa
                    destruir_mensaje(&mensaje);
                } end

            } end

        } end

        describe("Con un header invalido") {
            header_t header = 42;

            it("No se crea con crear_mensaje, que devuelve ERROR_HEADER_INVALIDO") {
                rv = crear_mensaje(42, &mensaje);

                should_int(rv) be equal to(ERROR_HEADER_INVALIDO);
                should_ptr(mensaje) be equal to(NULL);

            } end

        } end

    } end

    describe("Un buffer") {
        int buffer = 0;

        it("aumenta con retornar_aumentando_buffer") {
            int rv = retornar_aumentando_buffer(42, &buffer);

            should_int(rv) be equal to(42);
            should_int(buffer) be equal to(42);
        } end

    } end

    describe("Un mensaje valido") {
        mensaje_t mensaje;
        mensaje.header = H_SET;   // Tamanio 1 (char)
        mensaje.tamanio_clave = 2;
        mensaje.clave = strdup("a");
        mensaje.payload_set = (payload_set_t *) malloc(sizeof(payload_set_t));
        (mensaje.payload_set)->tamanio_valor = mensaje.tamanio_clave;
        (mensaje.payload_set)->valor = strdup(mensaje.clave);

        describe("que le estoy pasando a la funcion serializar_mensaje") {
            
            it("con NULL en el segundo parametro, esta me devuelve 0 y no modifica el puntero") {
                should_int(serializar_mensaje(mensaje, NULL)) be equal to(0);
            } end

            describe("Con un puntero valido en el segundo parametro") {
                char *bytes;
                int numero_bytes = serializar_mensaje(mensaje, &bytes);

                it("serializar_mensaje devuelve el tamanio total del struct sin paddear") {
                    int valor_esperado = sizeof(mensaje.header) +
                                         sizeof(mensaje.tamanio_clave) +
                                         mensaje.tamanio_clave +
                                         (mensaje.payload_set)->tamanio_valor +
                                         sizeof(mensaje.payload_set)->tamanio_valor;

                    should_int(numero_bytes) be equal to(valor_esperado);                     
                } end

                it("cada campo de la memoria serializada coincide con los del struct") {
                    should_char(bytes[0]) be equal to(mensaje.header);
                    should_char(bytes[1]) be equal to(mensaje.tamanio_clave);
                    // No uso el operador [] porque quiero el puntero al principio de la clave
                    // MAGIA NEGRA DE C PERRO
                    should_string(bytes + 2) be equal to(mensaje.clave);
                    should_int(bytes[4]) be equal to((mensaje.payload_set)->tamanio_valor);
                    // En esta arquitectura un int son 4 bytes, CONVENDRIA HACERLO UINT o algo asi?
                    should_string(bytes + 8) be equal to((mensaje.payload_set)->valor);
                } end
            
            } end

        } end

    } end

}