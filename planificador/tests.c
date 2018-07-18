#include <cspecs/cspec.h>
#include "include/estructuras.h"

context(tests_structs_planif) {

    describe("Un esi") {
        info_esi *esi;

        before {
            esi = crear_esi(1, 1);
        } end

        after {
            destruir_esi(&esi);
        } end

        it("Tiene sus parametros correctos") {
            should_ptr(esi) not be equal to(NULL);
            should_int(esi->socket) be equal to(1);
            should_int(esi->estimacion) be equal to(1);
        } end

        it("es destruido sin problemas") {
            destruir_esi(&esi);
            should_ptr(esi) be equal to(NULL);
        } end

        describe("y otro esi") {
            info_esi *otro_esi;
            before {
                otro_esi = crear_esi(1, 1);
            } end
            after {
                destruir_esi(&otro_esi);
            } end

            it("son iguales si tienen el mismo socket") {
                should_bool(esi_equals(otro_esi, esi)) be equal to(true);
            } end

            it("son distintos si cualquiera de los dos es null") {
                // Faltaria revisar el otro caso, meh
                otro_esi = NULL;
                should_bool(esi_equals(otro_esi, esi)) be equal to(false);
            } end

            it("son distintos si no tienen el mismo socket") {
                otro_esi->socket = 2;
                should_bool(esi_equals(otro_esi, esi)) be equal to(false);
            } end
        } end

        describe("y una cola de listos") {
            listos = list_create();

            describe("a la cual yo le agrego un esi con agregar_esi") {
                // before y after con describes anidados es una poronga.
                int indice;
                esi = crear_esi(1, 1);
                indice = agregar_esi(listos, esi);
                
                it("contiene al esi en cuestion") {
                    // TODO: Hablar con los pibes, hay cosas locas de punteros que no estoy
                    // entendiendo.
                    should_int(list_size(listos)) be equal to(1);
                    info_esi *data = (info_esi *) list_get(listos, indice);
                    should_bool(esi_equals(data, esi)) be equal to(true);
                } end

                it("y se remueve con remover_esi") {
                    remover_esi(listos, esi->socket);
                    should_int(list_size(listos)) be equal to(0);
                    indice = agregar_esi(listos, esi);
                } end

                describe("y una cola de bloqueados") {
                    // Cuando haces specs pero terminas escribiendo units porque
                    // manejar memoria es magia negra

                    finalizados = list_create();
                    bloqueados = list_create();
                    info_esi *nuevo_esi = crear_esi(1, 1);

                    agregar_esi(finalizados, nuevo_esi);

                    after {
                        list_destroy(finalizados);
                        list_destroy(bloqueados);
                    } end

                    it("se puede mover un esi entre las dos listas") {
                        mover_esi(finalizados, bloqueados, 1);
                        should_int(list_size(finalizados)) be equal to(0);
                        should_int(list_size(bloqueados)) be equal to(1);
                    } end
                } end
            } end
        } end

    } end
}