#include <cspecs/cspec.h>
#include <stdlib.h>
#include "include/estructuras.h"

context(un_ejemplo) {

    describe("void_ptr_to_int") {
        int v;

        it("de NULL es 0") {
            v = void_ptr_to_int(NULL);
            should_int(v) be equal to(0);
        } end

        it("de algo que apunta a 1 es 1") {
            int uno = 1;
            v = void_ptr_to_int(&uno);
            should_int(v) be equal to(1);
        } end
    } end

    describe("Un diccionario") {
        
        t_dictionary * dict = dictionary_create();

        describe("Que tiene insertado \"asd\": 1") {
            int uno = 1;

            before {
                dictionary_put(dict, "asd", (void *) &uno);
            } end
            after {
                dictionary_remove(dict, "asd");
            } end

            it("Se puede obtener un valor con buscar_clave()") {
                should_int(buscar_clave(dict, "asd")) be equal to(1);
            } end
        } end

        describe("Que no tiene insertado \"asd\": 1") {
            it("buscar_clave devuelve 0") {
                should_int(buscar_clave(dict, "asd")) be equal to(0);
            } end
        } end

    } end

    describe("Una lista de info_instancias, a la cual le agrego una instancia") {
        t_list *instancias = list_create();
        info_instancia *instancia = (info_instancia *) malloc(sizeof(info_instancia));
        before {
            instancia->socket = 1;
            // Acordate de poner parentesis aca, [] tiene mas precedencia que -> parece
            (instancia->empieza_con)[0] = 'a';
            (instancia->empieza_con)[1] = 'c';
            list_add(instancias, (void *) instancia);
        } end
        after {
            list_remove(instancias, 0);
        } end

        it("deberia tener la instancia que le agregue") {
            should_ptr(list_get(instancias, 0)) be equal to(instancia);
        } end

        it("obtener_instancia deberia tener el socket que corresponde a la instancia") {
            should_int(obtener_instancia(instancias, "asd")) be equal to(1);
            should_int(obtener_instancia(instancias, "bsd")) be equal to(1);
            should_int(obtener_instancia(instancias, "csd")) be equal to(1);
        } end

    } end
}