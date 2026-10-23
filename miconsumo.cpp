/*
 * =====================================================================
 * PROGRAMA: MiConsumo - Calculo y Simulacion de Ahorro Energetico
 * CURSO:    Programacion Orientada a Objetos - IU Digital de Antioquia
 * ESTUDIANTE: Jorge Rubio
 * DOCENTE:  Carlos Arturo Castro Castro
 * ANO:      2026-2
 * =====================================================================
 *
 * DESCRIPCION:
 * Programa orientado a objetos que modela el consumo energetico semanal
 * y mensual de un hogar frente al impacto tarifario del fenomeno de 
 * El Nino 2026. Utiliza:
 *   - Clases con atributos privados (encapsulamiento), constructores y metodos.
 *   - Vector unidimensional para horas diarias de uso.
 *   - Vector de objetos para los equipos electricos del hogar.
 *   - Matriz bidimensional (7 filas x MAX_EQUIPOS columnas) para registrar 
 *     el consumo diario en kWh de cada aparato.
 *   - Algoritmos de acumulacion, busqueda de maximos/minimos, estadisticas
 *     y simulacion de ahorro de habitos de consumo.
 * =====================================================================
 */

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// ---------------------------------------------------------------------
// CONSTANTES GLOBALES
// ---------------------------------------------------------------------
const int DIAS = 7;           // 7 dias de la semana (Lunes a Domingo)
const int MAX_EQUIPOS = 5;    // Capacidad maxima de equipos en el hogar

// Vector constante con nombres abreviados de los dias
const string NOMBRE_DIA[DIAS] = {"Lun", "Mar", "Mie", "Jue", "Vie", "Sab", "Dom"};

// =====================================================================
// CLASE Equipo
// Representa un aparato electrico individual de la casa.
// =====================================================================
class Equipo {
private:
    string nombre;         // Nombre descriptivo del equipo (ej: "Nevera LG")
    float potenciaW;       // Potencia nominal en vatios (W) segun etiqueta
    float horas[DIAS];     // Vector de horas diarias de uso (Lun = 0 ... Dom = 6)

public:
    // -----------------------------------------------------------------
    // Constructor por defecto
    // Requerido al inicializar el arreglo de objetos en la clase Hogar.
    // -----------------------------------------------------------------
    Equipo() {
        nombre = "";
        potenciaW = 0.0f;
        for (int d = 0; d < DIAS; d++) {
            horas[d] = 0.0f;
        }
    }

    // -----------------------------------------------------------------
    // Constructor con parametros
    // -----------------------------------------------------------------
    Equipo(string n, float p, const float h[DIAS]) {
        nombre = n;
        potenciaW = p;
        for (int d = 0; d < DIAS; d++) {
            horas[d] = h[d];
        }
    }

    // Getters
    string getNombre() const {
        return nombre;
    }

    float getPotenciaW() const {
        return potenciaW;
    }

    float getHorasDia(int d) const {
        if (d >= 0 && d < DIAS) {
            return horas[d];
        }
        return 0.0f;
    }

    // Muestra datos basicos del equipo y el vector de horas
    void mostrar() const {
        cout << nombre << " (" << fixed << setprecision(1) << potenciaW << " W) - horas:";
        for (int d = 0; d < DIAS; d++) {
            cout << " " << horas[d];
        }
        cout << endl;
    }

    // Energia consumida en un dia especifico: kWh = (W * horas) / 1000
    float energiaDia(int d) const {
        if (d >= 0 && d < DIAS) {
            return (potenciaW * horas[d]) / 1000.0f;
        }
        return 0.0f;
    }

    // Energia acumulada de toda la semana en kWh
    float energiaSemana() const {
        float total = 0.0f;
        for (int d = 0; d < DIAS; d++) {
            total += energiaDia(d);
        }
        return total;
    }

    // -----------------------------------------------------------------
    // METODOS NUEVOS AMPLIADOS (Rúbrica Evidencia 1)
    // -----------------------------------------------------------------

    // Total de horas de uso en la semana (suma del vector horas)
    float horasSemana() const {
        float totalH = 0.0f;
        for (int d = 0; d < DIAS; d++) {
            totalH += horas[d];
        }
        return totalH;
    }

