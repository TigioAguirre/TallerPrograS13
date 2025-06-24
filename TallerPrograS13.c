#include "libS13.h"
//uso de zonas con la funcion obtener nombre zona
//DATOS SACADOS DE: https://aqicn.org/historical/es/
//fALTA CALCULO DE PROMEDIO HISTORICO Y LA CONSULTA DE UN VALOR ANTERIOR
//DEFINICION DE FUNCIONES
void seleccionarYCargarZona(DatosHorarios datos[], int maxDatos, int *numDatos,int *zselect);
void mostrarPrediccion(int zonaSeleccionada);
//LLAMADO PRINCIPAL
/*void main(){

double pepe= regresionLineal();
opcionesDeZonas();
DesplegarMenu();
// Genera ICA
}*/

int main(){
procesarZonas(); 
MostrarCabecera();
//LA ZONA QUE SE ELIGE SE GUARDA EN UNA ESTRUCTURA DENTRO DE SLECCIONAR Y CARGAR ZONA
DatosHorarios datos[50];
ContaminanteInfo promedios[4];
int zSelec;
int numDatos;
int opcion;
char NomZon[100];
seleccionarYCargarZona(datos, 50, &numDatos,&zSelec);
if (numDatos <= 0) {
    printf("No se pudieron cargar los datos.\n");
    return 1;
}
calcularPromedioDelDia(datos, numDatos, promedios);
do {
    DesplegarMenu();
    printf("Seleccione una opcion: ");
    scanf("%d",&opcion);
    switch (opcion)
    {
    case 1:
    mostrarDatosPorHoras(datos, numDatos);
    break;
    case 2:
    mostrarPrediccion(zSelec);
    break;

    case 3:
    mostrarAnalisisPromedioDiario(promedios);
    break;  
    
    case 4:
    menuExportacion(datos, numDatos, promedios, zSelec);
    break;
    case 5:
    printf("===============================================\n");
    float promedioICA = obtenerPromedioUlt30(zSelec);
    printf("|INDICE DE CALIDAD DEL AIRE PROMEDIO ULTIMOS 30 DIAS: %.2f\n", promedioICA);
    printf("===============================================\n");
    break;

    case 6:
    printf("|Gracias por usar Air +\n|Saliendo del Programa...\n");
    break;
    
    default:
    printf("Opcion Invalida: Intente nuevamente\n");
    break;
    }
    
    // Pausa para que el usuario pueda ver los resultados
    if (opcion != 6) {
        printf("\nPresione Enter para continuar...");
        getchar(); // Consume el \n del scanf anterior
        getchar(); // Espera a que el usuario presione Enter
    }
    
} while(opcion != 6);

return 0;
}


//DESARROLLO DE FUNCIONES
void seleccionarYCargarZona(DatosHorarios datos[], int maxDatos, int *numDatos,int *zselect) {
    ZonasMonitoreo zonaSeleccionada;
    char *nombreArchivo;
    
    opcionesDeZonas();
    printf("|Seleccione una zona (1-5):\n| ");
    scanf("%d", (int*)&zonaSeleccionada);
    
    if (zonaSeleccionada < BELISARIO || zonaSeleccionada > VALLE_DE_LOS_CHILLOS) {
        printf("Opcion invalida. Seleccionando Belisario por defecto.\n");
        zonaSeleccionada = BELISARIO;
    }
    
    
    nombreArchivo = obtenerArchivoZona(zonaSeleccionada);
    
    if (nombreArchivo == NULL) {
        printf("Error: No se pudo determinar el archivo para la zona seleccionada.\n");
        *numDatos = -1;
        return;
    }
    
    printf("|Cargando datos de la zona: %s\n", obtenerNombreZona(zonaSeleccionada));    
    *numDatos = cargarDatosA(nombreArchivo, datos, maxDatos);
    *zselect=zonaSeleccionada;
    
}
