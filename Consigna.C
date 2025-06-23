#include "libreriaS13.h"

void seleccionarYCargarZona(DatosHorarios datos[], int maxDatos, int *numDatos) {
    ZonasMonitoreo zonaSeleccionada;
    char *nombreArchivo;
    
    opcionesDeZonas();
    printf("Seleccione una zona (1-5): ");
    scanf("%d", (int*)&zonaSeleccionada);
    
    
    if (zonaSeleccionada < BELISARIO || zonaSeleccionada > VALLE_DE_LOS_CHILLOS) {
        printf("Opción inválida. Seleccionando Belisario por defecto.\n");
        zonaSeleccionada = BELISARIO;
    }
    
    
    nombreArchivo = obtenerArchivoZona(zonaSeleccionada);
    
    if (nombreArchivo == NULL) {
        printf("Error: No se pudo determinar el archivo para la zona seleccionada.\n");
        *numDatos = -1;
        return;
    }
    
    printf("Cargando datos de la zona: %s\n", obtenerNombreZona(zonaSeleccionada));
    
    
    *numDatos = cargarDatosBelisario(nombreArchivo, datos, maxDatos);
}

int main(){
  
  DatosHorarios datos[50];
  ContaminanteInfo promedios[4];
  int numDatos;
  int opcion;
  char NomZon[100];
  
  seleccionarYCargarZona(datos, 50, &numDatos);


  if (numDatos <= 0) {
      printf("No se pudieron cargar los datos.\n");
      return 1;
  }
  
  calcularPromedios(datos, numDatos, promedios);


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
      
      printf("Función de predicción aún no implementada.\n");
      break;

    case 3:
      mostrarAnalisisPromedios(promedios);
      break;  
    
    case 4:
      
      menuExportacion(datos, numDatos, promedios);
      break;

    case 5:
      printf("Saliendo del programa...\n");
      break;
    
    default:
      printf("Opcion Invalida: Intente nuevamente\n");
      break;
    }
    
    // Pausa para que el usuario pueda ver los resultados
    if (opcion != 5) {
        printf("\nPresione Enter para continuar...");
        getchar(); // Consume el \n del scanf anterior
        getchar(); // Espera a que el usuario presione Enter
    }
    
  } while(opcion != 5);

  return 0;
}