    // Indice del dia de mayor uso (0 = Lun ... 6 = Dom)
    int diaDeMayorUso() const {
        int mayor = 0;
        for (int d = 1; d < DIAS; d++) {
            if (horas[d] > horas[mayor]) {
                mayor = d;
            }
        }
        return mayor;
    }

    // Reduce las horas de todos los dias del vector en un porcentaje dado
    // Ejemplo: con porcentaje = 20, reduce un 20% (multiplica por 0.8)
    void reducirHoras(float porcentaje) {
        if (porcentaje <= 0.0f) return;
        if (porcentaje > 100.0f) porcentaje = 100.0f;
        float factor = 1.0f - (porcentaje / 100.0f);
        for (int d = 0; d < DIAS; d++) {
            horas[d] = horas[d] * factor;
        }
    }
};

// =====================================================================
// CLASE Hogar
// Modela la vivienda mediante composicion: contiene un vector de objetos
// Equipo y una matriz bidimensional con el consumo energetico en kWh.
// =====================================================================
class Hogar {
private:
    string nombre;                         // Nombre de la vivienda
    float tarifa;                          // Tarifa de energia (COP / kWh)
    Equipo equipos[MAX_EQUIPOS];           // Vector de objetos Equipo
    int cantidad;                          // Cantidad actual de equipos registrados
    float consumo[DIAS][MAX_EQUIPOS];      // Matriz [dias][equipos] en kWh

public:
    // Constructor
    Hogar(string n, float t) {
        nombre = n;
        tarifa = t;
        cantidad = 0;
        for (int d = 0; d < DIAS; d++) {
            for (int e = 0; e < MAX_EQUIPOS; e++) {
                consumo[d][e] = 0.0f;
            }
        }
    }

    // Actualiza los datos generales de la vivienda
    void setDatos(string n, float t) {
        nombre = n;
        tarifa = t;
    }

    string getNombre() const {
        return nombre;
    }

    float getTarifa() const {
        return tarifa;
    }

    int getCantidad() const {
        return cantidad;
    }

    // Reinicia la coleccion de equipos
    void vaciar() {
        cantidad = 0;
        for (int d = 0; d < DIAS; d++) {
            for (int e = 0; e < MAX_EQUIPOS; e++) {
                consumo[d][e] = 0.0f;
            }
        }
    }

    // Agrega un equipo al vector de objetos si hay cupo disponible
    void agregarEquipo(Equipo e) {
        if (cantidad < MAX_EQUIPOS) {
            equipos[cantidad] = e;
            cantidad++;
        } else {
            cout << "No caben mas equipos (limite: " << MAX_EQUIPOS << ")" << endl;
        }
    }

    // Muestra la lista de equipos registrados con su indice
    void mostrarEquipos() const {
        cout << defaultfloat << setprecision(6);
        if (cantidad == 0) {
            cout << "Todavia no hay equipos registrados en el hogar." << endl;
            return;
        }
        cout << endl << "--- Equipos registrados en " << nombre << " ---" << endl;
        for (int e = 0; e < cantidad; e++) {
            cout << e << ". ";
            equipos[e].mostrar();
        }
    }

    // Llena la matriz recorriendo filas (dias) y columnas (equipos)
    void calcularMatriz() {
        for (int d = 0; d < DIAS; d++) {
            for (int e = 0; e < cantidad; e++) {
                consumo[d][e] = equipos[e].energiaDia(d);
            }
        }
    }

    // Suma de una fila d: consumo de toda la casa en el dia d (kWh)
    float totalDia(int d) const {
        float total = 0.0f;
        for (int e = 0; e < cantidad; e++) {
            total += consumo[d][e];
        }
        return total;
    }

    // Suma de una columna e: consumo semanal del equipo e (kWh)
    float totalEquipo(int e) const {
        float total = 0.0f;
        for (int d = 0; d < DIAS; d++) {
            total += consumo[d][e];
        }
        return total;
    }

    // Suma total de toda la matriz: consumo semanal del hogar (kWh)
    float totalSemana() const {
        float total = 0.0f;
        for (int d = 0; d < DIAS; d++) {
            total += totalDia(d);
        }
        return total;
    }

