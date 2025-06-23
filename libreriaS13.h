#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define LIMITE_CO 25000
#define LIMITE_SO2 40 
#define LIMITE_NO2 25
#define LIMITE_PM25 15

// Estructura para almacenar datos por hora
typedef struct {
    char hora[10];
    float CO;
    float SO2;
    float PM25;
    float NO2;
} DatosHorarios;

// Estructura para contaminantes con sus límites
typedef struct {
    char nombre[20];
    float valor;
    float limite;
    char unidad[10];
    char estado[15];
} ContaminanteInfo;

typedef enum {
    BELISARIO = 1,
    CENTRO_HISTORICO = 2,
    EL_CAMAL = 3,
    TUMBACO = 4,
    VALLE_DE_LOS_CHILLOS = 5
} ZonasMonitoreo;

void opcionesDeZonas(){
    printf("\n|            OPCIONES DE ZONAS                |\n");
    printf("-----------------------------------------------\n");
    printf("|1. Belisario                                 |\n");
    printf("|2. Centro Historico de Quito                 |\n");
    printf("|3. El camal                                  |\n");
    printf("|4. Tumbaco                                   |\n");
    printf("|5. Valle de los Chillos                      |\n");
    printf("==============================================\n");

}

char* obtenerNombreZona(ZonasMonitoreo zona) {
    switch(zona) {
        case BELISARIO:
            return "Belisario";
        case CENTRO_HISTORICO:
            return "Centro Historico de Quito";
        case EL_CAMAL:
            return "El Camal";
        case TUMBACO:
            return "Tumbaco";
        case VALLE_DE_LOS_CHILLOS:
            return "Valle de los Chillos";
        default:
            return "Zona desconocida";
    }
}

char* obtenerArchivoZona(ZonasMonitoreo zona) {
    switch(zona) {
        case BELISARIO:
            return "C:\\Users\\keama\\OneDrive\\Documents\\Belisario_2.csv";
        case CENTRO_HISTORICO:
            return "C:\\Users\\keama\\OneDrive\\Documents\\CentroHistorico.csv";
        case EL_CAMAL:
            return "C:\\Users\\keama\\OneDrive\\Documents\\ElCamal.csv";
        case TUMBACO:
            return "C:\\Users\\keama\\OneDrive\\Documents\\Tumbaco.csv";
        case VALLE_DE_LOS_CHILLOS:
            return "C:\\Users\\keama\\OneDrive\\Documents\\ValleChillos.csv";
        default:
            return NULL;
    }
}

//FUNCIONES PARA CONTROLAR LOS ARCHIVOS

int cargarDatosBelisario(char *nombreArchivo, DatosHorarios datos[], int maxDatos) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo '%s'\n", nombreArchivo);
        return -1;
    }
    
    char linea[300];
    int contador = 0;
    int primeraLinea = 1; // Para saltar el header
    
    printf("Cargando datos desde '%s'...\n", nombreArchivo);
    
    while (fgets(linea, sizeof(linea), archivo) && contador < maxDatos) {
        // Saltar la primera línea (header)
        if (primeraLinea) {
            primeraLinea = 0;
            continue;
        }
        
        // El formato es: hora;CO;SO2;PM2.5;NO2
        // Ejemplo: 0:00;386;2;5;9
        
        char *tokens[5]; // Array para almacenar los tokens (5 columnas)
        int tokenCount = 0;
        
        // Dividir la línea por punto y coma
        char *token = strtok(linea, ";");
        while (token != NULL && tokenCount < 5) {
            tokens[tokenCount] = token;
            tokenCount++;
            token = strtok(NULL, ";");
        }
        
        // Verificar que tenemos los 5 tokens necesarios
        if (tokenCount >= 5) {
            // Extraer datos en orden
            strcpy(datos[contador].hora, tokens[0]);     // Hora
            datos[contador].CO = atof(tokens[1]);        // CO
            datos[contador].SO2 = atof(tokens[2]);       // SO2
            datos[contador].PM25 = atof(tokens[3]);      // PM2.5
            
            // Para NO2, remover salto de línea si está presente
            char *no2_str = tokens[4];
            char *newline = strchr(no2_str, '\n');
            if (newline) *newline = '\0';
            datos[contador].NO2 = atof(no2_str);         // NO2
            
            contador++;
        }
    }
    
    fclose(archivo);
    printf("Se cargaron %d registros horarios.\n", contador);
    return contador;
}


void evaluarEstado(ContaminanteInfo *contaminante) {
    float porcentaje = (contaminante->valor / contaminante->limite) * 100;
    
    if (porcentaje <= 50) {
        strcpy(contaminante->estado, "BUENO");
    } else if (porcentaje <= 80) {
        strcpy(contaminante->estado, "MODERADO");
    } else if (porcentaje <= 100) {
        strcpy(contaminante->estado, "MALO");
    } else {
        strcpy(contaminante->estado, "PELIGROSO");
    }
}


