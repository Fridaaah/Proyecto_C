#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructuras
typedef struct Producto {
    char nombre[50];
    float precio;
    struct Producto* siguiente; // Apuntador al siguiente producto
    struct Producto* anterior; // Apuntador al producto anterior
} Producto;

typedef struct Usuario {
    char nombre[50];
    char numeroCelular[11];  // 10 dígitos + 1 para '\0'
    float totalPagar;
    Producto* carrito;
} Usuario;


// Gestión de productos

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


void agregarAlCarrito(Usuario* usuario, Producto* productoActual) {
    // Nuevo nodo con la información del producto actual
    Producto* nuevo = (Producto*)malloc(sizeof(Producto));
    if (nuevo == NULL) {
        printf("Error al asignar memoria para el carrito.\n");
        exit(1);
    }

    strcpy(nuevo->nombre, productoActual->nombre); // copiar el nombre
    nuevo->precio = productoActual->precio; // copiar precio
    nuevo->siguiente = usuario->carrito; // enlazar el nuevo nodo al inicio del carrito
    nuevo->anterior = NULL;

    if (usuario->carrito != NULL) {
        usuario->carrito->anterior = nuevo;
    }

    usuario->carrito = nuevo;  // el carrito empieza cpn ese producto
    usuario->totalPagar += nuevo->precio; // lo suma al total

    printf("Producto agregado al carrito: %s (%.2f)\n", nuevo->nombre, nuevo->precio);
    getchar(); getchar();
} 

// Menú de navegación
void navegarProductos(Producto* actual, Usuario* usuario) {
    char tecla;
    while (1) {
        system("clear");
        printf("Presiona 'S' para siguiente, 'A' para anterior, 'C' para agregar al carrito, 'Q' para salir.\n");
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
        } else if (tecla == 'C' || tecla == 'c') {
                agregarAlCarrito(usuario, actual); {
            }
        } else if (tecla == 'Q' || tecla == 'q') {
            break;
        }
    }
}

// Gestion de usuario

Usuario crearUsuario() {
    Usuario u;

    printf("=== Registro de usuario ===\n");

    printf("Ingresa tu nombre: ");
    scanf(" %[^\n]", u.nombre);  // lee espacios hasta Enter
    printf("Ingresa tu número celular (10 dígitos): ");
    scanf(" %10s", u.numeroCelular);  // hasta 10 caracteres (seguro)

    u.totalPagar = 0.0;
    u.carrito = NULL;

    printf("\n¡Registro exitoso!\n");
    getchar(); getchar();  // pausa para leer

    return u;
}

void mostrarUsuario(Usuario u) {
    printf("Nombre: %s\n", u.nombre);
    printf("Número: %s\n", u.numeroCelular);
    printf("Total a pagar: %.2f\n", u.totalPagar);
}


// Menú principal

void mostrarMenu() {
    printf("=== Menú Principal ===\n");
    printf("1. Ver mi carrito de compras\n");
    printf("2. Ver mi información de usuario\n");
    printf("3. Ver la lista de productos\n");
    printf("4. Salir\n");
}


int main() {
    Producto* inicio = cargarProductos("productos.txt");
    Producto* actual = inicio; 
    Producto* temp; 
    Usuario usuario = crearUsuario();
    int opcion;

    if (actual == NULL) {
        printf("No se cargaron productos.\n");
        return 1;
    }
    
    while (1) {
        system("clear");
        mostrarMenu();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Aqui va el carrito de compras.\n");
                getchar(); getchar();
                break;

            case 2:
                mostrarUsuario(usuario);
                getchar(); getchar();
                break;

            case 3:
                navegarProductos(actual, &usuario);
                break;

            case 4:
                printf("Saliendo de la tiendita...\n");
                
                
                // Liberar memoria
                Producto* temp;
                while (inicio != NULL) {
                    temp = inicio;
                    inicio = inicio->siguiente;
                    free(temp);
                }
                return 0;

            default:
                printf("Opción no válida.\n");
                getchar(); getchar();
                break;
        }
    }
}