    // Presentacion tabular de la matriz con totales por fila y columna
    void mostrarMatriz() const {
        cout << fixed << setprecision(2);
        cout << endl << "================ MATRIZ DE CONSUMO EN kWh ================" << endl;
        cout << "(Filas = Dias de la semana, Columnas = Equipos del hogar)" << endl << endl;

        // Encabezados
        cout << setw(6) << "Dia";
        for (int e = 0; e < cantidad; e++) {
            // Recorta visualmente el nombre si es muy extenso para alineacion impecable
            string nom = equipos[e].getNombre();
            if (nom.length() > 16) nom = nom.substr(0, 16);
            cout << setw(18) << nom;
        }
        cout << setw(12) << "Total (kWh)" << endl;

        cout << string(6 + cantidad * 18 + 12, '-') << endl;

        // Filas por cada dia
        for (int d = 0; d < DIAS; d++) {
            cout << setw(6) << NOMBRE_DIA[d];
            for (int e = 0; e < cantidad; e++) {
                cout << setw(18) << consumo[d][e];
            }
            cout << setw(12) << totalDia(d) << endl;
        }

        cout << string(6 + cantidad * 18 + 12, '=') << endl;

        // Fila final con sumatoria por equipo
        cout << setw(6) << "Total";
        for (int e = 0; e < cantidad; e++) {
            cout << setw(18) << totalEquipo(e);
        }
        cout << setw(12) << totalSemana() << endl;
    }

    // Indice del dia con mayor consumo (fila de mayor suma)
    int diaMasCaro() const {
        int mayor = 0;
        for (int d = 1; d < DIAS; d++) {
            if (totalDia(d) > totalDia(mayor)) {
                mayor = d;
            }
        }
        return mayor;
    }

    // Indice del equipo que mas gasta (columna de mayor suma)
    int equipoQueMasGasta() const {
        int mayor = 0;
        for (int e = 1; e < cantidad; e++) {
            if (totalEquipo(e) > totalEquipo(mayor)) {
                mayor = e;
            }
        }
        return mayor;
    }

    // -----------------------------------------------------------------
    // METODOS NUEVOS AMPLIADOS (Rúbrica Evidencia 1)
    // -----------------------------------------------------------------

    // Indice del dia de menor consumo energetico (fila con la menor suma)
    int diaMasBarato() const {
        int menor = 0;
        for (int d = 1; d < DIAS; d++) {
            if (totalDia(d) < totalDia(menor)) {
                menor = d;
            }
        }
        return menor;
    }

    // Porcentaje que representa el equipo e sobre el consumo total semanal
    float porcentajeEquipo(int e) const {
        float sem = totalSemana();
        if (sem <= 0.0001f || e < 0 || e >= cantidad) return 0.0f;
        return (totalEquipo(e) / sem) * 100.0f;
    }

    // Consumo promedio diario de la vivienda en kWh
    float promedioDiario() const {
        return totalSemana() / static_cast<float>(DIAS);
    }

    // Acceso a un equipo para consultar sus metodos
    const Equipo& getEquipo(int e) const {
        return equipos[e];
    }

    // Simula un ahorro reduciendo las horas del equipo e en un porcentaje.
    // Recalcula la matriz y muestra costo antes, costo despues y ahorro en COP.
    void simularAhorro(int e, float porcentaje) {
        if (e < 0 || e >= cantidad) {
            cout << "Error: Indice de equipo no valido." << endl;
            return;
        }

        // Costo previo a la reduccion
        calcularMatriz();
        float kwhAntes = totalSemana();
        float costoAntes = kwhAntes * tarifa;

        // Aplicar reduccion sobre el objeto equipo seleccionado
        equipos[e].reducirHoras(porcentaje);

        // Recalcular matriz y nuevo costo
        calcularMatriz();
        float kwhDespues = totalSemana();
        float costoDespues = kwhDespues * tarifa;
        float ahorroPesos = costoAntes - costoDespues;
        float ahorroKwh = kwhAntes - kwhDespues;

        cout << fixed << setprecision(2);
        cout << endl << "================ RESULTADO DE LA SIMULACION ================" << endl;
        cout << "Equipo modificado:     " << equipos[e].getNombre() << endl;
        cout << "Porcentaje de ahorro:  " << porcentaje << " %" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << "Consumo semanal antes: " << kwhAntes << " kWh" << endl;
        cout << "Consumo semanal nuevo: " << kwhDespues << " kWh (-" << ahorroKwh << " kWh)" << endl;
        cout << setprecision(0);
        cout << "Costo semanal ANTES:   $ " << costoAntes << " COP" << endl;
        cout << "Costo semanal DESPUES: $ " << costoDespues << " COP" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << ">>> AHORRO ESTIMADO SEMANAL: $ " << ahorroPesos << " COP" << endl;
        cout << ">>> AHORRO ESTIMADO MENSUAL: $ " << (ahorroPesos * 4.0f) << " COP" << endl;
        cout << "============================================================" << endl;
    }