void calcularPromedios(DatosHorarios datos[], int numDatos, ContaminanteInfo promedios[]) {
    float sumaCO = 0, sumaSO2 = 0, sumaPM25 = 0, sumaNO2 = 0;
    
    // Sumar todos los valores
    for (int i = 0; i < numDatos; i++) {
        sumaCO += datos[i].CO;
        sumaSO2 += datos[i].SO2;
        sumaPM25 += datos[i].PM25;
        sumaNO2 += datos[i].NO2;
    }
    
    // Calcular promedios
    strcpy(promedios[0].nombre, "CO");
    promedios[0].valor = sumaCO / numDatos;
    promedios[0].limite = LIMITE_CO;
    strcpy(promedios[0].unidad, "ppb");
    
    strcpy(promedios[1].nombre, "SO2 ");
    promedios[1].valor = sumaSO2 / numDatos;
    promedios[1].limite = LIMITE_SO2;
    strcpy(promedios[1].unidad, "ppb ");
    
    strcpy(promedios[2].nombre, "PM2.5");
    promedios[2].valor = sumaPM25 / numDatos;
    promedios[2].limite = LIMITE_PM25;
    strcpy(promedios[2].unidad, "ug/m3");
    
    strcpy(promedios[3].nombre, "NO2 ");
    promedios[3].valor = sumaNO2 / numDatos;
    promedios[3].limite = LIMITE_NO2;
    strcpy(promedios[3].unidad, "ppb ");
    
    
    for (int i = 0; i < 4; i++) {
        evaluarEstado(&promedios[i]);
    }
}


void mostrarDatosPorHoras(DatosHorarios datos[], int numDatos) {
    printf("\n=============== DATOS DEL DIA ===============\n");
    printf("| Hora  |   CO   |  SO2  | PM2.5 |  NO2  |\n");
    printf("===============================================\n");
    
    for (int i = 0; i < numDatos; i++) {
        printf("| %5s | %6.1f | %5.1f | %5.1f | %5.1f |\n",
               datos[i].hora,
               datos[i].CO,
               datos[i].SO2,
               datos[i].PM25,
               datos[i].NO2);
    }
    printf("===============================================\n");
}


void mostrarAnalisisPromedios(ContaminanteInfo promedios[]) {
    printf("\n====================ANALISIS DE PROMEDIOS=========================\n");
    printf("| Contaminante | Promedio     | Limite          | Estado     |\n");
    printf("==================================================================\n");
    
    for (int i = 0; i < 4; i++) {
        printf("| %-12s | %6.2f %-5s | %6.2f %-8s | %-10s |\n",
               promedios[i].nombre,
               promedios[i].valor, promedios[i].unidad,
               promedios[i].limite, promedios[i].unidad,
               promedios[i].estado);
    }
    printf("==================================================================\n");
}

//ALERTAS Y RECOMENDACIONES

void MensajesAlerta(){
  printf("=========================================\n");
  printf("ALERTA: Niveles de contaminacion Altos!!!");
  printf("==========================================");
}
void Recomendaciones(){
  printf("==========================================\n");
  printf("|RECOMENDACIONES\n");
  printf("|1. Fomentar el uso del transporte publico, bicicletas y caminatas para reducir emisiones.\n");
  printf("|2. Evitar el uso de transporte contaminante y aplicar restricciones a los mismos en dias de alta contaminacion.\n");
  printf("|3. Incrementar las areas verdes urbanas para mejorar la calidad del aire.\n");
  printf("==========================================");
}


void mostrarAlertas(ContaminanteInfo promedios[]) {
    MensajesAlerta();
    int hayAlertas = 0;
    
    for (int i = 0; i < 4; i++) {
        if (promedios[i].valor > promedios[i].limite) {
            printf(" ¡¡¡ALERTA!!!: %s excede el limite permitido!\n", promedios[i].nombre);
            printf("   Promedio: %.2f %s (Limite: %.2f %s)\n",
                   promedios[i].valor, promedios[i].unidad,
                   promedios[i].limite, promedios[i].unidad);
            hayAlertas = 1;
        }
    }
    Recomendaciones();
    if (!hayAlertas) {
        printf(" Todos los contaminantes dentro de limites aceptables\n");
    }
    printf("===============================================\n");
}

//MENU PRINCIPAL

void DesplegarMenu() {
    printf("\n");
    printf("==============================================\n");
    printf("|                                             |\n");
    printf("|                 M E N U                     |\n");
    printf("|                                             |\n");
    printf("==============================================\n");
    printf("| 1. Monitoreo de contaminacion actual        |\n");
    printf("| 2. Prediccion de niveles futuros            |\n");
    printf("| 3. Calculo de promedios historicos          |\n");
    printf("| 4. Exportacion de datos                     |\n");
    printf("| 5. Salir                                    |\n");
    printf("==============================================\n");
}

//FUNCIONES DE EXPORTACION DE LOS ARCHIVOS

