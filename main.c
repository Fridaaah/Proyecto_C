#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Producto {
    char nombre[50];
    float precio;
    struct Producto* siguiente; // Apuntador al siguiente producto
    struct Producto* anterior; // Apuntador al producto anterior
} Producto;

// Nodo de producto
Producto* crearProducto(char* nombre, float precio) {
    Producto* nuevo = (Producto*)malloc(sizeof(Producto));
    if (nuevo == NULL) {
        printf("Error al asignar memoria.\n");
        exit(1);
    }
    strcpy(nuevo->nombre, nombre);
    nuevo->precio = precio;
    nuevo->siguiente = NULL;
    nuevo->anterior = NULL;
    return nuevo;
}

// Leer el archivo txt y listar
Producto* cargarProductos(const char* archivo) {
    FILE* f = fopen(archivo, "r");
    if (f == NULL) {
        printf("Error al abrir el archivo.\n");
        return NULL;
    }

    Producto* inicio = NULL;
    Producto* anterior = NULL;
    char nombre[50];
    float precio;
    
    while (fscanf(f, "%s %f", nombre, &precio) != EOF) {
        Producto* nuevo = crearProducto(nombre, precio); //Nuevo nodo
        if (inicio == NULL) {
            inicio = nuevo;
        } else {
            anterior->siguiente = nuevo;
            nuevo->anterior = anterior;
        }
        anterior = nuevo;
    }

    fclose(f);
    return inicio;
}

// Mostrar prodcucto
void mostrarProducto(Producto* p) {
    printf("Nombre: %s\n", p->nombre);
    printf("Precio: %.2f\n", p->precio);
}

// Menú principal
int main() {
    Producto* actual = cargarProductos("productos.txt");
    char tecla;

    if (actual == NULL) {
        printf("No se cargaron productos.\n");
        return 1;
    }
    
    while (1) {
        system("clear");
        printf("Presiona 'S' para siguiente, 'A' para anterior, 'Q' para salir.\n");
        mostrarProducto(actual);

        scanf(" %c", &tecla);

        if (tecla == 'S' || tecla == 's') {
            if (actual->siguiente != NULL) {
                actual = actual->siguiente;
            } else {
                printf("Ya estás en el último producto.\n"); // Control de desplazamiento si es que llegamos al final de la lista
                getchar(); getchar();
            }
        } else if (tecla == 'A' || tecla == 'a') {
            if (actual->anterior != NULL) {
                actual = actual->anterior;
            } else {
                printf("Ya estás en el primer producto.\n");
                getchar(); getchar(); 
            }
        } else if (tecla == 'Q' || tecla == 'q') {
            break;
        }
    }

    // Liberar la memoria
    Producto* temp;
    while (actual->anterior != NULL) {
        actual = actual->anterior;
    }
    while (actual != NULL) {
        temp = actual;
        actual = actual->siguiente;
        free(temp);
    }

    return 0;
}