    // Genera el reporte ejecutivo completo de la vivienda
    void reporte() const {
        int dCaro = diaMasCaro();
        int dBarato = diaMasBarato();
        int eGaston = equipoQueMasGasta();
        float totalKwh = totalSemana();
        float costoSemana = totalKwh * tarifa;
        float promDia = promedioDiario();

        cout << fixed;
        cout << endl << "============================================================" << endl;
        cout << "              REPORTE ENERGETICO: " << nombre << endl;
        cout << "============================================================" << endl;
        cout << setprecision(2);
        cout << "Tarifa aplicada:         $ " << tarifa << " COP/kWh" << endl;
        cout << "Consumo total semanal:     " << totalKwh << " kWh" << endl;
        cout << "Consumo promedio diario:   " << promDia << " kWh/dia" << endl;
        cout << setprecision(0);
        cout << "Costo total de la semana: $ " << costoSemana << " COP" << endl;
        cout << "Proyeccion mensual (x 4): $ " << (costoSemana * 4.0f) << " COP" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << "Dia mas CARO:   " << NOMBRE_DIA[dCaro]
             << " (" << setprecision(2) << totalDia(dCaro) << " kWh | $" 
             << setprecision(0) << (totalDia(dCaro) * tarifa) << " COP)" << endl;
        cout << "Dia mas BARATO: " << NOMBRE_DIA[dBarato]
             << " (" << setprecision(2) << totalDia(dBarato) << " kWh | $" 
             << setprecision(0) << (totalDia(dBarato) * tarifa) << " COP)" << endl;
        cout << "Equipo de MAYOR gasto: " << equipos[eGaston].getNombre()
             << " (" << setprecision(2) << totalEquipo(eGaston) << " kWh | $"
             << setprecision(0) << (totalEquipo(eGaston) * tarifa) << " COP en la semana)" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << "PARTICIPACION PORCENTUAL POR EQUIPO EN EL CONSUMO TOTAL:" << endl;
        cout << fixed << setprecision(2);
        for (int e = 0; e < cantidad; e++) {
            cout << "  [" << e << "] " << setw(25) << left << equipos[e].getNombre()
                 << ": " << right << setw(6) << totalEquipo(e) << " kWh  ("
                 << setw(5) << porcentajeEquipo(e) << " %)" << endl;
        }
        cout << "============================================================" << endl;
    }
};

// =====================================================================
// FUNCIONES AUXILIARES DE CARGA DE DATOS REALES
// =====================================================================

// Carga los datos reales de la casa de Jorge con sus 5 equipos y factura real
void cargarCasaReal(Hogar &casa) {
    casa.vaciar();
    // Tarifa real extraida de la factura EPM: 960.34 COP / kWh
    casa.setDatos("Hogar Rubio - Datos Reales", 960.34f);

    // 1. Nevera LG Side by Side 519L (GS51BPP):
    //    Etiqueta: 38.9 kWh/mes. Potencia media efectiva continua: 54 W.
    //    Operacion: 24 horas continuas de lunes a domingo.
    float hNevera[DIAS] = {24.0f, 24.0f, 24.0f, 24.0f, 24.0f, 24.0f, 24.0f};
    casa.agregarEquipo(Equipo("Nevera LG 519L", 54.0f, hNevera));

    // 2. Smart TV LG 75 pulg UHD ThinQ:
    //    Potencia nominal: 230 W.
    //    Uso: 6 h lunes a viernes, 10 h sabado y domingo.
    float hTV[DIAS] = {6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 10.0f, 10.0f};
    casa.agregarEquipo(Equipo("Smart TV LG 75", 230.0f, hTV));

    // 3. Lavadora Samsung:
    //    Potencia ciclo: 500 W.
    //    Uso: 35 min (0.58 h) 2 veces por semana (Miercoles y Sabado).
    float hLavadora[DIAS] = {0.0f, 0.0f, 0.58f, 0.0f, 0.0f, 0.58f, 0.0f};
    casa.agregarEquipo(Equipo("Lavadora Samsung", 500.0f, hLavadora));

    // 4. Computador Mac Mini:
    //    Potencia en operacion: 50 W.
    //    Uso: 6 h lun a vie, 2 h sabado, 0 h domingo.
    float hMacMini[DIAS] = {6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 2.0f, 0.0f};
    casa.agregarEquipo(Equipo("Computador Mac Mini", 50.0f, hMacMini));

    // 5. Freidora de Aire Black & Decker:
    //    Potencia nominal: 1500 W.
    //    Uso diario de 15 minutos (0.25 h cada dia).
    float hFreidora[DIAS] = {0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f};
    casa.agregarEquipo(Equipo("Freidora B&D", 1500.0f, hFreidora));

    casa.calcularMatriz();
}