int exportarDatosPorHoras(DatosHorarios datos[], int numDatos, char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("Error: No se pudo crear el archivo '%s'\n", nombreArchivo);
        return 0;
    }
    
    
    fprintf(archivo, "=============== DATOS POR HORAS ===============\n");
    fprintf(archivo, "| Hora  |   CO   |  SO2  | PM2.5 |  NO2  |\n");
    fprintf(archivo, "===============================================\n");
    
    // Escribir datos
    for (int i = 0; i < numDatos; i++) {
        fprintf(archivo, "| %5s | %6.1f | %5.1f | %5.1f | %5.1f |\n",
                datos[i].hora,
                datos[i].CO,
                datos[i].SO2,
                datos[i].PM25,
                datos[i].NO2);
    }
    fprintf(archivo, "===============================================\n");
    
    // informacion
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(archivo, "\nReporte generado el: %02d/%02d/%04d a las %02d:%02d:%02d\n",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(archivo, "Total de registros: %d\n", numDatos);
    
    fclose(archivo);
    return 1;
}


int exportarAnalisisPromedios(ContaminanteInfo promedios[], char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("Error: No se pudo crear el archivo '%s'\n", nombreArchivo);
        return 0;
    }
    
    fprintf(archivo, "============= ANÁLISIS DE PROMEDIOS =============\n");
    fprintf(archivo, "| Contaminante | Promedio     | Limite       | Estado     |\n");
    fprintf(archivo, "=================================================\n");
    
    for (int i = 0; i < 4; i++) {
        fprintf(archivo, "| %-12s | %6.2f %-5s | %6.2f %-5s | %-10s |\n",
                promedios[i].nombre,
                promedios[i].valor, promedios[i].unidad,
                promedios[i].limite, promedios[i].unidad,
                promedios[i].estado);
    }
    fprintf(archivo, "=================================================\n\n");
    

    fprintf(archivo, "=================== ALERTAS ===================\n");
    int hayAlertas = 0;
    
    for (int i = 0; i < 4; i++) {
        if (promedios[i].valor > promedios[i].limite) {
            fprintf(archivo, " ALERTA: %s excede el limite permitido!\n", promedios[i].nombre);
            fprintf(archivo, "   Promedio: %.2f %s (Limite: %.2f %s)\n",
                    promedios[i].valor, promedios[i].unidad,
                    promedios[i].limite, promedios[i].unidad);
            hayAlertas = 1;
        }
    }
    
    if (!hayAlertas) {
        fprintf(archivo, " Todos los contaminantes dentro de limites aceptables\n");
    }
    fprintf(archivo, "===============================================\n\n");
    
    // recomendaciones
    fprintf(archivo, "==========================================\n");
    fprintf(archivo, "RECOMENDACIONES\n");
    fprintf(archivo, "==========================================\n");
    fprintf(archivo, "1. Fomentar el uso del transporte público, bicicletas y caminatas para reducir emisiones.\n");
    fprintf(archivo, "2. Evitar el uso de transporte contaminante y aplicar restricciones a los mismos en días de alta contaminación.\n");
    fprintf(archivo, "3. Incrementar las áreas verdes urbanas para mejorar la calidad del aire.\n");
    fprintf(archivo, "==========================================\n\n");
    
    // informacion
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(archivo, "Reporte generado el: %02d/%02d/%04d a las %02d:%02d:%02d\n",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    
    fclose(archivo);
    return 1;
}