// =====================================================================
// PROGRAMA PRINCIPAL CON MENU AMPLIADO
// =====================================================================
int main() {
    Hogar casa("Mi Casa", 0.0f);
    int opcion = -1;

    do {
        cout << endl;
        cout << "============================================================" << endl;
        cout << "         SISTEMA DE GESTION ENERGETICA - MiConsumo          " << endl;
        cout << "============================================================" << endl;
        cout << "1. Ingresar datos de la casa (Nombre y Tarifa)" << endl;
        cout << "2. Agregar un equipo manualmente" << endl;
        cout << "3. Ver lista de equipos registrados" << endl;
        cout << "4. Ver matriz de consumo semanal (kWh)" << endl;
        cout << "5. Ver reporte completo y proyeccion mensual" << endl;
        cout << "6. Cargar datos reales de mi casa (5 equipos - Factura EPM)" << endl;
        cout << "7. Simular un ahorro energetico en un equipo" << endl;
        cout << "8. Ver detalle y estadisticas de un equipo" << endl;
        cout << "0. Salir" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << "Seleccione una opcion (0-8): ";

        if (!(cin >> opcion)) {
            cout << "Entrada invalida. Por favor digite un numero." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (opcion) {
        case 1: {
            string nombre;
            float tarifa;
            cout << endl << "--- Datos de la Casa ---" << endl;
            cout << "Ingrese el nombre de la vivienda: ";
            getline(cin >> ws, nombre);
            do {
                cout << "Tarifa segun factura (COP por kWh, mayor a 0): ";
                if (!(cin >> tarifa) || tarifa <= 0) {
                    cout << "Error: La tarifa debe ser un numero positivo." << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    tarifa = -1;
                }
            } while (tarifa <= 0);

            casa.setDatos(nombre, tarifa);
            cout << ">> Datos de la casa actualizados correctamente." << endl;
            break;
        }

        case 2: {
            if (casa.getCantidad() >= MAX_EQUIPOS) {
                cout << "Capacidad maxima alcanzada (" << MAX_EQUIPOS << " equipos)." << endl;
                break;
            }
            string nombre;
            float potencia;
            float horas[DIAS];

            cout << endl << "--- Agregar Equipo ---" << endl;
            cout << "Nombre del equipo: ";
            getline(cin >> ws, nombre);

            // Validacion de potencia
            do {
                cout << "Potencia en vatios (W, segun etiqueta): ";
                if (!(cin >> potencia) || potencia <= 0) {
                    cout << "Error: La potencia debe ser un valor positivo mayor a 0." << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    potencia = -1;
                }
            } while (potencia <= 0);

            // Validacion de horas diarias (0 a 24)
            for (int d = 0; d < DIAS; d++) {
                do {
                    cout << "Horas de uso el " << NOMBRE_DIA[d] << " (0 a 24): ";
                    if (!(cin >> horas[d]) || horas[d] < 0 || horas[d] > 24) {
                        cout << "Error: Las horas deben estar en el rango de 0 a 24." << endl;
                        cin.clear();
                        cin.ignore(10000, '\n');
                        horas[d] = -1;
                    }
                } while (horas[d] < 0 || horas[d] > 24);
            }

            casa.agregarEquipo(Equipo(nombre, potencia, horas));
            casa.calcularMatriz();
            cout << ">> Equipo \"" << nombre << "\" agregado con exito." << endl;
            break;
        }

        case 3:
            casa.mostrarEquipos();
            break;

        case 4:
            if (casa.getCantidad() == 0) {
                cout << "Aun no hay equipos. Agregue uno (opcion 2) o cargue su casa (opcion 6)." << endl;
            } else {
                casa.calcularMatriz();
                casa.mostrarMatriz();
            }
            break;

        case 5:
            if (casa.getCantidad() == 0) {
                cout << "Aun no hay equipos. Agregue uno (opcion 2) o cargue su casa (opcion 6)." << endl;
            } else {
                casa.calcularMatriz();
                casa.reporte();
            }
            break;

        case 6:
            cargarCasaReal(casa);
            cout << endl << ">> ¡Datos reales de la casa cargados exitosamente!" << endl;
            cout << "   - Vivienda: Hogar Rubio" << endl;
            cout << "   - Tarifa:   $ 960.34 COP/kWh (Factura real EPM)" << endl;
            cout << "   - Equipos:  5 equipos modelados" << endl;
            cout << "   - Matriz y consumos calculados." << endl;
            break;

        case 7: {
            if (casa.getCantidad() == 0) {
                cout << "Aun no hay equipos registrados. Primero cargue o agregue equipos." << endl;
                break;
            }
            cout << endl << "--- Simulacion de Ahorro Energetico ---" << endl;
            casa.mostrarEquipos();

            int eqSel;
            float porcentaje;

            // Validacion de equipo
            do {
                cout << "Seleccione el numero del equipo a simular (0 a " << (casa.getCantidad() - 1) << "): ";
                if (!(cin >> eqSel) || eqSel < 0 || eqSel >= casa.getCantidad()) {
                    cout << "Error: Seleccione un numero de equipo valido dentro del rango." << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    eqSel = -1;
                }
            } while (eqSel < 0 || eqSel >= casa.getCantidad());

            // Validacion de porcentaje (1 a 100)
            do {
                cout << "Porcentaje de reduccion de uso (1 a 100 %): ";
                if (!(cin >> porcentaje) || porcentaje < 1.0f || porcentaje > 100.0f) {
                    cout << "Error: El porcentaje debe estar comprendido entre 1 y 100." << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    porcentaje = -1.0f;
                }
            } while (porcentaje < 1.0f || porcentaje > 100.0f);

            casa.simularAhorro(eqSel, porcentaje);
            break;
        }

        case 8: {
            if (casa.getCantidad() == 0) {
                cout << "Aun no hay equipos registrados. Primero cargue o agregue equipos." << endl;
                break;
            }
            cout << endl << "--- Detalle y Estadisticas de un Equipo ---" << endl;
            casa.mostrarEquipos();

            int eqSel;
            do {
                cout << "Seleccione el numero del equipo a consultar (0 a " << (casa.getCantidad() - 1) << "): ";
                if (!(cin >> eqSel) || eqSel < 0 || eqSel >= casa.getCantidad()) {
                    cout << "Error: Indice invalido." << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    eqSel = -1;
                }
            } while (eqSel < 0 || eqSel >= casa.getCantidad());

            casa.calcularMatriz();
            const Equipo &eq = casa.getEquipo(eqSel);
            int diaMayor = eq.diaDeMayorUso();

            cout << fixed << setprecision(2);
            cout << endl << "=========== FICHA TECNICA DEL EQUIPO ===========" << endl;
            cout << "Nombre del equipo:          " << eq.getNombre() << endl;
            cout << "Potencia nominal:           " << eq.getPotenciaW() << " W" << endl;
            cout << "Horas de uso en la semana:  " << eq.horasSemana() << " horas" << endl;
            cout << "Dia de mayor utilizacion:   " << NOMBRE_DIA[diaMayor] 
                 << " (" << eq.getHorasDia(diaMayor) << " horas)" << endl;
            cout << "Consumo semanal en kWh:     " << casa.totalEquipo(eqSel) << " kWh" << endl;
            cout << "Participacion en el consumo: " << casa.porcentajeEquipo(eqSel) << " % del total" << endl;
            cout << setprecision(0);
            cout << "Costo semanal asociado:     $ " << (casa.totalEquipo(eqSel) * casa.getTarifa()) << " COP" << endl;
            cout << "================================================" << endl;
            break;
        }

        case 0:
            cout << endl << "Gracias por usar MiConsumo. ¡Hasta pronto!" << endl;
            break;

        default:
            cout << "Opcion no valida. Seleccione un numero de 0 a 8." << endl;
            break;
        }

    } while (opcion != 0);

    return 0;
}