int exportarReporteCompleto(DatosHorarios datos[], int numDatos, ContaminanteInfo promedios[], char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("Error: No se pudo crear el archivo '%s'\n", nombreArchivo);
        return 0;
    }
    
    
    fprintf(archivo, "========================================================\n");
    fprintf(archivo, "           REPORTE COMPLETO DE CALIDAD DEL AIRE\n");
    fprintf(archivo, "========================================================\n\n");
    
    
    fprintf(archivo, "=============== DATOS POR HORAS ===============\n");
    fprintf(archivo, "| Hora  |   CO   |  SO2  | PM2.5 |  NO2  |\n");
    fprintf(archivo, "===============================================\n");
    
    for (int i = 0; i < numDatos; i++) {
        fprintf(archivo, "| %5s | %6.1f | %5.1f | %5.1f | %5.1f |\n",
                datos[i].hora,
                datos[i].CO,
                datos[i].SO2,
                datos[i].PM25,
                datos[i].NO2);
    }
    fprintf(archivo, "===============================================\n\n");
    
    // promedios
    fprintf(archivo, "============= ANÁLISIS DE PROMEDIOS =============\n");
    fprintf(archivo, "| Contaminante | Promedio     | Limite       | Estado     |\n");
    fprintf(archivo, "=================================================\n");
    
    for (int i = 0; i < 4; i++) {
        fprintf(archivo, "| %-12s | %6.2f %-5s | %6.2f %-5s | %-10s |\n",
                promedios[i].nombre,
                promedios[i].valor, promedios[i].unidad,
                promedios[i].limite, promedios[i].unidad,
                promedios[i].estado);
    }
    fprintf(archivo, "=================================================\n\n");
    
    // Escribir alertas
    fprintf(archivo, "=================== ALERTAS ===================\n");
    int hayAlertas = 0;
    
    for (int i = 0; i < 4; i++) {
        if (promedios[i].valor > promedios[i].limite) {
            fprintf(archivo, " ALERTA: %s excede el limite permitido!\n", promedios[i].nombre);
            fprintf(archivo, "   Promedio: %.2f %s (Limite: %.2f %s)\n",
                    promedios[i].valor, promedios[i].unidad,
                    promedios[i].limite, promedios[i].unidad);
            hayAlertas = 1;
        }
    }
    
    if (!hayAlertas) {
        fprintf(archivo, " Todos los contaminantes dentro de limites aceptables\n");
    }
    fprintf(archivo, "===============================================\n\n");
    
    // recomendaciones
    fprintf(archivo, "==========================================\n");
    fprintf(archivo, "RECOMENDACIONES\n");
    fprintf(archivo, "==========================================\n");
    fprintf(archivo, "1. Fomentar el uso del transporte público, bicicletas y caminatas para reducir emisiones.\n");
    fprintf(archivo, "2. Evitar el uso de transporte contaminante y aplicar restricciones a los mismos en días de alta contaminación.\n");
    fprintf(archivo, "3. Incrementar las áreas verdes urbanas para mejorar la calidad del aire.\n");
    fprintf(archivo, "==========================================\n\n");
    
    // Informacion
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(archivo, "========================================================\n");
    fprintf(archivo, "Reporte generado el: %02d/%02d/%04d a las %02d:%02d:%02d\n",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(archivo, "Total de registros procesados: %d\n", numDatos);
    fprintf(archivo, "========================================================\n");
    
    fclose(archivo);
    return 1;
}

void menuExportacion(DatosHorarios datos[], int numDatos, ContaminanteInfo promedios[]) {
    int opcionExport;
    char nombreArchivo[200];
    int exito = 0;
    
    printf("\n=============== MENU DE EXPORTACION ===============\n");
    printf("| 1. Exportar solo datos por horas                 |\n");
    printf("| 2. Exportar solo analisis de promedios           |\n");
    printf("| 3. Exportar reporte completo                     |\n");
    printf("| 4. Volver al menu principal                      |\n");
    printf("===================================================\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcionExport);
    
    if (opcionExport == 4) {
        return; 
    }
    
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    switch (opcionExport) {
        case 1:
            sprintf(nombreArchivo, "DatosPorHoras_%02d%02d%04d_%02d%02d.txt",
                    tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
            exito = exportarDatosPorHoras(datos, numDatos, nombreArchivo);
            if (exito) {
                printf(" Datos por horas exportados correctamente a:\n%s\n", nombreArchivo);
            }
            break;
            
        case 2:
            sprintf(nombreArchivo, "AnalisisPromedios_%02d%02d%04d_%02d%02d.txt",
                    tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
            exito = exportarAnalisisPromedios(promedios, nombreArchivo);
            if (exito) {
                printf(" Analisis de promedios exportado correctamente a:\n%s\n", nombreArchivo);
            }
            break;
            
        case 3:
            sprintf(nombreArchivo, "ReporteCompleto_%02d%02d%04d_%02d%02d.txt",
                    tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
            exito = exportarReporteCompleto(datos, numDatos, promedios, nombreArchivo);
            if (exito) {
                printf(" Reporte completo exportado correctamente a:\n%s\n", nombreArchivo);
            }
            break;
            
        default:
            printf(" Opción inválida.\n");
            break;
    }
    
    if (!exito && opcionExport >= 1 && opcionExport <= 3) {
        printf(" Error al exportar el archivo.\n");
    }
}
//REGRESION LINEAL
//DEFINICION DE FUNCIONES
    void procesarZona1();  
    void procesarZona2();  
    void procesarZona3();  
    void procesarZona4();  
    void procesarZona5();  
    void procesarZona6();  
    void procesarZona7();  
    int contarLineas(char *nombreArchivo);
    int obtenerDia(char *lineaOriginal);
    double obtenerPenultimo(char *lineaOriginal);
    float obtenerICA(char *lineaOriginal);
//DIRECCIONES DE ARCHIVOS
    char BaseDeDatosZ1[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\BELISARIO.csv";
    char ArchivoSalidaZ1[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\BELISARIO_PROCESADO.csv";
    char BaseDeDatosZ2[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\CAMAL.csv";
    char ArchivoSalidaZ2[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\CAMAL_PROCESADO.csv";
    char BaseDeDatosZ3[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\CARAPUNGO.csv";
    char ArchivoSalidaZ3[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\CARAPUNGO_PROCESADO.csv";
    char BaseDeDatosZ4[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\CENTRO.csv";
    char ArchivoSalidaZ4[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\CENTRO_PROCESADO.csv";
    char BaseDeDatosZ5[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\CHILLOS.csv";
    char ArchivoSalidaZ5[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\CHILLOS_PROCESADO.csv";
    char BaseDeDatosZ6[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\COTOCOLLAO.csv";
    char ArchivoSalidaZ6[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\COTOCOLLAO_PROCESADO.csv";
    char BaseDeDatosZ7[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\TUMBACO.csv";
    char ArchivoSalidaZ7[] = "C:\\Users\\USUARIO\\Desktop\\ProyectoFinalS13\\BASES DE DATOS\\TUMBACO_PROCESADO.csv";
//DESARROLLO DE FUNCIONES
int regresionLineal() {
    int opcion;
    char *dirZona = NULL;

A:
    printf("===============================================\n");
    printf("|        SELECCIONE LA ZONA A PROYECTAR       |\n");
    printf("===============================================\n");
    printf("|1. Belisario                                 |\n");
    printf("|2. El camal                                  |\n");
    printf("|3. Carapungo                                 |\n");
    printf("|4. Centro Historico de Quito                 |\n");
    printf("|5. Valle de los Chillos                      |\n");
    printf("|6. Cotocollao                                |\n");
    printf("|7. Tumbaco                                   |\n");
    printf("===============================================\n");
    printf("Ingrese su opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1: dirZona = ArchivoSalidaZ1; break;
        case 2: dirZona = ArchivoSalidaZ2; break;
        case 3: dirZona = ArchivoSalidaZ3; break;
        case 4: dirZona = ArchivoSalidaZ4; break;
        case 5: dirZona = ArchivoSalidaZ5; break;
        case 6: dirZona = ArchivoSalidaZ6; break;
        case 7: dirZona = ArchivoSalidaZ7; break;
        default:
            printf("===============================================\n");
            printf("|       OPCION INVALIDA. INGRESE DE NUEVO      |\n");
            goto A;
    }

    int n = contarLineas(dirZona);
    printf("Numero de lineas en el archivo: %d\n", n);

    FILE *archivo = fopen(dirZona, "r");
    if (archivo == NULL) {
        printf("Problema al abrir el archivo %s. Fin del programa.\n", dirZona);
        exit(-1);
    }

    char linea[200];
    fgets(linea, sizeof(linea), archivo); // Leer encabezado

    double sumDias = 0.0;
    double sumICA = 0.0;
    double sumXY = 0.0;
    double sumXX = 0.0;

    for (int i = 0; i < n - 1; i++) {
        if (fgets(linea, sizeof(linea), archivo) == NULL) break;

        int dia = obtenerDia(linea);
        double ica = obtenerPenultimo(linea);

        sumDias += dia;
        sumICA += ica;
        sumXY += dia * ica;
        sumXX += dia * dia;
    }
    fclose(archivo);

    // n de datos sin encabezado:
    int ndatos = n - 1;
    double a, b;

    a = ((ndatos * sumXY) - (sumDias * sumICA)) / ((ndatos * sumXX) - (sumDias * sumDias));
    b = (sumICA - (a * sumDias)) / ndatos;

    printf("Ecuacion de para la regresion: y = %lf * x + %lf\n", a, b);

    double y;
    printf("Ingrese valor para predecir ICA (dia): ");
    scanf("%lf", &y);

    double prediccion = a * y + b;
    printf("Valor predicho de ICA para dia %.2lf: %.4lf\n", y, prediccion);

    return prediccion;
}
int obtenerDia(char *lineaOriginal) {
    char copia[200];
    strcpy(copia, lineaOriginal);

    char *token = strtok(copia, ",");
    char *ultimo_token = NULL;

    while (token != NULL) {
        while (*token == ' ') token++;
        ultimo_token = token;
        token = strtok(NULL, ",");
    }

    if (ultimo_token != NULL) {
        return atoi(ultimo_token);
    } else {
        return 0;
    }
}

double obtenerPenultimo(char *lineaOriginal) {
    char copia[1024];
    strncpy(copia, lineaOriginal, sizeof(copia));
    copia[sizeof(copia) - 1] = '\0';

    char *token = strtok(copia, ",");
    char *anterior = NULL;
    char *actual = NULL;

    while (token != NULL) {
        while (*token == ' ') token++;
        anterior = actual;
        actual = token;
        token = strtok(NULL, ",");
    }

    if (anterior != NULL) {
        return atof(anterior);
    } else {
        return 0.0;
    }
}
int contarLineas(char *nombreArchivo) {
    FILE *arch = fopen(nombreArchivo, "r");
    if (arch == NULL) {
        printf("Problema al abrir el archivo %s. Fin del programa.\n", nombreArchivo);
        exit(-1);
    }
    int n = 0;
    char c;
    while ((c = fgetc(arch)) != EOF) {
        if (c == '\n') {
            n++;
        }
    }
    fclose(arch);
    return n;
}

float obtenerICA(char *lineaOriginal) {
    float ICAprom = 0;
    int contador = 0, suma = 0;
    char copia[100];
    strcpy(copia, lineaOriginal);  // Copiamos para no dañar el original

    char *token = strtok(copia, ","); // Leer fecha (1er campo)

    for (int i = 0; i < 6; i++) {
        token = strtok(NULL, ",");
        if (token != NULL) {
            // Eliminar espacios en blanco
            while (*token == ' ') token++;

            if (strlen(token) > 0 && token[0] != '\n') {
                int valor = atoi(token);
                suma += valor;
                contador++;
            }
        }
    }

    if (contador > 0)
        ICAprom = (float)suma / contador;
    else
        ICAprom = 0;

    return ICAprom;
}


void MostrarCabecera() {
    printf("\n");
    printf("===============================================\n");
    printf("|                    Air +                    |\n");
    printf("|  Sistema Integral de Gestion y Prediccion   |\n");
    printf("|         de Contaminacion del Aire           |\n");
    printf("===============================================");
}
void opcionesDeZonas(){
    printf("\n|            OPCIONES DE ZONAS                |\n");
    printf("===============================================\n");
    printf("|1. Belisario                                 |\n");
    printf("|2. El camal                                  |\n");
    printf("|3. Carapungo                                 |\n");
    printf("|4. Centro Historico de Quito                 |\n");
    printf("|5. Valle de los Chillos                      |\n");
    printf("|6. Cotocollao                                |\n");
    printf("|7. Tumbaco                                   |\n");
    printf("===============================================\n");
}
void DesplegarMenu() {
    printf("\n");
    printf("===============================================\n");
    printf("|               OPCIONES ZONALES              |\n");
    printf("===============================================\n");
    printf("| 1. Monitoreo de contaminacion actual        |\n");
    printf("| 2. Prediccion de niveles futuros            |\n");
    printf("| 3. Calculo de promedios historicos          |\n");
    printf("| 4. Exportacion de datos                     |\n");
    printf("| 5. Salir                                    |\n");
    printf("===============================================\n");
}

void procesarZonas(){
procesarZona1();  
procesarZona2();  
procesarZona3();  
procesarZona4();  
procesarZona5();  
procesarZona6();  
procesarZona7();  
}

int contadorInverso(int totalLineas, int lineaActual) {
    return -lineaActual; // simplemente retorna el negativo de la línea actual
}

//PROCESAMIENTO DE DATOS POR ZONAS
void procesarZona1() {
    int total = contarLineas(BaseDeDatosZ1) - 1; // excluye encabezado
    FILE *baseDatos = fopen(BaseDeDatosZ1, "r");
    if (baseDatos == NULL) {
        printf("Problema al abrir el archivo %s.\n", BaseDeDatosZ1);
        exit(-1);
    }

    FILE *archSalida = fopen(ArchivoSalidaZ1, "w");
    if (archSalida == NULL) {
        printf("Problema al abrir el archivo %s.\n", ArchivoSalidaZ1);
        exit(-1);
    }

    char linea[100];
    fgets(linea, sizeof(linea), baseDatos); // Leer encabezado
    linea[strcspn(linea, "\r\n")] = 0;
    fprintf(archSalida, "%s,ICA,nDia\n", linea); // Escribir encabezado con ICA

    int lineaActual = 0;
    while (fgets(linea, sizeof(linea), baseDatos)) {
        linea[strcspn(linea, "\r\n")] = 0;
        float ICA = obtenerICA(linea);
        int nDia = contadorInverso(total, lineaActual);
        fprintf(archSalida, "%s,%.2f,%d\n", linea, ICA, nDia);
        lineaActual++;
    }

    fclose(baseDatos);
    fclose(archSalida);
}

void procesarZona2() {
    int total = contarLineas(BaseDeDatosZ2) - 1; // excluye encabezado
    FILE *baseDatos = fopen(BaseDeDatosZ2, "r");
    if (baseDatos == NULL) {
        printf("Problema al abrir el archivo %s.\n", BaseDeDatosZ2);
        exit(-1);
    }

    FILE *archSalida = fopen(ArchivoSalidaZ2, "w");
    if (archSalida == NULL) {
        printf("Problema al abrir el archivo %s.\n", ArchivoSalidaZ2);
        exit(-1);
    }

    char linea[100];
    fgets(linea, sizeof(linea), baseDatos); // Leer encabezado
    linea[strcspn(linea, "\r\n")] = 0;
    fprintf(archSalida, "%s,ICA,nDia\n", linea); // Escribir encabezado con ICA

    int lineaActual = 0;
    while (fgets(linea, sizeof(linea), baseDatos)) {
        linea[strcspn(linea, "\r\n")] = 0;
        float ICA = obtenerICA(linea);
        int nDia = contadorInverso(total, lineaActual);
        fprintf(archSalida, "%s,%.2f,%d\n", linea, ICA, nDia);
        lineaActual++;
    }

    fclose(baseDatos);
    fclose(archSalida);
}
void procesarZona3() {
    int total = contarLineas(BaseDeDatosZ3) - 1; // excluye encabezado
    FILE *baseDatos = fopen(BaseDeDatosZ3, "r");
    if (baseDatos == NULL) {
        printf("Problema al abrir el archivo %s.\n", BaseDeDatosZ3);
        exit(-1);
    }

    FILE *archSalida = fopen(ArchivoSalidaZ3, "w");
    if (archSalida == NULL) {
        printf("Problema al abrir el archivo %s.\n", ArchivoSalidaZ3);
        exit(-1);
    }

    char linea[100];
    fgets(linea, sizeof(linea), baseDatos); // Leer encabezado
    linea[strcspn(linea, "\r\n")] = 0;
    fprintf(archSalida, "%s,ICA,nDia\n", linea); // Escribir encabezado con ICA

    int lineaActual = 0;
    while (fgets(linea, sizeof(linea), baseDatos)) {
        linea[strcspn(linea, "\r\n")] = 0;
        float ICA = obtenerICA(linea);
        int nDia = contadorInverso(total, lineaActual);
        fprintf(archSalida, "%s,%.2f,%d\n", linea, ICA, nDia);
        lineaActual++;
    }

    fclose(baseDatos);
    fclose(archSalida);
}
void procesarZona4() {
    int total = contarLineas(BaseDeDatosZ4) - 1; // excluye encabezado
    FILE *baseDatos = fopen(BaseDeDatosZ4, "r");
    if (baseDatos == NULL) {
        printf("Problema al abrir el archivo %s.\n", BaseDeDatosZ4);
        exit(-1);
    }

    FILE *archSalida = fopen(ArchivoSalidaZ4, "w");
    if (archSalida == NULL) {
        printf("Problema al abrir el archivo %s.\n", ArchivoSalidaZ4);
        exit(-1);
    }

    char linea[100];
    fgets(linea, sizeof(linea), baseDatos); // Leer encabezado
    linea[strcspn(linea, "\r\n")] = 0;
    fprintf(archSalida, "%s,ICA,nDia\n", linea); // Escribir encabezado con ICA

    int lineaActual = 0;
    while (fgets(linea, sizeof(linea), baseDatos)) {
        linea[strcspn(linea, "\r\n")] = 0;
        float ICA = obtenerICA(linea);
        int nDia = contadorInverso(total, lineaActual);
        fprintf(archSalida, "%s,%.2f,%d\n", linea, ICA, nDia);
        lineaActual++;
    }

    fclose(baseDatos);
    fclose(archSalida);
}
void procesarZona5() {
    int total = contarLineas(BaseDeDatosZ5) - 1; // excluye encabezado
    FILE *baseDatos = fopen(BaseDeDatosZ5, "r");
    if (baseDatos == NULL) {
        printf("Problema al abrir el archivo %s.\n", BaseDeDatosZ5);
        exit(-1);
    }

    FILE *archSalida = fopen(ArchivoSalidaZ5, "w");
    if (archSalida == NULL) {
        printf("Problema al abrir el archivo %s.\n", ArchivoSalidaZ5);
        exit(-1);
    }

    char linea[100];
    fgets(linea, sizeof(linea), baseDatos); // Leer encabezado
    linea[strcspn(linea, "\r\n")] = 0;
    fprintf(archSalida, "%s,ICA,nDia\n", linea); // Escribir encabezado con ICA

    int lineaActual = 0;
    while (fgets(linea, sizeof(linea), baseDatos)) {
        linea[strcspn(linea, "\r\n")] = 0;
        float ICA = obtenerICA(linea);
        int nDia = contadorInverso(total, lineaActual);
        fprintf(archSalida, "%s,%.2f,%d\n", linea, ICA, nDia);
        lineaActual++;
    }

    fclose(baseDatos);
    fclose(archSalida);
}
void procesarZona6() {
    int total = contarLineas(BaseDeDatosZ6) - 1; // excluye encabezado
    FILE *baseDatos = fopen(BaseDeDatosZ6, "r");
    if (baseDatos == NULL) {
        printf("Problema al abrir el archivo %s.\n", BaseDeDatosZ6);
        exit(-1);
    }

    FILE *archSalida = fopen(ArchivoSalidaZ6, "w");
    if (archSalida == NULL) {
        printf("Problema al abrir el archivo %s.\n", ArchivoSalidaZ6);
        exit(-1);
    }

    char linea[100];
    fgets(linea, sizeof(linea), baseDatos); // Leer encabezado
    linea[strcspn(linea, "\r\n")] = 0;
    fprintf(archSalida, "%s,ICA,nDia\n", linea); // Escribir encabezado con ICA

    int lineaActual = 0;
    while (fgets(linea, sizeof(linea), baseDatos)) {
        linea[strcspn(linea, "\r\n")] = 0;
        float ICA = obtenerICA(linea);
        int nDia = contadorInverso(total, lineaActual);
        fprintf(archSalida, "%s,%.2f,%d\n", linea, ICA, nDia);
        lineaActual++;
    }

    fclose(baseDatos);
    fclose(archSalida);
}
void procesarZona7() {
    int total = contarLineas(BaseDeDatosZ7) - 1; // excluye encabezado
    FILE *baseDatos = fopen(BaseDeDatosZ7, "r");
    if (baseDatos == NULL) {
        printf("Problema al abrir el archivo %s.\n", BaseDeDatosZ7);
        exit(-1);
    }

    FILE *archSalida = fopen(ArchivoSalidaZ7, "w");
    if (archSalida == NULL) {
        printf("Problema al abrir el archivo %s.\n", ArchivoSalidaZ7);
        exit(-1);
    }

    char linea[100];
    fgets(linea, sizeof(linea), baseDatos); // Leer encabezado
    linea[strcspn(linea, "\r\n")] = 0;
    fprintf(archSalida, "%s,ICA,nDia\n", linea); // Escribir encabezado con ICA

    int lineaActual = 0;
    while (fgets(linea, sizeof(linea), baseDatos)) {
        linea[strcspn(linea, "\r\n")] = 0;
        float ICA = obtenerICA(linea);
        int nDia = contadorInverso(total, lineaActual);
        fprintf(archSalida, "%s,%.2f,%d\n", linea, ICA, nDia);
        lineaActual++;
    }

    fclose(baseDatos);
    fclose(archSalida);
}
int obtenerDia(char *lineaOriginal) {
    char copia[200];
    strcpy(copia, lineaOriginal);

    char *token = strtok(copia, ",");
    char *ultimo_token = NULL;

    while (token != NULL) {
        while (*token == ' ') token++;
        ultimo_token = token;
        token = strtok(NULL, ",");
    }

    if (ultimo_token != NULL) {
        return atoi(ultimo_token);
    } else {
        return 0;
    }
}

double obtenerPenultimo(char *lineaOriginal) {
    char copia[1024];
    strncpy(copia, lineaOriginal, sizeof(copia));
    copia[sizeof(copia) - 1] = '\0';

    char *token = strtok(copia, ",");
    char *anterior = NULL;
    char *actual = NULL;

    while (token != NULL) {
        while (*token == ' ') token++;
        anterior = actual;
        actual = token;
        token = strtok(NULL, ",");
    }

    if (anterior != NULL) {
        return atof(anterior);
    } else {
        return 0.0;
    }
}

int regresionLineal() {
    int opcion;
    char *dirZona = NULL;

A:
    printf("===============================================\n");
    printf("|        SELECCIONE LA ZONA A PROYECTAR       |\n");
    printf("===============================================\n");
    printf("|1. Belisario                                 |\n");
    printf("|2. El camal                                  |\n");
    printf("|3. Carapungo                                 |\n");
    printf("|4. Centro Historico de Quito                 |\n");
    printf("|5. Valle de los Chillos                      |\n");
    printf("|6. Cotocollao                                |\n");
    printf("|7. Tumbaco                                   |\n");
    printf("===============================================\n");
    printf("Ingrese su opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1: dirZona = ArchivoSalidaZ1; break;
        case 2: dirZona = ArchivoSalidaZ2; break;
        case 3: dirZona = ArchivoSalidaZ3; break;
        case 4: dirZona = ArchivoSalidaZ4; break;
        case 5: dirZona = ArchivoSalidaZ5; break;
        case 6: dirZona = ArchivoSalidaZ6; break;
        case 7: dirZona = ArchivoSalidaZ7; break;
        default:
            printf("===============================================\n");
            printf("|       OPCION INVALIDA. INGRESE DE NUEVO      |\n");
            goto A;
    }

    int n = contarLineas(dirZona);
    printf("Numero de lineas en el archivo: %d\n", n);

    FILE *archivo = fopen(dirZona, "r");
    if (archivo == NULL) {
        printf("Problema al abrir el archivo %s. Fin del programa.\n", dirZona);
        exit(-1);
    }

    char linea[200];
    fgets(linea, sizeof(linea), archivo); // Leer encabezado

    double sumDias = 0.0;
    double sumICA = 0.0;
    double sumXY = 0.0;
    double sumXX = 0.0;

    for (int i = 0; i < n - 1; i++) {
        if (fgets(linea, sizeof(linea), archivo) == NULL) break;

        int dia = obtenerDia(linea);
        double ica = obtenerPenultimo(linea);

        sumDias += dia;
        sumICA += ica;
        sumXY += dia * ica;
        sumXX += dia * dia;
    }
    fclose(archivo);

    // n de datos sin encabezado:
    int ndatos = n - 1;
    double a, b;

    a = ((ndatos * sumXY) - (sumDias * sumICA)) / ((ndatos * sumXX) - (sumDias * sumDias));
    b = (sumICA - (a * sumDias)) / ndatos;

    printf("Ecuacion de para la regresion: y = %lf * x + %lf\n", a, b);

    double y;
    printf("Ingrese valor para predecir ICA (dia): ");
    scanf("%lf", &y);

    double prediccion = a * y + b;
    printf("Valor predicho de ICA para dia %.2lf: %.4lf\n", y, prediccion);

    return 0;
}
